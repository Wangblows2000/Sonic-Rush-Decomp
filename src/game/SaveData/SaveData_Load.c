#include <nitro/types.h>

typedef struct { u8 pad[3]; u8 flags; } St;
extern St gSaveState;
extern s32 gSaveDataBusy;
extern u8 gSaveData[];
s32 SaveData_ReadSlotPair(void *a);
BOOL SaveData_Validate(void);

s32 SaveData_Load(void) {
    s32 r;
    gSaveDataBusy = 1;
    if ((gSaveState.flags & 0x80) != 0) {
        gSaveDataBusy = 0;
        return 6;
    }
    gSaveState.flags |= 1;
    r = SaveData_ReadSlotPair(gSaveData);
    if (SaveData_Validate() == 0) {
        gSaveDataBusy = 0;
        return 3;
    }
    gSaveDataBusy = 0;
    return r;
}
