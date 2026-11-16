#include <nitro/types.h>

typedef struct UnkStruct_022c9d8c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c9d8c;

void Boss1Neck_State_Unk_ov000_022c9ce8(void);

void Boss1Neck_Action_Unk_ov000_022c9d8c(UnkStruct_022c9d8c *s) {
    s->func = Boss1Neck_State_Unk_ov000_022c9ce8;
}
