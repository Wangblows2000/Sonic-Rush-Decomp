#include <nitro/types.h>

extern u32 gBrightnessFadeTask;
void BrightnessFade_Destructor(void) { gBrightnessFadeTask = 0; }
