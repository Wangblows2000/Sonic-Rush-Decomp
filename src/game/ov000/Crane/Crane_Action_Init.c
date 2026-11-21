#include <nitro/types.h>

typedef struct UnkStruct_0230613c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230613c;

void Crane_State_Unk_ov000_02305ea0(void);

void Crane_Action_Init(UnkStruct_0230613c *s) {
    s->func = Crane_State_Unk_ov000_02305ea0;
}
