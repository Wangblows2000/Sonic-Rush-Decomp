/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369acc[];
s32 func_ov000_022cb03c();
s32 func_ov000_022c8d54();
s32 func_ov000_022c6924();

void func_ov000_022cbed8(s32 p0) {
    s32 t0 = (*(u32 *)data_ov000_02369acc + 0x200);
    *(u16 *)((u8 *)t0 + 0x28) = p0;
    func_ov000_022cb03c(*(u32 *)data_ov000_02369acc, t0, data_ov000_02369acc);
    func_ov000_022c8d54(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x210));
    func_ov000_022c6924(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x214));
}
