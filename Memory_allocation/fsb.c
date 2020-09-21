#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue.c>

#define MAXLEN 128
#define NPROCESS 10

typedef struct memory_requests
{
   int pid;
   int arr_time; // arrival time
   int mem;
   int time;
} mem_req;

mem_req req[NPROCESS];
int nblocks;

int main(int argc, char **argv)
{
   if (argc != 4)
   {
        fprintf(stderr, "Usage: %s file Memory(GB) blocksize(MB) /n", argv[0]);
	exit (1);
   }

   int memory = atoi(argv[2]);
   int block_size = atoi(argv[3]);
   
   nblocks = (memory * 1024)/(block_size);

   char line[MAXLEN];
   char *ptr = NULL;
   FILE *fp = NULL;

   if ((fp = fopen(argv[1], "r")) == NULL)
   {
        fprintf(stderr, "Error opening file %s \n", argv[1]);
	exit (2);
   }

   int i = 0;
   while (fgets(line, MAXLEN, fp) != NULL)
   {
        ptr = strtok(line, ",");
       	req[i].pid = atoi(ptr);

	ptr = strtok(NULL, ",");
	req[i].arr_time = atoi(ptr);

	ptr = strtok(NULL, ",");
	req[i].mem = atoi(ptr);

	ptr = strtok(NULL, ",");
	req[i].time = atoi(ptr);
        /*
	printf("%d %d %d %d\n", req[i].pid, req[i].arr_time, 
			req[i].mem, req[i].time);*/
	i++;
   } 
 
   exit (0);
}
