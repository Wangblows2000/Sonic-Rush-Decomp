/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gDisplayMain[];
s32 TalkDemo_ClearFlags();
s32 TalkDemo_SetFlags();

void TalkDemoWindow_State_CloseCenter(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x8) = (*(s16 *)((u8 *)p0 + 8) + 0x1);
    *(u8 *)((u8 *)p0 + 0x16) = (((0x1f * *(s16 *)((u8 *)p0 + 8)) / 0xa) + 0x41);
    *(u8 *)((u8 *)gDisplayMain + 0x15) = *(u8 *)((u8 *)p0 + 0x16);
    *(u8 *)((u8 *)gDisplayMain + 0x17) = *(u8 *)((u8 *)p0 + 0x16);
    *(u8 *)((u8 *)p0 + 0x17) = (((-30 * *(s16 *)((u8 *)p0 + 8)) / 0xa) + 0x7e);
    *(u8 *)((u8 *)gDisplayMain + 0x14) = *(u8 *)((u8 *)p0 + 0x17);
    *(u8 *)((u8 *)gDisplayMain + 0x16) = *(u8 *)((u8 *)p0 + 0x17);
    if (*(s16 *)((u8 *)p0 + 8) >= 0xa) {
        *(u32 *)p0 = 0;
        *(u32 *)((u8 *)gDisplayMain + 0x1c) = 0;
        TalkDemo_ClearFlags(1, 0x20);
        TalkDemo_SetFlags(1, 0x80000040);
    }
}

#pragma thumb reset
