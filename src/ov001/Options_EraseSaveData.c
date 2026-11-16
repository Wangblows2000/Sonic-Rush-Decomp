#include <nitro/types.h>

void func_0205a33c(void *p);
void Options_LoadSettingsFromSave(void *p);

#pragma thumb on
void Options_EraseSaveData(u8 *p) {
    func_0205a33c(p);
    Options_LoadSettingsFromSave(p + 8);
}
#pragma thumb reset
