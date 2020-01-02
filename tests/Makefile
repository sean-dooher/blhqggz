TEST_LIB_SRCS = test.c
TEST_LIB_HEADERS = test.h

export TEST_LIST =  string_unit \
					timer_test \
					bootable \
					bitmap_test \
					vm_test \
					vm_test_create_pt \
					thread_init_test

TEST_DIR = tests
TEST_SRCS = $(addprefix $(TEST_DIR)/, $(addsuffix .c, $(TEST_LIST)))

TEST_LIB_DIR = tests/lib

TEST_BUILD_DIR = $(BUILD_DIR)/test
TEST_KERNEL_DIR = $(BIN_DIR)/test
TEST_OBJ_DIR = $(TEST_BUILD_DIR)/obj
TEST_DIRS = $(TEST_BUILD_DIR) $(TEST_OBJ_DIR) $(TEST_KERNEL_DIR)

TEST_OBJS = $(addprefix $(TEST_OBJ_DIR)/, $(addsuffix .o, $(TEST_LIST)))
TEST_LIB_OBJS = $(TEST_OBJ_DIR)/test_runner.o
TEST_LIB_OBJS += $(addprefix $(TEST_OBJ_DIR)/, $(patsubst %.c, %.o, $(TEST_LIB_SRCS)))
TEST_KERNELS = $(addprefix $(TEST_KERNEL_DIR)/, $(addsuffix .elf, $(TEST_LIST)))

CFLAGS += -I$(TEST_LIB_DIR)

all: $(TEST_OBJS) $(TEST_LIB_OBJS) $(TEST_KERNELS)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/lib/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_KERNEL_DIR)/%.elf: $(TEST_LIB_OBJS) $(TEST_OBJS) $(KERNEL_BASE)
	$(LD) $(LD_FLAGS) $(LD_SCRIPT) $(KERNEL_BASE) $(TEST_LIB_OBJS) $(TEST_OBJ_DIR)/$(notdir $(patsubst %.elf, %.o, $@)) -o $@

run: $(TEST_KERNELS)
	$(TEST_DIR)/run-tests.sh

$(shell mkdir -p $(TEST_DIRS))
