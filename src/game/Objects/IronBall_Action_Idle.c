#include <nitro/types.h>

typedef struct UnkStruct_0230d260 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230d260;

void IronBall_State_Idle(void);

void IronBall_Action_Idle(UnkStruct_0230d260 *s) {
    s->func = IronBall_State_Idle;
}
