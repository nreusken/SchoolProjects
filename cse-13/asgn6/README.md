This program does the following:

•  Read in a list of non sense words, setting the corresponding bit for each word in the Bloom filter.  
	This list is the oldspeak.txt file.
•  Create a HatterSpeak struct for each forbidden word. 
	The word should be stored in oldspeak, and hatterspeak should be set to NULL; forbidden words do not have translations.

•  Read in a space-separated list of oldspeak, hatterspeak pairs. 
	This list is the hatterspeak.txt file.
•  Create a HatterSpeak struct for each oldspeak, hatterspeak pair, placing them in oldspeak and hatterspeak respectively.
•  The hash index for each non senseword is determined by using oldspeak as the key.

•  Read text from standard input(I/O redirection must be supported).
•  Words that pass through the Bloom filter but have no translation are forbidden, which constitutes a nontalk.

•  The use of nonsense words constitutes a nontalk. If only forbidden words were used, you will send them a nontalk message.
•  The use of only oldspeak words that have hatterspeak translations elicits a hatterspeak message to ensure better speech in the future.
•  The use of nonsense words and words that have hatterspeak translations warrants a combination of a nontalk message and a hatterspeak message.

So when running the program, the user can type in any message and the program will check in any non talk or hatterspeak is used.
If so, the program will give an message and state which words and with translation if there is.

To compile the program using the Makefile use the command: make or make all
        This will compile the program and compile the necesarry files to run the program.

After calling the make command, the following files will be created: hatterspeak, hatterspeak.o, hash.o, bf.o, bv.o, ll.o, speck.o, parser.o, and hs.o.

After these files are created within the directory, the program is ready to run.

To run the program, type ./sorting and following arguments to get the desired thing you want the program to do while running.
        Using the following command line arguments, you can decide length of bloom filter and hash table.
	
	–./hatterspeak -s will suppress the letter from the censor, and instead print the statistics that werecomputed as illustrated below
		*Seeks: number of seeks performed
		*Average seek length: links searched / total seeks
		*Average Linked List Length: average length of non-zero linked lists in hash table
		*Hash table load: percentage of loading for the hash table
		*Bloom filter load: percentage of loading for the bloom table
	–./hatterspeak -h size specifies that the hash table will have size entries (the default will be 10000).
	–./hatterspeak -f size specifies that the Bloom filter will have size entries (the default will be 2^20).
	–./hatterspeak -m will use the move-to-front rule.
	–./hatterspeak -b will not use the move-to-front rule.
	–ANY combination of these flags except for-m -b is be supported.

When running the program, the user can type any messages, to stop press ctrl+d.
Than the program will output if there has been an issue.

If no commmand line arguments are given, the program will run with defualt values and move_to_front set to false.

After running the program, to clean the files created, use command: make clean.

If you want to run infer or valgrind, just run the command make infer or make valgrind.

When running make infer:
	Infer gives 5 errors of memory leak
	but running valgrind to check for memory leaks 0 were given
	this might be because of the unreliability of infer where often some errors might not be true.

Citation:
	Eugene's lab section on Wednesday 11/18/2020 and 11/25/2020:
		The ll_insert and ll_lookup are based from psuedo code give in lab section
		The code for reading text from standard input(I/O redirection must be supported)

