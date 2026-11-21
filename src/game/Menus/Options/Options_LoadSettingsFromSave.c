#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 flags; } UnkStruct_022c4680;
extern UnkStruct_022c4680 gSaveData;

#pragma thumb on
void Options_LoadSettingsFromSave(BOOL *p) {
    if (gSaveData.flags & 1) {
        p[0] = TRUE;
    } else {
        p[0] = FALSE;
    }
    if (gSaveData.flags & 2) {
        p[1] = FALSE;
    } else {
        p[1] = TRUE;
    }
}
#pragma thumb reset
