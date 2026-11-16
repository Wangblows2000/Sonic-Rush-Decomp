#include <nitro/types.h>

typedef struct UnkStruct_022dac2c {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022dac2c;

extern UnkStruct_022dac2c *gBoss3;

void Boss3_Action_Unk_ov000_022d9da0(UnkStruct_022dac2c *s);
void Boss3PartCC_Action_Unk_ov000_022d8ba8(void *p);

void func_ov000_022dac2c(void) {
    Boss3_Action_Unk_ov000_022d9da0(gBoss3);
    Boss3PartCC_Action_Unk_ov000_022d8ba8(gBoss3->unk214);
}
