#include "bv.h"
#include "sieve.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//function sets all fibonacci numbers to 1 in the bitvector
void fib(BitVector *v) {
  //sets n = 0 and m = 1 and next = 1.
  uint32_t n = 0;
  uint32_t m = 1;
  uint32_t next = 1;
  //sets 0 and 1 in the betvector.
  bv_set_bit(v, n);
  bv_set_bit(v, m);
  //while loop to go through the fibonacci numbers since the next number is always the two previous numbers added.
  while (next < bv_get_len(v)) {
    bv_set_bit(v, next);
    next = n + m;
    m = n;
    n = next;
  }
  return;
}

//functions sets all lucas numbers to 1 in the bitvector.
void lucas(BitVector *v) {
  //set n = 1 and m = 2 and next = 1.
  uint32_t n = 1;
  uint32_t m = 2;
  uint32_t next = 1;
  //sets 0 and 1 in the bitvector.
  bv_set_bit(v, n);
  bv_set_bit(v, m);
  //while loop to go through the lucas numbers since the next number is always the two previous numbers added.
  while (next < bv_get_len(v)) {
    bv_set_bit(v, next);
    next = n + m;
    m = n;
    n = next;
  }
  return;
}

//functions sets all mersenne numbers to 1 in the bitvector.
void mersenne(BitVector *v) {
  //sets x = 4
  uint32_t x = 4;
  //sets 1 in the bitvector.
  bv_set_bit(v, 1);
  //while loop to go through the mersenne numbers since the mersenne numbers all are (2^n)-1.
  while ((x - 1) < bv_get_len(v)) {
    bv_set_bit(v, (x - 1));
    x *= 2;
  }
  return;
}

//function changes the base from 10 to given base for number i and checks if it is palindrome.
//psuedocode for the palindrome checker is from the lab manual.
void change_base_palin_check(uint32_t base, uint32_t i) {
  //sets digits array to possible numebers or letters for base change
  //idea from Maxwell Dunnes' presenation on 11/5/2020.
  char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  //sets array new, this is where the changed base number is going to be.
  char new[10000];
  //sets temp = i, so we don't lose value i.
  uint32_t temp = i;
  //sets x t0 1 as a pointer when parsing through the array new.
  int x = 0;
  //while loop converting the bases, idea from Maxwell Dunnes' presenation on 11/5/2020.
  //the base chane will put the number backwards but this doesn't matter because a palindrome should be the same backwards
  while (temp != 0) {
    new[x] = digits[temp % base];
    x += 1;
    temp /= base;
  }
  //decreases x by 1 to set pointer to last position of the number.
  x -= 1;
  //psuedocode for the palindrome checker is from the lab manual.
  //set palin to true
  bool palin = true;
  //loops through the first half of number.
  for (int y = 0; y <= (x / 2); y += 1) {
    //compares if the first half to the second half.
    //if not the same meaning it isn't a palindrome, set palin to false.
    if (new[y] != new[x - y]) {
      palin = false;
    }
  }

  //if palin is still true.
  if (palin) {
    //print number i
    printf("%d = ", i);
    //than print array backwards since the number is backwards
    while (x >= 0) {
      printf("%c", new[x]);
      x -= 1;
    }
    printf("\n");
  }
  return;
}

//Main function which mainly calls the other functions.
int main(int argc, char **argv) {
  int c = 0;

  //sets max to 1000 which is the default length for number of primes.
  uint32_t max = 1000;

  //sets bools for getopt loop.
  bool all_primes = false;
  bool pal_primes = false;

  //checks what command line arguments are given.
  while ((c = getopt(argc, argv, "n:sp")) != -1) {
    switch (c) {
    //if -n is given, set the x after -n to max.
    case 'n':
      sscanf(optarg, "%d", &max);
      break;
    //if -s is given set all_primes true.
    case 's':
      all_primes = true;
      break;
    //if - is gives set pal_primes true.
    case 'p':
      pal_primes = true;
      break;
    }
  }
  //if no arguments given, return an error.
  if (argc == 1) {
    puts("Error: no  arguments  supplied!");
    return -1;
  }
  //if give max is less than 1 return an error.
  if (max < 1) {
    puts("Error: the number of disks is too low!");
    return -1;
  }
  //creates prime_numbers vector
  BitVector *prime_numbers = bv_create(max);
  //finds all the primes inside the vector.
  sieve(prime_numbers);

  //if all_primes is true meaning print out all primes and state whether or not they are interesting.
  if (all_primes) {
    //creates 3 vectors for each list of numbers.
    BitVector *fib_numbers = bv_create(max);
    BitVector *lucas_numbers = bv_create(max);
    BitVector *mers_numbers = bv_create(max);

    //finds all the fibonacci numbers inside the vector.
    fib(fib_numbers);
    //finds all the lucas numbers inside the vector.
    lucas(lucas_numbers);
    //finds all the mersenne numbers inside the vector.
    mersenne(mers_numbers);

    //loops through the prime numbers to see which ones are prime.
    for (uint32_t i = 0; i < bv_get_len(prime_numbers); i += 1) {
      //if prime print the number and prime.
      if (bv_get_bit(prime_numbers, i)) {
        printf("%d: prime", i);
        // if i is a mersenne number print mersenne behind the number.
        if (bv_get_bit(mers_numbers, i)) {
          printf(", mersenne");
        }
        // if i is a lucas number print lucas behind the number.
        if (bv_get_bit(lucas_numbers, i)) {
          printf(", lucas");
        }
        // if i is a fibonacci number print fibonacci behind the number.
        if (bv_get_bit(fib_numbers, i)) {
          printf(", fibonacci");
        }
        printf("\n");
      }
    }
    //frees up the allocated vectors to prevent memory leak.
    bv_delete(fib_numbers);
    bv_delete(lucas_numbers);
    bv_delete(mers_numbers);
  }
  //adds a blank space between the two if both are true
  if (all_primes && pal_primes) {
    printf("\n");
  }
  //if pal_primes is true print primes in palindromic primes in bases 2, 9, 10, 10+last name initial(r = 18th number in alphabet)= 28.
  if (pal_primes) {
    //sets base 2.
    uint32_t base = 2;
    //prints the base header.
    printf("Base  %d\n", base);
    printf("---- --\n");
    //iterates over the prime_number vector to see what numbers are prime.
    for (uint32_t i = 0; i < bv_get_len(prime_numbers); i += 1) {
      //if prime meaning it is prime call change_base_palin function to see if palindromic in base.
      if (bv_get_bit(prime_numbers, i)) {
        change_base_palin_check(base, i);
      }
    }

    //sets base 9.
    base = 9;
    //prints the base header.
    printf("\nBase  %d\n", base);
    printf("---- --\n");
    //iterates over the prime_number vector to see what numbers are prime.
    for (uint32_t i = 0; i < bv_get_len(prime_numbers); i += 1) {
      //if prime meaning it is prime call change_base_palin function to see if palindromic in base.
      if (bv_get_bit(prime_numbers, i)) {
        change_base_palin_check(base, i);
      }
    }

    //sets base 10.
    base = 10;
    //prints the base header.
    printf("\nBase %d\n", base);
    printf("---- --\n");
    //iterates over the prime_number vector to see what numbers are prime.
    for (uint32_t i = 0; i < bv_get_len(prime_numbers); i += 1) {
      //if prime meaning it is prime call change_base_palin function to see if palindromic in base.
      if (bv_get_bit(prime_numbers, i)) {
        change_base_palin_check(base, i);
      }
    }

    //sets base 28.
    base = 28;
    //prints the base header.
    printf("\nBase %d\n", base);
    printf("---- --\n");
    //iterates over the prime_number vector to see what numbers are prime.
    for (uint32_t i = 0; i < bv_get_len(prime_numbers); i += 1) {
      //if prime meaning it is prime call change_base_palin function to see if palindromic in base.
      if (bv_get_bit(prime_numbers, i)) {
        change_base_palin_check(base, i);
      }
    }
  }
  //frees up the bitvector prime_numbers
  bv_delete(prime_numbers);
  return 0;
}
