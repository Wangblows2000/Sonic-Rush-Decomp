#include <nitro/types.h>

typedef struct {
    s32 step;
    u8 pad4[0xdb8 - 4];
    void *res[5];
} UnkWork_022dd4b8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dd4b8 *work;
} UnkTask_022dd4b8;

extern UnkTask_022dd4b8 *data_022b4574;
extern u8 data_02072cb8;
extern void **data_ov001_0231c18c[];

void *func_02033fbc(void *p, s32 a);
void func_ov001_022dd42c(void);

#pragma thumb on
static inline u32 GetLang_022dd4b8(void) {
    u32 l;
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        l = data_02072cb8;
        break;
    default:
        l = 1;
        break;
    }
    return l;
}

void func_ov001_022dd4b8(void) {
    UnkWork_022dd4b8 *w = data_022b4574->work;
    void **tbl = data_ov001_0231c18c[GetLang_022dd4b8()];
    s32 i = w->step;
    UnkWork_022dd4b8 *dst;

    tbl += i;
    dst = (UnkWork_022dd4b8 *)((void **)w + i);
    if (i < 5) {
        dst->res[0] = func_02033fbc(*tbl, 0);
        i++;
    }
    w->step++;
    if (i >= 5) {
        w->step = 0;
        data_022b4574->func = func_ov001_022dd42c;
    }
}
#pragma thumb reset
