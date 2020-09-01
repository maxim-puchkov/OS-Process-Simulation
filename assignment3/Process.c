//
//  Process.c
//  assignment3
//
//  Created by admin on 2018-03-17.
//  Copyright © 2018 maximpuchkov. All rights reserved.
//

#include "Process.h"

int _processIndex = 0;
Process processes[N_PROCESSES];

Process *ProcessNext() {
    return &processes[_processIndex++];
}

void ProcessWillBecomeActive(Process *p) {
    if (p->proc_message && p->blocked) {
        printf(">> Success. Message received\n");
        printf(">> Data(sender:%d, content:\"%s\")\n", p->message_sender->pid, p->proc_message);
        p->message_sender->blocked = false;
        free(p->proc_message);
        p->proc_message = NULL;
        p->blocked = false;
    }
}

void ProcessDidBecomeActive(Process *p) {
    printf(">> Now running Process (pid:%d, priority:%d)\n",
           p->pid, p->priority);
}
