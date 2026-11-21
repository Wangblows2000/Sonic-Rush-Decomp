#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022c86f4;

extern UnkTask_022c86f4 *gTaskCurrent;
extern s32 gOptionsWfsHostError;

s32 StageLoader_LoadCommonFiles(void);
void OptionsWfsHost_State_InitZoneFiles(void);
void OptionsWfsHost_Destroy(void);

#pragma thumb on
void OptionsWfsHost_State_LoadCommonFiles(void) {
    switch (StageLoader_LoadCommonFiles()) {
    case 0:
        break;
    case 1:
        gTaskCurrent->func = OptionsWfsHost_State_InitZoneFiles;
        break;
    case 2:
        OptionsWfsHost_Destroy();
        gOptionsWfsHostError = 1;
        break;
    }
}
#pragma thumb reset
