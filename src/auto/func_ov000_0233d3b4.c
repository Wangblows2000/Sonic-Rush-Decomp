/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac18[];
void func_ov000_0233d324();
s32 func_020372d0();
s32 func_ov000_0233d554();

void func_ov000_0233d3b4(void) {
    s32 t0 = *(u32 *)data_ov000_0236ac18;
    func_020372d0((u32)func_ov000_0233d324, 0, 7, 0xf014, 3, 0);
    func_ov000_0233d554(t0);
}
