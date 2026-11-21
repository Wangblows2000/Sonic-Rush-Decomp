#include <nitro/types.h>

typedef struct { s32 x; s32 y; s32 z; s32 timer; } UnkWork_02057174;
typedef struct { u8 pad0[0x10]; UnkWork_02057174 *work; } UnkTask_02057174;
extern UnkTask_02057174 *gTaskCurrent;
extern void *gPlayerDemoTask;
extern u32 gRandSeed;

void Task_Destroy(UnkTask_02057174 *t);

#pragma thumb on
static inline u16 Rand_02057174(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void PlayerDemoCameraShake_Main(void) {
    UnkWork_02057174 *w = gTaskCurrent->work;

    if (gPlayerDemoTask == NULL) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->timer--;
    if (w->timer == 0) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->x = 0xfff - (Rand_02057174() & 0x1ffe);
    w->y = 0xfff - (Rand_02057174() & 0x1ffe);
    w->z = 0xfff - (Rand_02057174() & 0x1ffe);
}
#pragma thumb reset
