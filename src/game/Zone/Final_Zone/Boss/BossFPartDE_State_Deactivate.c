/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_SndPlayerStopSeq();
void BossFPartDE_State_Leave();

void BossFPartDE_State_Deactivate(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x4a4) = 1;
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x13e) & ~4);
    *(u16 *)((u8 *)p0 + 0x13e) = t0;
    NNS_SndPlayerStopSeq(((u8 *)p0 + 0x4a8), 0x3c, ((u8 *)p0 + 0x100), t0);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartDE_State_Leave;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}
