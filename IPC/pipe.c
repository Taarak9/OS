#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// CONSTANT DEFINITIONS
// maximum length of a line
#define MAXLEN 128

// MAIN STARTS HERE
int main(int argc, char **argv)
{
   if (argc != 2)
   {
        fprintf(stderr, "Usage : %s filename \n", argv[0]);
        exit (1);
   }

   int pipefd[2];
   pid_t pid;

   // creates a pipe
   if (pipe(pipefd) == -1)
   {
        fprintf(stderr, "pipe error \n");
        exit (2);
   }
   
   // forking
   if ((pid = fork()) < 0)
   {
        fprintf(stderr, "fork error \n");
        exit (3);
   }
   else if (pid > 0) // parent
   {
        // closes the read fd of the parent 
	close(pipefd[0]);
	
        int fd;
        int i = 0;
        char line[MAXLEN];

        // opens the file in read mode
        if ((fd = open(argv[1], O_RDONLY)) == -1)
        {
             fprintf(stderr, "file error \n");
             exit (4);
        }   

        // reads buffer from file (one character at a time)
        while(read(fd, &line[i], 1) > 0)  
        {
             i++;
             if (line[i-1] == '\n')
             {
                  line[i] = '\0';
                  // writes buffer into the pipe (one line at a time)
                  write(pipefd[1], line, strlen(line)); 
                  i = 0;
             }
        }
        
        // closes the file descriptor of the file
        close(fd);
        // closes the write fd of the parent
        close(pipefd[1]);

        // waits for the child to complete
        waitpid(pid, NULL, 0);
        exit (0);
   }
   else // child
   {
        // closes the write fd of the child
        close(pipefd[1]);
        
        char buf;
		
        // reads buffer from the pipe (one character at a time)
        while (read(pipefd[0], &buf, 1) > 0)
            // writes into STDOUT (one character at a time) 
            write(1, &buf, 1);

        // closes the read fd of the child
        close(pipefd[0]);
   }
   exit (0);
}
