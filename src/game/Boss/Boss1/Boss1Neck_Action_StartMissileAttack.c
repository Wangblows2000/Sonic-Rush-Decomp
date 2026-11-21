#include <nitro/types.h>

typedef struct UnkStruct_022c7f1c {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad2[0x68 - 0x30];
    void (*func)(void);
} UnkStruct_022c7f1c;

void Boss1Neck_State_MissileWaitStart(void);

void Boss1Neck_Action_StartMissileAttack(UnkStruct_022c7f1c *s) {
    s->unk2E = 0;
    s->func = Boss1Neck_State_MissileWaitStart;
}
