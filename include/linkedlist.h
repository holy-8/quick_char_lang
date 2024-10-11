#include <stdlib.h>
#include <stdbool.h>
#include "garbagecollector.h"

#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

typedef struct Node Node;
typedef struct LinkedList LinkedList;

struct Node {
    void* value;
    Node* next;
    Node* prev;
};

Node* NodeNew(void* value);

void NodeDestroy(Node* this);

struct LinkedList {
    size_t size;
    Node* head;
    Node* tail;
    Node* iterNode;
    size_t iterIndex;
};

void LinkedListPush(LinkedList* this, void* value);

void* LinkedListGet(LinkedList* this, size_t index, bool reversed);

void* LinkedListPop(LinkedList* this);

void LinkedListDestroy(void* this);

LinkedList* LinkedListNew(GarbageCollector* gc);

LinkedList* LinkedListReverse(LinkedList* this);

#endif /* __LINKEDLIST_H_ */
