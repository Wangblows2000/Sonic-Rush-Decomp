#include <nitro/types.h>

typedef struct UnkStruct_022cbe88 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbe88;

extern UnkStruct_022cbe88 *gBoss1;

void Boss1_Action_WaitTripleSlam(UnkStruct_022cbe88 *s);
void Boss1Neck_Action_StartTripleSlam(void *p);
void Boss1Hammer_Action_Idle_ov000_022c5fa8(void *p);

void Boss1_Action_StartTripleSlam(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_WaitTripleSlam(gBoss1);
    Boss1Neck_Action_StartTripleSlam(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c5fa8(gBoss1->unk214);
}
