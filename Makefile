# Tools
CROSS_COMP = riscv64-unknown-elf
CC = $(CROSS_COMP)-gcc
LD = $(CROSS_COMP)-ld
GDB = $(CROSS_COMP)-gdb
OBJCOPY = $(CROSS_COMP)-objcopy
QEMU = qemu-system-riscv64

export CC
export LD
export QEMU

# KERNELs
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin
DIRS=$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR)

export BUILD_DIR
export BIN_DIR
export OBJ_DIR

KERNEL=$(BIN_DIR)/kernel.elf
KERNEL_BASE=$(OBJ_DIR)/prebuild_kernel.o
export KERNEL_BASE

TARGET ?= kernel.elf
BIN_TARGET = $(addprefix $(BIN_DIR)/, $(TARGET))

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

export LD_FLAGS
export CFLAGS

# Object files
OBJS = $(addprefix $(OBJ_DIR)/, $(C_SRCS:.c=.o) $(S_SRCS:.s=.o))
export OBJS

all: $(KERNEL)

$(OBJ_DIR)/%.o: %.s $(HEADERS)
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(HEADERS) main.c
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c main.c -o $@

$(KERNEL_BASE): $(OBJS) $(HEADERS)
	@echo "LD $(KERNEL_BASE)"
	@$(LD) $(LD_FLAGS) -r $(OBJS) -o $(KERNEL_BASE)

$(KERNEL): $(KERNEL_BASE) $(OBJ_DIR)/main.o
	@echo "LD $(KERNEL)"
	@$(LD) $(LD_FLAGS) $(KERNEL_BASE) $(OBJ_DIR)/main.o -o $(KERNEL)

$(BIN_DIR)/kernel.img: $(KERNEL)
	@$(OBJCOPY) -O binary $(KERNEL) $(BIN_DIR)/kernel.img

tests: $(KERNEL_BASE)
	$(MAKE) -f tests/Makefile

clean:
	rm -rf build

run_disas: $(BIN_TARGET)
	@$(QEMU) $(QEMU_FLAGS) -kernel $(BIN_TARGET) -d in_asm

run: $(BIN_TARGET)
	@$(QEMU) $(QEMU_FLAGS) -kernel $(BIN_TARGET)

debug: $(BIN_TARGET)
	@$(QEMU) $(QEMU_FLAGS) -kernel $(BIN_TARGET) -S -gdb tcp::9000

gdb:
	$(GDB) --eval-command="target remote :9000" $(BIN_TARGET)

$(shell mkdir -p $(DIRS))