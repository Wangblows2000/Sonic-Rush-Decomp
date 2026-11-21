/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 PlayerDemoLight_Update();
s32 PlayerDemoModel_DrawModels();
s32 PlayerDemoModel_UpdateCamera();
s32 PlayerDemoModel_DrawCurrentModel();
s32 PlayerDemoModel_NextScene();
void PlayerDemoModel_State_Done();

s32 PlayerDemoModel_State_Play(u8 *p0) {
    PlayerDemoLight_Update(p0, ((u8 *)p0 + 0x1334));
    PlayerDemoModel_DrawModels(p0);
    PlayerDemoModel_UpdateCamera(p0);
    PlayerDemoModel_DrawCurrentModel(p0);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x4);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x4);
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x4) & 0x100);
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x4) = (t1 & -257);
        s32 t3 = PlayerDemoModel_NextScene(p0, t0, -257, t2);
        return t3;
    }
    s32 v4 = t1;
    if ((t1 & 0x200) != 0) {
        *(u32 *)((u8 *)t0 + 0x4) = (v4 & -513);
        PlayerDemoModel_NextScene(p0, t0, -513, (t1 & 0x200));
        *(u32 *)p0 = (u32)PlayerDemoModel_State_Done;
        v4 = (s32)(u32)PlayerDemoModel_State_Done;
    }
    return v4;
}

#pragma thumb reset
