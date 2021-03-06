#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mem_manager.h"
#include "page_replacement.h"

int mem_mapper(FILE *infile, FILE *outfile, int pid, int choice)
{
   int base;
   char buf[MAXLEN];
   int P, X;
   int l = 512, u = 4096;
   int A = (int) rand_uniform(u, l);
   int log_addr, phys_addr;
   int page_fault;
   int free_frame;
   
   while (A != 0)
   {
        fscanf(infile, "%[^\n]", buf);

        fgetc(infile); // reads '\n'

        if (feof(infile))
            return 0;

        log_addr = atoi(buf);

        P = (int) (log_addr / 4096);
        X = log_addr % 4096;

        base = is_page(P, pid);
        if (base >= 0)
        {
             page_fault = 0;
             phys_addr = (base * 4096) + X;
        }
        else // page not found
        {
             page_fault = 1;     
             free_frame = get_free_frame();
             if (free_frame >= 0)
             {
                  phys_mem[free_frame].page_no = P;
		  phys_mem[free_frame].pid = pid;
                  phys_addr = (free_frame * 4096) + X;
             }
             else 
             {
                  if (choice == 1)
                  {
                       oldest_frame = page_fifo_global();
                       phys_mem[oldest_frame].page_replaced++;
                  }
		  else if (choice == 2)
                  {
                       oldest_frame = page_fifo_local(first_frame_table, pid);
                       // print_phys_mem();  
                  }  
                  // printf("page replaced in frame %d \n", oldest_frame);
                  phys_mem[oldest_frame].page_no = P;
                  phys_mem[oldest_frame].pid = pid;
                  phys_addr = (oldest_frame * 4096) + X;
             }
        }
        fprintf(outfile, "%d, %d, %d \n", log_addr, phys_addr, page_fault);
        A--;
   }
   return 1;
}

// searches for a free frame in the physical memory
int get_free_frame(void)
{
   int i;
   for (i = 0; i < F; i++)
   {
        if ((phys_mem[i].page_no == -1) && (phys_mem[i].pid == -1))
            return i;
   }
   return -1;
}

// searches for the given page in the physical memory
int is_page(int P, int pid)
{
   int i;
   for (i = 0; i < F; i++)
   {
        if ((phys_mem[i].page_no == P) && (phys_mem[i].pid == pid))
            return i;
   }
   return -1;
}
