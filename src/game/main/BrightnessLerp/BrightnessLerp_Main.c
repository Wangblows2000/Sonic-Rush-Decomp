#include <nitro/types.h>

typedef struct {
    s32 count;
    s32 total;
    s16 from;
    s16 to;
    u32 flags;
    u16 *done;
} UnkWork_02048eb4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02048eb4 *work;
} UnkTask_02048eb4;

typedef struct {
    u8 pad0[0x48];
    s16 bright;
} UnkDisp_02048eb4;

extern UnkTask_02048eb4 *gTaskCurrent;
extern UnkDisp_02048eb4 gDisplayMain;
extern UnkDisp_02048eb4 gDisplaySub;

void Task_Destroy(UnkTask_02048eb4 *t);

#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER (*(vu32 *)0x04000290)
#define REG_DIV_DENOM (*(volatile u64 *)0x04000298)
#define REG_DIV_RESULT_S16 (*(volatile s16 *)0x040002a0)

static inline void CP_SetDiv32_32(u32 numer, u32 denom) {
    REG_DIVCNT = 0;
    REG_DIV_NUMER = numer;
    REG_DIV_DENOM = denom;
}

static inline void CP_WaitDiv(void) {
    while (REG_DIVCNT & 0x8000)
        ;
}

void BrightnessLerp_Main(void) {
    UnkWork_02048eb4 *w = gTaskCurrent->work;
    s16 b;

    if (w->done != NULL && *w->done != 0) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->count++;
    if (w->count >= w->total) {
        if (w->done != NULL)
            *w->done = 1;
        if (w->flags & 1)
            gDisplayMain.bright = w->to;
        if (w->flags & 2)
            gDisplaySub.bright = w->to;
        Task_Destroy(gTaskCurrent);
        return;
    }
    CP_SetDiv32_32(w->count * w->to + w->from * (w->total - w->count), w->total);
    CP_WaitDiv();
    b = REG_DIV_RESULT_S16;
    if (w->flags & 1)
        gDisplayMain.bright = b;
    if (w->flags & 2)
        gDisplaySub.bright = b;
}
