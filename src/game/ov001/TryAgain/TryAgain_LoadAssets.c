/* paramclone of TitleDemo_LoadAssets (src/ov001_t/TitleDemo_LoadAssets.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5e0];
    void *res[5];
} UnkStruct_022d4048;

extern u8 gGameLanguage;
extern void **gTryAgainFileTable[];

void *File_Load(void *p, s32 a);

#pragma thumb on
static inline u32 GetLang_022d4048(void) {
    u32 l;
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        l = gGameLanguage;
        break;
    default:
        l = 1;
        break;
    }
    return l;
}

void TryAgain_LoadAssets(UnkStruct_022d4048 *p) {
    void **tbl = gTryAgainFileTable[GetLang_022d4048()];
    s32 i;

    for (i = 0; i < 5; i++) {
        p->res[i] = File_Load(tbl[i], 0);
    }
}
#pragma thumb reset
