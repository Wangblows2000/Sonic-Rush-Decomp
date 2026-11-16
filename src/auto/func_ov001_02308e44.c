/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f5bc[];
s32 AnimatorSprite_Draw();
s32 func_ov001_02308430();
s32 func_ov001_02308600();

void func_ov001_02308e44(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_ov001_0231f5bc + 0xbcc);
    s32 t1 = *(u32 *)data_ov001_0231f5bc;
    if (t0 >= 1) {
        if (t0 < 0x78) {
            AnimatorSprite_Draw((t1 + 0x7c8));
        }
    }
    func_ov001_02308430((*(u32 *)((u8 *)t1 + 0xbcc) - 0x1));
    func_ov001_02308600();
}
