#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02345758;

extern UnkCfg_02345758 gGameState;
s32 Stage_GetSplitLineY(s32 x);

BOOL Stage_IsGravityFlipped(s32 x, s32 y) {
    if (gGameState.unk1C == 6 && gGameState.unk1C < 9 && gGameState.unk1D < 2) {
        if (gGameState.unk1D == 1) {
            if (y < Stage_GetSplitLineY(x))
                return TRUE;
        } else {
            if (y > Stage_GetSplitLineY(x))
                return TRUE;
        }
    }
    return FALSE;
}
