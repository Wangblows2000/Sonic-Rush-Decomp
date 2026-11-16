/* paramclone of func_ov001_022d6c98 (src/ov001_t/func_ov001_022d6c98.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5e0];
    void *res[5];
} UnkStruct_022d4048;

extern u8 data_02072cb8;
extern void **data_ov001_0231b478[];

void *func_02033fbc(void *p, s32 a);

#pragma thumb on
static inline u32 GetLang_022d4048(void) {
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

void func_ov001_022d4048(UnkStruct_022d4048 *p) {
    void **tbl = data_ov001_0231b478[GetLang_022d4048()];
    s32 i;

    for (i = 0; i < 5; i++) {
        p->res[i] = func_02033fbc(tbl[i], 0);
    }
}
#pragma thumb reset
