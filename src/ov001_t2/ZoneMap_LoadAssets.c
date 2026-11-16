#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1cd4];
    void *files[6];
} UnkStruct_022e89b4;

extern u8 gGameLanguage;
extern const char **gZoneMapFileTable[];

void *File_Load(const char *path, void *dst);
void MsgBox_PreloadFiles(s32 a, s32 b, s32 c);

#pragma thumb on
void ZoneMap_LoadAssets(UnkStruct_022e89b4 *p) {
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
    tbl = gZoneMapFileTable[idx];
    for (i = 0; i < 6; i++) {
        p->files[i] = File_Load(tbl[i], NULL);
    }
    MsgBox_PreloadFiles(0x11, 3, 2);
    MsgBox_PreloadFiles(0x12, 3, 1);
}
#pragma thumb reset
