#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_02058ab0;

extern UnkState_02058ab0 gSaveState;

BOOL SaveData_ReadSlot(u8 slot, void *buf);
BOOL SaveData_VerifyChecksum(void *buf);

s32 SaveData_ReadSlotPair(void *buf) {
    BOOL f = FALSE;
    u32 s = gSaveState.b2 << 1;

    if (SaveData_ReadSlot(s, buf)) {
        if (SaveData_VerifyChecksum(buf))
            return 0;
    } else {
        f = TRUE;
    }
    if (SaveData_ReadSlot(s + 1, buf)) {
        if (SaveData_VerifyChecksum(buf))
            return 0;
    } else if (f) {
        return 1;
    }
    return 3;
}
