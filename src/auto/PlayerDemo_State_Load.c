/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gPlayerDemoId[];
s32 PlayerDemo_LoadData();
extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 PlayerDemo_Unk_02057094();
s32 PlayerDemo_Unk_0205708c();
s32 PlayerDemoBg_Create();
s32 PlayerDemoModel_Create();
s32 PolyDemoUnder2_Create();
s32 PlayerDemo_LoadSound();
void PlayerDemo_State_RunScript();

void PlayerDemo_State_Load(u8 *p0) {
    s32 t0 = PlayerDemo_LoadData(*(s16 *)((u8 *)gPlayerDemoId + 0), gPlayerDemoId);
    *(u32 *)((u8 *)p0 + 0x14) = t0;
    if (*(u32 *)((u8 *)p0 + 0x14) == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    s32 t1 = PlayerDemo_Unk_02057094();
    *(u16 *)((u8 *)p0 + 0x1a) = *(u32 *)((u8 *)t1 + 0xc);
    s32 t2 = PlayerDemo_Unk_0205708c(*(u32 *)((u8 *)p0 + 0x14), *(u16 *)((u8 *)p0 + 0x18));
    *(u32 *)((u8 *)p0 + 0x1c) = t2;
    PlayerDemoBg_Create();
    PlayerDemoModel_Create();
    PolyDemoUnder2_Create();
    PlayerDemo_LoadSound(p0);
    *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) | 1);
    *(u32 *)p0 = (u32)PlayerDemo_State_RunScript;
}

#pragma thumb reset
