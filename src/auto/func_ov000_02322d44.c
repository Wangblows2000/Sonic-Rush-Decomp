/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a660[];

void func_ov000_02322d44(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)data_ov000_0236a660 + (*(u32 *)((u8 *)p0 + 0x8) << 2)) = 0;
    }
    *(u32 *)((u8 *)p0 + 0xc) = 0;
}
