/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 func_ov000_02336f28();
s32 func_ov000_02336da8();
s32 func_ov000_02336cec();
s32 func_ov000_023369bc();
s32 func_ov000_023367ac();
s32 func_ov000_0233624c();
s32 func_ov000_02335ee0();

void func_ov000_02337278(void) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x290);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t1 + 0x294) = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x290);
    func_ov000_02336f28(t0, *(u32 *)((u8 *)t1 + 0x28c), 0x3e8);
    if ((*(u32 *)t1 & 2) == 0) {
        func_ov000_02336da8(t1);
    }
    *(u32 *)((u8 *)t1 + 0x684) = *(u32 *)((u8 *)t1 + 0x680);
    func_ov000_02336cec((t1 + 0x680), *(u32 *)((u8 *)t1 + 0x67c));
    if ((*(u32 *)t1 & 1) == 0) {
        func_ov000_023369bc(t1);
    }
    func_ov000_023367ac(t1);
    if ((*(u32 *)t1 & 4) == 0) {
        func_ov000_0233624c(t1);
    }
    *(u32 *)((u8 *)t1 + 0xaa4) = *(u32 *)((u8 *)t1 + 0xaa0);
    func_ov000_02336f28((t1 + 0xaa0), *(u16 *)((u8 *)(t1 + 0x900) + 0x28), 0xa);
    if ((*(u32 *)t1 & 8) != 0) {
        return;
    }
    func_ov000_02335ee0(t1);
}
