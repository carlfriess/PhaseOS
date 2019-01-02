//
//  context.h
//  PhaseOS-c28x
//
//  Created by Carl Friess on 01/01/2019.
//  Copyright © 2019 Phase. All rights reserved.
//

#ifndef context_h
#define context_h

#include <stdint.h>

struct context {
    uint16_t sp;
};

extern struct context *current_context;

#endif /* context_h */
