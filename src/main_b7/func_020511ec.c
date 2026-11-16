#include <nitro/types.h>

typedef struct {
    const void *unk0;
    const void *unk4;
    const void *unk8;
    const void *unkc;
    const void *unk10;
    u32 unk14;
    u8 unk18;
} UnkInfo_020511ec;

extern u8 data_02072cb8;
extern const void **data_0207ae00[];
extern u8 data_0207af6c[];
extern u8 data_0207af7c[];
extern u8 data_0207af94[];

#pragma thumb on
void func_020511ec(UnkInfo_020511ec *info) {
    u32 lang;
    const void **t;
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        lang = data_02072cb8;
        break;
    default:
        lang = 1;
        break;
    }
    t = data_0207ae00[lang];
    info->unk0 = data_0207af6c;
    info->unk4 = t[0];
    info->unk8 = t[1];
    info->unkc = data_0207af7c;
    info->unk10 = data_0207af94;
    info->unk14 = 0x51;
    info->unk18 = 9;
}
#pragma thumb reset
