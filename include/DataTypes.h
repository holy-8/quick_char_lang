#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct VMState VMState;
typedef struct InstructionStack InstructionStack;

typedef enum InstructionType
{
    iNone,
    iPushZero,
    iIncrement,
    iDecrement,
    iAdd,
    iSubtract,
    iReverse,
    iReverseNull,
    iAssign,
    iAssignNull,
    iDelete,
    iDeleteNull,
    iPush,
    iPushNull,
    iAssignLocal,
    iAssignLocalNull,
    iOutput,
    iOutputNull,
    iInput,
    iInputNull,
    iDefineProcedure,
    iDefineProcedureNull,
    iCall,
    iCallNull,
    iInfiniteStart,
    iInfiniteStartNull,
    iInfiniteEnd,
    iRepeatStart,
    iRepeatStartNull,
    iRepeatEnd,
    iEnd,
    iContinue,
    iConditionalStart,
    iConditionalStartNull,
    iConditionalEnd
} InstructionType;

typedef struct ShortStack
{
    short* data;
    size_t capacity;
    size_t length;
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
    InstructionStack* instruction_array;
    long long instruction_pointer;
} Procedure;

typedef struct ProcedureStack
{
    Procedure** data;
    size_t capacity;
    size_t length;
} ProcedureStack;

typedef struct Instruction
{
    InstructionType type;
    char argument;
    struct
    {
    Procedure* procedure;
    size_t jump_address;
    size_t counter;
    } optional;
} Instruction;

struct InstructionStack
{
    Instruction* data;
    size_t capacity;
    size_t length;
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
    size_t capacity;
    size_t length;
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

typedef struct ReadBuffer
{
    FILE* file;
    char* buffer;
    size_t capacity;
    size_t length;
} ReadBuffer;
/* Generated from src/datatypes/DataTypes.c */
