//
//  LIST.c
//  Assignment1
//
//  Created by admin on 2018-01-26.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#include "LIST.h"

/* Private Functions */
Node *MakeNode(void *item, Node *next, Node *prev) {
    if (emptyNode) {
        Node *node = emptyNode;
        emptyNode = emptyNode->next;
        node->item = item;
        node->next = next;
        node->prev = prev;
        return node;
    }
    return NULL;
}

void ListMakeEmpty(LIST *list) {
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    list->length = 0;
}

/* Public Interface */
LIST *ListCreate() {
    if (!allLists[0].next) {
        for (int i = 0; i < MAX_LISTS - 1; i += 1) {
            allLists[i].next = &allLists[i + 1];
        }
        for (int i = 0; i < MAX_NODES - 1; i += 1) {
            allNodes[i].next = &allNodes[i+1];
        }
        
        emptyList = &allLists[0];
        emptyNode = &allNodes[0];
    }
    if (emptyList) {
        LIST *list = emptyList;
        emptyList = emptyList->next;
        list->length = 0;
        return list;
    }
    return NULL;
}

int ListCount(LIST *list) {
    return list->length;
}

void *ListFirst(LIST *list) {
    if (!list->head) return NULL;
    list->current = list->head;
    return list->current->item;
}

void *ListLast(LIST *list) {
    if (!list->tail) return NULL;
    list->current = list->tail;
    return list->current->item;
}

void *ListNext(LIST *list) {
    if (!list->current) return NULL;
    if (!list->current->next) return NULL;
    list->current = list->current->next;
    return list->current->item;
}

void *ListPrev(LIST *list) {
    if (!list->current) return NULL;
    list->current = list->current->prev;
    return list->current->item;
}

void *ListCurr(LIST *list) {
    return list->current->item;
}

int ListAdd(LIST *list, void *item) {
    if (!list->current) {
        list->current = list->tail;
    }
    Node *current = list->current;
    Node *node = NULL;
    
    if (!list->head) {
        node = MakeNode(item, NULL, NULL);
        if (!node) return FAILURE;
        
        list->head = node;
        list->tail = node;
        list->current = node;
    } else if (current) {
        node = MakeNode(item, current->next, current);
        if (!node) return FAILURE;
        
        if (list->current->next == NULL) {
            list->current->next = node;
            list->current = node;
            list->tail = node;
        } else {
            list->current->next->prev = node;
            list->current->next = node;
            list->current = node;
        }
    }
    
    list->length = list->length + 1;
    return SUCCESS;
}

int ListInsert(LIST *list, void *item) {
    Node *current = list->current;
    Node *node = NULL;
    
    if (!list->head) {
        node = MakeNode(item, NULL, NULL);
        if (!node) return FAILURE;
        
        list->head = node;
        list->tail = node;
        list->current = node;
    } else if (current) {
        node = MakeNode(item, current, current->prev);
        if (!node) return FAILURE;
        
        if (!current->prev) {
            current->prev = node;
            list->head = node;
        } else {
            current->prev->next = node;
            current->prev = node;
        }
        
        current = node;
        
    }
    
    list->length = list->length + 1;
    return SUCCESS;
}

int ListAppend(LIST *list, void *item) {
    Node *current = list->current;
    Node *node = NULL;
    
    if (!list->head) {
        node = MakeNode(item, NULL, NULL);
        if (!node) return FAILURE;
        
        list->head = node;
        list->tail = node;
        list->current = node;
    } else if (current) {
        node = MakeNode(item, NULL, list->tail);
        if (!node) return FAILURE;
        
        list->tail->next = node;
        list->tail = node;
        list->current = node;
    }
    
    list->length = list->length + 1;
    return SUCCESS;
}

int ListPrepend(LIST *list, void *item) {
    Node *current = list->current;
    Node *node = NULL;
    
    if (!list->head) {
        node = MakeNode(item, NULL, NULL);
        if (!node) return FAILURE;
        
        list->head = node;
        list->tail = node;
        list->current = node;
    } else if (current) {
        node = MakeNode(item, list->head, NULL);
        if (!node) return FAILURE;
        
        list->head->prev = node;
        list->head = node;
        list->current = node;
    }
    
    list->length = list->length + 1;
    return SUCCESS;
}

void *ListRemove(LIST *list) {
    Node *current = list->current;
    void *ret = current->item;
    Node *next = list->current->next;
    
    if (list->length == 1) {
        ListMakeEmpty(list);
        emptyList = list;
        return ret;
    }
    if (current == list->head) {
        list->head = list->head->next;
    } else if (current == list->tail) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } else {
        Node *prev = current->prev;
        Node *next = current->next;
        if (prev && next) {
            prev->next = next;
            next->prev = prev;
        }
    }
    
    //emptyNode = current;
    
    list->current = next;
    list->length = list->length - 1;
    
    return ret;
}

void ListConcat(LIST *list_1, LIST *list_2) {
    Node *node = list_2->head;
    while (node) {
        node->prev = list_1->tail;
        list_1->tail = node;
        list_1->tail->prev->next = node;
        list_1->length = list_1->length + 1;
        node = node->next;
    }
    ListMakeEmpty(list_2);
    emptyList = list_2;
}

void ListFree(LIST *list, void (*itemFree)(void *itemToBeFreed)) {
    Node *node = list->head;
    while (node) {
        (*itemFree)(node->item);
        node->prev = NULL;
        node = node->next;
        emptyNode = node;
    }
    ListMakeEmpty(list);
}

void *ListTrim(LIST *list) {
    Node *last = list->tail;
    void *ret = last->item;
    Node *prev = list->tail->prev;
    
    if (list->length == 1) {
        ListMakeEmpty(list);
        emptyList = list;
        return ret;
    }
    
    list->tail = prev;
    list->tail->next = NULL;
    
    //emptyNode = last;
    
    list->current = prev;
    list->length = list->length - 1;
    
    return ret;
}

void *ListSearch(LIST *list, int (*comparator)(void *, void *), char *comparisonArg) {
    Node *node = list->current;
    while (node) {
        if (node->item == comparisonArg) {
            list->current = node;
            return node->item;
        }
        node = node->next;
    }
    return NULL;
}
