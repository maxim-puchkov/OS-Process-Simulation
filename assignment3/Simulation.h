//
//  Simulation.h
//  assignment3
//
//  Created by admin on 2018-03-17.
//  Copyright © 2018 maximpuchkov. All rights reserved.
//

#ifndef Simulation_h
#define Simulation_h

#define N_QUEUES 3
#define N_COMMANDS 13
#define MESSAGE_SIZE 40

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Process.h"
#include "Semaphore.h"
#include "LIST.h"

// MARK: - Simulation Controls

/**
 Prepare simulation for starting.
 */
void SimulationBegin(void);

/**
 Read user commands and execute them.
 */
void SimulationReadCommand(void);

/**
 Prepare simulation for finishing.
 */
void SimulationTerminate(void);

/**
 Display the list of all available commands.
 */
void SimulationDisplayCommandList(void);

/**
 Check if simmulation is running.
 @returns YES if Process(pid:0) exists. NO otherwise.
 */
bool SimulationIsRunning(void);

// MARK: - Helpers

/**
 Bound check priority.
 @returns YES if within bounds. NO otherwise.
 */
bool validPriority(int);

/**
 Convert char input to int.
 @return Converted char.
 */
int toInt(char);

/**
 Get command index from command character.
 @returns Command index.
 */
int getCommand(char);

/**
 Run command with its index.
 */
void command_run(int);

/**
 Execute passed-in function on behalf of some process.
 
 @code
 process->execute = &exec;
 process->execute(func, arg);
 @endcode
 */
void exec(void (*)(struct Semaphore *), struct Semaphore *);

/**
 Print all queues.
 */
void printAllQueues(void);

/**
 Adds process to its queue.
 */
void addToQueue(Process *);

/**
 Removes process from its queue.
 */
void removeFromQueue(Process *);

/**
 Find process by its Process ID.
 @returns Pointer to Process if found. NULL otherwise.
 */
Process *findProcess(int);

/**
 Create new default process with specified priority.
 @returns Pointer to new Process.
 */
Process *newProcessWithPriority(int);

/**
 Choose the next highest priority process to execute.
 Will always execute higher priority queues first.
 @returns Pointer to Process if found. NULL otherwise.
 */
Process *nextHighest(void);

// MARK: - Commands

void Create(void);

void Fork(void);

void Kill(void);

void Exit(void);

void Quantum(void);

void Send(void);

void Receive(void);

void Reply(void);

void NewSemaphore(void);

void SemaphoreP(void);

void SemaphoreV(void);

void Procinfo(void);

void Totalinfo(void);

#endif /* Simulation_h */
