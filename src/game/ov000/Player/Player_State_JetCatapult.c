/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_Action_JetCatapultLaunch();

void Player_State_JetCatapult(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x1c) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x20) = *(u32 *)((u8 *)p0 + 0x14);
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x14) - 0xc00);
    *(u8 *)((u8 *)p0 + 0x1) = *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x1);
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 1) == 0) {
        if (*(s16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x34) == 0) {
            return;
        }
        *(u8 *)((u8 *)p0 + 0x3d1) = 0x10;
        *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 1);
        return;
    }
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x58) & 0x100);
    if (t0 == 0) {
        return;
    }
    Player_Action_JetCatapultLaunch(p0, t0);
}
