#include <nitro/types.h>

extern u8 *data_ov001_0231f094;
u8 *func_ov001_022fd428(s32 a, void *b, s32 c, s32 d, s32 e);
void *ExtraBoss_GetPlayer(void);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_ov001_022fb590(void);

u8 *func_ov001_022fb678(void *p) {
    u8 *t = func_ov001_022fd428(0xc, p, 0, 0, 0);
    if (t == NULL) {
        return NULL;
    }
    *(u32 *)(t + 0x14) &= ~1;
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u32 *)(t + 0x14) |= 0x10;
    *(u16 *)(t + 0x98) = 0xffff;
    *(void (**)(void))(t + 0x74) = func_ov001_022fb590;
    data_ov001_0231f094[1] = 0;
    func_ov001_022fe8d4(t + 0xe8, t, 1, 1, 0x3c000, 0x1e000, 0x32000, -0x3c000, 0, 0, 0);
    *(u32 *)(t + 0xf0) |= 0x3200;
    *(u8 **)(t + 0xa0) = t + 0xe8;
    *(s32 *)(t + 0x1d0) = 0;
    return t;
}
