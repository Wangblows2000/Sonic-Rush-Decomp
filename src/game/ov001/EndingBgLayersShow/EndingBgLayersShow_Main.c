#include <nitro/types.h>

extern void *gTaskCurrent;
void Task_Destroy(void *p);

void EndingBgLayersShow_Main(void) {
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x700;
    Task_Destroy(gTaskCurrent);
}
