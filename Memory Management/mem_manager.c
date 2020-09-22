#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mem_management.h"

void mem_manager(FILE *infile, int F, FILE *outfile)
{
   int base;
   double ar;
   int P, X;
   int phys_mem[F];
   int pos;
   int npf = 0;
   initialize_array(phys_mem, F);

   fseek(infile, 0L, SEEK_SET);
   while (fscanf(infile, "%lf", &ar) != EOF)
   {
        int log_addr = (int) ar;
        int phys_addr;

	P = floor(log_addr / 4096);
	X = log_addr % 4096;

        int page_fault;
        int frame_found;
        
        base = search_page(P, phys_mem, F);

        if (base >= 0)
        {
             page_fault = 0;
             phys_addr = (base * 4096) + X;
        }
        else // page not found
        {
             page_fault = 1;
             npf++;
             frame_found = free_frame(phys_mem, F);
             if (frame_found >= 0)
             {
                  phys_mem[frame_found] = P;
                  phys_addr = (frame_found * 4096) + X;
             }
             else 
             {
                  printf("page faults : %d\n", npf);     
                  fprintf(stderr, "Memory Full \n");
                  return ;
             }
        }
        fprintf(outfile, "%d, %d, %d \n", log_addr, phys_addr, page_fault); 
   }
   int k;
   printf("page faults : %d\n", npf);     
   printf("phys_mem\n");
   for (k = 0; k < F; k++)
       printf("%d\n", phys_mem[k]);
   return;
}

void initialize_array(int phys_mem[], int F)
{
   int i; 
   for (i = 0; i < F; i++)
       phys_mem[i] = -1;

   return;
}

// searches for a free frame in the physical memory
int free_frame(int phys_mem[], int F)
{
   int i;
   for (i = 0; i < F; i++)
   {
        if (phys_mem[i] == -1)
            return i;
        else
            continue;
   }
   if (i == F)
       return -1;
}

// searches for the given page in the physical memory
int search_page(int P, int phys_mem[], int F)
{
   int i;
   for (i = 0; i < F; i++)
   {
        if (phys_mem[i] == P)
            return i;
        else 
           continue;
   }
   if (i == F)
       return -1;
}
