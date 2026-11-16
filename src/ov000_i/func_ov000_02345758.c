#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02345758;

extern UnkCfg_02345758 data_022c4560;
s32 func_ov000_0233b078(s32 x);

BOOL func_ov000_02345758(s32 x, s32 y) {
    if (data_022c4560.unk1C == 6 && data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2) {
        if (data_022c4560.unk1D == 1) {
            if (y < func_ov000_0233b078(x))
                return TRUE;
        } else {
            if (y > func_ov000_0233b078(x))
                return TRUE;
        }
    }
    return FALSE;
}
