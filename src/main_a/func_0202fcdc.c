#include <nitro/types.h>

typedef void (*UnkFunc_0202fcdc)(void);
extern UnkFunc_0202fcdc ARM9_CTOR_START[];

void func_0202fcdc(void) {
    UnkFunc_0202fcdc *p;
    for (p = ARM9_CTOR_START; p != NULL && *p != NULL; p++) {
        (*p)();
    }
}
