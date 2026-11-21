#include <nitro/types.h>

extern u8 data_ov001_0231ea5c[];
void *SpecialStage_GetGimmickManager(void);
u8 *SpecialStageGimmick_Spawn(void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void SpecialStageTrickReward_Update(void);

void SpecialStageGimmick_SpawnTrickReward(s32 a, s32 x, s32 y, s32 z) {
    u8 *o = SpecialStageGimmick_Spawn(SpecialStage_GetGimmickManager(), 0, 0, 0, 0, 0, 0, 0);
    *(s32 *)(o + 0x20) = x;
    *(s32 *)(o + 0x24) = y;
    *(s32 *)(o + 0x28) = z;
    *(s32 *)(o + 4) = 0x155;
    *(s32 *)(o + 8) = 0x155;
    *(s32 *)(o + 0xc) = 0x155;
    *(u16 *)(o + 0x64) |= 0x4000;
    *(void (**)(void))(o + 0xcc) = SpecialStageTrickReward_Update;
    *(void **)(o + 0xbc) = data_ov001_0231ea5c;
}
