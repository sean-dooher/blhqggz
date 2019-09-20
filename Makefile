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
KERNEL_BASE=$(OBJ_DIR)/prebuild_kernel.o

# Source files

LIB_DIR = lib/

C_SRCS =

HEADERS = 

S_SRCS = 

SRC_SUBDIRS = threads \
			  lib \
			  devices

include $(addsuffix /Makefile, $(SRC_SUBDIRS))

# Tool Options
LD_FLAGS = -T link.ld -nostartfiles -nostdlib -nostdinc -static
CFLAGS = -I$(LIB_DIR) -I. -Wall -Werror -O0 -nostdinc -nostdlib -nostartfiles -mcmodel=medany -ffreestanding -lgcc -g
QEMU_FLAGS = -M sifive_u -display none -serial stdio -serial null

# Object files

all: $(TARGET)

OBJS = $(addprefix $(OBJ_DIR)/, $(C_SRCS:.c=.o) $(S_SRCS:.s=.o))

$(OBJ_DIR)/%.o: %.s $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o $@

$(KERNEL_BASE): $(OBJS) $(HEADERS)
	@$(LD) $(LD_FLAGS) -r $(OBJS) -o $(KERNEL_BASE)

$(TARGET): $(KERNEL_BASE) $(OBJ_DIR)/main.o
	@$(LD) $(LD_FLAGS) $(KERNEL_BASE) $(OBJ_DIR)/main.o -o $(TARGET)

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