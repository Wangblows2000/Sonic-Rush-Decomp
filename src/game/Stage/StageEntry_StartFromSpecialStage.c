#include <nitro/types.h>

void Camera_Reset(void);
void StageEvent_Reset(void);
void StageStart_Create(s32 a);
void StageEntry_StartFromSpecialStage(void) { Camera_Reset(); StageEvent_Reset(); StageStart_Create(3); }
