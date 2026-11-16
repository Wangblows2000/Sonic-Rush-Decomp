#include <nitro/types.h>

s32 PlayerDemo_IsActive(void *p);
void ExtraEnding_SetState(void *p, void (*f)(void));
void ExtraEnding_State_End(void);

void ExtraEnding_State_PlayDemo(void *p) {
    if (PlayerDemo_IsActive(p) == 1) {
        return;
    }
    ExtraEnding_SetState(p, ExtraEnding_State_End);
}
