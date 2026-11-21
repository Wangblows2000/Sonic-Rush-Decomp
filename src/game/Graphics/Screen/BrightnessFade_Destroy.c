#include <nitro/types.h>

extern void *gBrightnessFadeTask;
void Task_Destroy(void *p);
void BrightnessFade_Destroy(void) { if (gBrightnessFadeTask == NULL) return; Task_Destroy(gBrightnessFadeTask); }
