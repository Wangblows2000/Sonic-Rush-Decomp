#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_0205899c;

extern UnkState_0205899c gSaveState;
extern u8 gSaveData[];

BOOL SaveData_AdvanceSlot(void);
BOOL SaveData_WriteSlot(u8 slot, void *buf);

s32 SaveData_Save(void) {
    u8 start = gSaveState.b2;

    if (!SaveData_AdvanceSlot())
        return 2;
    while (start != gSaveState.b2) {
        if (SaveData_WriteSlot(gSaveState.b2 << 1, gSaveData) && SaveData_WriteSlot((gSaveState.b2 << 1) + 1, gSaveData))
            return 0;
        if (!SaveData_AdvanceSlot())
            return 2;
    }
    return 2;
}
