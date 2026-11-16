/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 PlayerDemo_Unk_02057098();
s32 PlayerDemo_Unk_02057fb4();
s32 BrightnessFade_Destroy();
s32 PlayerDemo_Unk_020574d0();
s32 PlayerDemo_Unk_0205761c();
extern u8 gPlayerDemoTask[];

void PlayerDemo_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    PlayerDemo_Unk_02057098(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x14));
    PlayerDemo_Unk_02057fb4(t0);
    BrightnessFade_Destroy();
    PlayerDemo_Unk_020574d0(t0);
    PlayerDemo_Unk_0205761c(t0);
    *(u32 *)gPlayerDemoTask = 0;
}

#pragma thumb reset
