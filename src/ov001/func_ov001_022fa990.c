#include <nitro/types.h>

u8 *func_ov001_022fd428(s32 a, s32 b, s32 c, s32 d, s32 e);
void *func_ov001_02300c94(void);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov001_022fd1c8(void);

void func_ov001_022fa990(s32 a, s32 b, s32 c, s32 d, s16 big) {
    u8 *t;
    if (big != 0) {
        t = func_ov001_022fd428(0x11, a, b, c, d);
    } else {
        t = func_ov001_022fd428(0x10, a, b, c, d);
    }
    if (t == NULL) {
        return;
    }
    *(void (**)(void))(t + 0x74) = func_ov001_022fd1c8;
    *(void **)(t + 0x80) = func_ov001_02300c94();
    *(u16 *)(t + 0x98) = 0xffff;
    *(s32 *)(t + 0x1cc) = 0x3c;
    func_02036754(0, 0, 0xb2);
}
