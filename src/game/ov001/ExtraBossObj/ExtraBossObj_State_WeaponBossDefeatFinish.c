/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetWork();

void ExtraBossObj_State_WeaponBossDefeatFinish(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x14) ^ 0x30000);
    *(u32 *)((u8 *)p0 + 0x14) = t0;
    if (*(u16 *)((u8 *)p0 + 0x1c8) == 0) {
        return;
    }
    s32 t1 = ExtraBoss_GetWork(*(u16 *)((u8 *)p0 + 0x1c8), t0);
    *(u32 *)t1 = (*(u32 *)t1 | 0x400);
    *(u32 *)((u8 *)p0 + 0x74) = 0;
}
