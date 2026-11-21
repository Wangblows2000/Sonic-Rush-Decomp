#include <nitro/types.h>

extern s32 gFixVsPushIconTask;

void StageObj_Destructor(void *p);

void FixVsPushIcon_Destructor(void *p) {
    gFixVsPushIconTask = 0;
    StageObj_Destructor(p);
}
