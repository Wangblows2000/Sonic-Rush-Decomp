/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void ClearDemoScore_State_RollTotal();
s32 Sound_PlaySeqArc();

s32 ClearDemoScore_State_ShowTotal(s32 p0, u8 *p1) {
    *(u32 *)((u8 *)p1 + 0x4) = (*(u32 *)((u8 *)p1 + 0x4) + 0x1);
    s32 t0 = *(u32 *)((u8 *)p1 + 0x4);
    s32 v1 = t0;
    if (t0 >= 0x3c) {
        *(u32 *)p1 = (u32)ClearDemoScore_State_RollTotal;
        *(u32 *)((u8 *)p1 + 0x4) = 0;
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x344) | 0x4000);
        *(u32 *)((u8 *)p0 + 0x344) = t2;
        s32 t3 = Sound_PlaySeqArc(((u8 *)p0 + 0x1634), *(u32 *)((u8 *)p0 + 0x1630), 1, t2);
        v1 = t3;
    }
    return v1;
}

#pragma thumb reset
