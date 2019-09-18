C_SRCS = main.c \
		 interrupts.c \
		 serial.c

HEADERS = serial.h \
		  stdint.h \
		  interrupts.h

S_SRCS = start.s

OBJS = $(C_SRCS:.c=.o) $(S_SRCS:.s=.o)


CROSS_COMP = riscv64-unknown-elf
CC = $(CROSS_COMP)-gcc
LD = $(CROSS_COMP)-ld
GDB = $(CROSS_COMP)-gdb
OBJCOPY = $(CROSS_COMP)-objcopy
QEMU = qemu-system-riscv64

TARGET=kernel.elf

LD_FLAGS = -T link.ld -nostartfiles -nostdlib -nostdinc -static
CFLAGS = -Wall -O0 -mcmodel=medany -ffreestanding -lstdint -nostdinc -nostdlib -nostartfiles -g

QEMU_FLAGS = -M sifive_u -display none -serial stdio

all: $(TARGET)

again: clean $(TARGET)

%.o: %.s $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(HEADERS)
	$(LD) $(LD_FLAGS) $(OBJS) -o kernel.elf

kernel.img: $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) kernel.img

clean:
	rm kernel.img $(TARGET) *.o >/dev/null 2>/dev/null || true

run_disas: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) -d in_asm

run: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET)

debug: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) -S -gdb tcp::9000

gdb:
	$(GDB) --eval-command="target remote localhost:9000" $(TARGET)
