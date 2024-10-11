#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "panic.h"
#include "garbagecollector.h"

typedef struct Node Node;
typedef struct LinkedList LinkedList;

struct Node {
    void* value;
    Node* next;
    Node* prev;
};

Node* NodeNew(void* value) {
    Node* this = panicMalloc(sizeof(Node), "at file linkedlist.c, line 17");
    this->value = value;
    this->next = NULL;
    this->prev = NULL;
    return this;
}

void NodeDestroy(Node* this) {
    if (this->prev != NULL)
        this->prev->next = this->next;
    if (this->next != NULL)
        this->next->prev = this->prev;
    free(this);
}

struct LinkedList {
    size_t size;
    Node* head;
    Node* tail;
    Node* iterNode;
    size_t iterIndex;
};

void LinkedListPush(LinkedList* this, void* value) {
    Node* node = NodeNew(value);
    if (this->size == 0) {
        this->head = node;
        this->tail = node;
    }
    else {
        node->prev = this->tail;
        this->tail->next = node;
        this->tail = node;
    }
    this->size++;
}

void* LinkedListGet(LinkedList* this, size_t index, bool reversed) {
    if (index >= this->size)
        return NULL;
    if (this->iterNode == NULL || index < this->iterIndex) {
        this->iterNode = reversed ? this->tail : this->head;
        this->iterIndex = 0;
    }
    for (; this->iterIndex < index; this->iterIndex++)
        this->iterNode = reversed ? this->iterNode->prev : this->iterNode->next;
    return this->iterNode->value;
}

void* LinkedListPop(LinkedList* this) {
    if (this->size == 0)
        return NULL;
    void* value = this->tail->value;
    Node* prev = this->tail->prev;
    NodeDestroy(this->tail);
    this->head = this->size == 1 ? NULL : this->head;
    this->tail = prev;
    this->size--;
    return value;
}

void LinkedListDestroy(void* this) {
    Node* node = ((LinkedList*)this)->head;
    Node* next = node == NULL ? NULL : node->next;
    while (node != NULL) {
        free(node);
        node = next;
        next = node == NULL ? NULL : node->next;
    }
    free(this);
}

LinkedList* LinkedListNew(GarbageCollector* gc) {
    LinkedList* this = panicMalloc(sizeof(LinkedList), "at file linkedlist.c, line 90");
    this->size = 0;
    this->head = NULL;
    this->tail = NULL;
    this->iterIndex = 0;
    this->iterNode = NULL;
    GarbageCollectorAddReference(gc, this, LinkedListDestroy);
    return this;
}

LinkedList* LinkedListReverse(GarbageCollector* gc, LinkedList* this) {
    LinkedList* reversed = LinkedListNew(gc);
    for (size_t i = 0; i < this->size; i++)
        LinkedListPush(reversed, LinkedListGet(this, i, true));
    return reversed;
}
