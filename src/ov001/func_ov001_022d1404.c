#include <nitro/types.h>

void func_ov001_022d0198(void);

#pragma thumb on
void func_ov001_022d1404(u8 *p, void (*f)(void)) {
    if (f == NULL) {
        *(void (**)(void))(p + 0xbd8) = func_ov001_022d0198;
    } else {
        *(void (**)(void))(p + 0xbd8) = f;
    }
    *(s32 *)(p + 0xbdc) = 0;
}
#pragma thumb reset
