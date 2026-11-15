/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b14[];
s32 func_ov000_022d9ab0();
s32 func_ov000_022d63c0();
s32 func_ov000_022d5744();

void func_ov000_022daab0(void) {
    func_ov000_022d9ab0(*(u32 *)data_ov000_02369b14);
    func_ov000_022d63c0(*(u32 *)((u8 *)*(u32 *)data_ov000_02369b14 + 0x214));
    s32 t0 = func_ov000_022d5744(*(u32 *)((u8 *)*(u32 *)data_ov000_02369b14 + 0x218));
}
