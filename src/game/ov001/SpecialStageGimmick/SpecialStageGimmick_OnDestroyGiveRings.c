/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStageActObj_SpawnAt();
s32 Sound_PlaySeqArc();
s32 SpecialStage_GetPlayer();

void SpecialStageGimmick_OnDestroyGiveRings(u8 *p0) {
    SpecialStageActObj_SpawnAt(1, *(u32 *)((u8 *)p0 + 0x20), *(u32 *)((u8 *)p0 + 0x24), *(u32 *)((u8 *)p0 + 0x28));
    Sound_PlaySeqArc(0, 0x10, 0x11);
    s32 t1 = SpecialStage_GetPlayer();
    *(u32 *)((u8 *)t1 + 0x148) = (*(u32 *)((u8 *)t1 + 0x148) + *(u16 *)((u8 *)p0 + 0x5e));
}
