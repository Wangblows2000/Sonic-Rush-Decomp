#include <nitro/types.h>

typedef struct UnkStruct_02315410 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02315410;

void SandSlider_State_Idle(void);

void SandSlider_Action_Idle(UnkStruct_02315410 *s) {
    s->func = SandSlider_State_Idle;
}
