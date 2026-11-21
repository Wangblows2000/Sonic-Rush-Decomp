#include <nitro/types.h>

typedef struct UnkStruct_022cbf78 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbf78;

extern UnkStruct_022cbf78 *gBoss1;

void Boss1_Action_WaitSlam(UnkStruct_022cbf78 *s);
void Boss1Neck_Action_StartSlam(void *p);
void Boss1Hammer_Action_Idle_ov000_022c6958(void *p);

void Boss1_Action_StartSlam(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_WaitSlam(gBoss1);
    Boss1Neck_Action_StartSlam(gBoss1->unk210);
    Boss1Hammer_Action_Idle_ov000_022c6958(gBoss1->unk214);
}
