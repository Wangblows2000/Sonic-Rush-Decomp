#include <nitro/types.h>

typedef struct UnkStruct_022cbf28 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbf28;

extern UnkStruct_022cbf28 *gBoss1;

void Boss1_Action_Unk_ov000_022cb088(UnkStruct_022cbf28 *s);
void Boss1Neck_Action_Unk_ov000_022c95a8(void *p);
void Boss1Hammer_Action_Unk_ov000_022c6944(void *p);

void Boss1_Action_Unk_ov000_022cbf28(u16 v) {
    gBoss1->unk22A = v;
    Boss1_Action_Unk_ov000_022cb088(gBoss1);
    Boss1Neck_Action_Unk_ov000_022c95a8(gBoss1->unk210);
    Boss1Hammer_Action_Unk_ov000_022c6944(gBoss1->unk214);
}
