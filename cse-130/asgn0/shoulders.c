#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<err.h>

int main(int argc, char** argv){
        
    //checks if there are any arguments given if not throws an error
    if(argc == 1){
        warnx("option requires an argument -- 'n'");
        return 0;
    }

    //converst the string number to an integer
    int lines = atoi(argv[1]);
    //converts the integer back to a string to compare if they are the same
    char number[strlen(argv[1])];
    sprintf(number, "%d", lines);
    
    //compares if they are the same and if it is not an negative number if so throws and error
    if(strcmp(number, argv[1]) != 0 || lines < 0){
        fprintf(stderr, "shoulders: invalid number of lines: ’%s’.\n", argv[1]);
        return 0;
    }

    //sets input file to standard out if no file is given or using when using < to direct files
    if(argc == 2){
        argc++;
        argv[2] = "-";
    }
    
    
    //loops through all the files given
    for(int i = 2; i < argc; i++){
        //if statement to either open the given file or set it to Standard Input
        int file;
        if(strcmp(argv[i], "-") != 0){
            file = open(argv[i], O_RDONLY);
        }
        else{
            file = STDIN_FILENO;
        }
        
        //checks if open() was able to open the file
        if(file == -1){
            warn("cannot open '%s' for reading", argv[i]);
            continue;
        }

        //loops till the right amount of lines are read
        for(int j = 0; j < lines; j){
            //sets buffer and temp_buffer
            char* buffer = (char *)calloc(1000, sizeof(char));
            char* temp_buffer = (char *)calloc(1000, sizeof(char));
            //reads the file to buffer
            int temp = read(file, buffer, 1000);
            //checks if the end of file has been read
            if(temp == 0){
                //free the buffers and breaks the loop
                free(buffer);
                free(temp_buffer);
                break;
            }
            //loops through the buffer looking for \n
            for(int i = 0; i < 1000; i++){
                //sets the byte from buffer to temp buffer
                temp_buffer[i] = buffer[i];
                //if there is a \n in buffer increment j
                if(buffer[i] == '\n'){
                    j++;
                }
                //if the asked number of \n have been read write to standard out and break loop
                if(j == lines){
                    write(STDOUT_FILENO, temp_buffer, i+1);
                    break;
                }
            }
            //if end of file has been reached but there are still bytes in buffer write to standard out 
            if(j < lines){
                write(STDOUT_FILENO, buffer, temp);
            }
            //frees the buffer and temp_buffer
            free(buffer);
            free(temp_buffer);
        }
        //closes the current file
        close(file);
    }
    //returns to end program
    return 0;

}
