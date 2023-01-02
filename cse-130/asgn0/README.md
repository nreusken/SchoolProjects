This program does the following:
    Reads the first n number of lines of the the files being given.

    A line is defined for this purpose as a sequence of characters ending on a newline (\n) character and not including any other newline before that.

    The program can take in as many files as possibles.

    By using - it can also read from standard input.

To compile the code just call the function "make" in the terminal.

It will create a shoulders and shoulders.o file, these can be removed by running make clean.
    
To run the code type: "./shoulder n filename".

    n = the first n number of lines that want to be read from the file.
        It only takes positive numbers as valid arguments.

    filename = is the wanted file to to be read.
        File has to be in same directory, unless you specify file location.
        If you want to read from standard input, use '-'
        You can also use file redirection to input files like < and >.
    
    Program will throw an error if invalid number or file is given.

