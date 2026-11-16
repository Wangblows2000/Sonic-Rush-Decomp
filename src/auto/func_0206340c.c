/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02063030();
extern u8 data_0208926c[];
extern u8 data_02087d44[];
s32 func_020371c8();
s32 func_02035368();
s32 func_02035cb4();
s32 func_02033754();
s32 func_020521b4();
s32 func_0205507c();
s32 func_02055060();

void func_0206340c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 v1;
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) != 0) {
        func_02063030(t0);
        *(u32 *)((u8 *)t0 + 0xc) = (*(u32 *)((u8 *)t0 + 0xc) + 0x1);
        *(u16 *)((u8 *)data_0208926c + 0x8) = (*(u32 *)((u8 *)t0 + 0xc) << 1);
        s32 t2 = (*(u32 *)((u8 *)t0 + 0xc) << 1);
        *(u16 *)((u8 *)data_02087d44 + 0x8) = t2;
        v1 = t2;
    } else {
        *(u32 *)((u8 *)t0 + 0xc) = 8;
    }
    if ((u32)*(u32 *)((u8 *)t0 + 0xc) >= (u32)8) {
        s32 t3 = *(u32 *)((u8 *)t0 + 0x4);
        func_020371c8(*(u32 *)data_022b4574, v1);
        func_02035368();
        func_02035cb4();
        func_02033754();
        func_020521b4();
        if (t3 != 0) {
            func_0205507c();
            return;
        }
        func_02055060();
    }
}

#pragma thumb reset
