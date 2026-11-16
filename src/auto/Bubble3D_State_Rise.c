/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjSpd_UpSet();
extern u8 gGameState[];
s32 MapCollision_ProbeDistance();
extern u8 gCamera[];

void Bubble3D_State_Rise(u8 *p0) {
    s32 t0 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x30), -13, *(u32 *)((u8 *)p0 + 0x8));
    *(u16 *)((u8 *)p0 + 0x30) = t0;
    *(u16 *)((u8 *)p0 + 0x2e) = ((s32)*(s16 *)((u8 *)p0 + 0x30) >> 1);
    s32 t1 = (*(u32 *)((u8 *)gGameState + 0x20) & 2);
    if (t1 != 0) {
        *(u16 *)((u8 *)p0 + 0x2e) = -*(s16 *)((u8 *)p0 + 0x2e);
    }
    s32 t2 = MapCollision_ProbeDistance(((s32)*(u32 *)((u8 *)p0 + 0x10) >> 8), ((s32)*(u32 *)((u8 *)p0 + 0x14) >> 8), 1, 2, 0, 0);
    if (*(u16 *)((u8 *)gCamera + 0x62) <= ((s32)*(u32 *)((u8 *)p0 + 0x14) >> 8)) {
        if (t2 >= 0) {
            return;
        }
    }
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}
