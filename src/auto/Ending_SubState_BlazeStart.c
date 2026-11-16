/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void func_ov001_022c7218();
s32 Ending_SetSubState();

void Ending_SubState_BlazeStart(s32 p0) {
    *(u16 *)((u8 *)gDisplayMain + 0x48) = 0x10;
    *(u16 *)((u8 *)gDisplaySub + 0x48) = 0x10;
    Ending_SetSubState(p0, (u32)func_ov001_022c7218, gDisplaySub, gDisplayMain);
}
