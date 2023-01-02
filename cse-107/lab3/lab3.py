from random import random


def printArray(array):
    print("   Y:", end="\t")
    for i in range(len(array)):
        print(i, end="\t")
    print()
    print('X', end=" ")
    for i in range((len(array)+1)*8-1):
        print('-', end="")
    print()
    for i in range(len(array)):
        print(i, '|', end="\t")
        for j in range(i+1):
            if j == 0:
                print('{:.4f}'.format(round(array[i][j], 4)), end="\t")
            else:
                print('{:.4f}'.format(round(array[i][j], 4)), end="\t")
        print()


n = 8
p = 0.6
q = 0.7


counter = [[0 for j in range(i+1)] for i in range(n+1)]


for i in range(100000):
    x = 0
    y = 0
    for j in range(n):
        throw1 = random()
        if throw1 < p:
            x += 1
            throw2 = random()
            if throw2 < q:
                y += 1
    counter[x][y] += 1

joint = [[0 for j in range(i+1)] for i in range(n+1)]
for i in range(len(counter)):
    for j in range(i+1):
        joint[i][j] = counter[i][j] / 100000

print("Joint PMF of X and Y")
printArray(joint)


margY = [0 for i in range(n+1)]
for i in range(len(joint)):
    for j in range(i+1):
        margY[j] += joint[i][j]

condXY = [[0 for j in range(i+1)] for i in range(n+1)]
for i in range(len(condXY)):
    for j in range(i+1):
        condXY[i][j] = joint[i][j] / margY[j]
print("\nConditional PMF of X given Y")
printArray(condXY)


margX = [0 for i in range(n+1)]
for i in range(len(joint)):
    for j in range(i+1):
        margX[i] += joint[i][j]

condYX = [[0 for j in range(i+1)] for i in range(n+1)]
for i in range(len(condYX)):
    for j in range(i+1):
        condYX[i][j] = joint[i][j] / margX[i]

print("\nConditional PMF of Y given X")
printArray(condYX)
