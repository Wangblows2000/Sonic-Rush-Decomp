/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObject_Spawn();
s32 Sound_PlaySeqArc();

void BossFPartD8_Unk_ov000_022f89f4(void) {
    MapObject_Spawn(0xd9, 0x52600, 0x1a000, 1, 0, 0, 0, 0, 0);
    Sound_PlaySeqArc(0, 0, 0xc7);
}
