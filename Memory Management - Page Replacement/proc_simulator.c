#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mem_manager.h"

int sim_process(int M, int sigma, FILE *outfile)
{
   int C = rand() % M;
   int D = (C * 4096) + 2048;
   int lower = 512;
   int upper = 4096;
   int L, N1, N2;
   int rand_num1, rand_num2;
   int nar = 0;

   sigma = (sigma * 500) + 1;

   int temp = M;
   while (temp != 0)
   {
        L = (rand() % (upper - lower + 1)) + lower;

        N1 = (int) round(0.9 * (double) L);
        nar += N1;
        for ( ; N1 != 0; N1--)
        {
             rand_num1 = (int) rand_normal(D, sigma);
             if (rand_num1 > (M-1)*4096)
                 rand_num1 = (M-1)*4096;
             fprintf(outfile, "%d\n", rand_num1);
        }

        C = rand() % M;
        D = (C * 4096) + 2048;

        N2 = (int) round(0.1 * (double) L);
        nar += N2;
        for ( ; N2 != 0; N2--)
        {
             rand_num2 = rand_uniform(0, D);
                    
             if (rand_num2 > (M-1)*4096)
                 rand_num2 = (M-1)*4096;
             fprintf(outfile, "%d\n", rand_num2);
        }
        temp--;
   }
   return nar;
}

// generates a random number between 0 and 1
double rand_double(void)
{
   double rand_num = ((double) rand() /(double) RAND_MAX);
   return rand_num;
}

// converts randum number in uniform distro to normal distro 
double rand_normal(int mean, int sigma)
{
   double rand_num = rand_double();
   double rand_num1 = (rand_num * (double) sigma) + (double) mean;

   return rand_num1;
}

double rand_uniform(int l, int u)
{
   double X, U1;
   U1 = rand_double();
   X = ((double) u - (double) l + (double) 1) * U1 + (double) l;

   return X;
}
