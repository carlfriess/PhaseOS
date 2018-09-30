CC = /Applications/ti/ccsv7/tools/compiler/ti-cgt-c2000_18.1.0.LTS/bin/cl2000
CFLAGS += -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --emit_warnings_as_errors
CFLAGS += --output_file=$@

ARCH_OBJ = boot28.o F2837xD_CodeStartBranch.o F2837xD_usDelay.o boot.o
ARCH_OBJ := $(patsubst %,$(BUILD_DIR)/arch/c28x/%,$(ARCH_OBJ))

$(BUILD_DIR)/PhaseOS: $(ARCH_OBJ) $(KERNEL_OBJS)
	@echo " "
	@echo "Linking target: \033[32m$@\033[39m"
	$(CC) $(CFLAGS) --run_linker --stack_size=0x200 -m"$(BUILD_DIR)/PhaseOS.map" -o "$(BUILD_DIR)/PhaseOS" "arch/c28x/2837xD_RAM_lnk_cpu1.cmd" $(ARCH_OBJ) $(KERNEL_OBJS)
	@echo " "
	@echo "\033[34m**** Build Finished ****\033[39m"

c28x: $(BUILD_DIR)/PhaseOS

c28xclean:
	@echo "Cleaning C28x build files..."
	rm -rf $(BUILD_DIR)


.PHONY: c28x c28xclean
