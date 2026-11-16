#include <nitro/types.h>

typedef struct {
    const void *unk0;
    const void *unk4;
    const void *unk8;
    const void *unkc;
    const void *unk10;
    u32 unk14;
    u8 unk18;
} UnkInfo_02051248;

extern u8 gGameLanguage;
extern const void **data_0207ade0[];
extern u8 data_0207afac[];
extern u8 data_0207afbc[];
extern u8 data_0207afd4[];

#pragma thumb on
void DownloadPlay_GetSonicPreview(UnkInfo_02051248 *info) {
    u32 lang;
    const void **t;
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
    t = data_0207ade0[lang];
    info->unk0 = data_0207afac;
    info->unk4 = t[0];
    info->unk8 = t[1];
    info->unkc = data_0207afbc;
    info->unk10 = data_0207afd4;
    info->unk14 = 0x51;
    info->unk18 = 9;
}
#pragma thumb reset
