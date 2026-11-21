/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_SnapObjToCameraX();
void ExtraBossObj_State_BigBeamStart();

void ExtraBossObj_State_BigBeamDelay(u8 *p0) {
    ExtraBoss_SnapObjToCameraX();
    *(u32 *)((u8 *)p0 + 0x1cc) = (*(u32 *)((u8 *)p0 + 0x1cc) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1cc);
    if (t0 <= 0) {
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) & ~0x10000);
        *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_BigBeamStart;
    }
}
