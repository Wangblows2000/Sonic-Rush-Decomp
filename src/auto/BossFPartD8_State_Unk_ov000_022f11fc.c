/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_Unk_ov000_022f48c4();
s32 Sound_PlaySeqArc();
void BossFPartD8_State_Unk_ov000_022f11dc();

void BossFPartD8_State_Unk_ov000_022f11fc(u8 *p0) {
    BossFPartD8_Unk_ov000_022f48c4();
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x554) == 0x24000) {
        Sound_PlaySeqArc(0, 0, 0xbf);
    }
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x554) == 0x3c000) {
        Sound_PlaySeqArc(0, 0, 0xbf);
    }
    s32 t0 = (*(u8 *)((u8 *)p0 + 0x57c) & 2);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f11dc;
    }
}
