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

extern UnkTask_02048eb4 *data_022b4574;
extern UnkDisp_02048eb4 data_0208922c;
extern UnkDisp_02048eb4 data_02087d04;

void func_020371c8(UnkTask_02048eb4 *t);

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

void func_02048eb4(void) {
    UnkWork_02048eb4 *w = data_022b4574->work;
    s16 b;

    if (w->done != NULL && *w->done != 0) {
        func_020371c8(data_022b4574);
        return;
    }
    w->count++;
    if (w->count >= w->total) {
        if (w->done != NULL)
            *w->done = 1;
        if (w->flags & 1)
            data_0208922c.bright = w->to;
        if (w->flags & 2)
            data_02087d04.bright = w->to;
        func_020371c8(data_022b4574);
        return;
    }
    CP_SetDiv32_32(w->count * w->to + w->from * (w->total - w->count), w->total);
    CP_WaitDiv();
    b = REG_DIV_RESULT_S16;
    if (w->flags & 1)
        data_0208922c.bright = b;
    if (w->flags & 2)
        data_02087d04.bright = b;
}
