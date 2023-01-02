This program will do two things: 
	1. Use a sieve to generate a list of prime numbers up to a value n, inclusive.  
		Go through eachprime number and determine whether it is 
			Fibonacci Prime,
			Lucas Prime, 
			or Mersenne Prime.
	2. For each prime number check if its a palindromic prime for the following bases:
			– Base 2
			– Base 9
			– Base 10
			–Base of the first letter of your last name + 10. So 28 since R is the 18th letter in the alphabet

To compile the program use the command: make.
        This will compile the program using the Makefile.

This will compile the program and create the following files: sequence, sequence.o, bv.o, sieve.o.

After compiling the game is ready to run.

To run the program, type ./sequence and following arguments to get the desired thing you want the program to run.
        using the following command line arguments, you can decide which implementation to run.
	
	1. -s:  Print out all primes and identify whether or not they are interesting (Lucas,  Mersenne,Fibonacci).
	2. -p: Print out palindromic primes in bases 2, 9, 10, and first letter of your last name + 10(28).
	3. -n <value>:  Specifies the largest value to consider, inclusively, for your prime sieve.  By de-fault your program runs up through 1000.

If no commmand line is given, the program will give an error.

If the given number of largest value to consider is less than one the program will give an error.

After running the program, to clean the files created, use command: make clean.

Infer and Valgrind
    Infer gave two errors:
        A memory leak at the end of bv_create 
        	This is because infer gives en error when allocated memory isn’t freed.
		But this is done in the function bv_delete so this is most likely a false positive.
        An uninitialized value
		This is true but isn’t really an issue since it isn’t really a big structure we are passing through
    Valgrind gave no error:
	This is why I am not so worried about the infer error since valgrind didn’t report any issues


Citation:
        Eugene's lab section on wednesday(bv functions)
	Lab Manual(sieve function)
	Thursday's lecture(for change base implementation)
