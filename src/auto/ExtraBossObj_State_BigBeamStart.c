/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_SnapObjToCameraX();
void ExtraBossObj_State_BigBeamFire();

void ExtraBossObj_State_BigBeamStart(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    ExtraBoss_SnapObjToCameraX();
    if ((*(u8 *)((u8 *)t0 + 0x10c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_BigBeamFire;
    *(u32 *)((u8 *)p0 + 0x1cc) = 0x3c;
    *(u32 *)((u8 *)p0 + 0xf0) = (*(u32 *)((u8 *)p0 + 0xf0) & ~0x200);
}
