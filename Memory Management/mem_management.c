#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_management.h"

#define MAXLEN 128

int main(int argc, char *argv[])
{
   if (argc != 6)
   {
        fprintf(stderr, "Usage: %s M S F infile outfile \n", argv[0]);
        exit (1);
   }

   // no of pages
   int M = atoi(argv[1]); 
   // behaviour of the program
   int S = atoi(argv[2]);
   // no of frames
   int F = atoi(argv[3]);

   FILE *infile = NULL, *outfile = NULL;

   if ((infile = fopen(argv[4], "w+")) == NULL)
   {
        fprintf(stderr, "Error opening %s \n", argv[4]);
        exit (2);
   }

   if ((outfile = fopen(argv[5], "w+")) == NULL)
   {
        fprintf(stderr, "Error opening %s \n", argv[5]);
        exit (3);
   }
   
   // process simulation
   sim_process(M, S, infile);
   // memory manager
   mem_manager(infile, F, outfile);

   char line[MAXLEN];
   char *ptr = NULL;
   int npf = 0, nar = 0;
   int page_fault;
   // file pointer is set to the begining of the file
   fseek(outfile, 0L, SEEK_SET);
   while (fgets(line, MAXLEN, outfile) != NULL)
   {
        ptr = strtok(line, ",");
        ptr = strtok(NULL, ",");
        ptr = strtok(NULL, ",");

        page_fault = atoi(ptr);

        if (page_fault == 1)
            npf++;
        nar++;
   }

   double pfr;
   pfr = (double) npf / (double) nar;

   printf("Page fault ratio : %lf \n", pfr);

   // Effective access time
   // Let in-memory access time = t
   // eat = (1-p)*t + p*(avg page fault time)
   double eat;

   eat = ((double) 1 - pfr) + (pfr * (double) 100);

   // Slow-down = (eat - t)/t
   double slow_down;

   slow_down = eat - (double) 1;

   printf("Slow-down : %lf \n", slow_down);

   fclose(infile);
   fclose(outfile);
   exit (0);
}
