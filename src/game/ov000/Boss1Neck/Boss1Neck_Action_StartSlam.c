#include <nitro/types.h>

typedef struct UnkStruct_022c9d8c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c9d8c;

void Boss1Neck_State_SlamApproach(void);

void Boss1Neck_Action_StartSlam(UnkStruct_022c9d8c *s) {
    s->func = Boss1Neck_State_SlamApproach;
}
