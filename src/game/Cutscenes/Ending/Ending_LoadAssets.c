#include <nitro/types.h>

typedef struct UnkStruct_022c7fe0 {
    u32 unk0;
    u8 pad4[0x10 - 4];
    void *res[0x13];
    u8 bgs[0x13][0x40];
    void *hdr[2];
} UnkStruct_022c7fe0;

extern u8 gGameLanguage;
extern char **gEndingSonicPictureTable[];
extern char **gEndingBlazePictureTable[];
extern char **gEndingSpriteFileTable[];
extern u8 data_ov001_02319f78[];
extern s32 data_ov001_02315ec4[];
extern void *gSoundHeap;

void *File_Load(void *p, s32 v);
void Sound_LoadSoundArchive(void *a);
void NNS_SndArcLoadGroup(s32 a, void *b);

static inline u32 GetLang(void) {
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return gGameLanguage;
    default:
        return 1;
    }
}

void Ending_LoadAssets(UnkStruct_022c7fe0 *w) {
    char **b;
    s32 i;
    char **a;

    if (w->unk0 == 0)
        a = gEndingSonicPictureTable[GetLang()];
    else
        a = gEndingBlazePictureTable[GetLang()];
    b = gEndingSpriteFileTable[GetLang()];
    for (i = 0; i < 0x13; i++)
        w->res[i] = File_Load(a[i], -1);
    for (i = 0; i < 2; i++)
        w->hdr[i] = File_Load(b[i], -1);
    Sound_LoadSoundArchive(data_ov001_02319f78);
    NNS_SndArcLoadGroup(data_ov001_02315ec4[w->unk0], gSoundHeap);
}
