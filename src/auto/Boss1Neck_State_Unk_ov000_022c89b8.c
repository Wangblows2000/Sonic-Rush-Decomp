/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Sound_PlaySeqArc();
void Boss1Neck_State_Unk_ov000_022c8978();

void Boss1Neck_State_Unk_ov000_022c89b8(u8 *p0) {
    *(u32 *)((u8 *)*(u32 *)gBoss1 + 0x20c) = (*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x20c) & ~1);
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214) + 0x200);
    *(u16 *)((u8 *)t0 + 0x9a) = (*(u16 *)((u8 *)(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214) + 0x200) + 0x9a) & ~4);
    Sound_PlaySeqArc(0, 0, 0xea, t0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Boss1Neck_State_Unk_ov000_022c8978;
}
