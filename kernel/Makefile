CFLAGS += -Ikernel/include

KERNEL_OBJS = kernel.o
KERNEL_OBJS := $(patsubst %,$(BUILD_DIR)/kernel/%,$(KERNEL_OBJS))

# Include serial driver
include lib/serial/Makefile
KERNEL_OBJS += $(LIBSERIAL_OBJS)
