/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WaterJet_Action_Wait();

void WaterJet_State_Spout(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x20c) = (*(u32 *)((u8 *)p0 + 0x20c) & ~1);
    }
    s32 t1 = (*(u16 *)((u8 *)p0 + 0x5c) & 8);
    if (t1 == 0) {
        return;
    }
    WaterJet_Action_Wait(p0, t1);
}
