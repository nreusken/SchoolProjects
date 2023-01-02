from random import random

probabilities = input(
    "Input the probability of throwing heads or enter for whole simulation:")
if len(probabilities) == 0:
    probabilities = [0.5, 0.2, 0.3, 0.4, 0.6, 0.7, 0.8]
else:
    probabilities = [float(probabilities)]


print("Probabilities: " + str(probabilities))


list_of_n = [5, 10, 50, 100]

for p in probabilities:
    for n in list_of_n:
        wins_for_bob = 0
        for i in range(1000):
            alice_heads = 0
            # alice throwing
            for j in range(n):
                throw = random()
                if throw < p:
                    alice_heads += 1

            bob_heads = 0
            # bob throwing
            for j in range(n+1):
                throw = random()
                if throw < p:
                    bob_heads += 1
            if bob_heads > alice_heads:
                wins_for_bob += 1

        print("Where p = " + str(p) + " and n = " + str(n))
        print("\tBob tossed more heads in " + str(wins_for_bob) + " trials")
        print("\tThe realtive frequency: " + str(wins_for_bob/1000) + "\n")
