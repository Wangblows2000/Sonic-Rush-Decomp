/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
s32 Ending_CreateBg0PictureTask();
s32 Ending_DrawNextPicture();
void Ending_SubState_BlazeShowPicture4();
s32 Ending_SetSubState();

void Ending_SubState_BlazeNextPicture4(s32 p0) {
    *(u16 *)((u8 *)gDisplayMain + 0x48) = 0x10;
    Ending_CreateBg0PictureTask(p0, gDisplayMain, 0x10);
    Ending_DrawNextPicture(p0);
    Ending_SetSubState(p0, (u32)Ending_SubState_BlazeShowPicture4);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & ~0x1f00) | 0x300);
}
