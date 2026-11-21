#include <nitro/types.h>

extern u32 gSystemFlags;
void OamFreezeTask_Create(BOOL on);
void DisableSwapBuffersTask_Create(u32 v);
void System_Freeze(BOOL a, BOOL b, BOOL c) { if (a) OamFreezeTask_Create(0); if (b) OamFreezeTask_Create(1); if (c) DisableSwapBuffersTask_Create(1); gSystemFlags |= 8; }
