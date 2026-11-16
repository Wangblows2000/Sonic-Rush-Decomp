/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_HandBoss_SelectNextAttack();

void ExtraBossObj_State_Unk_ov001_022efe50(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x18) + *(u32 *)((u8 *)p0 + 0x50));
    if (t1 > 0x153000) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x18) = 0x153000;
    *(u32 *)((u8 *)p0 + 0x54) = 0;
    s32 t2 = *(u32 *)((u8 *)p0 + 0x54);
    *(u32 *)((u8 *)p0 + 0x50) = t2;
    ExtraBoss_HandBoss_SelectNextAttack(p0, t2, t1, t0);
}
