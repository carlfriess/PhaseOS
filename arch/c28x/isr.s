;;
;;  isr.s
;;  PhaseOS-c28x
;;
;;  Created by Carl Friess on 01/01/2019.
;;  Copyright © 2019 Phase. All rights reserved.
;;

    .global     _kernel_invoke
    .global     _current_context


    .sect       ".text:retain"
    .retain
    .retainrefs
    .global     _c28x_interrupt

_c28x_interrupt:

    ASP                 ; Align stack. Will modify SPA bit!

    ; Some CPU registers were pushed onto the stack during the interrupt. Save
    ; the remaining registers onto the stack.
    PUSH    AR1H:AR0H
    PUSH    XAR2
    PUSH    XAR3
    PUSH    XAR4
    PUSH    XAR5
    PUSH    XAR6
    PUSH    XAR7
    PUSH    XT
    PUSH    DP:ST1      ; ST1 needs to be backed up again because of SPA bit.
    PUSH    RPC

    ; Save FPU registers onto the stack
    MOV32   *SP++,R0H
    MOV32   *SP++,R1H
    MOV32   *SP++,R2H
    MOV32   *SP++,R3H
    MOV32   *SP++,R4H
    MOV32   *SP++,R5H
    MOV32   *SP++,R6H
    MOV32   *SP++,R7H
    MOV32   *SP++,STF
    PUSH    RB

    ; Reconfigure execution environment
    SETFLG  RNDF32=1, RNDF64=1
    SPM     #0
    CLRC    OVM|PAGE0
    CLRC    AMODE

    ; Save SP to context
    MOVW    DP,#_current_context
    MOVL    XAR4,@_current_context
    MOV     AL,SP
    MOV     *+XAR4[0],AL

    ; Invoke the kernel
    LCR       #_kernel_invoke

    ; Restore SP from context
    MOVW    DP,#_current_context
    MOVL    XAR4,@_current_context
    MOV     AL,*+XAR4[0]
    MOV     SP,AL

    ; Restore FPU registers
    POP     RB
    MOV32   STF, *--SP
    MOV32   R7H, *--SP
    MOV32   R6H, *--SP
    MOV32   R5H, *--SP
    MOV32   R4H, *--SP
    MOV32   R3H, *--SP
    MOV32   R2H, *--SP
    MOV32   R1H, *--SP
    MOV32   R0H, *--SP

    ; Restore CPU registers
    POP     RPC
    POP     DP:ST1
    POP     XT
    POP     XAR7
    POP     XAR6
    POP     XAR5
    POP     XAR4
    POP     XAR3
    POP     XAR2
    POP     AR1H:AR0H

    SETC    INTM|DBGM

    NASP                ; Undo ASP

    IRET

