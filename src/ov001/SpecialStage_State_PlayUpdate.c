#include <nitro/types.h>

extern u16 gPad[];
extern u8 *gSpecialStageWork;
BOOL SpecialStage_Unk_ov001_02309894(void);
void PauseMenu_Create(s32 a);
void SpecialStageData_Update(void);
void SpecialStage_Unk_ov001_023147d8(void);
void SpecialStagePlayer_Update(void);
void SpecialStageCamera_Update(void);
void SpecialStageMap_Update(void);
void SpecialStageGimmick_Update(void *p);
void SpecialStage_UpdateObjects(void);
void SpecialStage_CheckGimmickCollisions(void);
void SpecialStageHud_Update(void);

void SpecialStage_State_PlayUpdate(void) {
    u8 *g = gSpecialStageWork;
    if ((gPad[2] & 8) && !SpecialStage_Unk_ov001_02309894()) {
        PauseMenu_Create(1);
        return;
    }
    if (*(s32 *)(g + 0x20) != 0) {
        return;
    }
    SpecialStageData_Update();
    SpecialStage_Unk_ov001_023147d8();
    if (*(s32 *)(g + 0x18) != 0) {
        SpecialStagePlayer_Update();
    }
    SpecialStageCamera_Update();
    SpecialStageMap_Update();
    SpecialStageGimmick_Update(*(void **)(g + 0x1c));
    SpecialStage_UpdateObjects();
    SpecialStage_CheckGimmickCollisions();
    SpecialStageHud_Update();
    *(s32 *)(g + 0x28) += 1;
}
