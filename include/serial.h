//
//  serial.h
//  PhaseOS-C28xx-doc
//
//  Created by Carl Friess on 30/09/2018.
//  Copyright © 2018 Phase. All rights reserved.
//

#ifndef serial_h
#define serial_h

/**
 *  Initialises the system debug serial interface.
 */
void serial_init(void);

/**
 *  Prints a character to the system debug serial interface.
 *  @param c The character to print.
 */
void serial_putchar(char c);

/**
 *  Reads a character from the system debug serial interface. Blocks until a
 *  a character has been received.
 *  @return The character that was read.
 */
char serial_getchar(void);

#endif /* serial_h */
