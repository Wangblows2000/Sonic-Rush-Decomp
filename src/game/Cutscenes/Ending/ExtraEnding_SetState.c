#include <nitro/types.h>

void ExtraEnding_State_None(void);

void ExtraEnding_SetState(u8 *p, void (*f)(void)) {
    if (f == NULL) {
        *(void (**)(void))(p + 4) = ExtraEnding_State_None;
    } else {
        *(void (**)(void))(p + 4) = f;
    }
    *(s32 *)(p + 8) = 0;
}
