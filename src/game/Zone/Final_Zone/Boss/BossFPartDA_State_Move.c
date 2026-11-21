/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArc();

void BossFPartDA_State_Move(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    if (t0 == 0xf) {
        Sound_PlaySeqArc(0, 0, 0xc3);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x10);
    if (t1 >= 0) {
        if (t1 <= 0x34000) {
            return;
        }
    }
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}
