#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u8 pad0[0x18]; u16 unk18; } UnkStruct_02072cb8;
typedef struct { u16 name[10]; u16 len; u16 unk16; } UnkName_02051598;
extern UnkStruct_0207ad9c data_0207ad9c;
extern u16 data_02072cbc[];
extern UnkStruct_02072cb8 gGameLanguage;
extern u8 data_022c44f8[];
extern u8 data_022c4500[];

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_02043758(void *a, s32 b, s32 c, UnkName_02051598 *d, s32 e);
void func_02043478(void *a, s32 b, UnkName_02051598 *c, s32 d);

#pragma thumb on
void func_02051598(void) {
    UnkName_02051598 nm;
    s32 i;
    u16 n;

    data_0207ad9c.unk0 = 0;
    nm.unk16 = 1;
    MIi_CpuCopy16(data_02072cbc, nm.name, 0x14);
    n = gGameLanguage.unk18;
    for (i = n; i < 10; i++)
        nm.name[i] = 0x20;
    nm.len = n;
    switch (data_0207ad9c.unk2) {
    case 0:
    case 1:
        func_02043758(data_022c44f8, 0, 0x40, &nm, 0x18);
        break;
    case 2:
    case 3:
        func_02043478(data_022c4500, 0x40, &nm, 0x18);
        break;
    }
}
#pragma thumb reset
