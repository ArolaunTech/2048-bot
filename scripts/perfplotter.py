import matplotlib.pyplot as plt
import numpy as np

perf = "0 0 0 0 11 697 9030 38779 46790 4692 1 0 0 0 0 0 0 0"

perf = list(map(int, perf.split()))
print(perf)

plt.bar(np.linspace(0, len(perf) - 1, num=len(perf)), perf)
plt.show()