/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 func_ov000_02353eb4();

void CranePartED_OnHit(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        if (*(u8 *)t1 != 1) {
            return;
        }
    }
    if (*(u32 *)((u8 *)t0 + 0x204) == 0) {
        *(u16 *)((u8 *)t0 + 0x30) = 0x100;
        Sound_PlaySeqArcEx(*(u32 *)((u8 *)t0 + 0x1dc), 0, 0x69);
    }
    *(u32 *)((u8 *)t0 + 0x204) = t1;
    func_ov000_02353eb4(t1, t0, -116, 0x12);
}
