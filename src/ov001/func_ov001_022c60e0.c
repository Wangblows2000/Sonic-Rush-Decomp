#include <nitro/types.h>

s32 PlayerDemo_IsActive(void *p);
void ExtraEnding_SetState(void *p, void (*f)(void));
void func_ov001_022c6094(void);

void func_ov001_022c60e0(void *p) {
    if (PlayerDemo_IsActive(p) == 1) {
        return;
    }
    ExtraEnding_SetState(p, func_ov001_022c6094);
}
