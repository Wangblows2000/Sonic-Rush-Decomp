#include <nitro/types.h>

void func_ov001_022d1404(void *p, void (*f)(void));

#pragma thumb on
void func_ov001_022d0d44(u8 *p) {
    if (*(s32 *)(p + 0xcc0) >= 0x131000) {
        *(s32 *)(p + 0xcd8) = 0;
        *(s32 *)(p + 0xcf0) = 0;
        func_ov001_022d1404(p, NULL);
    }
}
#pragma thumb reset
