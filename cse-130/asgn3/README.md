This program does the following:
    It is a httpproxy, which loadbalances and has chaching implemented.
    It will loadbalance between given port numbers.
    Also dealing with exterior request by doing healthrequest to other servers.

    This is a basic implementation of httpproxy, so it only allows for GET requests.
    
    It has one file restriction, it only handles file that are in the same directory, so going to other directories is not possible.
    

To compile the code just call the function "make" in the terminal.

It will create a httpproxy and httpproxy.o RequestHandler.o DataStructs.o files.
  These can be removed by running make clean.
    
To run the program and get the server running type: "./httpproxy port-number port-number1".

    port-number = the port you want the computer to run on. It should be anything above 1024.
                    This will be the portnumber the client will connec to.

    port-number1 = the port number of the server that you want to connect to(could be more than one).
                    This will be the port number that the httpserver is running on.

    Program will throw an error if invalid port number is given.

The code also has the following options:

  -N    requires a number of threads that you want the server the run with
  -R    requires a number for interval for every R requests made to the proxy to do healthchecks on the servers
  -s    requires a number for the amount of files cached at ones/
  -m    requires a number for the maximum size of a file that can be cached

Default values for options:
  -N defaul value is 5
  -R defaul value is 5
  -s defaul value is 3
  -m defaul value is 1024
        
Caching is disabled if either -s or -m options is set to 0 or a negative number.

So before running the command: "./httpproxy port-number port-number1".
You will also need to run: "./httpserver portnumber1 -l log_file" so the proxy can forward its requests.
It will return an error if so.
Logging has to be enabled to succesfull get files since it need
The server is live and running on port-number and feeding the request to portnumber1. 

Now to access the server on portnumeber1 while connecting to portnumber.
It is recommned using curl to send http requests, since it will do the formatting for you

If you want to run a memory check, you can run valgrind.
    It will run httpserver using port number 8080 8081


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