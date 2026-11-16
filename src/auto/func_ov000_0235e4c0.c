/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov000_0235e124();
s32 func_ov000_0235e068();
s32 func_ov000_0235e018();
s32 func_ov000_0235dfc4();
s32 func_ov000_0235def0();
s32 func_ov000_0235c9f8();
s32 func_ov000_0235ce3c();
s32 func_ov000_0235ddac();
s32 func_ov000_0235cec8();
s32 MsgBox_Create();
void func_ov000_0235e6b0();
void func_ov000_0235e340();
void func_ov000_0235e430();

void func_ov000_0235e4c0(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov000_0235e124(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    func_ov000_0235e068(t0);
    func_ov000_0235e018(t0);
    func_ov000_0235dfc4(t0);
    func_ov000_0235def0(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    s32 t1 = func_ov000_0235c9f8(0, 0x44, 0xa, *(u32 *)t0, 0x2000);
    func_ov000_0235ce3c(t0, t1);
    if (*(u32 *)((u8 *)t0 + 0x10) == 0) {
        if (*(u32 *)((u8 *)t0 + 0x8) == 2) {
            func_ov000_0235ddac(t0);
        }
    }
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0xa) {
        *(u32 *)t0 = 0;
        func_ov000_0235cec8(t0);
        if (*(u32 *)((u8 *)t0 + 0x10) != 0) {
            if (*(u32 *)((u8 *)t0 + 0x8) == 2) {
                MsgBox_Create(9, 3, 1, 1, 0, 0, 0xe, 0x200010);
                *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_0235e6b0;
                return;
            }
            *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_0235e340;
            return;
        }
        if (*(u32 *)((u8 *)t0 + 0x8) == 2) {
            *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_0235e430;
            return;
        }
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_0235e340;
    }
}

#pragma thumb reset
