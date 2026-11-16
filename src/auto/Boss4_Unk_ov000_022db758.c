/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossArena_ArcToCirclePos();

void Boss4_Unk_ov000_022db758(u8 *p0, s32 p1, s32 p2, s32 p3) {
    if ((s32)p0 != 0) {
        BossArena_ArcToCirclePos(0x128fb8, 0x749fed, ((s32)p2 << 4), p0, ((u8 *)p0 + 0x8), p1);
        *(u32 *)((u8 *)p0 + 0x4) = (-(s32)p3 << 4);
        return;
    }
    BossArena_ArcToCirclePos(0x128fb8, 0x749fed, ((s32)p2 << 4), 0, 0, p1);
}
