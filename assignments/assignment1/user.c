/*-------------------------------------------------------
File: user.c

Name:
Student Number:

Description: This program is designed to test the severs
             program using pipes.
             Please follow the instructions in the comments
--------------------------------------------------------*/

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <signal.h>

// Utilities

// Time Information Structure
typedef struct TIME_INFO
{
    int hour;
    int minute;
    int second;
}TimeInfo;

// This function would close the pipe
void closePipe(int *pipe)
{
    close(pipe[0]);
    close(pipe[1]);
}

// This function will get current time and store it in TimeInfo struct
TimeInfo getTimeInfo()
{
    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);
    
    TimeInfo timeInfo = {tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec};
    return timeInfo;
}

// This function is used for print the time information storing in the TimeInfo
void printTimeInfo(TimeInfo timeInfo)
{
    printf("%d@%d:%d:%d\n", getpid(), timeInfo.hour, timeInfo.minute, timeInfo.second);
}

// This function is to print the current time
void printTime()
{
    TimeInfo timeInfo = getTimeInfo();
    printTimeInfo(timeInfo);
}

// read function in block mode, same usage as read(int fd, const void *buf, size_t count)
int blockRead(int fd, const void *buf, size_t count)
{
    int readNum = 0;
    for(;readNum < count;)
    {
        char data = 0;
        int retVal = read(fd, &data, sizeof(data));
        if(retVal > 0)
        {
            readNum += sizeof(data);
            *(char *)buf = data;
            (char *)buf++;
        }
    }
    return readNum;
}


// define your parameters here
int queryTimesMax = 1;
int queryInterval = 1;
int serverPrintTimesMax =1 ;
int serverPrintInterval =1 ;

#define QueryCommand 't' // Used for Querying Time
#define QuitCommand 'q'

/*---------------------------------------------------------------
Function: main

Description: Complete the functions listed in the comments.
             Each function is 10 points.

Assignment: Complete this function to ensure proper calls to 
            the other functions.
---------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // You may liek to use the utility functions provided above //
    int pipefd1[2];
    int pipefd2[2];
    int pipefd3[2];
    int pipefd4[2];
    int routerpid, userpid, serverpid, status;
    TimeInfo bufserver[20];
    char bufuser[20];
    //char message1[] = "hello \n"   ;
    //char message2[] = "goodbye \n";
    // Create pipes
	  if (pipe(pipefd1) == -1) /*return the two integrers   */
    {
       printf("Pipe creation failed.\n");  /* Error */
       exit(-1);
    }
    if (pipe(pipefd2) == -1) /* retur n the two integrers   */
    {
       printf("Pipe creation failed.\n");  /* Error */
       exit(-1);
    }
    if (pipe(pipefd3) == -1) /* retur n the two integrers     */
    {
       printf("Pipe creation failed.\n");  /* Error */
       exit(-1);
    }
    if (pipe(pipefd4) == -1) /* retur n the two integrers       */
    {
       printf("Pipe creation failed.\n");  /* Error */
       exit(-1);
    }    
    
    // Create router process
/*	   if (prpid == -1)  /* Error 
    {
        printf("First fork failed.\n");
        exit(-1);
    }else if(serverpid==0){
      close(pipefd1[1]);
      close(pipefd2[0]);
      close(pipefd3[0]);
      close(pipefd4[1]);
      execlp("router",pipefd1[1],pipefd1[1],pipefd1[1],pipefd1[1]);
  */  
  //  }  else{   
    // Create Server process
        // 1. Server process should create a thread to display current time for at least 2 times
    // 2. Server should respond the following command from Router
    //     -"t": send current time to the router
    //     -"q": stop waiting for the command, kill thread and exit.
    // 3. Server should end the process by itself instead of User process.
    serverpid = fork();
    if (serverpid == -1)  /* Error */
    {
        printf("First fork failed.\n");
        exit(-1);
    }else if(serverpid==0){
      
      close(pipefd1[1]);
      close(pipefd2[0]);
      closePipe(pipefd3);
      closePipe(pipefd4);
      printf("Server Pid: %d\n", getpid());
      int count=0,n;
      pthread_t tid;
      pthread_attr_t attr;
      while(n=read(pipefd1[0], bufuser,sizeof(bufuser))>0){
        
        TimeInfo time = getTimeInfo();
        //pthread_create(&tid, &attr, printTimeInfo,(void *) time); //might not work
        //pthread_join(tid,NULL);
        if(bufuser[0]=='t'){
          
          write(pipefd2[1], &time, sizeof(time));
        }
        if(bufuser[0]=='q'){
          
          printf("kill the process");
          break;
        }
        count++;
        if(count>2){
         break;
        }
      }
      close(pipefd1[0]);
      close(pipefd2[1]);  
      }else{
          
    // Create User process
    // 4. User queries time for at least 2 times by sending 't' command to Router
    // 5. Then User sends 'q' command to Router
    // 6. User waits for the Server ending itself
    // 7. User kills the Router process
        close(pipefd1[0]);
        close(pipefd2[1]);
        closePipe(pipefd3);
        closePipe(pipefd4);
        printf("User Pid: %d\n", getpid());
        printf("Start Time: ");
        TimeInfo startTime = getTimeInfo()
        printTimeInfo(startTime);
        char message1[]="t\n";
        char message2[]="t\n";
        char message3[]="q\n";
        write(pipefd1[1], message1, sizeof(message1));
        printf("user message1:%s\n". message1);        
        sleep(2);
        write(pipefd1[1], message2, sizeof(message2));
        printf("user message2:%s\n". message2);
        sleep(2);
        write(pipefd1[1], message3, sizeof(message3));
        printf("user message3:%s\n". message3);
        int t,count=0;
        while(t=read(pipefd2[0],bufserver,sizeof(bufserver))>0){
           printf("recieved time: %d value: %d\n", count,t); //there are two values that get sent
           printTimeInfo(*bufserver);    //but when i write in a loop the second          
           count++;                                          //write doesnt write to the document.
        }
        close(pipefd1[1]);
        close(pipefd2[0]);
        }	

    
    
    return 0;
}
