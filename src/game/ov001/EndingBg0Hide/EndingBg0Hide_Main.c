#include <nitro/types.h>

extern void *gTaskCurrent;
void Task_Destroy(void *t);

void EndingBg0Hide_Main(void) {
    {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~1) << 8);
    }
    Task_Destroy(gTaskCurrent);
}
