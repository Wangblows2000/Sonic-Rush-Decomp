#include <nitro/types.h>

typedef struct UnkStruct_022ee73c {
    u8 pad0[0x24];
    s32 unk24;
    u8 pad28[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022ee73c;

extern u16 data_ov001_0231f064;
extern s32 data_ov001_0231d404;

BOOL ExtraBoss_StoreNodeMtxIfMatch(UnkStruct_022ee73c *p, s32 a, s32 b);

void ExtraBoss_HandCatchNodeCallback(UnkStruct_022ee73c *p) {
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1d, data_ov001_0231d404)) {
        data_ov001_0231f064 = 0;
        p->unk24 = 0;
        p->unk92 = 0;
        return;
    }
}
