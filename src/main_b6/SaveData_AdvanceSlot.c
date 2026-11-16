#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_020588f8;

typedef struct {
    u32 unk0;
    u32 unk4;
} UnkSave_020588f8;

extern UnkState_020588f8 gSaveState;
extern UnkSave_020588f8 gSaveData;

u32 SaveData_CalcChecksum(UnkSave_020588f8 *s);

BOOL SaveData_AdvanceSlot(void) {
    gSaveState.b2 = (gSaveState.b2 + 1) % 5;
    gSaveData.unk4++;
    if ((gSaveData.unk4 - gSaveState.b2) % 5 != 0)
        return FALSE;
    gSaveData.unk0 = SaveData_CalcChecksum(&gSaveData);
    return TRUE;
}
