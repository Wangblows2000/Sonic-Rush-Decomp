#include <nitro/types.h>

typedef struct UnkStruct_022dac60 {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022dac60;

extern UnkStruct_022dac60 *gBoss3;

void Boss3_Action_Idle(UnkStruct_022dac60 *s);
void Boss3PartCC_Action_Idle(void *p);

void Boss3_IdleAll(void) {
    Boss3_Action_Idle(gBoss3);
    Boss3PartCC_Action_Idle(gBoss3->unk214);
}
