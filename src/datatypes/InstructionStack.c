#include <stdlib.h>

#include "ProcedureStack.h"

typedef enum InstructionType
{
    iPushZero,
    iIncrement,
    iDecrement,
    iAdd,
    iSubtract,
    iReverse,
    iAssign,
    iDelete,
    iPush,
    iAssignLocal,
    iOutput,
    iInput,
    iDefineProcedure,
    iCall,
    iInfiniteStart,
    iInfiniteEnd,
    iRepeatStart,
    iRepeatEnd,
    iEnd,
    iContinue,
    iConditionalStart,
    iConditionalEnd
} InstructionType;

typedef struct Instruction
{
    InstructionType type;
    char arg;
    union
    {
    Procedure* procedure;
    size_t jumpptr;
    size_t counter;
    } optional;
} Instruction;

typedef struct InstructionStack
{
    Instruction* data;
    size_t cap;
    size_t size;
} InstructionStack;

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
InstructionStack_append(InstructionStack* this, const Instruction item)
{
    if (this->size >= this->cap)
    {
        InstructionStack_resize(this, (size_t) this->cap * 1.5);
    }
    this->data[this->size] = item;
    this->size++;
}
