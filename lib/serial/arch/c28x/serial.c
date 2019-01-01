//
//  serial.c
//  PhaseOS-c28x
//
//  Created by Carl Friess on 30/09/2018.
//  Copyright © 2018 Phase. All rights reserved.
//

#include <serial.h>

#include <F2837xD_device.h>
#include <F2837xD_gpio.h>

void serial_init()
{
    
    EALLOW;
    GpioCtrlRegs.GPCDIR.bit.GPIO85 = 0;     // Input
    GpioCtrlRegs.GPCDIR.bit.GPIO84 = 1;     // Output
    GpioCtrlRegs.GPCPUD.bit.GPIO85 = 1;     // Pullup disabled
    GpioCtrlRegs.GPCQSEL2.bit.GPIO84 = 3;   // Async
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;    // SCITXA
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;    // SCIRXA
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    
    CpuSysRegs.PCLKCR7.bit.SCI_A = 1;       // Enable peripheral clock
    EDIS;
    
    SciaRegs.SCICCR.all = 0x0007;           // 1 stop bit,  No loopback
                                            // No parity,8 char bits,
                                            // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;          // enable TX, RX, internal SCICLK,
                                            // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0000;
    SciaRegs.SCIFFTX.all = 0xE000;          // Enable RX and TX FIFO
    
    SciaRegs.SCIHBAUD.all = 0x0000;         // Set BAUD rate at 115200 (115740 (+0,47%))
    SciaRegs.SCILBAUD.all = 0x0035;         // @LSPCLK = 50 MHz (200 MHz SYSCLK)
    
    SciaRegs.SCICTL1.bit.SWRESET = 1;       // Release SCI from Reset
    
}

void serial_putchar(char c)
{
    
    while (SciaRegs.SCIFFTX.bit.TXFFST >= 0x10);
    SciaRegs.SCITXBUF.bit.TXDT = c;
    
}

char serial_getchar()
{
    
    while (SciaRegs.SCIFFRX.bit.RXFFST == 0);
    return SciaRegs.SCIRXBUF.bit.SAR;
    
}

