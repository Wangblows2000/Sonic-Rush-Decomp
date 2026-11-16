#include <nitro/types.h>

u8 *ExtraBoss_GetWork(void);
u8 *func_ov001_022fd428(s32 a, s32 b, s32 c, s32 d, s32 e);
void *func_ov001_02300c94(void);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov001_022fa768(void);

void func_ov001_022fa830(s32 a, s32 b, s32 c) {
    s32 v = *(s32 *)(ExtraBoss_GetWork() + 0x31c);
    u8 *t = func_ov001_022fd428(0x14, a, v, b, c);
    if (t == NULL) {
        return;
    }
    *(void **)(t + 0x80) = func_ov001_02300c94();
    *(u16 *)(t + 0x98) = 0xffff;
    *(void (**)(void))(t + 0x74) = func_ov001_022fa768;
    func_ov001_022fe8d4(t + 0xe8, t, 1, 1, 0x100000, 0x20000, 0x10000, 0, 0, 0, 0);
    *(u32 *)(t + 0xf0) |= 0x2210;
    *(u8 **)(t + 0xa0) = t + 0xe8;
    Sound_PlaySeqArc(0, 0, 0xb8);
}
