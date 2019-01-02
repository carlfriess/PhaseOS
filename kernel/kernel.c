//
//  kernel.c
//  PhaseOS
//
//  Created by Carl Friess on 30/09/2018.
//  Copyright © 2018 Phase. All rights reserved.
//

#include <kernel/kernel.h>

#include <serial.h>

void kernel_start(struct sys_mem_block *mem_blocks, size_t num_mem_blocks)
{
    
    serial_init();
    serial_putchar('H');
    serial_putchar('e');
    serial_putchar('l');
    serial_putchar('l');
    serial_putchar('o');
    serial_putchar(serial_getchar());
    
}
