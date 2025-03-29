#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct VMState VMState;
typedef struct InstructionStack InstructionStack;

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

typedef struct ShortStack
{
    short* data;
    size_t cap;
    size_t size;
} ShortStack;

typedef struct Variable
{
    bool is_defined;
    short value;
} Variable;

typedef struct Procedure
{
    VMState* vm;
    Variable local_vars[52];
    InstructionStack* iarr;
    size_t iptr;
} Procedure;

typedef struct ProcedureStack
{
    Procedure** data;
    size_t cap;
    size_t size;
} ProcedureStack;

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

struct InstructionStack
{
    Instruction* data;
    size_t cap;
    size_t size;
};

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

typedef struct ProcedureVariable
{
    bool is_defined;
    Procedure* value;
} ProcedureVariable;

struct VMState
{
    ShortStack* stack;
    Variable global_vars[52];
    ProcedureVariable procedures[52];
    ProcedureStack* call_stack;
    ProcedureStack* all_procedures;
};
