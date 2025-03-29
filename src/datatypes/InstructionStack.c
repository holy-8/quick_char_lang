#include <stdlib.h>

#include "DataTypes.h"

InstructionStack*
InstructionStack_new(const size_t cap)
{
    InstructionStack* this = calloc(1, sizeof(InstructionStack));
    this->data = calloc(cap, sizeof(Instruction));
    this->cap = cap;
    this->size = 0;
    return this;
}

void
InstructionStack_free(InstructionStack* this)
{
    free(this->data);
    free(this);
}

void
InstructionStack_resize(InstructionStack* this, const size_t cap)
{
    this->data = realloc(this->data, cap);
    this->cap = cap;
}

void
InstructionStack_append(InstructionStack* this, Instruction item)
{
    if (this->size >= this->cap)
    {
        InstructionStack_resize(this, (size_t) this->cap * 1.5);
    }
    this->data[this->size] = item;
    this->size++;
}
