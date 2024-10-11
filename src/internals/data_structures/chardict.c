#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "panic.h"
#include "garbagecollector.h"

typedef struct KeyValuePair {
    char key;
    void* value;
} KeyValuePair;

KeyValuePair* KeyValuePairNew(char key, void* value) {
    KeyValuePair* this = panicMalloc(sizeof(KeyValuePair), "at chardict.c, line 13");
    this->key = key;
    this->value = value;
    return this;
}

void KeyValuePairDestroy(KeyValuePair* this) {
    free(this);
}

typedef struct CharDict {
    size_t size;
    size_t allocatedSize;
    KeyValuePair** data;
} CharDict;

/*
    Key is assumed to be a letter character in range [65, 90] [97, 122].
    It's a specific usecase for CCL, as each name can ever be one character long,
    and only english letters are allowed in a name.
*/
void CharDictInsert(CharDict* this, char key, void* value) {
    KeyValuePair* variable = KeyValuePairNew(key, value);
    /*
        toupper(key) will return an uppercase version of a key: in range [65, 90]
        (key - 65) will return an actual index of the key: in range [0, 25]
        (26 * islower(key)) will add 26 to an index, if key is lowercase.
            for 'A': (65 - 65) + (26 * 0) = 0
            for 'Z': (90 - 65) + (26 * 0) = 25
            for 'a': (65 - 65) + (26 * 1) = 26
    */
    size_t index = (toupper(key) - 65) + (26 * (bool)islower(key));
    this->data[index] = variable;
    this->size++;
}

/*
    Key is assumed to be a letter character in range [65, 90] [97, 121].
    Returns NULL if such key doesn't exist.
*/
void* CharDictGet(CharDict* this, char key) {
    size_t index = (toupper(key) - 65) + (26 * (bool)islower(key));
    KeyValuePair* variable = this->data[index];
    return variable == 0 ? NULL : variable->value;
}

void CharDictDestroy(void* this) {
    for (size_t index = 0; index < ((CharDict*)this)->allocatedSize; index++) {
        if (((CharDict*)this)->data[index] != NULL)
            KeyValuePairDestroy(((CharDict*)this)->data[index]);
    }
    free(this);
}

CharDict* CharDictNew(GarbageCollector* gc) {
    CharDict* this = panicMalloc(sizeof(CharDict), "at file chardict.c, line 68");
    this->size = 0;
    this->allocatedSize = 52;
    /*
        VariableDict assumes that each key is an english letter, uppercase or lowercase.
        Therefore, allocated size is 26 * 2 = 52.
    */
    this->data = panicCalloc(this->allocatedSize, sizeof(KeyValuePair*), "at file chardict.c, line 75");
    GarbageCollectorAddReference(gc, this, CharDictDestroy);
    return this;
}
