#include <nitro/types.h>

typedef struct UnkStruct_022ee784 {
    u8 pad0[0x24];
    s32 unk24;
    u8 pad28[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022ee784;

extern u16 data_ov001_0231f064;
extern s32 data_ov001_0231d440[];

BOOL ExtraBoss_Unk_ov001_022ffc4c(UnkStruct_022ee784 *p, s32 a, s32 b);

void ExtraBoss_Unk_ov001_022ee784(UnkStruct_022ee784 *p) {
    switch (data_ov001_0231f064) {
    case 0:
        if (ExtraBoss_Unk_ov001_022ffc4c(p, 0x1d, data_ov001_0231d440[0]))
            data_ov001_0231f064++;
        return;
    case 1:
        if (ExtraBoss_Unk_ov001_022ffc4c(p, 0x1c, data_ov001_0231d440[1]))
            data_ov001_0231f064++;
        return;
    case 2:
        if (ExtraBoss_Unk_ov001_022ffc4c(p, 0x1e, data_ov001_0231d440[2]))
            data_ov001_0231f064++;
        return;
    case 3:
        if (ExtraBoss_Unk_ov001_022ffc4c(p, 0x1b, data_ov001_0231d440[3]) == 0)
            return;
        data_ov001_0231f064 = 0;
        p->unk24 = 0;
        p->unk92 = 0;
        return;
    }
}
