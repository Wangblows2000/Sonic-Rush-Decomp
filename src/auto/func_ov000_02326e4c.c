/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 func_ov000_02326d18();
s32 func_ov000_02326ac8();
s32 func_ov000_02326b2c();
void func_ov000_02326dd8();
void func_ov000_02326744();

void func_ov000_02326e4c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x134);
    if (*(u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x134) == 1) {
        if ((*(u32 *)((u8 *)t1 + 0x32c) & 0x80) == 0) {
            *(u32 *)((u8 *)t0 + 0x68) = (u32)func_ov000_02326d18;
            func_ov000_02326d18((u32)func_ov000_02326d18);
            return;
        }
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x29c);
    s32 t3 = *(u32 *)((u8 *)*(u32 *)((u8 *)t0 + 0x29c) + 0x18);
    *(u32 *)((u8 *)t0 + 0x29c) = t3;
    func_ov000_02326ac8(t0, t2);
    func_ov000_02326b2c(t3, t1, *(u32 *)((u8 *)t0 + 0x138));
    *(u32 *)((u8 *)t0 + 0x2a0) = (*(u32 *)((u8 *)t0 + 0x2a0) + 0x1);
    s32 t4 = *(u32 *)((u8 *)t0 + 0x2a0);
    s32 t5 = *(u32 *)((u8 *)t0 + 0x2a4);
    if (t4 >= t5) {
        *(u32 *)((u8 *)t0 + 0x2a0) = t5;
        *(u32 *)((u8 *)t0 + 0x68) = (u32)func_ov000_02326dd8;
    }
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)t0 + 0x70) = (u32)func_ov000_02326744;
}
