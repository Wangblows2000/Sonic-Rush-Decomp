/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f5bc[];
s32 Sound_PlaySeqArc();

void func_ov001_02309994(void) {
    s32 t0 = *(u32 *)data_ov001_0231f5bc;
    *(u32 *)((u8 *)t0 + 0xbc8) = -1;
    *(u32 *)((u8 *)t0 + 0xbcc) = 0;
    Sound_PlaySeqArc(0, 0x10, 1, t0);
}
