#include <nitro/types.h>

extern u32 gSystemFlags;
void OamUnfreezeTask_Create(s32 a);
void DisableSwapBuffersTask_Create(s32 a);
void System_Unfreeze(void) { OamUnfreezeTask_Create(0); OamUnfreezeTask_Create(1); DisableSwapBuffersTask_Create(0); gSystemFlags &= ~8; }
