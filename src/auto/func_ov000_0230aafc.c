/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void func_ov000_0230aa24();

void func_ov000_0230aafc(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    s32 v1 = 2;
    if (t0 != 0) {
        s32 v2 = v1;
        if (*(u16 *)((u8 *)t0 + 0x9c) == 4) {
            v2 = 5;
        }
        v1 = v2;
    }
    StageObj_SetAnimation(p0, v1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0230aa24;
}
