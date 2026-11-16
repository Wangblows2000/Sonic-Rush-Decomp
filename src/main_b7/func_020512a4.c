#include <nitro/types.h>

typedef struct {
    const void *unk0;
    const void *unk4;
    const void *unk8;
    const void *unkc;
    const void *unk10;
    u32 unk14;
    u8 unk18;
} UnkInfo_020512a4;

extern u8 data_02072cb8;
extern const void **data_0207adc0[];
extern u8 data_0207afec[];
extern u8 data_0207affc[];
extern u8 data_0207b014[];

#pragma thumb on
void func_020512a4(UnkInfo_020512a4 *info) {
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
    t = data_0207adc0[lang];
    info->unk0 = data_0207afec;
    info->unk4 = t[0];
    info->unk8 = t[1];
    info->unkc = data_0207affc;
    info->unk10 = data_0207b014;
    info->unk14 = 0x51;
    info->unk18 = 2;
}
#pragma thumb reset
