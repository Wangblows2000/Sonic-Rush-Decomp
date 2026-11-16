#include <nitro/types.h>

void ZoneMap_SetSonicProgressFromStep(s32 a);
void ZoneMap_SetBlazeProgressFromStep(s32 a);
void SaveData_StoreLives(void);
void SaveData_CommitOrShowError(void (*f)(void));

#pragma thumb on
void ZoneMap_SaveProgress(s32 *p) {
    if (p[0] == 0) {
        ZoneMap_SetSonicProgressFromStep(p[1]);
    } else {
        ZoneMap_SetBlazeProgressFromStep(p[2]);
    }
    SaveData_StoreLives();
    SaveData_CommitOrShowError(NULL);
}
#pragma thumb reset
