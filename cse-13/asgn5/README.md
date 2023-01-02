This program will run 4 different sorting implementations using a random generated list.
	The following soritn algorithms will be used:
		1. Bubble Sort
		2. Shell Sort
		3. Quick Sort
		4, Bubble Insertion Sort
	The program will allow you to change:
		1. The lenght of the list of numbers.
		2. The random seed used to generate random numbers for the list.
		3. The amount of first elements of the array that are printed.

To compile the program using the Makefile use the command: make or make all
	This will compile the program and compile the necesarry files to run the program.

After calling the make command, the following files will be created: sorting, sorting.o, bubble.o, shell.o, quick.o, and bubble.o.

After these files are created within the directory, the program is ready to run.

To run the program, type ./sorting and following arguments to get the desired thing you want the program to run.
        Using the following command line arguments, you can decide which sort algorithm to run, and edit the random number list and what is printed.
		1. -A means employ all sorting algorithms.
		2. -b means enable Bubble Sort.
		3. -s means enable Shell Sort.
		4. -q means enable Quick Sort.
		5. -i means enable Binary Insertion Sort.
		6. -p nmeans print the firstnelements of the array.  
			However if the -p n flag is not specified, the default n value is 100.
		7. -r s means set the random seed to s. 
			The default s value is 8222022.
		8. -n c means set the array size to c. 
			The default c value is 100.

If no commmand line is given, the program will run all algorithms wiht defualt values.
	
If the given number for list length is less than one the program will give an error.

After running the program, to clean the files created, use command: make clean.

If you want to run infer or valgrind, just run the command make infer or make valgrind.

When running make infer.
	An error pops up, stating that there is a memory leak
	
	But when running valgrind to check if there is an actual memory leak.
		The program returns that there are no possible memory leak.
	
	Most likely because infer is still unreliable sometimes.

Citaion:
	The psuedocode for all the sorting algorithms are from the lab manual(Bubble sort, shell sort, quick sort, bubble insertion sort).


