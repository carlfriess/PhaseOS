# Determine target architecture
ifneq (,$(findstring c28x,$(MAKECMDGOALS)))
ARCH = c28x
endif

# Include path
CFLAGS += -Iinclude -Iinclude/arch/$(ARCH)

# Set build directory based on target architecture
BUILD_DIR = build/$(ARCH)

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
include arch/$(ARCH)/Makefile
