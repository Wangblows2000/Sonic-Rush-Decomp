#include <nitro/types.h>

typedef struct UnkStruct_022ee874 {
    u8 pad0[0x24];
    s32 unk24;
    u8 pad28[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022ee874;

extern u16 data_ov001_0231f064;
extern s32 data_ov001_0231d430[];

BOOL ExtraBoss_StoreNodeMtxIfMatch(UnkStruct_022ee874 *p, s32 a, s32 b);

void ExtraBoss_WeaponNodeCallback(UnkStruct_022ee874 *p) {
    switch (data_ov001_0231f064) {
    case 0:
        if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1b, data_ov001_0231d430[0]))
            data_ov001_0231f064++;
        return;
    case 1:
        if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1c, data_ov001_0231d430[1]))
            data_ov001_0231f064++;
        return;
    case 2:
        if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1d, data_ov001_0231d430[2]))
            data_ov001_0231f064++;
        return;
    case 3:
        if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1e, data_ov001_0231d430[3]) == 0)
            return;
        data_ov001_0231f064 = 0;
        p->unk24 = 0;
        p->unk92 = 0;
        return;
    }
}
