#include "../engine.h"
#include "../../board/board.h"

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>

class GPTEngine : public Engine {

private:

    struct Hash {
        std::size_t operator()(uint64_t x) const {
            return x;
        }
    };


    mutable std::unordered_map<uint64_t, double, Hash> cache;


    static constexpr double NEG =
        -std::numeric_limits<double>::infinity();


    uint64_t hash(const Board& b) const {

        uint64_t h = 0;

        for(int i=0;i<4;i++) {
            for(int j=0;j<4;j++) {
                h <<= 5;
                h |= b.getCell(i,j);
            }
        }

        return h;
    }



    int empty(const Board& b) const {

        int n = 0;

        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                if(b.getCell(i,j)==0)
                    n++;

        return n;
    }



    double smoothness(const Board& b) const {

        double s = 0;


        for(int i=0;i<4;i++) {

            for(int j=0;j<4;j++) {

                int x = b.getCell(i,j);

                if(!x)
                    continue;


                if(i<3 && b.getCell(i+1,j))
                    s -= abs(x-b.getCell(i+1,j));


                if(j<3 && b.getCell(i,j+1))
                    s -= abs(x-b.getCell(i,j+1));
            }
        }


        return s;
    }



    double monotonicity(const Board& b) const {

        double totals[4]={0,0,0,0};


        for(int i=0;i<4;i++) {

            for(int j=0;j<3;j++) {

                int a=b.getCell(i,j);
                int c=b.getCell(i,j+1);


                if(a>c)
                    totals[0]+=c-a;
                else
                    totals[1]+=a-c;
            }
        }


        for(int j=0;j<4;j++) {

            for(int i=0;i<3;i++) {

                int a=b.getCell(i,j);
                int c=b.getCell(i+1,j);


                if(a>c)
                    totals[2]+=c-a;
                else
                    totals[3]+=a-c;
            }
        }


        return std::max(totals[0],totals[1])
             + std::max(totals[2],totals[3]);
    }



    double mergeScore(const Board& b) const {

        double s=0;


        for(int i=0;i<4;i++) {

            for(int j=0;j<4;j++) {

                int x=b.getCell(i,j);

                if(!x)
                    continue;


                if(i<3 && b.getCell(i+1,j)==x)
                    s += x*x;


                if(j<3 && b.getCell(i,j+1)==x)
                    s += x*x;
            }
        }


        return s;
    }



    double gradient(const Board& b) const {


        static constexpr double g[4][4]={
            {65536,16384,4096,1024},
            {512,256,128,64},
            {32,16,8,4},
            {2,1,.5,.25}
        };


        double best=NEG;


        for(int mode=0;mode<4;mode++) {

            double s=0;


            for(int i=0;i<4;i++) {

                for(int j=0;j<4;j++) {

                    int r=i;
                    int c=j;


                    if(mode&1)
                        r=3-r;

                    if(mode&2)
                        c=3-c;


                    s +=
                        g[r][c] *
                        b.getCell(i,j);
                }
            }


            best=std::max(best,s);
        }


        return best;
    }



    double evaluate(const Board& b) const {

        return
            100000.0 * empty(b)
          + 1000.0   * gradient(b)
          + 100.0    * monotonicity(b)
          + 50.0     * mergeScore(b)
          + 10.0     * smoothness(b);
    }



    double search(const Board& b,
                  int depth,
                  bool player) const {


        uint64_t key =
            hash(b)
            ^ ((uint64_t)depth<<60)
            ^ ((uint64_t)player<<63);


        auto it=cache.find(key);

        if(it!=cache.end())
            return it->second;



        double result;


        if(depth==0 || b.isLoss()) {

            result=evaluate(b);

        } else if(player) {

            result=maximize(b,depth);

        } else {

            result=chance(b,depth);
        }


        cache[key]=result;

        return result;
    }



    double maximize(const Board& b,
                    int depth) const {


        double best=NEG;


        // left and up first are usually strongest
        static constexpr int order[4]={
            2,0,3,1
        };


        for(int k=0;k<4;k++) {

            Board next=b;


            int move=order[k];


            if(!next.makeMove(move))
                continue;


            best=std::max(
                best,
                search(next,
                       depth-1,
                       false)
            );
        }


        if(best==NEG)
            return evaluate(b);


        return best;
    }



    double chance(const Board& b,
                  int depth) const {


        std::vector<std::pair<int,int>> cells;


        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                if(b.getCell(i,j)==0)
                    cells.push_back({i,j});


        if(cells.empty())
            return evaluate(b);



        double result=0;


        for(auto [r,c]:cells) {


            Board two=b;

            two.setCell(1,r,c);

            result +=
                0.9 *
                search(two,
                       depth-1,
                       true);



            Board four=b;

            four.setCell(2,r,c);

            result +=
                0.1 *
                search(four,
                       depth-1,
                       true);
        }


        return result/cells.size();
    }



    int maxDepth(const Board& b) const {

        int e=empty(b);


        if(e>=10) return 2;
        if(e>=8)  return 3;
        if(e>=6)  return 4;
        if(e>=4)  return 5;
        if(e>=2)  return 7;

        return 8;
    }



public:


    std::uint8_t makeDecision(const Board& b) override {


        cache.clear();


        int depth=maxDepth(b);


        double best=NEG;

        uint8_t answer=0;


        for(int move=0;move<4;move++) {


            Board next=b;


            if(!next.makeMove(move))
                continue;



            double value =
                search(next,
                       depth-1,
                       false);



            if(value>best) {

                best=value;
                answer=move;
            }
        }


        return answer;
    }

};