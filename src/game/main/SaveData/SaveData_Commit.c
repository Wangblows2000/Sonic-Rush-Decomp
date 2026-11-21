#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 flags;
} UnkState_0205943c;

extern s32 gSaveDataBusy;
extern UnkState_0205943c gSaveState;

void SaveData_Validate(void);
void SaveData_Load(void);
s32 SaveData_IsDirty(void);
s32 SaveData_Save(void);

s32 SaveData_Commit(void) {
    gSaveDataBusy = 1;
    SaveData_Validate();
    if (gSaveState.flags & 0x80) {
        gSaveDataBusy = 0;
        return 6;
    }
    if ((gSaveState.flags & 1) == 0) {
        SaveData_Load();
        gSaveDataBusy = 0;
        return 0;
    }
    if (SaveData_IsDirty() == 0) {
        gSaveDataBusy = 0;
        return 0;
    }
    if (SaveData_Save()) {
        gSaveDataBusy = 0;
        return 2;
    } else {
        gSaveDataBusy = 0;
        return 0;
    }
}
