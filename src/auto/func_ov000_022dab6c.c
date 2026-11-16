/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b14[];
s32 func_ov000_022d9cbc();
s32 func_ov000_022d7204();

void func_ov000_022dab6c(s32 p0) {
    s32 t0 = (*(u32 *)data_ov000_02369b14 + 0x200);
    *(u16 *)((u8 *)t0 + 0x22) = p0;
    func_ov000_022d9cbc(*(u32 *)data_ov000_02369b14, t0, data_ov000_02369b14);
    func_ov000_022d7204(*(u32 *)((u8 *)*(u32 *)data_ov000_02369b14 + 0x214));
}
