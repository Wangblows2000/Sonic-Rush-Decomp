#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1cd4];
    void *files[6];
} UnkStruct_022e89b4;

extern u8 data_02072cb8;
extern const char **data_ov001_0231c538[];

void *func_02033fbc(const char *path, void *dst);
void func_0204ff50(s32 a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022e89b4(UnkStruct_022e89b4 *p) {
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
    tbl = data_ov001_0231c538[idx];
    for (i = 0; i < 6; i++) {
        p->files[i] = func_02033fbc(tbl[i], NULL);
    }
    func_0204ff50(0x11, 3, 2);
    func_0204ff50(0x12, 3, 1);
}
#pragma thumb reset
