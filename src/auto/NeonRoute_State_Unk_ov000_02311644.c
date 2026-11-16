/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSwitchTimers[];
s32 NeonRoute_Action_Unk_ov000_02311500();

void NeonRoute_State_Unk_ov000_02311644(u8 *p0) {
    s32 t0 = (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) << 1);
    s32 t1 = *(s16 *)((u8 *)gSwitchTimers + (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) << 1));
    if (t1 == 0) {
        return;
    }
    NeonRoute_Action_Unk_ov000_02311500(p0, t1, t0);
}
