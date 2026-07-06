# 2048-bot

Last updated July 6, 2026

## Performance

| Search depth | 1024  | 2048  | 4096  | 8192   | Total tests |
|--------------|-------|-------|-------|--------|-------------|
| 1            | 98.4% | 94.2% | 57.6% | 0.008% | 86947       |
| 2            | 99.6% | 97.1% | 61.8% | 0.06%  | 3106        |

## Algorithm

The bot uses expectimax search with an n-tuple network for evaluation. Tuples are taken from *Multistage Temporal Difference Learning for 2048-Like Games by Yeh et al.* and are trained using 1000000 games.