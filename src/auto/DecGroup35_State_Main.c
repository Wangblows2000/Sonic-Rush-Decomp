/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void DecGroup35_State_Main(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x54) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    s32 t1 = *(u32 *)((u8 *)p0 + 0xc);
    if ((u32)t1 > (u32)t0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~2);
        *(u32 *)((u8 *)p0 + 0xc) = 0;
    }
}
