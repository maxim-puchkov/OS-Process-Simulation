//
//  LIST.h
//  Assignment1
//
//  Created by admin on 2018-01-26.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#ifndef LIST_h
#define LIST_h

#include <stdio.h>

#define SUCCESS 0
#define FAILURE -1
#define MAX_NODES 50
#define MAX_LISTS 3

// Node type
typedef struct Node {
    void *item;
    struct Node *next;
    struct Node *prev;
} Node;

// LIST type
typedef struct LIST {
    struct Node *head;
    struct Node *tail;
    struct Node *current;
    struct LIST *next;
    int length;
} LIST;

// Static arrays
Node allNodes[MAX_NODES];
LIST allLists[MAX_LISTS];

// Memory address of the next free node/list
Node *emptyNode;
LIST *emptyList;

/* Public Interface */
LIST *ListCreate(void);

int ListCount(LIST *);

void *ListFirst(LIST *);

void *ListLast(LIST *);

void *ListNext(LIST *);

void *ListPrev(LIST *);

void *ListCurr(LIST *);

int ListAdd(LIST *, void *);

int ListInsert(LIST *, void *);

int ListAppend(LIST *, void *);

int ListPrepend(LIST *, void *);

void *ListRemove(LIST *);

void ListConcat(LIST *, LIST *);

void ListFree(LIST *, void (*)(void *));

void *ListTrim(LIST *);

void *ListSearch(LIST *list, int (*)(void *, void *), char *);

#endif /* LIST_h */
