/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f5a8[];
s32 func_ov001_022ffce8();
s32 AnimatorSprite_Draw();

void func_ov001_023062f0(void) {
    s32 t0 = (*(u32 *)*(u32 *)data_ov001_0231f5a8 & 1);
    if (t0 != 0) {
        s32 t1 = func_ov001_022ffce8(t0);
        s32 t2 = *(u32 *)((u8 *)t1 + 0x3c);
        AnimatorSprite_Draw((*(u32 *)((u8 *)t1 + 0x3c) * 0x48 + (*(u32 *)data_ov001_0231f5a8 + 0x98)), (*(u32 *)data_ov001_0231f5a8 + 0x98));
        AnimatorSprite_Draw(((t2 + 0x2) * 0x48 + (*(u32 *)data_ov001_0231f5a8 + 0x98)), (t2 + 0x2), (*(u32 *)data_ov001_0231f5a8 + 0x98));
    }
    if ((*(u32 *)*(u32 *)data_ov001_0231f5a8 & 2) == 0) {
        return;
    }
    AnimatorSprite_Draw((*(u32 *)data_ov001_0231f5a8 + 0x1b8), *(u32 *)data_ov001_0231f5a8);
    AnimatorSprite_Draw((*(u32 *)data_ov001_0231f5a8 + 0x200));
    AnimatorSprite_Draw((*(u32 *)data_ov001_0231f5a8 + 0x248));
    AnimatorSprite_Draw((*(u32 *)data_ov001_0231f5a8 + 0x290));
}
