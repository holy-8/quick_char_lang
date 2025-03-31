#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"
#include "InstructionStack.h"

Procedure*
Procedure_new(VMState* vm)
{
    Procedure* this = calloc(1, sizeof(Procedure));
    this->vm = vm;
    for (size_t i = 0; i < 52; i++)
    {
        this->local_vars[i] = (Variable) { .is_defined = false };
    }
    this->instruction_array = InstructionStack_new(8);
    this->instruction_pointer = 0;
    return this;
}

void
Procedure_free(Procedure* this)
{
    InstructionStack_free(this->instruction_array);
    free(this);
}

Procedure*
Procedure_copy(Procedure* this)
{
    Procedure* copy = Procedure_new(this->vm);
    for (size_t i = 0; i < this->instruction_array->length; i++)
    {
        InstructionStack_append(
            copy->instruction_array, this->instruction_array->data[i]
        );
    }
    return copy;
}

ProcedureStack*
ProcedureStack_new(const size_t capacity)
{
    ProcedureStack* this = calloc(1, sizeof(ProcedureStack));
    this->data = calloc(capacity, sizeof(Procedure*));
    this->capacity = capacity;
    this->length = 0;
    return this;
}

void
ProcedureStack_free(ProcedureStack* this)
{
    for (size_t i = 0; i < this->length; i++)
    {
        Procedure_free(this->data[i]);
    }
    free(this->data);
    free(this);
}

void
ProcedureStack_resize(ProcedureStack* this, const size_t capacity)
{
    this->data = realloc(this->data, capacity * sizeof(Procedure*));
    this->capacity = capacity;
}

void
ProcedureStack_append(ProcedureStack* this, Procedure* item)
{
    if (this->length >= this->capacity)
    {
        ProcedureStack_resize(this, (size_t) (this->capacity * 1.5));
    }
    this->data[this->length] = item;
    this->length++;
}
