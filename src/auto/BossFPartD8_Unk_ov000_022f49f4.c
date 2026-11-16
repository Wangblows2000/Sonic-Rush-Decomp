/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 BossFPartD8_Unk_ov000_022f4a84();

void BossFPartD8_Unk_ov000_022f49f4(s32 p0) {
    BossFPartD8_Unk_ov000_022f4a84(p0, *(u32 *)((u8 *)*(u32 *)gPlayers + 0x10), 0x4000, 0x300, 0x80, 0x40, 0x600);
}
