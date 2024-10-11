#include <stdlib.h>
#include "garbagecollector.h"

void* shortNew(GarbageCollector* gc, short value) {
    short* obj = panicMalloc(sizeof(short), "at file garbagecollector.c, line 51");
    *obj = value;
    GarbageCollectorAddReference(gc, obj, free);
    return obj;
}
