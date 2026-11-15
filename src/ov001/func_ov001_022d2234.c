#include <nitro/types.h>

void func_ov001_022d1a00(void);

#pragma thumb on
void func_ov001_022d2234(u8 *p, void (*f)(void)) {
    if (f == NULL) {
        *(void (**)(void))(p + 0x10) = func_ov001_022d1a00;
    } else {
        *(void (**)(void))(p + 0x10) = f;
    }
    *(s32 *)(p + 0x14) = 0;
}
#pragma thumb reset
