#include <nitro/types.h>

extern void *gOptionsWfsHostTask;
extern s32 gOptionsWfsHostError;
void Task_Destroy(void *p);

#pragma thumb on
void OptionsWfsHost_Destroy(void) {
    if (gOptionsWfsHostTask != NULL) {
        Task_Destroy(gOptionsWfsHostTask);
        gOptionsWfsHostTask = NULL;
        gOptionsWfsHostError = 0;
    }
}
#pragma thumb reset
