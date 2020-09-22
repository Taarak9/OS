// proc_simulator declarations
void sim_process(int M, int S, FILE *infile);
double rand_double();
double convert_distro(double rand_num, int mean, int sigma);

// mem_manager declarations
void mem_manager(FILE *infile, int F, FILE *outfile);
void initialize_array(int phys_mem[], int F);
int free_frame(int phys_mem[], int F);
int search_page(int P, int phys_mem[], int F);
