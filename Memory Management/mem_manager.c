#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "mem_manager.h"

int main(int argc, char **argv)
{
   if (argc != 2)
   {
        fprintf(stderr, "Usage: %s proc_requests_file\n", argv[0]);
        exit (1);
   }

   //step1
   int i;
   //int nar;
   int M;
   int sigma;

   FILE *ifp_table[NP];
   init_fp_table(ifp_table);

   FILE *fp = NULL;
   if ((fp = fopen(argv[1], "r")) == NULL)
   {
        fprintf(stderr, "Error opening %s \n", argv[1]);
        exit (2);
   }

   char line[MAXLEN];
   char *ptr = NULL;
   for (i = 0; i < NP; i++)
   {
        if ((fgets(line, MAXLEN, fp) != NULL) && (i < NP))
        {
             ptr = strtok(line, ",");
	     M = atoi(ptr);

             ptr = strtok(NULL, ",");
             sigma = atoi(ptr);

	    // printf("pid : %d M: %d S: %d\n", i+1, M, sigma);
        }
	else
        {
             printf("Need more data in proc_req.csv \n");
             exit (3);	     
        }

        name_file(i+1, IN_FILE);
        ifp_table[i] = write_file(ifp_table[i]);
        /*nar = */sim_process(M, sigma, ifp_table[i]);
        //printf("\n%d addresses written into %s \n", nar, fname);
        fclose(ifp_table[i]);
   }
   
   //step2
   for (i = 0; i < NP; i++)
   {
        name_file(i+1, IN_FILE);
        ifp_table[i] = read_file(ifp_table[i]);	
   }

   //step3
   FILE *ofp_table[NP];
   init_fp_table(ofp_table);

   for (i = 0; i < NP; i++)
   {
        name_file(i+1, OUT_FILE);
        ofp_table[i] = write_file(ofp_table[i]);
   }

   //step4
   QUEUE proc_queue;

   init_queue(&proc_queue);

   int pid;
   for (pid = 0; pid < NP; pid++)
        insert_queue(&proc_queue, pid+1); 
   shuffle_queue(&proc_queue);
   
   printf("-------------------------------------------------\n");
   printf("initial sequence \n");
   print_queue(&proc_queue);
   init_phys_mem();
   
   int cur_pid;
   int status;
   int exec[NP] = {0};
   int k = 0;
   while (!(is_queue_empty(&proc_queue)))
   {
        //step5
        cur_pid = delete_queue(&proc_queue);
	
        //step6

        status = mem_mapper(ifp_table[cur_pid-1], ofp_table[cur_pid-1], cur_pid);
        if (status < 0)
        {
             fprintf(stderr, "Memory not available \n");
             exit (4);
        }
        else if (status == 0)
        {
             /*
             printf("Process with pid %d completed; removing from queue\n",
             cur_pid);*/
             exec[k] = cur_pid;
	     k++;
             //print_queue(&proc_queue);
	     fclose(ifp_table[cur_pid-1]);
             fclose(ofp_table[cur_pid-1]);
        }
        else
        {
            insert_queue(&proc_queue, cur_pid);
        }
   }

   printf("-------------------------------------------------\n");
   printf("PHYSICAL MEMEORY\n");
   print_phys_mem();

   printf("-------------------------------------------------\n");
   printf("execution sequence\n");
   for (k = 0; k < NP; k++)
       printf("%d ", exec[k]);
   printf("\n");
   exit (0);
}

void init_fp_table(FILE **fp_table)
{
   int i;
   for (i = 0; i < NP; i++)
       fp_table[i] = NULL;
   return ;
}

void init_phys_mem(void)
{
   int i;
   for (i = 0; i < F; i++)
   {
        phys_mem[i].page_no = -1;
        phys_mem[i].pid = -1;
   }
}

void print_phys_mem(void)
{
   int i;
   printf("page\tpid\t\n");
   for (i = 0; i < F; i++)
   {
        printf("%d\t%d\t\n", phys_mem[i].page_no, phys_mem[i].pid);
   }
   return ;
}

void name_file(int pid, int type)
{
   if (type == 1)
   {
        if (pid < 10)
            snprintf(fname, MAXLEN, "AR_0%d.dat", pid);
        else
            snprintf(fname, MAXLEN, "AR_%d.dat", pid);
   }
   else if (type == 2)
   {
        if (pid < 10)
            snprintf(fname, MAXLEN, "AR_0%d_out.dat", pid);
        else
            snprintf(fname, MAXLEN, "AR_%d_out.dat", pid);
   }
   return ;
}

FILE* read_file(FILE *fp)
{
   if ((fp = fopen(fname, "r")) == NULL)
   {
        fprintf(stderr, "error reading %s \n", fname);
        exit (2);
   }
   return fp;
}

FILE* write_file(FILE *fp)
{
   if ((fp = fopen(fname, "w")) == NULL)
   {
        fprintf(stderr, "error writing %s \n", fname);
        exit (3);
   }
   return fp;
}
