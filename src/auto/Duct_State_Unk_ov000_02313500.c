/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0235b804();
s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();

s32 Duct_State_Unk_ov000_02313500(u8 *p0) {
    s32 t0 = func_ov000_0235b804();
    if (t0 != 0) {
        if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x10) != 0) {
            Sound_PlaySeqArcEx(0, 0, 0x65);
        }
        Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
        s32 t1 = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
        *(u16 *)((u8 *)p0 + 0x5c) = t1;
        return t1;
    }
    s32 t2 = *(u16 *)((u8 *)p0 + 0x5c);
    s32 v3 = t2;
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) != 0) {
        *(u16 *)((u8 *)p0 + 0x5c) = (v3 | 0x10);
        v3 = (v3 | 0x10);
    }
    return v3;
}
