//
//  boot.c
//  PhaseOS-C28xx
//
//  Created by Carl Friess on 17/08/2018.
//  Copyright © 2018 Carl Friess. All rights reserved.
//

#include <F2837xD_device.h>
#include "sys_pll.h"
#include <kernel.h>

extern void F28x_usDelay(long LoopCount);

#define CPU_RATE   5.00L   // for a 200MHz CPU clock speed (SYSCLKOUT)
#define DELAY_US(A)  F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

// Called by startup routine before memory is initialized. Can be used for
// application specific low level initialization instructions
int _system_pre_init(void)
{
    
    // Enable pull-ups on unbonded IOs as soon as possible to reduce power
    // consumption.

    // bits 8-10 have pin count
    unsigned char pin_count = ((DevCfgRegs.PARTIDL.all & 0x00000700) >> 8) ;

    // 5 = 100 pin
    // 6 = 176 pin
    // 7 = 337 pin
    if(pin_count == 5) {
        EALLOW;
        GpioCtrlRegs.GPCPUD.all = ~0x80000000;  // GPIO 95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  // GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  // GPIOs 128-159 except for 133
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  // GPIOs 160-168
        EDIS;
    }
    else if (pin_count == 6) {
        EALLOW;
        GpioCtrlRegs.GPAPUD.all = ~0xFFC003E3;  // GPIOs 0-1, 5-9, 22-31
        GpioCtrlRegs.GPBPUD.all = ~0x03FFF1FF;  // GPIOs 32-40, 44-57
        GpioCtrlRegs.GPCPUD.all = ~0xE10FBC18;  // GPIOs 67-68, 74-77, 79-83, 93-95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  // GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFFF;  // GPIOs 128-159
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  // GPIOs 160-168();
        EDIS;
    }
    else {
        //do nothing - this is 337 pin package
    }
    
    
    // Check if device is trimmed
    
    EALLOW;
    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_D = 1;
    
    if(*((Uint16 *)0x5D1B6) == 0x0000){
        // Device is not trimmed--apply static calibration values
        AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
    }
    
    CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
    CpuSysRegs.PCLKCR13.bit.ADC_D = 0;
    EDIS;
    
    
    // Disable Watchdog?
    
    
    // Set up system clock
    // This gives us a 200 MHz system clock for an extenal 10MHz oscilator
    InitSysPll(XTAL_OSC, IMULT_40, FMULT_0, PLLCLK_BY_2);
    
    
    
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
    
    kernel_start();
    
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
