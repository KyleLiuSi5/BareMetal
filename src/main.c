#include "uart.h"
#include "mbox.h"
#include "reset.h"
#include "bootloader.h"
#include "cpio.h"
#inlcude "utils.h"

extern volatile unsigned long _start; //from linker script
extern volatile unsigned long _end;
extern volatile unsigned char _binary_ramdisk_start;

char arr[128] = {0}; //for shell
int counter = 0;     //for shell

void shell()
{
        uart_puts("$ ");
        while (1)
        {
                arr[counter] = uart_getc();
                if (arr[counter] != 8)
                {
                        uart_send(arr[counter]);
                }
                else if (arr[counter] == 8 && counter > 0)
                { //backspace
                        uart_puts("\b\40\b");
                        counter -= 2;
                }

                if (arr[counter] == '\n')
                { //shell
                        if (arr[counter - 1] == 'p' && arr[counter - 2] == 'l' && arr[counter - 3] == 'e' && arr[counter - 4] == 'h')
                        {
                                uart_puts("\n\
  help      : Print this help menu\n\
  hello     : Print Hello World!\n\
  clear     : Clean screen\n\
  ls        : List directory\n\
  cat       : Print file content\n\
  bl        : Run bootloader (supported by bootloader.py)\n\
  board     : Print board revision\n\
  mem       : Print ARM memory base address and size\n\
  reboot    : Reboot the device\n\
  \n");
                        }
                        else if (arr[counter - 1] == 'o' && arr[counter - 2] == 'l' && arr[counter - 3] == 'l' && arr[counter - 4] == 'e' && arr[counter - 5] == 'h')
                        {
                                uart_puts("\n  Hello World!\n\n");
                        }

                        else if (arr[counter - 1] == 't' && arr[counter - 2] == 'o' && arr[counter - 3] == 'o' && arr[counter - 4] == 'b' && arr[counter - 5] == 'e' && arr[counter - 6] == 'r')
                        {
                                uart_puts("\n  Reboot!\n\n");
                                reset(1);
                        }
                        // Print board revision and ARM memory base address and size.
                        else if (arr[counter - 1] == 'd' && arr[counter - 2] == 'r' && arr[counter - 3] == 'a' && arr[counter - 4] == 'o' && arr[counter - 5] == 'b')
                        {
                                get_board_revision();
                        }
                        else if (arr[counter - 1] == 'm' && arr[counter - 2] == 'e' && arr[counter - 3] == 'm')
                        {
                                get_memory();
                        }
                        else if (arr[counter - 1] == 'r' && arr[counter - 2] == 'a' && arr[counter - 3] == 'e' && arr[counter - 4] == 'l' && arr[counter - 5] == 'c')
                        {
                                uart_puts("\f");
                        }
                        else if (arr[counter - 1] == 'l' && arr[counter - 2] == 'b')
                        {
                                move_kernel(shell-(OLD_KERNEL_POS-NEW_KERNEL_POS));  //function pointer to 0x80508-0x20000 like "void (*funcPtr1)() = &shell;" 
                                // this will load kernel1 to cover kernel2
                                // kernel1 is from lab1
                        }
                        else if (arr[counter - 1] == 's' && arr[counter - 2] == 'l')
                        {
                                ls((char*)&_binary_ramdisk_start);
                        }
                        else if (arr[counter - 1] == 't' && arr[counter - 2] == 'a' && arr[counter - 3] == 'c')
                        {
                                
                                uart_puts("Which one?\n");
                                ls((char*)&_binary_ramdisk_start);
                                char choose = uart_getc();
                                //uart_send(choose);
                                cat((char*)&_binary_ramdisk_start, choose);
                        }

                        uart_puts("\r$ ");
                        counter = 0; // if you press enter, reset.
                }
                //if(arr[counter]=='\n') uart_puts("\r$ "); // make shell pretty
                else if (arr[counter] != '\n' && arr[counter] != 8)
                { //dont ++ while backspace or will have trouble
                        counter++;
                }
        }
}
void main()
{
        uart_init();
        uart_puts("Operation system booting sucessfully!!\n");
        char* string1 = simple_malloc(8);
        char* string2 = simple_malloc(12);
        char* string3 = simple_malloc(20);

        shell();
}
