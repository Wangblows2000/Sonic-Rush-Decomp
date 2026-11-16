#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[0x106c - 8];
    void *files[14];
} UnkStruct_022ec16c;

extern u8 data_02072cb8;
extern const char **data_ov001_0231d0d4[];

void *func_02033fbc(const char *path, void *dst);

#pragma thumb on
void func_ov001_022ec16c(UnkStruct_022ec16c *p) {
    u32 idx;
    const char **tbl;
    s32 i;

    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = data_02072cb8;
        break;
    default:
        idx = 1;
        break;
    }
    tbl = data_ov001_0231d0d4[idx];
    for (i = 0; i < 14; i++) {
        if (((p->mode == 0 || p->mode == 2) && i == 9) || ((p->mode == 1 || p->mode == 3) && i == 8)) {
            continue;
        }
        p->files[i] = func_02033fbc(tbl[i], NULL);
    }
}
#pragma thumb reset
