/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSpecialStageTrick[];
s32 memset();

void func_ov001_023145d8(void) {
    s32 t0 = *(u32 *)gSpecialStageTrick;
    memset(*(u32 *)gSpecialStageTrick, 0xff, 0x100);
    *(u16 *)(t0 + 0x100) = 0;
    *(u16 *)((u8 *)(t0 + 0x100) + 0x2) = 0;
    *(u16 *)((u8 *)(t0 + 0x100) + 0x4) = 0;
}
