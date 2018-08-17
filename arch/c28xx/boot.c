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

// Called by startup routine before memory is initialized. Can be used for
// application specific low level initialization instructions
int _system_pre_init(void)
{
    // Return non-zero value to perform C/C++ global data initialization
    return 1;
}

// Called by startup routine to process .binit copy table used for copying some
// sections from FLASH to RAM at startup.
void copy_in(void *copy_table)
{
    return;
}

// Called by startup routine after .cinit has been processed and before .pinit
// is processed.
void _system_post_cinit(void)
{
    return;
}

// Called at the end of the startup routine when startup is complete.
void _args_main(void)
{
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

// Called after _args_main returns.
void exit(void)
{
    // Spin forever
    for (;;);
}
