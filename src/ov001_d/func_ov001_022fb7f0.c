#include <nitro/types.h>

u8 *func_ov001_022fd428(s32 a, void *b, s32 c, s32 d, s32 e);
void *func_ov001_02300c94(void);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_ov001_022fb7b0(void);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);

void func_ov001_022fb7f0(void *p, s32 x, s32 y, s32 z, s32 n, u16 ang, u16 rz) {
    u8 *t = func_ov001_022fd428(0xa, p, x, y, z);
    if (t == NULL)
        return;
    *(void **)(t + 0x80) = func_ov001_02300c94();
    *(u32 *)(t + 0x14) |= 0x10;
    *(u16 *)(t + 0x98) = 0xffff;
    *(void (**)(void))(t + 0x74) = func_ov001_022fb7b0;
    *(u16 *)(t + 0x40) = ang;
    *(u16 *)(t + 0x6c) = rz;
    func_ov001_022fe8d4(t + 0xe8, t, 1, 1, 0x5000, 0x200000, 0x9000, 0, 0, 0, 0);
    *(u8 **)(t + 0xa0) = t + 0xe8;
    func_ov001_022fe8d4(t + 0x120, t, 1, 1, 0x200000, 0x5000, 0x9000, 0, 0, 0, 0);
    *(u8 **)(t + 0xa4) = t + 0x120;
    *(s32 *)(t + 0x1cc) = n;
    Sound_PlaySeqArc(0, 0, 0xab);
}
