#include <nitro/types.h>

void func_ov001_022d1404(void *p, void (*f)(void));
void func_ov001_022d0d78(void);

#pragma thumb on
void func_ov001_022d0e10(u8 *p) {
    if (*(s32 *)(p + 0xcbc) >= 0x11d000) {
        func_ov001_022d1404(p, func_ov001_022d0d78);
    }
}
#pragma thumb reset
