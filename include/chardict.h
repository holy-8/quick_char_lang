#include <stdlib.h>
#include "garbagecollector.h"

#ifndef __CHARDICT_H_
#define __CHARDICT_H_

typedef struct KeyValuePair {
    char key;
    void* value;
} KeyValuePair;


typedef struct CharDict {
    size_t size;
    size_t allocatedSize;
    KeyValuePair** data;
} CharDict;

void CharDictInsert(CharDict* this, char key, void* value);

void* CharDictGet(CharDict* this, char key);

void CharDictDestroy(void* this);

CharDict* CharDictNew(GarbageCollector* gc);

#endif /* __CHARDICT_H_ */
