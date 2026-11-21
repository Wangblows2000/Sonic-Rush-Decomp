#include <nitro/types.h>

typedef struct UnkDemoRes_02056fa4 {
    u32 *offsets;
    u32 count;
} UnkDemoRes_02056fa4;

extern u8 gGameLanguage;
extern u32 data_02075b6c[];
extern UnkDemoRes_02056fa4 data_020763e4[];
extern u8 data_0207c460[];
extern s16 gPlayerDemoId;

static inline u32 GetLang(void)
{
    u32 lang;
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        lang = gGameLanguage;
        break;
    default:
        lang = 1;
        break;
    }
    return lang;
}

void *File_LoadArchiveEntry(void *archive, u16 index, s32 arg);

#pragma thumb on
void *PlayerDemoBg_LoadImageRes(u32 idx)
{
    UnkDemoRes_02056fa4 *res = &data_020763e4[gPlayerDemoId];

    if (idx >= res->count)
        return NULL;
    return File_LoadArchiveEntry(data_0207c460, res->offsets[idx] + data_02075b6c[GetLang()], -1);
}
#pragma thumb reset
