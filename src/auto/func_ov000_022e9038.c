/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 AnimatorSprite_SetAnimation();
void func_ov000_022e8e6c();

void func_ov000_022e9038(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x3d8) & 0x80) != 0) {
        return;
    }
    AnimatorSprite_SetAnimation((*(u32 *)((u8 *)p0 + 0x484) + 0x1c4), ((*(u32 *)((u8 *)p0 + 0x3d0) == 0) ? 1 : 3));
    *(u32 *)((u8 *)p0 + 0x484) = 0;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)func_ov000_022e8e6c;
}
