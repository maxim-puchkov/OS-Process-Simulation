//
//  Simulation.c
//  assignment3
//
//  Created by admin on 2018-03-17.
//  Copyright © 2018 maximpuchkov. All rights reserved.
//

#include "Simulation.h"

// MARK: Global variables

int _size = 0;
int _index = 0;
Process *_initial = NULL;
Process *_running = NULL;
LIST *queues[N_QUEUES];
Semaphore *semaphores[N_SEMAPHORES];

int dynamicCount = 0;

// MARK: - Commands Helpers

char *commands_names[N_COMMANDS] = {
    "Create", "Fork",
    "Kill", "Exit", "Quantum",
    "Send", "Receive", "Reply",
    "New Semaphore", "Semaphore P Operation", "Semaphore V Operation",
    "Process Info", "Total Info"
};

char commands_id[N_COMMANDS] = {
    'C', 'F',
    'K', 'E', 'Q',
    'S', 'R', 'Y',
    'N', 'P', 'V',
    'I', 'T'
};

void (*commands_func[N_COMMANDS])(void) = {
    &Create, &Fork,
    &Kill, &Exit, &Quantum,
    &Send, &Receive, &Reply,
    &NewSemaphore, &SemaphoreP, &SemaphoreV,
    &Procinfo, &Totalinfo
};

void command_run(int cid) {
    printf(">> Executing Simulation command: %c [%s]\n",
           commands_id[cid], commands_names[cid]);
    commands_func[cid]();
}

// MARK: - Simulation Controls

void SimulationBegin() {
    printf(">> Simulation has begun. Enter your commands\n");
    
    for (int i = 0; i < N_QUEUES; i += 1) {
        queues[i] = ListCreate();
    }
    for (int i = 0; i < N_SEMAPHORES; i += 1) {
        semaphores[i] = NULL;
    }
    Process *p = newProcessWithPriority(2);
    p->initial = true;
    addToQueue(p);
    _running = p;
    _initial = p;
}

void SimulationReadCommand() {
    bool done = false;
    char input;
    while (!done) {
        scanf(" %c", &input);
        int command = getCommand(input);
        
        done = command != -1;
        if (!done) {
            printf(">> Not a valid command: %c\n", input);
        } else {
            command_run(command);
        }
    }
}

void SimulationTerminate() {
    int scount = 0;
    for (int i = 0; i < N_SEMAPHORES; i += 1) semaphores[i] && ++scount;
    printf("\n>> Simulation will now terminate\n");
    
    // Show statistics and statuses
    printf("\tTotal processes remaining: %d ", _size);
    printf("(%s)\n", _size == 0 ? "OK" : "ERROR");
    
    printf("\tRunning process: %d ", _running ? _running->pid : -1);
    printf("(%s)\n", !_running ? "OK" : "ERROR");
    
    printf("\tSemaphores created: %d ", scount);
    printf("(%s)\n", scount >= 0 ? "OK" : "ERROR");
    
    printf("\tDynamic memory loss: %d ", dynamicCount);
    printf("(%s)\n", dynamicCount == 0 ? "OK" : "ERROR");
    
    // Display overall
    if ((_size == 0) && (!_running) && (scount >= 0) && (dynamicCount == 0)) printf(">> Success. No errors found\n");
    else printf(">> Warning. Errors found during execution\n\n");
}

void SimulationDisplayCommandList() {
    int i = 0;
    printf("\nAvailable Commands:\n");
    for (i = 0; i < N_COMMANDS; i += 1) {
        printf("\t%d. %c [%s]\n", (i + 1), commands_id[i], commands_names[i]);
    }
    printf("\n");
}

bool SimulationIsRunning() {
    return (_size > 0) ? true : false;
}

// MARK: - Helper functions

bool validPriority(int priority) {
    return priority >= 0 && priority < N_QUEUES;
}

int toInt(char c) {
    return c - '0';
}

int getCommand(char command) {
    int index = -1;
    for (int i = 0; i < N_COMMANDS; i += 1) {
        if (command == commands_id[i]) {
            index = i;
            break;
        }
    }
    return index;
}

void exec(void (*func)(Semaphore *), Semaphore *arg) {
    // TODO: Call in new thread and may busy wait semaphores
    func(arg);
}

void printAllQueues() {
    // For all queues
    for (int i = 0; i < N_QUEUES; i += 1) {
        LIST *queue = queues[i];
        
        // Print all elements
        ListFirst(queue);
        Node *current = queue->head;
        printf("\tQueue %d. {", i);
        while (current) {
            Process *p = (Process *)current->item;
            printf("(pid:%d", (int)p->pid);
            if (p->proc_message) printf(", msg:\"%s\"", p->proc_message);
            if (p->blocked) printf(", BLOCKED");
            if (p == _running) printf(", RUNNING");
            printf(")");
            if ((current = current->next)) printf(", ");
        }
        printf("}; Size=%d\n", queue->length);
    }
}

void addToQueue(Process *p) {
    LIST *queue = queues[p->priority];
    ListLast(queue);
    ListAdd(queue, p);
    if (_running == _initial) _running = p;
    _size++;
}

void removeFromQueue(Process *p) {
    if (_size > 1 && p == _initial) {
        printf(">> Fail. Running process (pid:%d) cannot be removed now\n", p->pid);
        return;
    }
    
    LIST *queue = queues[p->priority];
    
    Process *other = (Process *)ListFirst(queue);
    Node *node = queue->head;
    while (node) {
        if (other->pid == p->pid) {
            Process *removed = ListRemove(queue);
            if (p == _running) _running = nextHighest();
            _size--;
            
            // Free dynamic if needed
            if (p->proc_message) {
                printf("\tcalled\n");
                free(p->proc_message);
                p->proc_message = NULL;
                dynamicCount -= 1;
            }
            
            printf(">> Success. Process (pid:%d) removed\n", removed->pid);
            return;
        }
        other = (Process *)ListNext(queue);
        node = node->next;
    }
    
    printf(">> Fail. Element removal failed\n");
}

Process *findProcess(int pid) {
    for (int i = 0; i < N_QUEUES; i += 1) {
        LIST *queue = queues[i];
        Node *current = queue->head;
        while (current) {
            Process *p = (Process *)current->item;
            if (p->pid == pid) return p;
            current = current->next;
        }
    }
    return NULL;
}

Process *newProcessWithPriority(int priority) {
    Process *p = ProcessNext();
    p->pid = _index++;
    p->priority = priority;
    p->blocked = false;
    p->initial = false;
    p->execute = &exec;
    return p;
}

// Chooses the next highest priority process which is not blocked
// * Assumes it will assign the returned process to '_running' process
Process *nextHighest() {
    Process *p;
    for (int i = 0; i < N_QUEUES; i += 1) {
        LIST *queue = queues[i];
        Node *node = queue->head;
        while (node) {
            p = (Process *)node->item;
            
            bool hasMessage = p->proc_message != NULL;
            ProcessWillBecomeActive(p);
            if (hasMessage && p->proc_message == NULL) dynamicCount -= 1; // hf
            
            if (p && !p->blocked && p != _running) {
                ProcessDidBecomeActive(p);
                return p;
            }
            node = node->next;
        }
    }
    //printf(">> Running (pid:0)\n");
    //if (!p) return findProcess(0);
    return NULL;
}

// MARK: - Commands Implementation

/* Create a process and put it on the appropriate ready Q */
void Create() {
    char input;
    printf("Priority (0-%d): ", N_QUEUES - 1);
    scanf(" %c", &input);
    int priority = toInt(input);
    if (!validPriority(priority)) {
        printf(">> Failed. Invalid priority\n");
        return;
    }
    
    Process *p = newProcessWithPriority(priority);
    addToQueue(p);
    
    printf(">> Success. Process(pid:%d, priority:%d) created\n", p->pid, p->priority);
}

/* Copy the currently running process and put it on the ready Q */
void Fork() {
    if (_running->pid == 0) {
        printf(">> Fail. Cannot fork initial process\n");
        return;
    }
    
    Process *p = newProcessWithPriority(_running->priority);
    printf(">> Success. Process(pid:%d, priority:%d) from Process(pid:%d, priority:%d)\n",
           p->pid, p->priority, _running->pid, _running->priority);
    addToQueue(p);
}

/* Kill the named process and remove it from the system */
void Kill() {
    int input;
    printf("PID: ");
    scanf("%d", &input);
    int pid = input;
    
    if (!findProcess(pid)) {
        printf(">> Fail. PID not found\n");
        return;
    }
    Process *p = findProcess(pid);
    removeFromQueue(p);
}

/* Kill the currently running process */
void Exit() {
    removeFromQueue(_running);
}

/* Time quantum of running process expires */
void Quantum() {
    printf(">> Success. Time Quantum of Process (pid:%d, priority:%d) expired\n",
           _running->pid, _running->priority);
    
    // Remove element of a queue and add it to the end
    // Then choose the first element in first (highest priority) queue, not blocked
    // Will not choose lower priority until higher priority processes exist
    LIST *queue = queues[_running->priority];
    ListFirst(queue);
    Process *process = NULL;
    Node *node = queue->head;
    while (node) {
        if (((Process *)node->item)->pid == _running->pid) {
            process = ListRemove(queue);
            ListLast(queue);
            ListAdd(queue, process);
            _running = nextHighest();
            !_running && (_running = _initial); // hotfix
            break;
        }
        node = node->next;
        ListNext(queue);
    }
}

/* Send a message to another process - block until reply */
void Send() {
    // Get input
    int input;
    printf("PID: ");
    scanf("%d", &input);
    int pid = input;
    
    char *msg = malloc(MESSAGE_SIZE); // Might not be freed
    dynamicCount += 1;
    printf("Message: ");
    scanf("\n");
    scanf("%39[^\n]%*c", msg);
    
    
    // Handle errors
    if (_running->pid == 0) {
        printf(">> Fail. Process (pid:0) cannot be blocked\n");
        return;
    }
    if (pid == _running->pid) {
        printf(">> Fail. Can only send to other\n");
        return;
    }
    Process *p = findProcess(pid);
    if (!findProcess(pid)) {
        printf(">> Fail. Process not found\n");
        return;
    }
    
    
    // Send to Process *p
    p->proc_message = msg;
    p->message_sender = _running;
    
    printf(">> Success. Message sent\n");
    printf(">> Data(sender:%d, receiver:%d, content:\"%s\")\n", _running->pid, pid, msg);
    printf(">> Process(pid:%d) is now blocked until replied\n", _running->pid);
    
    _running->blocked = true;
    _running = nextHighest();
}

/* Receive a message - block until one arrives */
void Receive() {
    if (_running == _initial) {
        printf(">> Fail. Cannot block Process(pid:0)\n");
        return;
    }
    
    if (!_running->proc_message) {
        printf(">> Fail. Process(pid:%d) is now blocked and waits for messages\n", _running->pid);
        _running->blocked = true;
        _running = nextHighest();
        return;
    }
    
    printf(">> Success. Message received\n");
    printf(">> Data(sender:%d, content:\"%s\")\n", _running->message_sender->pid, _running->proc_message);
    _running->message_sender->blocked = false;
    free(_running->proc_message);
    _running->proc_message = NULL;
    _running->blocked = false;
}

/* Unblocks sender and delivers reply (when sender is scheduled) */
void Reply() {
    if (!_running->proc_message) {
        printf(">> Fail. Nothing to reply to\n");
        return;
    }
    printf(">> Replying to: \"%s\" from Process(pid:%d)\n",
           _running->proc_message, _running->message_sender->pid);
    
    int input;
    printf("PID: ");
    scanf("%d", &input);
    int pid = input;
    
    char *msg = malloc(MESSAGE_SIZE); // Might not be freed
    dynamicCount += 1;
    printf("Message: ");
    scanf("\n");
    scanf("%39[^\n]%*c", msg);
    
    if (_running->pid == 0) {
        printf(">> Fail. Process (pid:0) cannot be blocked\n");
        return;
    }
    if (pid == _running->pid) {
        printf(">> Fail. Can only send to other\n");
        return;
    }
    Process *p = findProcess(pid);
    if (!p) {
        printf(">> Fail. Process not found\n");
        return;
    }
    
    p->proc_message = msg;
    p->message_sender = _running;
    p->blocked = false;
    free(_running->proc_message);
    _running->proc_message = NULL;
    dynamicCount -= 1;
    
    printf(">> Success. Reply sent\n");
    printf(">> Data(sender:%d, receiver:%d, content:\"%s\")\n", _running->pid, pid, msg);
    printf(">> Process(pid:%d) is now unblocked and will recieve reply\n", pid);
}

/* Initialize the named semaphore with the value given */
void NewSemaphore() {
    int input;
    printf("Semaphore ID: ");
    scanf("%d", &input);
    int sid = input;

    printf("Initial Value (0 or higher): ");
    scanf("%d", &input);
    int initialValue = input;
    
    if (semaphores[sid]) {
        printf(">> Fail. Semaphore(id:%d) already initialized\n", sid);
        return;
    }
    
    Semaphore *s = SemaphoreCreate();
    if (!s) {
        printf(">> Fail. Cannot create new semaphore\n");
        return;
    }
    s->val = initialValue;
    semaphores[sid] = s;
    printf(">> Success. Semaphore(id:%d, val:%d) created\n", sid, s->val);
}

// Execute Semaphore P on behalf of this process
void SemaphoreP() {
    int input;
    printf("Semaphore ID: ");
    scanf("%d", &input);
    if (!semaphores[input]) {
        printf(">> Fail. No semaphore\n");
        return;
    }
    _running->execute(P, semaphores[input]);
}

// Execute Semaphore V on behalf of this process
void SemaphoreV() {
    // Get input
    int input;
    printf("Semaphore ID: ");
    scanf("%d", &input);
    if (!semaphores[input]) {
        printf(">> Fail. No semaphore\n");
        return;
    }
    
    // Execute V
    _running->execute(V, semaphores[input]);
}

/* View process information */
void Procinfo() {
    int pid;
    printf("PID: ");
    scanf("%d", &pid);
    Process *p = findProcess(pid);
    
    if (!p) {
        printf(">> Fail. Process(pid:%d) does not exist\n", pid);
        return;
    }
    
    printf("Process(pid:%d) info\n", pid);
    printf("PID: %d\n", p->pid);
    printf("Priority: %d\n", p->priority);
    printf("Blocked: %s\n", p->blocked ? "YES":"NO");
    printf("Running: %s\n", p == _running ? "YES":"NO");
}

/* View all process queues. Shows RUNNING, BLOCKED, and process messages if applicable */
void Totalinfo() {
    printf(">> Printing all queues\n");
    printAllQueues();
    printf(">> Details\n");
    printf("\tTotal queues: %d\n", N_QUEUES);
    printf("\tTotal processes: %d\n", _size);
    printf("\tCurrently running Process(pid:%d)\n", _running->pid);
}
