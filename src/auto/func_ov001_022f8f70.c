/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f084[];
extern u8 data_022b4574[];
void func_ov001_022f8f54();
s32 func_020316f4();
s32 func_020315e0();

void func_ov001_022f8f70(void) {
    s32 t0 = *(u32 *)data_ov001_0231f084;
    s32 t1 = (*(u32 *)data_ov001_0231f084 + 0xc90);
    s32 v2 = (s32)data_ov001_0231f084;
    s32 v3 = t0;
    s32 v4;
    if ((*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) & 3) != 0) {
        *(u32 *)((u8 *)t1 + 0x114) = 0;
        *(u32 *)*(u32 *)((u8 *)t1 + 0xe8) = (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0xe4) + 0x8) + 0x4) << 0xc);
        s32 t5 = *(u32 *)((u8 *)t1 + 0xe4);
        *(u32 *)t5 = *(u32 *)*(u32 *)((u8 *)t1 + 0xe8);
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022f8f54;
        s32 t6 = *(u32 *)v2;
        *(u32 *)((u8 *)t6 + 0xe38) = (*(u32 *)((u8 *)*(u32 *)v2 + 0xe38) & ~0x10);
        s32 t7 = *(u32 *)v2;
        *(u32 *)((u8 *)t7 + 0xe38) = (*(u32 *)((u8 *)*(u32 *)v2 + 0xe38) | 0x20);
        v2 = t7;
        v3 = t6;
        v4 = t5;
    }
    func_020316f4(t1, v2, v3, v4);
    func_020315e0(t1);
    if ((*(u8 *)((u8 *)t1 + 0x10c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)t1 + 0x114) = 0;
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022f8f54;
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) | 0x10);
}
