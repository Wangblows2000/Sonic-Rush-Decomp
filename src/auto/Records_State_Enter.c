/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022ec72c();
s32 func_ov001_022ed490();
s32 func_ov001_022ed3e0();
s32 func_ov001_022ed390();
s32 func_ov001_022ed19c();
s32 func_ov001_022ed188();
s32 func_ov001_022ed14c();
s32 func_ov001_022ed104();
s32 func_ov001_022ed08c();
void func_ov001_022ed6c4();

void Records_State_Enter(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (t1 >= 0) {
        s32 t2 = func_ov001_022ec72c(-160, 0, 0x10, t1, 0x2000);
        s32 t3 = func_ov001_022ec72c(0x90, 0x90, 0x10, t1, 0x2000);
        func_ov001_022ed490(t0, t2, t3);
    }
    func_ov001_022ed3e0(t0);
    func_ov001_022ed390(t0, 0);
    func_ov001_022ed19c(t0);
    func_ov001_022ed188(t0);
    func_ov001_022ed14c(t0);
    func_ov001_022ed104(t0);
    func_ov001_022ed08c(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0x10) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022ed6c4;
    }
}

#pragma thumb reset
