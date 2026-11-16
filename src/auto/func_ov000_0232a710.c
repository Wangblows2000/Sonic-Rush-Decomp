/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_0232a710(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x10) = (*(u32 *)((u8 *)t0 + 0x10) + *(s16 *)((u8 *)p0 + 0x2e));
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x14) + *(s16 *)((u8 *)p0 + 0x30));
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    s32 t1 = *(u32 *)((u8 *)p0 + 0xc);
    if (t1 > 0x20) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
