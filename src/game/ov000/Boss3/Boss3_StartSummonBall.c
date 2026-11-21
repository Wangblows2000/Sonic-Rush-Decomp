#include <nitro/types.h>

typedef struct UnkStruct_022dac2c {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022dac2c;

extern UnkStruct_022dac2c *gBoss3;

void Boss3_Action_WaitSummonBall(UnkStruct_022dac2c *s);
void Boss3PartCC_Action_StartSummonBall(void *p);

void Boss3_StartSummonBall(void) {
    Boss3_Action_WaitSummonBall(gBoss3);
    Boss3PartCC_Action_StartSummonBall(gBoss3->unk214);
}
