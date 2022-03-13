CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJ_CPY = aarch64-linux-gnu-objcopy
QEMU = qemu-system-aarch64

SRC = src
INCLUDE = include
#SRCS = $(wildcard *.c)
SRCS = $(wildcard $(SRC)/*.c )
#OBJS = $(SRCS:.c=.o)
OBJS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(SRCS))
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -w -g

VPATH = $(SRC)
vpath %.c %.S $(SRC)

IMG = kernel3.img
ELF = kernel3.elf

all: clean $(IMG)

$(SRC)/start.o: $(SRC)/start.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(IMG): $(SRC)/start.o $(OBJS)
	find ./rootfs | cpio -o -H newc > ramdisk
	$(LD) -r -b binary -o $(SRC)/rd.o ramdisk
	$(LD) -nostdlib -nostartfiles $(SRC)/start.o $(SRC)/rd.o $(OBJS) -T link.ld -o $(ELF)
	$(OBJ_CPY) -O binary $(ELF) $(IMG)


clean:
	rm $(ELF) ramdisk $(SRC)/*.o $(INCLUDE)/*.o *.o >/dev/null 2>/dev/null || true

run:
	$(QEMU) -M raspi3 -kernel $(IMG) -display none -serial null -serial stdio
 
runboot:
	$(QEMU) -M raspi3 -kernel $(IMG) -display none -serial null -serial pty
