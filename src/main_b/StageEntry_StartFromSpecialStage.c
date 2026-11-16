#include <nitro/types.h>

void Camera_Reset(void);
void func_ov000_02335e70(void);
void StageStart_Create(s32 a);
void StageEntry_StartFromSpecialStage(void) { Camera_Reset(); func_ov000_02335e70(); StageStart_Create(3); }
