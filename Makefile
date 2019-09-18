# Tools
CROSS_COMP = riscv64-unknown-elf
CC = $(CROSS_COMP)-gcc
LD = $(CROSS_COMP)-ld
GDB = $(CROSS_COMP)-gdb
OBJCOPY = $(CROSS_COMP)-objcopy
QEMU = qemu-system-riscv64

# Targets
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin
DIRS=$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR)

TARGET=$(BIN_DIR)/kernel.elf

# Source files

LIB_DIR = lib/

C_SRCS = main.c \
		 interrupts.c

HEADERS = interrupts.h

S_SRCS = start.s

include lib/Makefile
include devices/Makefile


# Tool Options
LD_FLAGS = -T link.ld -nostartfiles -nostdlib -nostdinc -static
CFLAGS = -I$(LIB_DIR) -Wall -O0 -mcmodel=medany -ffreestanding -lgcc -nostdinc -nostdlib -nostartfiles -g
QEMU_FLAGS = -M sifive_u -display none -serial stdio -serial null

# Object files

OBJS = $(addprefix $(OBJ_DIR)/, $(C_SRCS:.c=.o) $(S_SRCS:.s=.o))

all: $(TARGET)

$(OBJ_DIR)/%.o: %.s $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(HEADERS)
	echo "$(OBJS)"
	echo "$(C_SRCS)"
	$(LD) $(LD_FLAGS) $(OBJS) -o $(TARGET)

$(BIN_DIR)/kernel.img: $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN_DIR)/kernel.img

clean:
	rm -rf build

run_disas: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) -d in_asm

run: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET)

debug: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) -S -gdb tcp::9000

gdb:
	$(GDB) --eval-command="target remote :9000" $(TARGET)

$(shell mkdir -p $(DIRS))