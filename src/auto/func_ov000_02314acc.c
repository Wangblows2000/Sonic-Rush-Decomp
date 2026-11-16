/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 RopeCSpring_Action_Idle();

void func_ov000_02314acc(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)p0 + 0x44);
    if (t0 > 0x100) {
        *(u16 *)((u8 *)p0 + 0x44) = (t0 - 0x20);
        *(u16 *)((u8 *)p0 + 0x46) = *(s16 *)((u8 *)p0 + 0x44);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x88);
    if (t1 == 0) {
        return;
    }
    if ((*(u32 *)((u8 *)t1 + 0x54) & 2) != 0) {
        return;
    }
    *(u16 *)((u8 *)p0 + 0x44) = 0;
    s32 t2 = *(s16 *)((u8 *)p0 + 0x44);
    *(u16 *)((u8 *)p0 + 0x46) = t2;
    RopeCSpring_Action_Idle(p0, t2);
}
