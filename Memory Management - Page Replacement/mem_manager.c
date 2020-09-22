#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "mem_manager.h"
#include "page_replacement.h"

int main(int argc, char **argv)
{
   if (argc != 3)
   {
        fprintf(stderr, "Usage: %s proc_requests_file replacement_algo(1 for global; 2 for local)\n", argv[0]);
        exit (1);
   }

   int choice = atoi(argv[2]);

   if (!(choice == 1 | choice == 2))
   {
       printf("Invalid choice, choose a valid one. \n");
       exit (2);
   }

   int i;
   int nar;
   int M;
   int sigma;
   char *ptr = NULL;
   char line[MAXLEN];
   int ideal_time[NP] = {0}; // ideal time

   FILE *ifp_table[NP];
   init_fp_table(ifp_table);

   FILE *fp = NULL;
   if ((fp = fopen(argv[1], "r")) == NULL)
   {
        fprintf(stderr, "Error opening %s \n", argv[1]);
        exit (3);
   }
   for (i = 0; i < NP; i++)
   {
        if (fgets(line, MAXLEN, fp) != NULL)
        {
             ptr = strtok(line, ",");
	     M = atoi(ptr);

             ptr = strtok(NULL, ",");
             sigma = atoi(ptr);
        }
	else
        {
             printf("Need more data in %s \n", argv[1]);
             exit (4);	     
        }

        name_file(i+1, IN_FILE);
        ifp_table[i] = write_file(ifp_table[i]);
        nar = sim_process(M, sigma, ifp_table[i]);
	ideal_time[i] = nar;
        //printf("PID %d \n", i+1);
        //printf("%d addresses written into %s \n", nar, fname);
        //printf("------------------------------------------------\n");
        fclose(ifp_table[i]);
   }
   
   for (i = 0; i < NP; i++)
   {
        name_file(i+1, IN_FILE);
        ifp_table[i] = read_file(ifp_table[i]);	
   }

   FILE *ofp_table[NP];
   init_fp_table(ofp_table);

   for (i = 0; i < NP; i++)
   {
        name_file(i+1, OUT_FILE);
        ofp_table[i] = write_file(ofp_table[i]);
   }

   QUEUE proc_queue;

   init_queue(&proc_queue);

   int pid;
   for (pid = 0; pid < NP; pid++)
   {
        first_frame_table[pid] = -1;
        insert_queue(&proc_queue, pid+1);
   } 
   shuffle_queue(&proc_queue);
   /* 
   printf("-------------------------------------------------\n");
   printf("initial sequence\n");
   print_queue(&proc_queue);
   printf("-------------------------------------------------\n");
   */init_phys_mem();
   
   int cur_pid;
   int status;
   int k = 0;
   //int exec[NP] = {0};
/*
   printf("PROCESS QUEUE\n");
   print_queue(&proc_queue);*/
   while (!(is_queue_empty(&proc_queue)))
   {
        cur_pid = delete_queue(&proc_queue);
	
        status = mem_mapper(ifp_table[cur_pid-1], ofp_table[cur_pid-1],
        cur_pid, choice);

        if (status == 0)
        {
	     /*
             printf("Process with pid %d completed; removing from queue\n",
             cur_pid);
             printf("PROCESS QUEUE\n");
             print_queue(&proc_queue);*/
             //exec[k] = cur_pid;
             k++;
	     fclose(ifp_table[cur_pid-1]);
             fclose(ofp_table[cur_pid-1]);
        }
        else if (status > 0)
        {
            insert_queue(&proc_queue, cur_pid);
        }
   }
   
   /*  
   printf("-------------------------------------------------\n");
   printf("PHYSICAL MEMEORY\n");
   print_phys_mem();*/

   ptr = NULL;
   int fault, npf;
   int pf[NP] = {0};
   //printf("process\t Page_faults\t\n");
   for (k = 0; k < NP; k++)
   {
	npf = 0;
        FILE *fp = NULL;
	name_file(k+1, OUT_FILE);
	fp = read_file(fp);
        while (fgets(line, MAXLEN, fp) != NULL)
        {
             ptr = strtok(line, ",");
             ptr = strtok(NULL, ",");
             ptr = strtok(NULL, ",");

             fault = atoi(ptr);

             if (fault == 1)
                 npf++;
        }
	fclose(fp);
	pf[k] = npf;
     //   printf("%d\t: %d\t\n", k+1, npf);
     //   tpf += npf;
   }
   //printf("Total no of page faults are %d\n", tpf);

   int p_time[NP] = {0};
   int pr[NP] = {0};
   int x = 0, y = 0;
   for (x = 0; x < NP; x++)
   {
       for (y = 0; y < F; y++)
       {
           if (phys_mem[y].pid == x+1)
               pr[x] += phys_mem[y].page_replaced;
       }
       p_time[x] = (ideal_time[x]-pf[x]) + ((pf[x]-pr[x])*50) 
       + (pr[x]*100);
   }
   double slow_down[NP] = {0};
   printf("-----------------------------------------------------------\n");
   printf("Process\tPage_faults\tPage_replacements\tSlow_down\t\n");
   printf("-----------------------------------------------------------\n");
   for (x = 0; x < NP; x++)
   {
        slow_down[x] = (double) (p_time[x]-ideal_time[x]);
        printf("%d\t%d\t\t%d\t\t\t%lf\t\n", x+1, pf[x],
        pr[x], 100*(slow_down[x]/ideal_time[x]));	
   }
   printf("-----------------------------------------------------------\n");

   int total = 0;
   double overall_slow_down = 0;
   for (x = 0; x < NP; x++)
   {
	total += ideal_time[x];
        overall_slow_down += slow_down[x];
   }
   printf("Overall Slow-down : %lf\n", 100*(overall_slow_down/total));
   /*
   printf("-------------------------------------------------\n");
   printf("execution sequence\n");
   for (k = 0; k < NP; k++)
       printf("%d  ", exec[k]);
   printf("\n");
   printf("-------------------------------------------------\n");*/
// avg value manipulation
   FILE *fp_table[NP+1];
   init_fp_table(fp_table);

   for (x = 0; x < NP+1; x++)
   {
        name_file(x+11, OUT_FILE);
        fp_table[x] = append_file(fp_table[x]);
   }

   for (x = 0; x < NP; x++)
   {
        fprintf(fp_table[x], "%d %d %d %lf\n", x+1, pf[x],
        pr[x], 100*(slow_down[x]/ideal_time[x]));
        fclose(fp_table[x]);
   }
   fprintf(fp_table[x], "%lf\n", 100*(overall_slow_down/total));
   fclose(fp_table[x]);

   int tpf = 0, tpr = 0;
   for (x = 0; x < NP; x++)
   {
        tpf += pf[x];
        tpr += pr[x];
   }

   printf("tpf : %d tpr : %d\n", tpf, tpr);
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
   first_frame = 0;
   for (i = 0; i < F; i++)
   {
        phys_mem[i].page_no = -1;
        phys_mem[i].pid = -1;
	phys_mem[i].page_replaced = 0;
   }
}

void print_phys_mem(void)
{
   int i;
   printf("page\tpid\tno of times page has been replaced\t\n");
   for (i = 0; i < F; i++)
   {
        printf("%d\t%d\t%d\t\n", phys_mem[i].page_no, phys_mem[i].pid,
        phys_mem[i].page_replaced);
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

FILE* append_file(FILE *fp)
{
   if ((fp = fopen(fname, "a")) == NULL)
   {
        fprintf(stderr, "error appending %s \n", fname);
        exit (4);
   }
   return fp;
}
