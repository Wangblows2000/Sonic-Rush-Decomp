#include <nitro/types.h>

typedef struct {
    s32 mode;
    u8 pad4[0x14];
    void *files[15];
    u8 pad54[0x414 - 0x54];
    void *files2[3];
} UnkStruct_022d2978;

extern u8 data_02072cb8;
extern const char *data_ov001_0231afe0[];
extern const char *data_ov001_0231b01c[];
extern const char **data_ov001_0231afc0[];
extern u8 data_ov001_0231b058[];

void *func_02033fbc(const char *path, void *dst);
void func_02036854(void *p);

#pragma thumb on
void func_ov001_022d2978(UnkStruct_022d2978 *p) {
    const char **tbl;
    u32 idx;
    const char **names;
    s32 i;

    if (p->mode == 2) {
        tbl = data_ov001_0231afe0;
    } else {
        tbl = data_ov001_0231b01c;
    }
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
    names = data_ov001_0231afc0[idx];
    for (i = 0; i < 15; i++) {
        if (tbl[i] != NULL) {
            p->files[i] = func_02033fbc(tbl[i], (void *)-1);
        }
    }
    for (i = 0; i < 3; i++) {
        p->files2[i] = func_02033fbc(names[i], (void *)-1);
    }
    func_02036854(data_ov001_0231b058);
}
#pragma thumb reset
