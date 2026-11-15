/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
extern u8 data_ov000_0236aae0[];

void func_ov000_0233905c(s32 p0, s32 p1) {
    *(u32 *)((u8 *)data_ov000_0236aae0 + (p1 * 0x64)) = (*(u32 *)((u8 *)data_ov000_0236aad8 + 0xd8) + *(u32 *)((u8 *)data_ov000_0236aae0 + (((s32)((u8 *)p1 + 0x1) & 1) * 0x64)));
}
