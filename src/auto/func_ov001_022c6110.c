/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
extern u8 gPlayerDemoId[];
s32 PlayerDemo_Start();
void func_ov001_022c60e0();
s32 ExtraEnding_SetState();

void func_ov001_022c6110(s32 p0) {
    *(u16 *)((u8 *)gDisplayMain + 0x48) = 0x10;
    *(u16 *)((u8 *)gDisplaySub + 0x48) = 0x10;
    *(u16 *)gPlayerDemoId = 0x2a;
    PlayerDemo_Start(p0, gPlayerDemoId, gDisplaySub, gDisplayMain);
    ExtraEnding_SetState(p0, (u32)func_ov001_022c60e0);
}
