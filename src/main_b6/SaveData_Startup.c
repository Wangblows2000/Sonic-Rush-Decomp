#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 flags;
} UnkState_020594f8;

extern s32 gSaveDataBusy;
extern UnkState_020594f8 gSaveState;

s32 SaveData_Init(void);
s32 SaveData_IdentifyBackup(void);
BOOL SaveData_CheckSignature(void);
BOOL func_0200f29c(void);
s32 SaveData_Format(void);
BOOL SaveData_WriteSignature(void);
s32 SaveData_SelectLatestSlot(void);

s32 SaveData_Startup(void) {
    gSaveDataBusy = 1;
    if (SaveData_Init() == 0) {
        gSaveDataBusy = 0;
        return 5;
    }
    if (SaveData_IdentifyBackup() == 0)
        return 1;
    gSaveState.flags &= ~0x80;
    if (SaveData_CheckSignature() == 0) {
        if (func_0200f29c()) {
            gSaveDataBusy = 0;
            return 1;
        }
        if (SaveData_Format()) {
            gSaveDataBusy = 0;
            return 1;
        }
        SaveData_WriteSignature();
        if (func_0200f29c()) {
            gSaveDataBusy = 0;
            return 1;
        } else {
            gSaveDataBusy = 0;
            return 4;
        }
    } else {
        if (SaveData_SelectLatestSlot() == 0) {
            gSaveDataBusy = 0;
            return 3;
        } else {
            gSaveDataBusy = 0;
            return 0;
        }
    }
}
