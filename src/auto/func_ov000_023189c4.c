/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
extern u8 data_ov000_0236a2bc[];
s32 func_020371c8();
extern u8 data_ov000_0236ac60[];
s32 func_0201bd80();

void func_ov000_023189c4(void) {
    if (*(s16 *)((u8 *)data_ov000_0236a2bc + (*(s16 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) << 1)) == 0x3fc) {
        return;
    }
    s32 t0 = *(u32 *)data_022b4574;
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t2 = *(s16 *)((u8 *)data_ov000_0236a2bc + (*(s16 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) << 1));
    s32 t3 = (*(s16 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) << 1);
    if (t2 != 0) {
        s32 t4 = (*(s16 *)((u8 *)data_ov000_0236a2bc + t3) - 0x1);
        *(u16 *)((u8 *)data_ov000_0236a2bc + t3) = t4;
    } else {
        func_020371c8(t0, t2, data_ov000_0236a2bc, t3);
        return;
    }
    s32 t5 = (*(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x32c) & 0x400);
    if (t5 != 0) {
        *(u16 *)((u8 *)t1 + 0x6) = 1;
    }
    if (*(u16 *)((u8 *)t1 + 0x6) != 0) {
        if ((*(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x32c) & 0x400) == 0) {
            func_020371c8(*(u32 *)data_022b4574);
            return;
        }
    }
    if (*(s16 *)((u8 *)data_ov000_0236a2bc + (*(s16 *)t1 << 1)) < 0x78) {
        s32 t6 = *(u16 *)((u8 *)t1 + 0x4);
        if (t6 == 0) {
            *(u16 *)((u8 *)t1 + 0x4) = 1;
        }
    }
    if (*(u16 *)((u8 *)t1 + 0x4) == 0) {
        return;
    }
    s32 t7 = *(u16 *)((u8 *)t1 + 0x4);
    s32 t8 = *(s16 *)((u8 *)data_ov000_0236a2bc + (*(s16 *)t1 << 1));
    if (t8 == 0x3c) {
        func_0201bd80(*(u32 *)((u8 *)t1 + 0x8), 0x200, t7);
        *(u16 *)((u8 *)t1 + 0x4) = 2;
        return;
    }
    if (t8 <= 0x3c) {
        return;
    }
    if (t7 != 2) {
        return;
    }
    func_0201bd80(*(u32 *)((u8 *)t1 + 0x8), 0x100);
    *(u16 *)((u8 *)t1 + 0x4) = 1;
}
