#include "sys_pll.h"

//
// SysIntOsc1Sel - This function switches to Internal Oscillator 1.
//
void SysIntOsc1Sel(void)
{
    EALLOW;
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 2;     // Clk Src = INTOSC1
    EDIS;
}

//
// SysIntOsc2Sel - This function switches to Internal oscillator 2.
//
void SysIntOsc2Sel(void)
{
    EALLOW;
    ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF=0;         // Turn on INTOSC2
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0;     // Clk Src = INTOSC2
    EDIS;
}

//
// SysXtalOscSel - This function switches to External CRYSTAL oscillator.
//
void SysXtalOscSel(void)
{
    EALLOW;
    ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF=0;            // Turn on XTALOSC
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;     // Clk Src = XTAL
    EDIS;
}

//
// InitSysPll - This function initializes the PLL registers.
//
// Note: The internal oscillator CANNOT be used as the PLL source if the
// PLLSYSCLK is configured to frequencies above 194 MHz.
//
// Note: This function uses the Watchdog as a monitor for the PLL. The user
// watchdog settings will be modified and restored upon completion.
//
void InitSysPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel)
{
    Uint16 SCSR, WDCR, WDWCR, intStatus;
    if((clock_source == ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)    &&
       (imult        == ClkCfgRegs.SYSPLLMULT.bit.IMULT)           &&
       (fmult        == ClkCfgRegs.SYSPLLMULT.bit.FMULT)           &&
       (divsel       == ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV))
    {
        //
        // Everything is set as required, so just return
        //
        return;
    }
    
    if(clock_source != ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)
    {
        switch (clock_source)
        {
            case INT_OSC1:
                SysIntOsc1Sel();
                break;
                
            case INT_OSC2:
                SysIntOsc2Sel();
                break;
                
            case XTAL_OSC:
                SysXtalOscSel();
                break;
        }
    }
    
    EALLOW;
    if(imult != ClkCfgRegs.SYSPLLMULT.bit.IMULT ||
       fmult != ClkCfgRegs.SYSPLLMULT.bit.FMULT)
    {
        Uint16 i;
        
        //
        // This bit is reset only by POR
        //
        if(DevCfgRegs.SYSDBGCTL.bit.BIT_0 == 1)
        {
            //
            // The user can optionally insert handler code here. This will only
            // be executed if a watchdog reset occurred after a failed system
            // PLL initialization. See your device user's guide for more
            // information.
            //
            // If the application has a watchdog reset handler, this bit should
            // be checked to determine if the watchdog reset occurred because
            // of the PLL.
            //
            // No action here will continue with retrying the PLL as normal.
            //
        }
        
        //
        // Bypass PLL and set dividers to /1
        //
        ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0;
        asm(" RPT #20 || NOP");
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 0;
        
        //
        // Lock the PLL five times. This helps ensure a successful start.
        // Five is the minimum recommended number. The user can increase this
        // number according to allotted system initialization time.
        //
        for(i = 0; i < 5; i++)
        {
            //
            // Turn off PLL
            //
            ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 0;
            asm(" RPT #20 || NOP");
            
            //
            // Write multiplier, which automatically turns on the PLL
            //
            ClkCfgRegs.SYSPLLMULT.all = ((fmult << 8U) | imult);
            
            //
            // Wait for the SYSPLL lock counter
            //
            while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1)
            {
                //
                // Uncomment to service the watchdog
                //
                // ServiceDog();
            }
        }
    }
    
    //
    // Set divider to produce slower output frequency to limit current increase
    //
    if(divsel != PLLCLK_BY_126)
    {
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel + 1;
    }else
    {
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel;
    }
    
    //
    //      *CAUTION*
    // It is recommended to use the following watchdog code to monitor the PLL
    // startup sequence. If your application has already cleared the watchdog
    // SCRS[WDOVERRIDE] bit this cannot be done. It is recommended not to clear
    // this bit until after the PLL has been initiated.
    //
    
    //
    // Backup User Watchdog
    //
    SCSR = WdRegs.SCSR.all;
    WDCR = WdRegs.WDCR.all;
    WDWCR = WdRegs.WDWCR.all;
    
    //
    // Disable windowed functionality, reset counter
    //
    EALLOW;
    WdRegs.WDWCR.all = 0x0;
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA;
    
    //
    // Disable global interrupts
    //
    intStatus = __disable_interrupts();
    
    //
    // Configure for watchdog reset and to run at max frequency
    //
    WdRegs.SCSR.all = 0x0;
    WdRegs.WDCR.all = 0x28;
    
    //
    // This bit is reset only by power-on-reset (POR) and will not be cleared
    // by a WD reset
    //
    DevCfgRegs.SYSDBGCTL.bit.BIT_0 = 1;
    
    //
    // Enable PLLSYSCLK is fed from system PLL clock
    //
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;
    
    //
    // Delay to ensure system is clocking from PLL prior to clearing status bit
    //
    asm(" RPT #20 || NOP");
    
    //
    // Clear bit
    //
    DevCfgRegs.SYSDBGCTL.bit.BIT_0 = 0;
    
    //
    // Restore user watchdog, first resetting counter
    //
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA;
    
    WDCR |= 0x28;                     // Setup WD key--KEY bits always read 0
    WdRegs.WDCR.all = WDCR;
    WdRegs.WDWCR.all = WDWCR;
    WdRegs.SCSR.all = SCSR & 0xFFFE;  // Mask write to bit 0 (W1toClr)
    
    //
    // Restore state of ST1[INTM]. This was set by the __disable_interrupts()
    // intrinsic previously.
    //
    if(!(intStatus & 0x1))
    {
        EINT;
    }
    
    //
    // Restore state of ST1[DBGM]. This was set by the __disable_interrupts()
    // intrinsic previously.
    //
    if(!(intStatus & 0x2))
    {
        asm(" CLRC DBGM");
    }
    
    //
    // 200 PLLSYSCLK delay to allow voltage regulator to stabilize prior
    // to increasing entire system clock frequency.
    //
    asm(" RPT #200 || NOP");
    
    //
    // Set the divider to user value
    //
    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel;
    EDIS;
}
