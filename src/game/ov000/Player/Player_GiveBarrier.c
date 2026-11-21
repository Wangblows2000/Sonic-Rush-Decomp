/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 Barrier_Create();

void Player_GiveBarrier(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x30000000) == 0) {
        Sound_PlaySeqArcEx(0, 0, 0x35);
        Barrier_Create(p0);
    }
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x10000000);
}
