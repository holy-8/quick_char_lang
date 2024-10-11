#ifndef __KEYINFO_H_
#define __KEYINFO_H_

typedef struct KeyInfo {
    int primary;
    int secondary;
} KeyInfo;

KeyInfo* KeyInfoNew(void);

void KeyInfoDestroy(KeyInfo* this);

#endif /* __KEYINFO_H_ */
