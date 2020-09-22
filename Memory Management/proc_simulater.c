#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include "mem_management.h"

void sim_process(int M, int S, FILE *infile)
{
   int C = rand() % M;
   int D = (C * 4096) + 2048;
   S = S * 500;
   int lower = 512;
   int upper = 4096;
   int L, N1, N2;
   double rand_num1, rand_num2;

   int temp = M;
   while (temp != 0)
   {
        L = (rand() % (upper - lower + 1)) + lower;

        N1 = (int) round(0.9 * (double) L);
        for ( ; N1 != 0; N1--)
        {
             rand_num1 = rand_double();
             rand_num1 = convert_distro(rand_num1, D, S);
             fprintf(infile, "%lf \n", rand_num1);
        }

        C = rand() % M;
        D = (C * 4096) + 2048;

        N2 = (int) round(0.1 * (double) L);
        for ( ; N2 != 0; N2--)
        {
             rand_num2 = rand_double();
             rand_num2 = (double) (D + 1) * rand_num2; 
             fprintf(infile, "%lf \n", rand_num2);
        }
        temp--;
   }
   return;
}

// generates a random number between 0 and 1
double rand_double()
{
   double rand_num = ((double) rand() /(double) RAND_MAX);
   return rand_num;
}

// converts randum number in uniform distro to normal distro 
double convert_distro(double rand_num, int mean, int std_dev)
{
   double rand_num1 = ((rand_num * (double) std_dev) + (double) mean);

   return rand_num1;
}
