/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b84[];

void func_ov000_022f7804(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)data_ov000_02369b84 + 0x218) + 0x340);
    *(u32 *)((u8 *)p0 + 0x10) = ((s32)*(u32 *)((u8 *)(*(u32 *)((u8 *)*(u32 *)data_ov000_02369b84 + 0x218) + 0x340) + 0x24) >> 4);
    *(u32 *)((u8 *)p0 + 0x14) = ((s32)-(*(u32 *)((u8 *)t0 + 0x28) - 0xa000) >> 4);
    *(u32 *)((u8 *)p0 + 0x18) = ((s32)(*(u32 *)((u8 *)t0 + 0x2c) + 0x46000) >> 4);
}
