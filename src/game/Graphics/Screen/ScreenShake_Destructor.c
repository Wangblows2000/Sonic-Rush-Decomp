#include <nitro/types.h>

extern u32 gScreenShakeTask;
void ScreenShake_Destructor(void) { gScreenShakeTask = 0; }
