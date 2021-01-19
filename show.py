import matplotlib.pyplot as plt
import numpy as np
from sys import stdin, argv

x = []
y = []
file = stdin
if len(argv) == 2:
    file = open(argv[1], "r")

gen = 1
for line in file:
    if 'Fitness' in line:
        fitnesses = line.split()[1:]
        x.append([gen] * len(fitnesses))
        y.append(list(map(float, fitnesses)))
        gen += 1

plt.figure(figsize=(10, 7))
plt.title('Agent Fitness')
plt.ylabel('Fitness')
plt.xlabel('Generation')
plt.plot(x, y, '.', color='blue')
plt.show()
