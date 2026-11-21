#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022c8674;

extern UnkTask_022c8674 *gTaskCurrent;
extern s32 gOptionsWfsHostError;

s32 StageLoader_LoadZoneFiles(void);
void OptionsWfsHost_State_WaitChildrenGone(void);
void OptionsWfsHost_Destroy(void);

#pragma thumb on
void OptionsWfsHost_State_LoadZoneFiles(void) {
    switch (StageLoader_LoadZoneFiles()) {
    case 0:
        break;
    case 1:
        gTaskCurrent->func = OptionsWfsHost_State_WaitChildrenGone;
        break;
    case 2:
        OptionsWfsHost_Destroy();
        gOptionsWfsHostError = 1;
        break;
    }
}
#pragma thumb reset
