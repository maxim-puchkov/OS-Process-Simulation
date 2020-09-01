//
//  Process.h
//  assignment3
//
//  Created by admin on 2018-03-17.
//  Copyright © 2018 maximpuchkov. All rights reserved.
//

#ifndef Process_h
#define Process_h

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Semaphore.h"

#define N_PROCESSES 1000

typedef struct Process {
    int pid;
    int priority;
    
    bool initial;
    bool blocked;
    
    char *proc_message;
    struct Process* message_sender;
    
    // executes semaphore P, V operations
    void (*execute)(void(struct Semaphore *), struct Semaphore *);
} Process;

/**
 Choose next process from a list.
 @returns Process's memory address.
 */
Process *ProcessNext(void);

/**
 Checks if this process is waiting for messages. If it is and one arrived,
 unblock it.
 */
void ProcessWillBecomeActive(Process *);

/**
 Displays running process information.
 */
void ProcessDidBecomeActive(Process *);

#endif /* Process_h */
