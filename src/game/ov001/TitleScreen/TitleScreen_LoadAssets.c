#include <nitro/types.h>

typedef struct {
    u8 pad0[0x15c];
    void *res[4];
} UnkStruct_022d4ed4;

extern u8 gGameLanguage;
extern void **gTitleScreenFileTable[];

void *File_Load(void *p, s32 a);

#pragma thumb on
static inline u32 GetLang_022d4ed4(void) {
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

void TitleScreen_LoadAssets(UnkStruct_022d4ed4 *p) {
    void **tbl = gTitleScreenFileTable[GetLang_022d4ed4()];
    s32 i;

    for (i = 0; i < 4; i++) {
        p->res[i] = File_Load(tbl[i], 0);
    }
}
#pragma thumb reset
