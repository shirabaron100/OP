// C program to demonstrate use of fork() and pipe() 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include "md5.h"
using namespace std;
//This function will be calld as a reaction to catching a signal.
char concat_str[20]; 
int str_len=2;
  
void catchSigT(int sig_num)
 {
  str_len=strlen(concat_str);
    if(str_len==32){
       printf("encrypted by process %d: %s\n",getpid(),concat_str);
       kill(0,SIGKILL);
       exit(0);
    }
    else{
        exit(1);
    }
 }

int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  string  md5Ans;
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 

    char input_str[20]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        puts ("Please enter a string of 20 characters or fewer.");
    scanf("%s", input_str); 
    while(strlen(input_str)>20)
    {
    puts ("Please enter a string of 20 characters or fewer.");
    scanf("%s", input_str); 
    }
  
        close(fd1[0]);  // Close reading end of first pipe 
  
        // Write input string and close writing end of first 
        // pipe. 
        
        write(fd1[1], input_str, strlen(input_str)+1); 
        close(fd1[1]); 
  
        // Wait for child to send a string 
        wait(NULL); 
  
        close(fd2[1]); // Close writing end of second pipe 
  
        // Read string from child, print it and close 
        // reading end. 
        read(fd2[0], concat_str, 100); 
       str_len=strlen(concat_str);

      kill(p,SIGINT);
      catchSigT(SIGINT);
        close(fd2[0]); 
         exit(0);
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipe 
  
        // Read a string using first pipe 
        char concat_str[20]; 
        read(fd1[0], concat_str, 20); 
  
       // Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
      
        // Write concatenated string and close writing end 
                
        md5Ans= md5(concat_str);
        char newChar[md5Ans.length()+1];
        strcpy(newChar,md5Ans.c_str());
        write(fd2[1],newChar,strlen(newChar)+1); 
        close(fd2[1]); 
  
        exit(0); 
    } 
}
