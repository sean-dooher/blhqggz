EXPORT_LIB_HEADERS = alloca.h \
					 stdarg.h \
					 stdint.h \
					 stddef.h \
					 stdbool.h \
					 pmp.h \
					 string.h \
					 exit.h \
					 printf.h \
					 ecall.h \
					 stdio.h

EXPORT_LIB_C_SRCS = string.c \
					exit.c \
					printf.c \
					stdio.c \
					ecall.c

EXPORT_LIB_S_SRCS = asm/switch.S

HEADERS += $(addprefix lib/, $(EXPORT_LIB_HEADERS))
C_SRCS += $(addprefix lib/, $(EXPORT_LIB_C_SRCS))
S_SRCS += $(addprefix lib/, $(EXPORT_LIB_S_SRCS))

C_LIB_OBJ_DIR = $(OBJ_DIR)/lib
DIRS += $(C_LIB_OBJ_DIR) $(C_LIB_OBJ_DIR)/asm
