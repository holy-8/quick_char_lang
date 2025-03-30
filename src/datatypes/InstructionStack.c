#include <stdlib.h>

#include "DataTypes.h"

InstructionStack*
InstructionStack_new(const size_t capacity)
{
    InstructionStack* this = calloc(1, sizeof(InstructionStack));
    this->data = calloc(capacity, sizeof(Instruction));
    this->capacity = capacity;
    this->length = 0;
    return this;
}

void
InstructionStack_free(InstructionStack* this)
{
    free(this->data);
    free(this);
}

void
InstructionStack_resize(InstructionStack* this, const size_t capacity)
{
    this->data = realloc(this->data, capacity);
    this->capacity = capacity;
}

void
InstructionStack_append(InstructionStack* this, Instruction item)
{
    if (this->length >= this->capacity)
    {
        InstructionStack_resize(this, (size_t) this->capacity * 1.5);
    }
    this->data[this->length] = item;
    this->length++;
}
