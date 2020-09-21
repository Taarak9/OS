#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

// FUNCTION DECLARATIONS
static void tell_child(int signo);
static void tell_parent(int signo);

// GLOBAL VARIABLES
int parent_done = 0;
int child_done = 0;

//MAIN STARTS HERE
int main(void)
{
   int pipefd[2];
   pid_t cpid; // child pid
  
   // creates a pipe
   if (pipe(pipefd) == -1)
   {
        fprintf(stderr, "pipe error. \n");
        exit (1);
   }

   // forking
   if ((cpid = fork()) < 0)
   {
        fprintf(stderr, "fork error. \n");
        exit (2);
   }
   else if (cpid > 0) // parent
   {
        // close the read fd of parent
        close(pipefd[0]);

        // I mean in a new line enter control D (EOF)
        printf("End the input by pressing Enter and Control D \n"); 

        // If anything is done at stdout it replicates it in pipefd[1]
        dup2(pipefd[1], 1);
        
        char buf1;

        // reads from stdin (one character at a time)
        while (read(0, &buf1, 1) > 0)
        {
             // if the character is in lower case
             if (islower(buf1))
             {
                  // converts to uppercase
                  buf1 = toupper(buf1);
             }

             // writes to stdout (one character at a time)
             write(1, &buf1, 1);
        }
         
        // closes the write fd of the parent
        close(pipefd[1]);

        if (parent_done == 0)
        {
             // Sends SIGUSR1 signal to the child
             kill(cpid, 10);
        }

        // when SIGUSR2 signal is caught, tell_parent handler is performed
        signal(12, tell_parent); 
        
        //for ( ; ; )
        //{
             // if child is done, send SIGTERM to the child to terminate it
             if (child_done == 1) 
			 {
                  kill(cpid, SIGTERM);
          //        exit (0);
             }
        //}
        exit (0);
   }
   else // child
   {
        // closes the read fd of the child
        close(pipefd[1]);
        // If anything is done at stdin it replicates it in pipefd[0]
        dup2(pipefd[0], 0);
        
        // If SIGUSR signal is caught, sig_usr1 handler is performed 
        signal(10, tell_child);
        
        // to get the parent pid
        pid_t ppid = getppid();

        char buf2;
        for ( ; ; )
        {
             if (parent_done == 1) // parent done
             {
                  // number of lines
                  int nline = 0;
                  // reads from STDIN (one character at a time) 
                  while(read(0, &buf2, 1) > 0)
                  {
                       // when enter key is pressed
                       if (buf2 != 10)
                       {
                            // writes all the buffer other than the enter char
                            write(1, &buf2, 1); 
                       }
                       else
                       {
                            nline++;
                            printf(" %d", nline);
                            // to flush the buffer stored by printf
                            fflush(stdout);
                            // writes the newline
                            write(1, &buf2, 1);
                       } 
                  }
                  break;
             }
        }
        if (child_done == 0)
        {
             // sends SIGUSR2 signal to the parent
             kill(ppid, 12);
        }
        // closes the read fd of the child
        close(pipefd[0]);
   }
}

// FUNCTION DEFINITIONS
static void tell_child(int signo)
{
   parent_done = 1;
   return;
}

static void tell_parent(int signo)
{
   child_done = 1;
   return;
}
