//
//  kernel.h
//  PhaseOS
//
//  Created by Carl Friess on 30/09/2018.
//  Copyright © 2018 Phase. All rights reserved.
//

#ifndef kernel_h
#define kernel_h

#include <stddef.h>

struct sys_mem_block {
    void *base;
    size_t size;
};

/**
 *  Called after system has been initialised to hand over control to the kernel.
 *  @param mem_blocks An array of system memory blocks for the kernel to use and
 *  allocate to user applications.
 *  @param num_mem_blocks Length of mem_blocks array.
 */
void kernel_start(struct sys_mem_block *mem_blocks, size_t num_mem_blocks);

/**
 *  Kernel invokation: syscall, system tick, trap, etc.
 */
void kernel_invoke(void);

#endif /* kernel_h */
