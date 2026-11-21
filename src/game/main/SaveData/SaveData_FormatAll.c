#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_02058d24;

extern UnkState_02058d24 gSaveState;

BOOL SaveData_FormatSlot(u8 slot);

BOOL SaveData_FormatAll(void) {
    BOOL err = FALSE;
    u8 i;

    for (i = 0; i < 5; i++) {
        err |= SaveData_FormatSlot(i * 2) == 0;
        err |= SaveData_FormatSlot(i * 2 + 1) == 0;
    }
    gSaveState.b2 = 0;
    return err == FALSE;
}
