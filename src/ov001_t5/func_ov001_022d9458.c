#include <nitro/types.h>

typedef struct {
    u8 pad0[0x808];
    void *res[9];
} UnkStruct_022d9458;

extern u8 data_02072cb8;
extern char **data_ov001_0231bd18[];

void *func_02033fbc(char *name, s32 a);

#pragma thumb on
void func_ov001_022d9458(UnkStruct_022d9458 *p) {
    u32 lang;
    char **names;
    s32 i;

    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        lang = data_02072cb8;
        break;
    default:
        lang = 1;
        break;
    }
    names = data_ov001_0231bd18[lang];
    for (i = 0; i < 9; i++) {
        p->res[i] = func_02033fbc(names[i], 0);
    }
}
#pragma thumb reset
