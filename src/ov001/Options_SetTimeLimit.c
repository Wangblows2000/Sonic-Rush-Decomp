#include <nitro/types.h>

void SaveData_SetTimeLimit(s32 v);

#pragma thumb on
void Options_SetTimeLimit(u8 *p, s32 v) {
    if (*(s32 *)(p + 0xc) != v) {
        *(s32 *)(p + 0xc) = v;
        SaveData_SetTimeLimit(v);
    }
}
#pragma thumb reset
