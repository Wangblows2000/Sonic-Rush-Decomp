#include <nitro/types.h>

void SaveData_FormatOrShowError(void *p);
void Options_LoadSettingsFromSave(void *p);

#pragma thumb on
void Options_EraseSaveData(u8 *p) {
    SaveData_FormatOrShowError(p);
    Options_LoadSettingsFromSave(p + 8);
}
#pragma thumb reset
