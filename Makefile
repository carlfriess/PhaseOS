# Determine build directory based on target architecture
ifneq (,$(findstring c28xx,$(MAKECMDGOALS)))
BUILD_DIR = build/c28xx
endif

KERNEL_OBJS = main.o
KERNEL_OBJS := $(patsubst %,$(BUILD_DIR)/%,$(KERNEL_OBJS))

$(BUILD_DIR)/%.o: %.s
	@echo " "
	@echo "Building file: \033[35m$<\033[39m"
	mkdir -p  $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@echo " "
	@echo "Building file: \033[35m$<\033[39m"
	mkdir -p  $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

# Include makefile for target architecture
ifneq (,$(findstring c28xx,$(MAKECMDGOALS)))
include arch/c28xx/Makefile
endif

.PHONY: kernel
