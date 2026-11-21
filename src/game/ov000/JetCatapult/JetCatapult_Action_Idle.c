#include <nitro/types.h>

typedef struct UnkStruct_0230e8b0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230e8b0;

void JetCatapult_State_Idle(void);

void JetCatapult_Action_Idle(UnkStruct_0230e8b0 *s) {
    s->func = JetCatapult_State_Idle;
}
