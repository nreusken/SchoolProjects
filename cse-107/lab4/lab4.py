from random import random


def printArray(c1, c2, array):
    print("   q:", end="\t")
    for i in range(len(array)):
        print(c2[i], end="\t")
    print()
    print('p', end=" ")
    for i in range((len(array)+1)*8-1):
        print('-', end="")
    print()
    for i in range(len(array)):
        print(c1[i], '|', end="\t")
        for j in range(len(array[i])):
            if j == 0:
                print('{:.3f}'.format(round(array[i][j], 4)), end="\t")
            else:
                print('{:.3f}'.format(round(array[i][j], 4)), end="\t")
        print()


c1 = [.1, .2, .3, .4, .5, .6, .7, .8, .9]
c2 = [.1, .2, .3, .4, .5, .6, .7, .8, .9]

mean = [[0 for j in range(len(c1))] for i in range(len(c2))]
var = [[0 for j in range(len(c1))] for i in range(len(c2))]

for i in range(len(c1)):
    for j in range(len(c2)):
        totalmean = 0
        totalvar = 0
        for _ in range(10000):
            n = 1
            throw = random()
            while throw >= c1[i]:
                n += 1
                throw = random()
            y = 0
            for _ in range(n):
                throw = random()
                if throw < c2[j]:
                    y += 1
            totalmean += y
            totalvar += y**2
        mean[i][j] = totalmean / 10000
        var[i][j] = ((totalvar) / 10000) - (totalmean / 10000)**2


print('mean:')
printArray(c1, c2, mean)

print('\nvariance:')
printArray(c1, c2, var)
