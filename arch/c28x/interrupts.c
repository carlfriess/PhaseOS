//
//  interrupt.c
//  PhaseOS-c28x
//
//  Created by Carl Friess on 01/01/2019.
//  Copyright © 2019 Phase. All rights reserved.
//

#include "interrupts.h"

#include <string.h>
#include <stdint.h>
#include <F2837xD_device.h>

static __interrupt void c28x_interrupt(void);

void interrupts_init()
{
    
    // Disable Interrupts at the CPU level:
    DINT;
    IER = 0x0000;           // Disable maskable CPU interrupts
    IFR = 0x0000;           // Clear maskable CPU interrupt flags
    
    // Disable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;
    
    // Clear all PIEIER (interrupt enable) registers:
    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;
    
    // Clear all PIEIFR (interrupt flag) registers:
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;
    
    EALLOW;
    
    // Clear all interrupt vectors except for RESET
    uint32_t *vect_table = ((uint32_t *) &PieVectTable) + 1;
    memset((void *) vect_table, 0, sizeof(PieVectTable) - 1);
    
    // Set initial interrupt vectors
    PieVectTable.USER1_INT = c28x_interrupt;    // User software interrupt
    
    EDIS;
    
    // Enable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    
    // Enable Interrupts at the CPU level:
    EINT;
    ERTM;   // Globally enable real-time interrupt DBGM

    
}

__interrupt void c28x_interrupt()
{
    
}
