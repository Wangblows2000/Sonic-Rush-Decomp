#include <nitro/types.h>

void func_ov001_022d1404(void *p, void (*f)(void));
void func_ov001_022d12a8(void);

#pragma thumb on
void func_ov001_022d1330(u8 *p) {
    if (*(s32 *)(p + 0xbe0) == 1) {
        func_ov001_022d1404(p, func_ov001_022d12a8);
    }
}
#pragma thumb reset
