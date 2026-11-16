/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_SndPlayerStopSeq();
void BossFPartDE_State_Unk_ov000_022f5ce4();

void BossFPartDE_State_Unk_ov000_022f5e18(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x4a4) = 1;
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x13e) & ~4);
    *(u16 *)((u8 *)p0 + 0x13e) = t0;
    NNS_SndPlayerStopSeq(((u8 *)p0 + 0x4a8), 0x3c, ((u8 *)p0 + 0x100), t0);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartDE_State_Unk_ov000_022f5ce4;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}
