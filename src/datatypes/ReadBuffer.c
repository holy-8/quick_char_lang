#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"

ReadBuffer*
ReadBuffer_new(FILE* file, const size_t capacity)
{
    ReadBuffer* this = calloc(1, sizeof(ReadBuffer));
    this->file = file;
    this->buffer = calloc(capacity, sizeof(char));
    this->capacity = capacity;
    this->length = 0;
    return this;
}

void
ReadBuffer_free(ReadBuffer* this)
{
    free(this->buffer);
    free(this);
}

bool
ReadBuffer_read(ReadBuffer* this)
{
    char current_char = fgetc(this->file);
    if (current_char == EOF)
    {
        fclose(this->file);
        return false;
    }
    this->buffer[this->length] = current_char;
    this->length++;
    return true;
}

char
ReadBuffer_pop(ReadBuffer* this)
{
    char last_char = this->buffer[this->length - 1];
    this->buffer[this->length - 1] = 0;
    this->length--;
    return last_char;
}

char
ReadBuffer_peek(ReadBuffer* this)
{
    return this->buffer[this->length - 1];
}

void
ReadBuffer_clear(ReadBuffer* this)
{
    for (size_t i = 0; i < this->length; i++)
    {
        this->buffer[i] = 0;
    }
    this->length = 0;
}
