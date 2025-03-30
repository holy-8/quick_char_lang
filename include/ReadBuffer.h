#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"

ReadBuffer*
ReadBuffer_new(FILE* file, const size_t capacity);


void
ReadBuffer_free(ReadBuffer* this);


bool
ReadBuffer_read(ReadBuffer* this);


char
ReadBuffer_pop(ReadBuffer* this);


char
ReadBuffer_peek(ReadBuffer* this);


void
ReadBuffer_clear(ReadBuffer* this);

/* Generated from src/datatypes/ReadBuffer.c */
