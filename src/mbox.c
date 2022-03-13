#include "uart.h"

#define GET_BOARD_REVISION  0x00010002
#define GET_MEMORY          0x00010005
#define REQUEST_CODE        0x00000000
#define REQUEST_SUCCEED     0x80000000
#define REQUEST_FAILED      0x80000001
#define TAG_REQUEST_CODE    0x00000000
#define END_TAG             0x00000000

#define MMIO_BASE       0x3f000000
#define MAILBOX_BASE    MMIO_BASE + 0xb880

#define MAILBOX_READ    ((volatile unsigned int*)(MAILBOX_BASE + 0x0))
#define MAILBOX_STATUS  ((volatile unsigned int*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE   ((volatile unsigned int*)(MAILBOX_BASE + 0x20))

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

//volatile unsigned int  __attribute__((aligned(16))) mbox[36];

//unsigned int mailbox[8];
volatile unsigned int  __attribute__((aligned(16))) mailbox[36];


int mailbox_call(unsigned int ch)
{
    unsigned int r = (((unsigned int)((unsigned long)&mailbox)&~0xF) | (ch&0xF));
    /* wait until we can write to the mailbox */
    do{asm volatile("nop");}while(*MAILBOX_STATUS & MAILBOX_FULL);
    /* write the address of our message to the mailbox with channel identifier */
    *MAILBOX_WRITE = r;
    /* now wait for the response */
    while(1) {
        /* is there a response? */
        do{asm volatile("nop");}while(*MAILBOX_STATUS & MAILBOX_EMPTY);
        /* is it a response to our message? */
        if(r == *MAILBOX_READ)
            /* is it a valid successful response? */
            return mailbox[1]==MAILBOX_FULL;
    }
    return 0;
}


void get_board_revision(){
  //unsigned int mailbox[7];
  mailbox[0] = 8 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_BOARD_REVISION; // tag identifier
  mailbox[3] = 8; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // value buffer
  mailbox[6] = 0; // value buffer
  // tags end
  mailbox[7] = END_TAG;

  mailbox_call(8); // message passing procedure call, you should implement it following the 6 steps provided above.
  uart_puts("\n  Board:\n");
  uart_puts("    Revision:");
  uart_hex(mailbox[5]);
  uart_puts("\n\n");
  //printf("0x%x\n", mailbox[5]); // it should be 0xa020d3 for rpi3 b+
}

void get_memory(){
  //unsigned int mailbox[8];
  mailbox[0] = 8 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_MEMORY; // tag identifier
  mailbox[3] = 8; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // value buffer
  mailbox[6] = 0; // value buffer
  // tags end
  mailbox[7] = END_TAG;

  mailbox_call(8); // message passing procedure call, you should implement it following the 6 steps provided above.
  uart_puts("\n  Memory: \n");
  uart_puts("    Base: ");
  uart_hex(mailbox[5]);
  uart_puts("\n    Size: ");
  uart_hex(mailbox[6]);
  uart_puts("\n\n");
}