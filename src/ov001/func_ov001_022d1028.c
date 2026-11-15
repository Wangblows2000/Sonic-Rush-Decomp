#include <nitro/types.h>

void func_ov001_022d1404(void *p, void (*f)(void));
void func_ov001_022d0f98(void);

#pragma thumb on
void func_ov001_022d1028(u8 *p) {
    if (*(s32 *)(p + 0xbdc) > 0x3c) {
        func_ov001_022d1404(p, func_ov001_022d0f98);
    }
}
#pragma thumb reset
