/*-------------------------------------------------------------------
File: router.c

Name:
Student Number:

Description: This is a Router designed to foward messages between User and Server.
-------------------------------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

/*-------------------------------------------------------------------
Function: main

Description: The main function processes the command line to get the
             pipes' file descriptors which it stores in a string.
             10 points each function.
-------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // 8. Router connects with User and Server via pipes
    // 9. Router receives any data from User and forwards the data to the Server
    // 10. Router receives any data from Server and forwards the data to the User
   if(argc != 2)
    {
      printf("Usage: router <param: \"int int int int\">\n");
      return 1;
    }
    /*while(true){
      
    read(pipefd1[0], buf, 10); 
    }*/
    return 0;
}
