#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Returns  the  position  of the  player  to the  left.
//pos:      The  position  of the  current  player.
//players: number  of  players  in the  game.
uint32_t left(uint32_t pos, uint32_t players) {
  return ((pos + players - 1) % players);
}

//Returns  the  position  of the  player  to the  right.
//pos:      The  position  of the  current  player.
//players: The  number  of  players  in the  game.
uint32_t right(uint32_t pos, uint32_t players) {
  return ((pos + 1) % players);
}

int main() {
  //all integers for program intialized
  uint32_t seed, random, players, pos, pot, check, total_hand;

  //char created for the eventual winner of the game
  const char *winner[1];

  //check, pos, pot set to 0
  check = 0;
  pos = 0;
  pot = 0;

  //asks user to input Random seed and amount of players
  printf("Random seed: ");
  scanf("%d", &seed);

  printf("How many players? ");
  scanf("%d", &players);

  //if the input amount of players by the user is not within 0-10 than it will got through the checker
  //the program will infitely ask for the right input untill it is within range
  if ((2 > players) || (players > 10)) {
    while (1) {
      printf("Error, wrong input, try again: ");
      scanf("%d", &players);
      if ((1 < players) && (players <= 10)) {
        break;
      }
    }
  }

  //creates the hand, die, and name arrays for the program
  uint32_t hand[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

  typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
  faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

  const char *names[] = { "Happy", "Sleepy", "Sneezy", "Dopey", "Bashful",
    "Grumpy", "Doc", "Mirror Mirror", "Snow White", "Wicked Queen" };

  srand(seed); // This  sets  your  random  seed to seed.

  //Start of the infite while loop to go through all the players till someone is the winner
  while (1) {
    //determines whether the player has 3 or more in their hand
    if (hand[pos] >= 3) {
      //prints who is rolling
      printf("%s rolls...", names[pos]);
      //Rolls the die three times
      for (int i = 1; i <= 3; i = i + 1) {
        printf(" ");
        random = rand() % 6; //sets random to a random number between 0-5
        //if statement if rolled left
        if (random == die[0]) {
          hand[pos] = hand[pos] - 1;
          hand[left(pos, players)] += 1;
          printf("gives $1 to %s", names[left(pos, players)]);
        }
        //if statement if rolled right
        else if (random == die[1]) {
          hand[pos] = hand[pos] - 1;
          hand[right(pos, players)] += 1;
          printf("gives $1 to %s", names[right(pos, players)]);
        }
        //if statement if rolled center
        else if (random == die[2]) {
          hand[pos] = hand[pos] - 1;
          pot += 1;
          printf("puts $1 in the pot");
        }
        //else statement because else must be a pass
        else {
          printf("gets a pass");
        }
      }
      printf("\n");
    }
    //determines whether the player has 2 dollars in their hand
    else if (hand[pos] == 2) {
      //prints who is rolling
      printf("%s rolls...", names[pos]);
      //rolls die twice
      for (int i = 1; i <= 2; i = i + 1) {
        printf(" ");
        random = rand() % 6; //sets random to a random number between 0-5
        //if statement if rolled left
        if (random == die[0]) {
          hand[pos] = hand[pos] - 1;
          hand[left(pos, players)] += 1;
          printf("gives $1 to %s", names[left(pos, players)]);
        }
        //if statement if rolled right
        else if (random == die[1]) {
          hand[pos] = hand[pos] - 1;
          hand[right(pos, players)] += 1;
          printf("gives $1 to %s", names[right(pos, players)]);
        }
        //if statement if rolled center
        else if (random == die[2]) {
          hand[pos] = hand[pos] - 1;
          pot += 1;
          printf("puts $1 in the pot");
        }
        //else statement because else must be a pass
        else {
          printf("gets a pass");
        }
      }
      printf("\n");

    }
    //determines whether the player has a dollar in their hand if the players has no money they don't roll
    else if (hand[pos] == 1) {
      //prints who is rolling
      printf("%s rolls...", names[pos]);
      //rolls dice once
      for (int i = 1; i <= 1; i = i + 1) {
        printf(" ");
        random = rand() % 6; //sets random to a random number between 0-5
            //if statement if rolled left
        if (random == die[0]) {
          hand[pos] = hand[pos] - 1;
          hand[left(pos, players)] += 1;
          printf("gives $1 to %s", names[left(pos, players)]);
        }
        //if statement if rolled right
        else if (random == die[1]) {
          hand[pos] = hand[pos] - 1;
          hand[right(pos, players)] += 1;
          printf("gives $1 to %s", names[right(pos, players)]);
        }
        //if statement if rolled center
        else if (random == die[2]) {
          hand[pos] = hand[pos] - 1;
          pot += 1;
          printf("puts $1 in the pot");
        }
        //else statement because else must be a pass
        else {
          printf("gets a pass");
        }
      }
      printf("\n");
    }

    check = 0;

    //goes throught the hand array to see how many people have moneny left
    for (uint32_t i = 0; i < players; i = i + 1) {
      if (hand[i] != 0) {
        check += 1;
        winner[0] = names[i];
        total_hand = hand[i];
      }
    }
    //if only one person has money left than there is a winner and while loop will break
    if (check == 1) {
      break;
    }
    //if the position is equal to players minnus 1(minus 1 becaue pos starts at 0)
    //pos will be reset to 0 to restart from the begennin of players
    //else pos will be increased to go to next player
    if (pos == (players - 1)) {
      pos = 0;
    } else {
      pos += 1;
    }
  }
  //prints the winner and how much left in pot and hand
  printf("%s wins the $%d pot with $%d left in the bank!\n", winner[0], pot,
      total_hand);
  return 0;
}
