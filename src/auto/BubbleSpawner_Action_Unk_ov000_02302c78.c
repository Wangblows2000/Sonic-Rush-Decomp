/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void BubbleSpawner_State_Unk_ov000_02302a28();

void BubbleSpawner_Action_Unk_ov000_02302c78(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BubbleSpawner_State_Unk_ov000_02302a28;
}
