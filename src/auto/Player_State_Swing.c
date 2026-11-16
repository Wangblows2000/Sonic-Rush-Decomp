/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_SwingAroundPivot();
s32 Player_Unk_ov000_02351300();
s32 Sound_PlaySeqArcEx();

void Player_State_Swing(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        s32 t0 = (*(u32 *)((u8 *)p0 + 0x32c) & 1);
        if (t0 != 0) {
            Player_SwingAroundPivot(p0, t0);
            s32 t1 = *(u32 *)((u8 *)p0 + 0x3f0);
            if (*(u32 *)((u8 *)p0 + 0x3f4) == t1) {
                *(u32 *)((u8 *)p0 + 0x3e8) = 0;
            }
        } else {
            Player_Unk_ov000_02351300(p0, t0);
            s32 t2 = *(u32 *)((u8 *)p0 + 0x3f0);
            if (*(u32 *)((u8 *)p0 + 0x3f4) == t2) {
                *(u32 *)((u8 *)p0 + 0x3e8) = 0;
            }
        }
    }
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        if ((*(u16 *)((u8 *)p0 + 0x422) & 3) == 0) {
            return;
        }
    }
    *(u8 *)((u8 *)p0 + 0x3) = 0;
    *(u32 *)((u8 *)p0 + 0x3e8) = 0;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x100);
    s32 t3 = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x100000);
    *(u32 *)((u8 *)p0 + 0x32c) = t3;
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        *(u8 *)((u8 *)p0 + 0x1) = 0;
        *(u16 *)((u8 *)p0 + 0x34) = 0;
        ((s32 (*)())*(u32 *)((u8 *)p0 + 0x340))(p0, *(u32 *)((u8 *)p0 + 0x340), t3);
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 1) == 0) {
        Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x40);
    }
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 1);
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
}
