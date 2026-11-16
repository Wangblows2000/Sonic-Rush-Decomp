/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f080[];
extern u8 data_ov001_0231d52c[];

void func_ov001_022f7224(s32 p0) {
    s32 t0 = ((s32)p0 * 0x44 + *(u32 *)data_ov001_0231f080);
    ((s32 (*)())*(u32 *)((u8 *)data_ov001_0231d52c + ((s32)p0 << 2)))(((s32)p0 * 0x44 + *(u32 *)data_ov001_0231f080), *(u32 *)((u8 *)data_ov001_0231d52c + ((s32)p0 << 2)), p0);
    *(u32 *)((u8 *)t0 + 0x40) = 0;
    *(u32 *)((u8 *)t0 + 0x3c) = *(u32 *)((u8 *)t0 + 0x40);
    *(u32 *)((u8 *)t0 + 0x38) = *(u32 *)((u8 *)t0 + 0x3c);
}
