#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1e08];
    void *res[16];
} UnkStruct_022ce9b0;

extern u8 data_02072cb8;
extern char **data_ov001_0231a9a4[];

void *func_02033fbc(char *name, s32 a);

#pragma thumb on
void func_ov001_022ce9b0(UnkStruct_022ce9b0 *p) {
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
    names = data_ov001_0231a9a4[lang];
    for (i = 0; i < 16; i++) {
        p->res[i] = func_02033fbc(names[i], 0);
    }
}
#pragma thumb reset
