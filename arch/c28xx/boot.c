//
//  boot.c
//  PhaseOS-C28xx
//
//  Created by Carl Friess on 17/08/2018.
//  Copyright © 2018 Carl Friess. All rights reserved.
//

extern void F28x_usDelay(long LoopCount);

#define CPU_RATE   5.00L   // for a 200MHz CPU clock speed (SYSCLKOUT)
#define DELAY_US(A)  F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

void _args_main (void) {
    volatile unsigned int *GPADIR = (unsigned int *) 0x00007C0A;
    volatile unsigned int *GPADAT = (unsigned int *) 0x00007F00;
    __asm("    EALLOW");
    *GPADIR |= 0x1000;
    __asm("    EDIS");
    while (1) {
        *GPADAT ^= 0x1000;
        DELAY_US(100000);
    }
}

void _system_post_cinit (void) {
    
}

void _system_pre_init (void) {
    
}

void copy_in (void) {
    
}

void exit (void) {
    
}
