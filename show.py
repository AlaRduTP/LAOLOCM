import matplotlib.pyplot as plt
import numpy as np
from sys import stdin, argv

data = []
file = stdin
if len(argv) == 2:
    file = open(argv[1], "r")

for line in file:
    if 'Fitness' in line:
        data.append(list(map(float, line.split()[1:])))

plt.figure(figsize=(10, 7))
plt.title('Agent Fitness')
plt.ylabel('Fitness')
plt.xlabel('Generation')
plt.boxplot(data)
loc = np.arange(1, len(data) + 1, len(data) // 30 + 1)
plt.xticks(loc, loc)
plt.show()
