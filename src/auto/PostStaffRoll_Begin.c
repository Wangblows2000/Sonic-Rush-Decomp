/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gGameState[];
extern u8 gSaveData[];
s32 PostStaffRoll_InitDisplay();
s32 Vram_SetBankForBGExtPltt();
void PostStaffRoll_State_TalkDemo();
s32 Task_Create();
extern u8 gTalkDemoId[];
s32 TalkDemo_Create();
s32 PostStaffRoll_ShowUnlockNotices();

void PostStaffRoll_Begin(void) {
    if (*(u32 *)gGameState == 0) {
        s32 t0 = (*(u8 *)((u8 *)gSaveData + 0xf) & 0x7f);
        if (t0 != 0x7f) {
            PostStaffRoll_InitDisplay(0x7f, t0);
            Vram_SetBankForBGExtPltt(0);
            Task_Create((u32)PostStaffRoll_State_TalkDemo, 0, 0, 0x1000, 0, 4);
            *(u16 *)gTalkDemoId = 0x37;
            TalkDemo_Create(gTalkDemoId, 0x37);
            return;
        }
    }
    PostStaffRoll_ShowUnlockNotices(0);
}

#pragma thumb reset
