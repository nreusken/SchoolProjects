from random import random

aBalls = [10, 50, 90]

i = None

if i:
    print('hi')

for ratios in aBalls:
    lastAzure = 0
    other = 0
    for i in range(2000):
        total = 100
        azure = ratios
        carmine = total - azure
        pre = None
        while total != 1:
            pulled = random()
            if pulled < azure / total and (pre or pre == None):
                azure -= 1
                total -= 1
                pre = 1
            elif pulled >= azure / total and (not pre or pre == None):
                carmine -= 1
                total -= 1
                pre = 0
            else:
                pre = None
        if azure == 1:
            lastAzure += 1
        else:
            other += 1
    print("Where there were", ratios, "azure balls.")
    print("Trials where azure was the last ball", lastAzure)
    print("Trials where carmine was the last ball", other)
    print("The realtive frequency: " + str(lastAzure/2000) + "\n")
