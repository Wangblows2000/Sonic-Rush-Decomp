#include <nitro/types.h>

u8 *func_ov001_022ffce8(void);
u8 *func_ov001_022fd428(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_ov001_02300c94(void);
void func_ov001_022fa5c8(void);

void func_ov001_022fa5f4(s32 a, s32 b, s32 c) {
    s32 v = *(s32 *)(func_ov001_022ffce8() + 0x31c);
    u8 *t = func_ov001_022fd428(0x15, a, v, b, c);
    if (t == NULL) {
        return;
    }
    (*(u8 **)(t + 0x84))[0x10f] |= 4;
    *(s32 *)(t + 0x80) = func_ov001_02300c94();
    *(void (**)(void))(t + 0x74) = func_ov001_022fa5c8;
}
