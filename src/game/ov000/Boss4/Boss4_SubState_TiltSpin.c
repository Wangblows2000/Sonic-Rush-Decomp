#include <nitro/types.h>

typedef struct {
    u8 pad0[0x224];
    s32 unk224;
    u8 pad228[0xbb8 - 0x228];
    s32 unkBB8;
    u8 padBBC[4];
    s32 unkBC0;
    u8 padBC4[4];
    s32 unkBC8;
} UnkStruct_022dd8c8;

void Boss4_StepTiltToward(UnkStruct_022dd8c8 *p, s32 a);
void Boss4_Action_StartSpin(UnkStruct_022dd8c8 *p);

void Boss4_SubState_TiltSpin(UnkStruct_022dd8c8 *p) {
    p->unkBB8 += p->unkBC0;
    Boss4_StepTiltToward(p, p->unkBC8);
    if (p->unk224 <= 0xb4)
        return;
    Boss4_Action_StartSpin(p);
}
