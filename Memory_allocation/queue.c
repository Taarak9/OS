typedef struct lqueue
{
   int info;
   struct lqueue *link;
} queue;

queue *front = NULL, *rear = NULL;

int isempty(queue *q)
{
   return ((front == NULL) || (rear == NULL));
}

void enqueue(queue *q, int val)
{
   queue *tmp;

   tmp = (struct queue *)malloc(sizeof(struct queue));

   tmp->info = val;
   tmp->link = NULL;

   if (front == NULL || rear == NULL)
   {
        front = tmp;
   }
   else
   {
        rear->link = tmp;
   }
   rear = tmp;

   return ;
}

int dequeue(queue *q)
{
   int temp;

   if (isempty(q))
   {
        printf("There are no elements to delete.\n");
        return -1;
   }
   else
   {
        temp = front->info;
        front = front->link;
   }
   return temp;
}

void display(queue *q)
{
   queue *tmp;

   tmp = (struct queue *)malloc(sizeof(struct queue));

   tmp = front;
   while (tmp != NULL)
   {
        printf("%d ", tmp->info);
        tmp = tmp->link;
   }
   printf("\n");

   return ;
}

void clearqueue(queue *q)
{
   while (front != NULL)
   {
      dequeue(q);
   }

   free(front);
   free(rear);

   return ;
}
