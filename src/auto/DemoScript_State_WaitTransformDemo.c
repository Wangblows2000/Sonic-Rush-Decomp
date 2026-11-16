/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TransformDemo_IsActive();
s32 DemoScript_StartDelay();

void DemoScript_State_WaitTransformDemo(s32 p0) {
    s32 t0 = TransformDemo_IsActive();
    if (t0 == 0) {
        DemoScript_StartDelay(p0);
    }
}

#pragma thumb reset
