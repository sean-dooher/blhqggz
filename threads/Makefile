EXPORT_THREADS_HEADERS = interrupts.h \
						 thread.h

EXPORT_THREADS_C_SRCS = interrupts.c \
						thread.c

EXPORT_THREADS_S_SRCS = start.S \
						exceptions.S

HEADERS += $(addprefix threads/, $(EXPORT_THREADS_HEADERS))
C_SRCS += $(addprefix threads/, $(EXPORT_THREADS_C_SRCS))
S_SRCS += $(addprefix threads/, $(EXPORT_THREADS_S_SRCS))

DIRS += $(OBJ_DIR)/threads
