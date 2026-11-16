/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
extern u8 data_ov000_0236aad8[];

void func_ov000_02335554(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (t1 != 0) {
        ((s32 (*)())t1)(t0, t1);
    }
    *(u32 *)((u8 *)t0 + 0xc) = *(u32 *)((u8 *)data_ov000_0236aad8 + 0x4);
    *(u32 *)((u8 *)t0 + 0x10) = *(u32 *)((u8 *)data_ov000_0236aad8 + 0x8);
    *(u32 *)((u8 *)t0 + 0x14) = *(u32 *)((u8 *)data_ov000_0236aad8 + 0x68);
    *(u32 *)((u8 *)t0 + 0x18) = *(u32 *)((u8 *)data_ov000_0236aad8 + 0x6c);
}
