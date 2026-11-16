/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022ed490();
s32 func_ov001_022ed3e0();
s32 func_ov001_022ed390();
s32 func_ov001_022ed19c();
s32 func_ov001_022ed188();
s32 func_ov001_022ed14c();
s32 func_ov001_022ed104();
s32 func_ov001_022ed08c();
s32 func_ov001_022ec72c();
s32 func_ov001_022ecf5c();
s32 func_ov001_022ecf04();
void func_ov001_022ed6c4();

void func_ov001_022ed614(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov001_022ed490(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), 0, 0x90);
    func_ov001_022ed3e0(t0);
    func_ov001_022ed390(t0, 0);
    func_ov001_022ed19c(t0);
    func_ov001_022ed188(t0);
    func_ov001_022ed14c(t0);
    func_ov001_022ed104(t0);
    func_ov001_022ed08c(t0);
    s32 t1 = *(u32 *)t0;
    if (t1 >= 0) {
        s32 t2 = func_ov001_022ec72c(*(u32 *)((u8 *)t0 + 0x1c), *(u32 *)((u8 *)t0 + 0x20), 8, t1, 0x2000);
        *(u32 *)((u8 *)t0 + 0x30) = t2;
    }
    s32 t3 = func_ov001_022ecf5c(t0);
    if (t3 != 0) {
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        *(u32 *)((u8 *)t0 + 0x8) = 0;
    } else {
        s32 t4 = func_ov001_022ecf04(t0);
        if (t4 != 0) {
            *(u32 *)((u8 *)t0 + 0x4) = 1;
            *(u32 *)((u8 *)t0 + 0x8) = 1;
        } else {
            *(u32 *)((u8 *)t0 + 0x4) = 6;
            *(u32 *)((u8 *)t0 + 0x8) = 6;
        }
    }
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)8) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x30) = *(u32 *)((u8 *)t0 + 0x20);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022ed6c4;
    }
}

#pragma thumb reset
