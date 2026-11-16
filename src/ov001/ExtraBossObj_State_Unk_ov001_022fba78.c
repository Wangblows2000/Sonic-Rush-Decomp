#include <nitro/types.h>

extern u8 *data_ov001_0231f094;
BOOL ExtraBossObj_IsOutOfArena(s32 a, s32 b, s32 c, s32 d);

void ExtraBossObj_State_Unk_ov001_022fba78(u8 *p) {
    if (!ExtraBossObj_IsOutOfArena(*(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), 0x30000, 0x30000)) {
        return;
    }
    *(u32 *)(p + 0x14) |= 8;
    if (*(s32 *)(p + 0x1c8) == 8) {
        *data_ov001_0231f094 &= ~1;
    } else {
        *data_ov001_0231f094 &= ~2;
    }
}
