/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Spring_Action_Idle();

void Spring_State_Activated(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x13e) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x13e) = t0;
    Spring_Action_Idle(p0, ((u8 *)p0 + 0x100), t0);
}
