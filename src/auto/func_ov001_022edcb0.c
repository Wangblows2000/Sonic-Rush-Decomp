/* paramclone of func_ov000_0235a270 (src/ov000_d/func_ov000_0235a270.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7e0];
    void *res[8];
} UnkStruct_022edcb0;

extern const char *data_ov001_0231d3a0[];
extern const char *data_ov001_0231d3c0[];
extern u8 data_02072cb8;

void *func_02033fbc(const char *path, void *dst);

#pragma thumb on

void func_ov001_022edcb0(UnkStruct_022edcb0 *w) {
    s32 i;
    u8 cur;
    s32 lang;
    const char **tbl;

    i = 0;
    tbl = data_ov001_0231d3a0;
    cur = data_02072cb8;
    for (; i < 8; i++) {
        if (i == 1) {
            switch (cur) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                lang = cur;
                break;
            default:
                lang = 1;
                break;
            }
            w->res[i] = func_02033fbc(data_ov001_0231d3c0[lang], NULL);
        } else {
            w->res[i] = func_02033fbc(tbl[i], NULL);
        }
    }
}

#pragma thumb reset
