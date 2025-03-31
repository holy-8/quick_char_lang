#include <stdlib.h>

#include "DataTypes.h"

BracketStack*
BracketStack_new(const size_t capacity)
{
    BracketStack* this = calloc(1, sizeof(BracketStack));
    this->data = calloc(capacity, sizeof(BracketPos));
    this->capacity = capacity;
    this->length = 0;
    return this;
}

void
BracketStack_free(BracketStack* this)
{
    free(this->data);
    free(this);
}

void
BracketStack_resize(BracketStack* this, const size_t capacity)
{
    this->data = realloc(this->data, capacity * sizeof(BracketPos));
    this->capacity = capacity;
}

void
BracketStack_append(BracketStack* this, const BracketPos item)
{
    if (this->length >= this->capacity)
    {
        BracketStack_resize(this, (size_t) (this->capacity * 1.5));
    }
    this->data[this->length] = item;
    this->length++;
}

BracketPos
BracketStack_pop(BracketStack* this)
{
    BracketPos value = this->data[this->length - 1];
    this->length--;
    return value;
}
