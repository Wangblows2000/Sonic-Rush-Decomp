#include <nitro/types.h>

void func_ov001_022d2234(void *p, void (*f)(void));
void func_ov001_022d1ec8(void);

#pragma thumb on
void func_ov001_022d1fd4(u8 *p) {
    if (*(s32 *)(p + 0x14) > 0x1d0) {
        func_ov001_022d2234(p, func_ov001_022d1ec8);
    }
}
#pragma thumb reset
