# Tools
export CROSS_COMP = riscv64-unknown-elf
export CC = $(CROSS_COMP)-gcc
export LD = $(CROSS_COMP)-ld
export GDB = $(CROSS_COMP)-gdb
export OBJCOPY = $(CROSS_COMP)-objcopy
export QEMU = qemu-system-riscv64

# KERNELs
export BUILD_DIR=build
export OBJ_DIR=$(BUILD_DIR)/obj
export BIN_DIR=$(BUILD_DIR)/bin
DIRS=$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR)

KERNEL=$(BIN_DIR)/kernel.elf
export KERNEL_BASE=$(OBJ_DIR)/prebuild_kernel.o

TARGET ?= kernel.elf
BIN_TARGET = $(addprefix $(BIN_DIR)/, $(TARGET))

# Platform information
PLATFORM_DIR = platform/
PLATFORM ?= virt

# Tool Options
export LD_FLAGS = -T link.ld -nostartfiles -nostdlib -nostdinc -static
export CFLAGS = -I$(LIB_DIR) -I. -Wall -Werror -O0 -nostdinc -nostdlib -nostartfiles -mcmodel=medany -ffreestanding -lgcc -DDEBUG -g
export QEMU_FLAGS = -M $(PLATFORM) -cpu rv64gcsu-v1.10.0 -bios none -display none -serial stdio -serial null

# Source files

LIB_DIR = lib/

C_SRCS =

HEADERS = 

S_SRCS = 

SRC_SUBDIRS = threads \
			  lib \
			  devices

include $(addsuffix /Makefile, $(SRC_SUBDIRS))

include $(addprefix $(PLATFORM_DIR), $(PLATFORM).mk)

# Object files
S_OBJS := $(S_SRCS:.s=.o)
S_OBJS := $(S_OBJS:.S=.o)
C_OBJS := $(C_SRCS:.c=.o)

OBJS = $(addprefix $(OBJ_DIR)/, $(C_OBJS) $(S_OBJS))
export OBJS

export TESTS ?= all

.PHONY: all, tests, run-tests, check, run, run-disas, gdb, clean, debug
all: $(KERNEL)

$(OBJ_DIR)/%.o: %.S $(HEADERS)
	@echo "AS $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.s $(HEADERS)
	@echo "AS $@"
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

run-tests:
	$(MAKE) -f tests/Makefile run

check: run-tests

clean:
	rm -rf build

run-disas: $(BIN_TARGET)
	@$(QEMU) $(QEMU_FLAGS) -kernel $(BIN_TARGET) -d in_asm

run: $(BIN_TARGET)
	@$(QEMU) $(QEMU_FLAGS) -kernel $(BIN_TARGET)

debug: $(BIN_TARGET)
	@$(QEMU) $(QEMU_FLAGS) -kernel $(BIN_TARGET) -S -gdb tcp::9000

gdb:
	$(GDB) --eval-command="target remote :9000" $(BIN_TARGET)

$(shell mkdir -p $(DIRS))
