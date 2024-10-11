#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#include "keyinfo.h"

KeyInfo* readKey(void) {
    while (!_kbhit()) {
    }
    return KeyInfoNew();
}

char readChar(void) {
    KeyInfo* keyInfo;
    while (true) {
        keyInfo = readKey();
        if (keyInfo->secondary != 0) {
            KeyInfoDestroy(keyInfo);
            continue;
        }
        if (isgraph(keyInfo->primary) || isspace(keyInfo->primary))
            break;
    }
    char chr = (char)keyInfo->primary;
    KeyInfoDestroy(keyInfo);
    return chr;
}
