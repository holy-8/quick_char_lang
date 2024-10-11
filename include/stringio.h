#include <stdlib.h>
#include "garbagecollector.h"

#ifndef __STRINGIO_H_
#define __STRINGIO_H_

typedef struct StringIO {
    size_t size;
    size_t allocatedSize;
    size_t overflowIncrement;
    char* buffer;
} StringIO;

void StringIOClear(StringIO* this, size_t initialSize);

void StringIOPutChar(StringIO* this, char chr);

void StringIOPutString(StringIO* this, char* str);

void StringIODestroy(void* this);

StringIO* StringIONew(GarbageCollector* gc, size_t initialSize, size_t overflowIncrement);

#endif /* __STRINGIO_H_ */
