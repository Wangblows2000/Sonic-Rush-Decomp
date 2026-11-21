#include <nitro/types.h>

typedef struct UnkStruct_02310e7c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02310e7c;

void NeedleG_State_Idle(void);

void NeedleG_Action_Idle(UnkStruct_02310e7c *s) {
    s->func = NeedleG_State_Idle;
}
