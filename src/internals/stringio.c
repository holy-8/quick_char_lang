#include <stdlib.h>
#include "panic.h"
#include "garbagecollector.h"

typedef struct StringIO
{
    size_t size;
    size_t allocatedSize;
    size_t overflowIncrement;
    char* buffer;
} StringIO;

void StringIOClear(StringIO* this, size_t initialSize) {
    if (this->buffer != NULL)
        free(this->buffer);
    this->size = 0;
    this->allocatedSize = initialSize;
    this->buffer = panicCalloc(initialSize, sizeof(char), "at file stringio.c, line 18");
}

void StringIOPutChar(StringIO* this, char chr) {
    if (this->size >= this->allocatedSize) {
        this->allocatedSize += this->overflowIncrement;
        this->buffer = panicRealloc(this->buffer, this->allocatedSize, "at file stringio.c, line 24");
    }
    this->buffer[this->size] = chr;
    this->size++;
}

void StringIOPutString(StringIO* this, char* str) {
    for (char* current_chr = str; *current_chr != 0; current_chr++)
        StringIOPutChar(this, *current_chr);
    StringIOPutChar(this, 0);
    this->size--;
}

void StringIODestroy(void* this) {
    if (((StringIO*)this)->buffer != NULL)
        free(((StringIO*)this)->buffer);
    free(this);
}

StringIO* StringIONew(GarbageCollector* gc, size_t initialSize, size_t overflowIncrement) {
    StringIO* this = panicMalloc(sizeof(StringIO), "at file stringio.c, line 44");
    this->overflowIncrement = overflowIncrement;
    this->buffer = NULL;
    StringIOClear(this, initialSize);
    GarbageCollectorAddReference(gc, this, StringIODestroy);
    return this;
}
