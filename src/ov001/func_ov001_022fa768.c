#include <nitro/types.h>

void func_ov001_022fa558(void *p);
void func_ov001_022fd268(void *p, s32 a);
u32 *func_ov001_022ffce8(void);
void func_ov001_02303c38(s32 a);
void func_ov001_022fa5f4(s32 a, s32 b, s32 c);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov001_022fa6e8(void);

void func_ov001_022fa768(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    func_ov001_022fa558(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    *(void (**)(void))(p + 0x74) = func_ov001_022fa6e8;
    func_ov001_022fd268(p, 1);
    (*(u8 **)(p + 0x84))[0x10c] |= 4;
    (*(u8 **)(p + 0x84))[0x10d] |= 4;
    (*(u8 **)(p + 0x84))[0x10e] |= 4;
    *(s32 *)(p + 0x1cc) = 0x12c;
    *func_ov001_022ffce8() |= 0x40;
    func_ov001_022ffce8()[0x310 / 4] = *(s32 *)(p + 0x1c);
    func_ov001_02303c38(*(s32 *)(p + 0x1c));
    func_ov001_022fa5f4(0, *(s32 *)(p + 0x1c), *(s32 *)(p + 0x20));
    Sound_PlaySeqArc(0, 0, 0xb9);
}
