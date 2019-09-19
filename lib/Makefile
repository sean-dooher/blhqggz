EXPORT_LIB_HEADERS = alloca.h \
					 stdint.h \
					 pmp.h \
					 string.h \
					 exit.h

EXPORT_LIB_C_SRCS = string.c \
					exit.c

EXPORT_LIB_S_SRCS = asm/bootup.s \
				    asm/switch.s

HEADERS += $(addprefix lib/, $(EXPORT_LIB_HEADERS))
C_SRCS += $(addprefix lib/, $(EXPORT_LIB_C_SRCS))
S_SRCS += $(addprefix lib/, $(EXPORT_LIB_S_SRCS))

C_LIB_OBJ_DIR = $(OBJ_DIR)/lib
DIRS += $(C_LIB_OBJ_DIR) $(C_LIB_OBJ_DIR)/asm
