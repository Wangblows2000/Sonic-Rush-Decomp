/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f084[];
extern u8 gTaskCurrent[];
void func_ov001_022f8f54();
void func_ov001_022f8f70();
s32 Sound_PlaySeqArc();

s32 func_ov001_022f904c(void) {
    s32 t0 = *(u32 *)data_ov001_0231f084;
    if ((*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) & 3) != 0) {
        *(u32 *)((u8 *)(t0 + 0xc90) + 0x114) = 0;
        *(u32 *)*(u32 *)((u8 *)(t0 + 0xc90) + 0xe8) = (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)(t0 + 0xc90) + 0xe4) + 0x8) + 0x4) << 0xc);
        *(u32 *)*(u32 *)((u8 *)(t0 + 0xc90) + 0xe4) = *(u32 *)*(u32 *)((u8 *)(t0 + 0xc90) + 0xe8);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022f8f54;
        *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) & ~0x10);
        *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe38) | 0x20);
    }
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe44) = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe44) - 0x1);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe44);
    if (t1 > 0) {
        return t1;
    }
    s32 t2 = *(u32 *)gTaskCurrent;
    *(u32 *)((u8 *)t2 + 0x8) = (u32)func_ov001_022f8f70;
    s32 t3 = Sound_PlaySeqArc(0, 8, 6, t2);
    return t3;
}
