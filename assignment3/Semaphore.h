//
//  Semaphore.h
//  assignment3
//
//  Created by admin on 2018-03-18.
//  Copyright © 2018 maximpuchkov. All rights reserved.
//

#ifndef Semaphore_h
#define Semaphore_h

#define N_SEMAPHORES 5

#include <stdio.h>
#include "Semaphore.h"

typedef struct Semaphore {
    struct LIST *list; // blocked on this semaphore
    int val;
} Semaphore;

Semaphore *SemaphoreCreate(void);

void P(Semaphore *);

void V(Semaphore *);

#endif /* Semaphore_h */
