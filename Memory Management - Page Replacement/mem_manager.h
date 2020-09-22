#define NP 10
#define F 64
#define MAXLEN 32
#define IN_FILE 1
#define OUT_FILE 2

char fname[MAXLEN];

typedef struct frame_s
{
   int page_no;
   int pid;
   int page_replaced;
} frame;

frame phys_mem[F];

int sim_process(int, int, FILE *);
double rand_double(void);
double rand_normal(int, int);
double rand_uniform(int, int);

int mem_mapper(FILE *, FILE *, int, int);
int get_free_frame(void);
int is_page(int, int);

void init_fp_table(FILE **);
void init_phys_mem(void);
void print_phys_mem(void);
void name_file(int, int);
FILE* read_file(FILE *);
FILE* write_file(FILE *);
FILE* append_file(FILE *);
