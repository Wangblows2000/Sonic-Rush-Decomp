/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
s32 Sound_PlaySeqArcEx();
s32 func_ov000_02353e6c();

void CraneSlide_OnHit(u8 *p0, u8 *p1) {
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
    if (*(u32 *)((u8 *)t0 + 0x204) != 0) {
        return;
    }
    *(u32 *)((u8 *)t0 + 0x204) = t1;
    *(u32 *)((u8 *)t0 + 0x1b8) = 0;
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 2);
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) & ~0x200);
    *(u32 *)((u8 *)t0 + 0x1f4) = (*(u32 *)((u8 *)t0 + 0x1f4) & ~3);
    *(u32 *)((u8 *)t0 + 0x58) = (*(u32 *)((u8 *)t0 + 0x58) & ~0x2000);
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) & ~0x2000);
    *(u16 *)((u8 *)t0 + 0x34) = *(s16 *)((u8 *)t1 + 0x28);
    *(u16 *)((u8 *)t0 + 0x2e) = *(s16 *)((u8 *)t0 + 0x34);
    if (*(s16 *)((u8 *)t0 + 0x34) != 0) {
        func_0201c070(*(u32 *)((u8 *)t0 + 0x1dc), 0);
        Sound_PlaySeqArcEx(*(u32 *)((u8 *)t0 + 0x1dc), 0, 0x71);
    }
    func_ov000_02353e6c(t1, t0, 0, 0x70);
}
