#include <nitro/types.h>

void func_ov001_022d2234(void *p, void (*f)(void));
void func_ov001_022d20e4(void);
void func_ov001_022d215c(void);

#pragma thumb on
void func_ov001_022d21d4(s32 *p) {
    if (*p == 2) {
        func_ov001_022d2234(p, func_ov001_022d20e4);
    } else {
        func_ov001_022d2234(p, func_ov001_022d215c);
    }
}
#pragma thumb reset
