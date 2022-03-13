//void initrd_list(char *buf);
#define MALLOC_START 0x100000
#define MALLOC_END 0x200000
void ls(char *buf);
void cat(char *buf, char choose);
static char* start_mem = (char*)MALLOC_START; 
char* simple_malloc(unsigned int size);