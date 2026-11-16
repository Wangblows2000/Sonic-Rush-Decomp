/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gDisplayMain[];
void TalkDemoDim_State_WaitClose();
s32 TalkDemo_SetFlags();

void TalkDemoDim_State_FadeIn(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x4) = (*(s16 *)((u8 *)p0 + 4) + 0x1);
    *(u16 *)((u8 *)gDisplayMain + 0x24) = ((*(s16 *)((u8 *)p0 + 4) << 3) / 0x1e);
    if (*(s16 *)((u8 *)p0 + 4) >= 0x1e) {
        *(u32 *)p0 = (u32)TalkDemoDim_State_WaitClose;
        TalkDemo_SetFlags(2, 4);
    }
}

#pragma thumb reset
