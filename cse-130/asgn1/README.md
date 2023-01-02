This program does the following:
    It is a basic single threaded server.

    It takes the three basic requests:
        GET
        HEAD
        PUT
    
    It has one file restriction, it only handles file that are in the same directory, so going to other directories is not possible.
    

To compile the code just call the function "make" in the terminal.

It will create a httpserver and httpserver.o file, these can be removed by running make clean.
    
To run the program and get the server running type: "./httpserver port-number".

    port-number = the port you want the computer to run on. It should be anything above 1024.

    Program will throw an error if invalid port number is given.

So after running the command: "./httpserver port-number".
The server is live and running. 

Now to access the server, it is recommned using curl to send http requests.
Since it will do the formatting for you

If you want to run a memory check, you can run valgrind.
    It will run httpserver using port number 8080


The server has the following HTTP codes implemented:  

    200 OK 
        To be used when a response can be responded to successfully, if no other code is more appropriate;
    201 Created 
        To be used if a successful response resulted in the creation of a file – note that PUT can be used to create a file, but it can also overwrite a file;
    400 Bad Request 
        To be used when a request cannot be parsed correctly, or it has problems that result in failure to respond, and no other code is more appropriate;
    403 Forbidden 
        To be used if the request is for a valid resource name, but the server does not have the permissions to access it;
    404 File Not Found 
        To be used the request is for a valid resource name, but the server cannot find it (e.g., the file doesn’t exist);
    500 Internal Server Error 
        To be used if the request is valid but the server cannot comply because of internal problems.
        (e.g., error when allocating memory for a datastructure that would necessary to process the request), and no other response code is appropriate;
    501 Not Implemented 
        To be used if the request is properly formatted, but uses a request type that the server has not implemented (that is, not GET, PUT, or HEAD).