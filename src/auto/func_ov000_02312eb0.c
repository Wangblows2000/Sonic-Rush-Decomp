/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02312f70();
s32 func_ov000_02353d6c();
s32 Sound_PlaySeqArcEx();

void func_ov000_02312eb0(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    if (*(u8 *)t1 != 1) {
        return;
    }
    s32 t2 = *(s16 *)((u8 *)t1 + 0x28);
    s32 t3 = *(s16 *)((u8 *)(t1 + 0x300) + 0x78);
    s32 v4 = t2;
    if (t2 < 0) {
        v4 = -v4;
    }
    if (v4 <= t3) {
        if (*(s8 *)((u8 *)*(u32 *)((u8 *)t0 + 0x1e0) + 0x7) == 0) {
            return;
        }
    }
    func_ov000_02312f70(t0, t1);
    func_ov000_02353d6c(t1, t0, 0, 0x28);
    Sound_PlaySeqArcEx((t1 + 0x388), 0, 0x5a);
    *(u32 *)((u8 *)t1 + 0x32c) = (*(u32 *)((u8 *)t1 + 0x32c) | 1);
}
