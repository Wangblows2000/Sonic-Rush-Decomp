/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();

void GoalRingPart_State_FollowRing(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x88) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x14);
    *(u16 *)((u8 *)p0 + 0xbe) = *(s16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0xbe);
    *(u16 *)((u8 *)p0 + 0x106) = *(s16 *)((u8 *)(*(u32 *)((u8 *)p0 + 0x88) + 0x100) + 0x6);
    if ((*(u32 *)((u8 *)t0 + 0x1f4) & 1) != 0) {
        if (*(u32 *)((u8 *)p0 + 0x8) == 0) {
            if ((*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x5c) & 8) != 0) {
                if (*(u16 *)((u8 *)p0 + 0x9c) == 5) {
                    StageObj_SetAnimation(p0, 6, ((u8 *)p0 + 0x100), t0);
                } else {
                    StageObj_SetAnimation(p0, 3);
                }
                *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
                *(u32 *)((u8 *)p0 + 0x8) = 1;
            }
        }
    }
    if (*(u32 *)((u8 *)p0 + 0x8) == 0) {
        return;
    }
    s32 t1 = *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x9c);
    if (t1 == 7) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
