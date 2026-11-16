#include <nitro/types.h>

typedef struct UnkStruct_022cbe38 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbe38;

extern UnkStruct_022cbe38 *gBoss1;

void Boss1_Action_Unk_ov000_022cafa4(UnkStruct_022cbe38 *s);
void Boss1Neck_Action_Unk_ov000_022c7f1c(void *p);
void Boss1Hammer_Action_Unk_ov000_022c5f94(void *p);

void Boss1_Action_Unk_ov000_022cbe38(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_Unk_ov000_022cafa4(gBoss1);
    Boss1Neck_Action_Unk_ov000_022c7f1c(gBoss1->unk210);
    Boss1Hammer_Action_Unk_ov000_022c5f94(gBoss1->unk214);
}
