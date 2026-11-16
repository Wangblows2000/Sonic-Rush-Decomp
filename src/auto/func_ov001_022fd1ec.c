/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBossObj_IsOutOfArena();

void func_ov001_022fd1ec(u8 *p0) {
    s32 t0 = ExtraBossObj_IsOutOfArena(*(u32 *)((u8 *)p0 + 0x18), *(u32 *)((u8 *)p0 + 0x1c), 0x30000, 0x30000);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) | 8);
    }
}
