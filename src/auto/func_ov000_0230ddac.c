/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_ov000_02366b3c[];

void func_ov000_0230ddac(u8 *p0) {
    if ((*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x20) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) = *(u8 *)((u8 *)data_ov000_02366b3c + (((s32)*(u32 *)((u8 *)p0 + 0xc) >> 5) & 7));
}
