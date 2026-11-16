/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TalkDemo_FreeScript();
s32 TalkDemo_GetFlags();
s32 TalkDemo_RestoreDisplay();
s32 TalkDemo_RestoreDisplayZoneMap();
s32 TalkDemo_MarkSeen();
s32 TalkDemo_ReleaseSound();
s32 System_Unfreeze();
extern u8 gTalkDemoTask[];

void TalkDemo_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    TalkDemo_FreeScript(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x24));
    s32 t1 = TalkDemo_GetFlags(0);
    if ((t1 & 1) == 0) {
        TalkDemo_RestoreDisplay(t0, 1);
    } else {
        TalkDemo_RestoreDisplayZoneMap(t0);
    }
    TalkDemo_MarkSeen(t0);
    TalkDemo_ReleaseSound(t0);
    System_Unfreeze();
    *(u32 *)gTalkDemoTask = 0;
}

#pragma thumb reset
