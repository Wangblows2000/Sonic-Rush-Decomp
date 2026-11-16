#include <nitro/types.h>

typedef struct {
    u8 pad0[0x15c];
    void *res[4];
} UnkStruct_022d4ed4;

extern u8 data_02072cb8;
extern void **data_ov001_0231b6f0[];

void *func_02033fbc(void *p, s32 a);

#pragma thumb on
static inline u32 GetLang_022d4ed4(void) {
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

void func_ov001_022d4ed4(UnkStruct_022d4ed4 *p) {
    void **tbl = data_ov001_0231b6f0[GetLang_022d4ed4()];
    s32 i;

    for (i = 0; i < 4; i++) {
        p->res[i] = func_02033fbc(tbl[i], 0);
    }
}
#pragma thumb reset
