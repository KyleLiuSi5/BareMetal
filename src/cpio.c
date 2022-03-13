#include "uart.h"
#include "cpio.h"
typedef struct {
		   char	   magic[6];
		   char	   ino[8];
		   char	   mode[8];
		   char	   uid[8];
		   char	   gid[8];
		   char	   nlink[8];
		   char	   mtime[8];
		   char	   filesize[8];
		   char	   devmajor[8];
		   char	   devminor[8];
		   char	   rdevmajor[8];
		   char	   rdevminor[8];
		   char	   namesize[8];
		   char	   check[8];
}__attribute__((packed)) cpio_t ;

int memcmp(void *s1, void *s2, int n)
{
    unsigned char *a=s1,*b=s2;
    while(n-->0){ if(*a!=*b) { return *a-*b; } a++; b++; }
    return 0;
}

int hex2bin(char *s, int n)
{
    int r=0;
    while(n-->0) {
        r<<=4;
        if(*s-'0'<10 && *s-'0'>=0) r+=*s++-'0';
        else r+=*s++-'A'+10;
    }
    return r;
}

/*
void initrd_list(char *buf)
{
    int first_time = 1;
    int rootname = 0;
    //uart_puts("  Magic    Size \t  Filename\n");

    while(!memcmp(buf,"070701",6) && memcmp(buf+sizeof(cpio_t),"TRAILER!!",9)){
        
        cpio_t *header = (cpio_t*)buf;
        int ns=hex2bin(header->namesize,8);
        int fs=hex2bin(header->filesize,8);
        int align_ns = 4-(sizeof(cpio_t)+ns-1)%4 ;
        int align_fs = (4-(fs)%4)==4 ? 0: (4-(fs)%4);

        if(first_time) rootname = ns;
        else{
            uart_puts(buf+sizeof(cpio_t)+rootname);      // filename
            uart_puts("\n");

            for(int i=0; i<fs ;i++){  // content
                uart_send(*(buf+sizeof(cpio_t)+ns+align_ns-1+i));
            }
            uart_puts("\n");
        }
        buf+=(sizeof(cpio_t)+ns+fs+align_ns-1+align_fs);
        first_time = 0;
    }
}
*/
void ls(char *buf)
{

    int first_time = 1;
    int rootname = 0;
    //uart_puts("  Magic    Size \t  Filename\n");
    int count = 0;
    while(!memcmp(buf,"070701",6) && memcmp(buf+sizeof(cpio_t),"TRAILER!!",9)){
        
        cpio_t *header = (cpio_t*)buf;
        int ns=hex2bin(header->namesize,8);
        int fs=hex2bin(header->filesize,8);
        int align_ns = 4-(sizeof(cpio_t)+ns-1)%4 ;
        int align_fs = (4-(fs)%4)==4 ? 0: (4-(fs)%4);

        if(first_time) rootname = ns;
        else{

            uart_puts("  ");
            uart_puts(buf+sizeof(cpio_t)+rootname);      // filename
            //uart_puts("\n");
        }
        buf+=(sizeof(cpio_t)+ns+fs+align_ns-1+align_fs);
        first_time = 0;
        count ++;
        uart_puts("\n");
    }
    uart_puts("\n");
}

void cat(char *buf, char choose)
{
    int first_time = 1;
    int rootname = 0;
    int count = 0;
    //uart_puts("  Magic    Size \t  Filename\n");

    while(!memcmp(buf,"070701",6) && memcmp(buf+sizeof(cpio_t),"TRAILER!!",9)){
        
        cpio_t *header = (cpio_t*)buf;
        int ns=hex2bin(header->namesize,8);
        int fs=hex2bin(header->filesize,8);
        int align_ns = 4-(sizeof(cpio_t)+ns-1)%4 ;
        int align_fs = (4-(fs)%4)==4 ? 0: (4-(fs)%4);
        
        if(first_time) rootname = ns;
        else{
            

            if((choose-'0')==count){
                uart_puts("Show content of ");
                uart_puts(buf+sizeof(cpio_t)+rootname);      // filename
                uart_puts(":\n");
                for(int i=0; i<fs ;i++){  // content
                    uart_send(*(buf+sizeof(cpio_t)+ns+align_ns-1+i));
                }

            }
            
            
        }
        buf+=(sizeof(cpio_t)+ns+fs+align_ns-1+align_fs);
        first_time = 0;
        count ++;

    }
    uart_puts("\n");
}


char* simple_malloc(unsigned int size){
    char *tmp = (char*)start_mem;
    start_mem+=size;
    return tmp;
}
/*
char* simple_malloc(int size) {

    static int start_pointer; 
    while(start_mem<MALLOC_END){
        start_pointer = start_mem;
        for(int j=0; j<size; j++){
            if(*(char*)(start_mem+j) != 0){ 
                start_mem = start_mem+j+1;
                break;
            }
        }
        
        //uart_hex(start_pointer);
        //uart_puts("-");
        //uart_hex(start_mem+size-1);
        //uart_puts("\n");
        
        start_mem = start_mem+size;
        return (char*)(start_pointer);
    }
}
*/