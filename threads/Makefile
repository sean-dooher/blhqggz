EXPORT_THREADS_HEADERS = interrupts.h

EXPORT_THREADS_C_SRCS = interrupts.c

EXPORT_THREADS_S_SRCS = start.s \
						exceptions.s

HEADERS += $(addprefix threads/, $(EXPORT_THREADS_HEADERS))
C_SRCS += $(addprefix threads/, $(EXPORT_THREADS_C_SRCS))
S_SRCS += $(addprefix threads/, $(EXPORT_THREADS_S_SRCS))

DIRS += $(OBJ_DIR)/threads
