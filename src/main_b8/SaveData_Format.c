#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 flags;
} UnkData_02059340;

extern UnkData_02059340 gSaveState;
extern s32 gSaveDataBusy;
extern u8 gSaveData[];

void SaveData_SetDefaults(void *p);
BOOL SaveData_FormatAll(void);

s32 SaveData_Format(void) {
    gSaveDataBusy = 1;
    if (gSaveState.flags & 0x80) {
        gSaveDataBusy = 0;
        return 6;
    }
    SaveData_SetDefaults(gSaveData);
    if (SaveData_FormatAll() == 0) {
        gSaveDataBusy = 0;
        return 2;
    }
    gSaveDataBusy = 0;
    return 0;
}
