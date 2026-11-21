#include <nitro/types.h>

BOOL SpecialStageObj_IsInReach(void *unused, u8 *p) {
    if (*(u16 *)(p + 0x60) & 0x100) {
        return FALSE;
    }
    if (*(s32 *)(p + 0x28) < -0x82000) {
        return FALSE;
    }
    if (*(s32 *)(p + 0x28) > -0xa000) {
        return FALSE;
    }
    if (*(s32 *)(p + 0x20) > 0x1e000) {
        return FALSE;
    }
    if (*(s32 *)(p + 0x20) < -0x1e000) {
        return FALSE;
    }
    return *(s32 *)(p + 0x24) >= -0x19000;
}
