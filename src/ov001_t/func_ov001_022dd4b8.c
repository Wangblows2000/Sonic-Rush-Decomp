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

extern UnkTask_022dd4b8 *gTaskCurrent;
extern u8 gGameLanguage;
extern void **data_ov001_0231c18c[];

void *File_Load(void *p, s32 a);
void func_ov001_022dd42c(void);

#pragma thumb on
static inline u32 GetLang_022dd4b8(void) {
    u32 l;
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        l = gGameLanguage;
        break;
    default:
        l = 1;
        break;
    }
    return l;
}

void func_ov001_022dd4b8(void) {
    UnkWork_022dd4b8 *w = gTaskCurrent->work;
    void **tbl = data_ov001_0231c18c[GetLang_022dd4b8()];
    s32 i = w->step;
    UnkWork_022dd4b8 *dst;

    tbl += i;
    dst = (UnkWork_022dd4b8 *)((void **)w + i);
    if (i < 5) {
        dst->res[0] = File_Load(*tbl, 0);
        i++;
    }
    w->step++;
    if (i >= 5) {
        w->step = 0;
        gTaskCurrent->func = func_ov001_022dd42c;
    }
}
#pragma thumb reset
