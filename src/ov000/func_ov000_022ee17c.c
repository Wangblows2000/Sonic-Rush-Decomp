#include <nitro/types.h>

typedef struct UnkStruct_022ee17c {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022ee17c;

extern UnkStruct_022ee17c *gBoss7;

void Boss7PartD5_Action_StartAI(void *p);

void func_ov000_022ee17c(void) {
    Boss7PartD5_Action_StartAI(gBoss7->unk214);
}
