#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1cd4];
    void *files[6];
} UnkStruct_022e89b4;

extern u8 gGameLanguage;
extern const char **data_ov001_0231c538[];

void *File_Load(const char *path, void *dst);
void func_0204ff50(s32 a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022e89b4(UnkStruct_022e89b4 *p) {
    u32 idx;
    const char **tbl;
    s32 i;

    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = gGameLanguage;
        break;
    default:
        idx = 1;
        break;
    }
    tbl = data_ov001_0231c538[idx];
    for (i = 0; i < 6; i++) {
        p->files[i] = File_Load(tbl[i], NULL);
    }
    func_0204ff50(0x11, 3, 2);
    func_0204ff50(0x12, 3, 1);
}
#pragma thumb reset
