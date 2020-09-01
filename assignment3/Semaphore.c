//
//  Semaphore.c
//  assignment3
//
//  Created by admin on 2018-03-18.
//  Copyright © 2018 maximpuchkov. All rights reserved.
//

#include "Semaphore.h"

int _sindex = 0;
Semaphore semaphores[N_SEMAPHORES];

Semaphore *SemaphoreCreate() {
    return (_sindex < N_SEMAPHORES) ? &semaphores[_sindex++] : NULL;
}

// Function is not implemented completely
void P(Semaphore *s) {
    printf(">> Success. Semaphore's P operation called\n");
    
    s->val -= 1;
    if (s->val < 0) {
        // add process to s->list
        // block
    }
}

// Function is not implemented completely
void V(Semaphore *s) {
    printf(">> Success. Semaphore's V operation called\n");
    
    s->val += 1;
    if (s->val <= 0) {
        // p = Get a process from s->list
        // wakeup(p)
    }
}
