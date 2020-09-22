#include <stdio.h>
#include "mem_manager.h"
#include "page_replacement.h"

int page_fifo_global(void)
{
   int temp = first_frame;
   first_frame++;
   
   if (first_frame >= F)
       first_frame %= F;

   return temp;  
}

int page_fifo_local(int first_frame_table[], int pid)
{
   int i = 0, j = 0;
   while (i != F)
   {
        if (phys_mem[i].pid == pid)
        {
             if (phys_mem[i].page_replaced == j)
             {
                  phys_mem[i].page_replaced++;
                  break;
             }
        }
        i++;

        if (i == F)
        {
             j++;
             i = 0;
        }
   }

   first_frame_table[pid-1] = i;

   return first_frame_table[pid-1];   
}
