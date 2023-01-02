This program will play the game: Tower of Hanoi.

The objective of the game is simple – move all the disks from column A to column B. 
	However thereare two simple rules:
		1.  Only one disk can be moved at a time.
		2.  No larger disk can be placed on top of a smaller disk.

The program will allow you to play the game using two different ways to solve the game.
	
	1. using recursion implementation
	2. using stack implementation

To compile the program use the command: make.
        This will compile the program using the Makefile.
	
This will compile the program and create the following files: tower, tower.o, stack.o.

After compiling the game is ready to run.

To run the program, type ./tower and following arguments to get the desired implementation to the solution of tower of Hanoi.
	using the following command line arguments, you can decide which implementation to run.

	1.  “-n x” : sets the number of disks tox(x is defaulted to 5 if this option isn’t supplied).
	2.  “-s” : print out the moves performed using the stack implementation.
	3.  “-r” : print out the moves performed using the recursive implementation. 

If no commmand line is given, the program will give an error.

If only the set disks is given, the program will give an error.

If the given number of disks is less than one the program will give an error.

After running the program, to clean the files, use comman: make clean.
	This will remove the following files: tower, tower.o, stack.o.


Citation:
	Eugene's office hours on wednesday(Explained recursion and stack implementation)
