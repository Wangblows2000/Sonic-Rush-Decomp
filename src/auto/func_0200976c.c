/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207eec0[];

void func_0200976c(s32 p0) {
    if ((((s32)(s32)p0 >> 8) & 0xff) != *(u8 *)((u8 *)(((s32)p0 & 0xff) * 0xc + (s32)data_0207eec0) + 0x8)) {
        return;
    }
    s32 t0 = *(u32 *)(((s32)p0 & 0xff) * 0xc + (s32)data_0207eec0);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(*(u32 *)((u8 *)(((s32)p0 & 0xff) * 0xc + (s32)data_0207eec0) + 0x4), t0, ((s32)p0 & 0xff), (((s32)p0 & 0xff) * 0xc + (s32)data_0207eec0));
}
