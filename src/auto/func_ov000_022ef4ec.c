/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
void func_ov000_022ef360();

void func_ov000_022ef4ec(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)gDisplayMain + 0x48);
    if (t0 > 0) {
        *(u16 *)((u8 *)gDisplayMain + 0x48) = (t0 - 0x2);
    }
    s32 t1 = *(s16 *)((u8 *)p0 + 0x434);
    *(u16 *)((u8 *)p0 + 0x434) = (*(s16 *)((u8 *)p0 + 0x434) + 0x1);
    if (t1 > 0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ef360;
    }
}
