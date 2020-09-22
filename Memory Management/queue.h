#define MAXQUEUE 32

typedef struct queue
{
   int front;
   int rear;
   int items[MAXQUEUE];
} QUEUE;

void init_queue(QUEUE *);
int is_queue_empty(QUEUE *);
int is_queue_full(QUEUE *);
void insert_queue(QUEUE *, int);
int delete_queue(QUEUE *);
void print_queue(QUEUE *);
void shuffle_queue(QUEUE *);
void swap(int *, int *);

