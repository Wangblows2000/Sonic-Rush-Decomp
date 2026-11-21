/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossArena_ArcToCirclePos();
s32 Model_Draw();

void RingManager_DrawRingArena1(u8 *p0, u8 *p1) {
    BossArena_ArcToCirclePos(0x1152f0, 0x6cd988, (*(u32 *)p1 << 4), ((u8 *)p0 + 0xec), ((u8 *)p0 + 0xf4), 0);
    *(u32 *)((u8 *)p0 + 0xf0) = (-*(u32 *)((u8 *)p1 + 0x4) << 4);
    Model_Draw(((u8 *)p0 + 0xa4));
}
