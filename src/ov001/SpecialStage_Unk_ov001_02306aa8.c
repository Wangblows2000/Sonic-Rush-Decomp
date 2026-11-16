#include <nitro/types.h>

void **SpecialStage_GetGimmickManager(void);
void *SpecialStageGimmick_Spawn(void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void SpecialStage_Unk_ov001_0231066c(void *a, void *b);

void SpecialStage_Unk_ov001_02306aa8(u8 *p) {
    void *r;
    if (*(s32 *)(p + 0x6c) != 0xa) {
        return;
    }
    r = SpecialStageGimmick_Spawn(SpecialStage_GetGimmickManager(), *(s32 *)(p + 0x78), 0, 0, 0, *(u16 *)(p + 0x16), *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c));
    SpecialStage_Unk_ov001_0231066c(*SpecialStage_GetGimmickManager(), r);
}
