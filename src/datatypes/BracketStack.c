#include <stdlib.h>

#include "ProcedureStack.h"

typedef struct BracketPos
{
    char symbol;
    size_t address;
    Procedure* procedure;
} BracketPos;

typedef struct BracketStack
{
    BracketPos* data;
    size_t cap;
    size_t size;
} BracketStack;

BracketStack*
BracketStack_new(const size_t cap)
{
    BracketStack* this = calloc(1, sizeof(BracketStack));
    this->data = calloc(cap, sizeof(BracketPos));
    this->cap = cap;
    this->size = 0;
    return this;
}

void
BracketStack_free(BracketStack* this)
{
    free(this->data);
    free(this);
}

void
BracketStack_resize(BracketStack* this, const size_t cap)
{
    this->data = realloc(this->data, cap);
    this->cap = cap;
}

void
BracketStack_append(BracketStack* this, const BracketPos item)
{
    if (this->size >= this->cap)
    {
        BracketStack_resize(this, (size_t) this->cap * 1.5);
    }
    this->data[this->size] = item;
    this->size++;
}

BracketPos
BracketStack_pop(BracketStack* this)
{
    BracketPos value = this->data[this->size - 1];
    this->data[this->size - 1] = (BracketPos) { .symbol = '\0', .address = 0, .procedure = NULL };
    this->size--;
    return value;
}
