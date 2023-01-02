There are two program within in this assignment. The two programs called encode and decode which perform LZ78 compression and decompression, respectively.
The program follows the following requirements:
	
	1. encode can compress any file, text or binary.
	2. decode can decompress any file, text or binary, that was compressed with encode.
	3. Both use variable bit-length codes.
	4. Both perform read and writes in efficient blocks of 4KB.

So the user has the following options for input and outout files when using encode and decode:
	
	1. using command line argument -i and -o to state the file names used.
	2. using io rediration of < and >, to  state the file names used.
	3. or don't state any and use stdin and stdout as input and output

To compile the program using the Makefile use the command: make or make all
        This will compile the program and compile the necesarry files to run encode and decode.
	The user could also just use make encode or make decode to just compile encode

After calling the make or make all command, the following files will be created: encode, decode, encode.o, decode.o, trie.o, word.o, io.o.
	If make encode is called, the following file will be created: encode, encode.o, trie.o, io.o.
	If make decode is called, the following file will be created: decode, decode.o, word.o, io.o.

After these files are created within the directory, decode and encode are ready to run.

To run either encode or decode, type ./encode or ./decode and following arguments to get the desired thing you want the program to do while running.
	
	Encode program supports the following options:
	• -v: Display compression statistics
	• -i <input>: Specify input to compress (stdin by default)
	• -o <output>: Specify output of compressed input (stdout by default)

	Decode supports the following options:
	• -v: Display decompression statistics
	• -i <input>: Specify input to decompress (stdin by default)
	• -o <output>: Specify output of decompressed input (stdout by default)

When running the program using STDIN as input, the user can type any messages, to stop press ctrl+d.

After running the program, to clean the files created, use command: make clean.

If you want to run infer, just run the command make infer.

When running make infer on the programs:
	Infer gives 4 errors of memory leak
        but running valgrind to check for memory leaks 0 were given
        this might be because of the unreliability of infer where often some errors might not be true.

Citation:
        Eugene's lab section on Wednesday 12/9/2020 and 12/2/2020
	Lab Manual
