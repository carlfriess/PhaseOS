//
//  interrupt.h
//  PhaseOS-c28x
//
//  Created by Carl Friess on 01/01/2019.
//  Copyright © 2019 Phase. All rights reserved.
//

#ifndef interrupt_h
#define interrupt_h

/**
 *  Initialises PIE and ePIE hardware and sets initial interrupt vectors.
 */
void interrupts_init(void);

#endif /* interrupt_h */
