/*
remember to uncommand move_kernel() in main()
1. make
2. make runboot
3. python3 bootloader.py 
4. screen /dev/pts/12 115200
*/
#include "uart.h"
#include "bootloader.h"

void move_kernel(void (*pointer_to_new_shell)())
{
    const int (*pointer_to_new_get_uart_img)() = get_uart_img-0x20000;
    for (int offset = 0; offset < 0x20000; offset += 0x1)
    {
        *((volatile unsigned char *)(NEW_KERNEL_POS + offset)) = *((volatile unsigned char *)(OLD_KERNEL_POS + offset));
    }
    (pointer_to_new_get_uart_img)(0x80000, pointer_to_new_get_uart_img); // to 0x60XXX
}

void get_uart_img(void (*pointer_to_new)(),void (*pointer_output)()){
    //uart_puts("I am here now: ");
    //uart_hex((unsigned int)pointer_output);
    //uart_puts("\n$ ");
    
    for(int offset=0; offset<2464; offset++){
        //uart_send(uart_getc());
        *((volatile unsigned char *)(OLD_KERNEL_POS + offset)) = uart_getc(); //load new image
    }
    (pointer_to_new)(); //  jump to 0x80000
}