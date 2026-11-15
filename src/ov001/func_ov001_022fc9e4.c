#include <nitro/types.h>

u8 *func_ov001_022fd428(s32 a, void *b, s32 c, s32 d, s32 e);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_ov001_022fd1ec(void);

void func_ov001_022fc9e4(u8 *p, s32 b, s32 c, s32 d) {
    u8 *t = func_ov001_022fd428(0, p, b, c, d);
    if (t == NULL) {
        return;
    }
    *(void (**)(void))(t + 0x74) = func_ov001_022fd1ec;
    *(u32 *)(t + 0x14) &= ~1;
    *(void **)(t + 0x80) = *(void **)(p + 0x80);
    (*(u8 **)(t + 0x84))[0x10f] |= 4;
    *(s32 *)(t + 0x50) = 0x5000;
    func_ov001_022fe8d4(t + 0xe8, t, 6, 1, 0x10000, 0x10000, 0x10000, 0, 0, 0, 0);
    *(u8 **)(t + 0xa0) = t + 0xe8;
}
