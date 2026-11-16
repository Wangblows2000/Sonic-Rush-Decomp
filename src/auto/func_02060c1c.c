/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gPad[];
s32 TalkDemo_SetFlags();
s32 TalkDemo_GetFlags();
extern u8 gTouchState[];
void func_02060bd8();

void func_02060c1c(u8 *p0) {
    if ((*(u16 *)((u8 *)gPad + 0x4) & 1) != 0) {
        TalkDemo_SetFlags(3, 0x10);
    }
    s32 t0 = TalkDemo_GetFlags(0);
    if ((t0 & 1) != 0) {
        if ((*(u32 *)((u8 *)gTouchState + 0x10) & 0x40) != 0) {
            TalkDemo_SetFlags(3, 0x10);
        }
    }
    if ((*(u16 *)gPad & 0x100) != 0) {
        TalkDemo_SetFlags(3, 0x40);
    }
    s32 t1 = TalkDemo_GetFlags(0);
    if ((t1 & 0x200) != 0) {
        s32 t2 = TalkDemo_GetFlags(2, 0x200);
        if ((t2 & 4) != 0) {
            TalkDemo_SetFlags(0, 0x400);
            *(u32 *)p0 = (u32)func_02060bd8;
        }
    }
    s32 t3 = TalkDemo_GetFlags(3);
    if ((t3 & 0x100) != 0) {
        *(u32 *)p0 = (u32)func_02060bd8;
    }
}

#pragma thumb reset
