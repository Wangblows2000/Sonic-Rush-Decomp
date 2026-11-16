#include <nitro/types.h>

typedef struct UnkStruct_022dac60 {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022dac60;

extern UnkStruct_022dac60 *gBoss3;

void Boss3_Action_Unk_ov000_022d9e1c(UnkStruct_022dac60 *s);
void Boss3PartCC_Action_Unk_ov000_022d8bf8(void *p);

void func_ov000_022dac60(void) {
    Boss3_Action_Unk_ov000_022d9e1c(gBoss3);
    Boss3PartCC_Action_Unk_ov000_022d8bf8(gBoss3->unk214);
}
