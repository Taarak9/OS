#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"

void init_queue(QUEUE *q)
{
   q->front = 0;
   q->rear = -1;
   return ;
}

int is_queue_empty(QUEUE *q)
{
   return q->rear == -1;
}

int is_queue_full(QUEUE *q)
{
   return q->rear == MAXQUEUE-1;
}

void insert_queue(QUEUE *q, int val)
{
   if (is_queue_full(q))
   {
        printf("QUEUE is full, couldn't insert.\n");
        return ;
   }
   else
   {
        q->rear++;
        q->items[q->rear] = val;
   }
   return ;
}

int delete_queue(QUEUE *q)
{
   int i, temp;

   if (is_queue_empty(q))
   {
      printf("QUEUE is empty, couldn't delete.\n");
      return -1;
   }
   else
   {
      temp = q->items[q->front];
      for (i = q->front; i < q->rear; i++)
      {
           q->items[i] = q->items[i+1];
      }
      q->rear--;
   }

   return temp;
}

void print_queue(QUEUE *q)
{
   int i;

   if (is_queue_empty(q))
   {
      printf("QUEUE is empty, couldn't print.\n");
      return ;
   }
   else
   {
      for (i = q->front; i <= q->rear; i++)
      {
         printf("%d  ", q->items[i]);
      }
      printf("\n");
   }

   return ;
}

void shuffle_queue(QUEUE *q)
{
   int i, j;
   time_t seed;

   srand((unsigned) seed);

   for (i = q->rear; i > q->front; i--)
   {
      j = rand() % i;
      swap(&q->items[i], &q->items[j]);
   }

   return ;
}

void swap(int *a, int *b)
{
   int tmp;

   tmp = *a;
   *a = *b;
   *b = tmp;

   return ;
}
