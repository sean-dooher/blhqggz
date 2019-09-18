EXPORT_HEADERS = alloca.h \
				 stdint.h \
				 pmp.h

EXPORT_S_SRCS = asm/bootup.s\
				asm/switch.s

EXPORT_C_SRCS = 

HEADERS += $(addprefix lib/, $(EXPORT_HEADERS))
C_SRCS += $(addprefix lib/, $(EXPORT_C_SRCS))
S_SRCS += $(addprefix lib/, $(EXPORT_S_SRCS))

C_LIB_OBJ_DIR = $(OBJ_DIR)/lib
DIRS += $(C_LIB_OBJ_DIR) $(C_LIB_OBJ_DIR)/asm
