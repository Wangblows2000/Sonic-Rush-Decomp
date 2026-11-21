/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayMain[];
void Boss7_State_WinCut2();

void Boss7_State_WinCutWait1(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)gDisplayMain + 0x48);
    if (t0 > 0) {
        *(u16 *)((u8 *)gDisplayMain + 0x48) = (t0 - 0x2);
    }
    s32 t1 = *(u16 *)((u8 *)p0 + 0x230);
    *(u16 *)((u8 *)p0 + 0x230) = (*(u16 *)((u8 *)p0 + 0x230) + 0x1);
    if ((u32)t1 > (u32)0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss7_State_WinCut2;
    }
}
