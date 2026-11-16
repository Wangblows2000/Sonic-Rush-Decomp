/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 PlayerDemoLight_Stop();
s32 PlayerDemoModel_ReleaseScene();
void PlayerDemoModel_Unk_02056840();
s32 PlayerDemoModel_StartScene();

void PlayerDemoModel_NextScene(u8 *p0) {
    PlayerDemoLight_Stop(p0, ((u8 *)p0 + 0x1334));
    PlayerDemoModel_ReleaseScene(p0);
    *(u8 *)((u8 *)p0 + 0x9) = (*(s8 *)((u8 *)p0 + 9) + 0x1);
    if (*(s8 *)((u8 *)p0 + 9) >= *(u8 *)((u8 *)p0 + 0xa)) {
        *(u32 *)p0 = (u32)PlayerDemoModel_Unk_02056840;
        return;
    }
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x10) | 0x40000000);
    *(u32 *)((u8 *)p0 + 0x10) = t0;
    PlayerDemoModel_StartScene(p0, t0);
}

#pragma thumb reset
