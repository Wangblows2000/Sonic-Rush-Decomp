/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossArena_CirclePosToArc();

void Boss4_ArenaToStagePos(u8 *p0, u8 *p1) {
    BossArena_CirclePosToArc(0x128fb8, 0x749fed, *(u32 *)p0, *(u32 *)((u8 *)p0 + 0x8), ((u8 *)p1 + 0x10));
    *(u32 *)((u8 *)p1 + 0x10) = ((s32)*(u32 *)((u8 *)p1 + 0x10) >> 4);
    *(u32 *)((u8 *)p1 + 0x14) = ((s32)-*(u32 *)((u8 *)p0 + 0x4) >> 4);
}
