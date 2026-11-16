/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 Sound_PlaySeqArc();
void Boss7_State_Unk_ov000_022ed5ac();

void Boss7_State_Unk_ov000_022ed658(u8 *p0) {
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x218) + 0x320) = 0x3000;
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x21c) + 0x320) = 0x3000;
    if (*(u8 *)((u8 *)*(u32 *)gPlayers + 0x324) == 0) {
        Sound_PlaySeqArc(0, 0, 9);
    } else {
        Sound_PlaySeqArc(0, 0, 0x16);
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss7_State_Unk_ov000_022ed5ac;
}
