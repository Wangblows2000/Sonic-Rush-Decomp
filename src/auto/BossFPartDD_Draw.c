/* paramclone of Boss1Neck_Draw (src/ov000_a/Boss1Neck_Draw.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct UnkStruct_022ca11c_3 {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x48 - 0xd];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022ca11c_3;

typedef struct UnkStruct_022f70f4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x218 - 0x5e];
    UnkStruct_022ca11c_3 unk228;
} UnkStruct_022f70f4;

typedef struct UnkStruct_022ca11c_2 {
    u8 pad[0x10];
    UnkStruct_022f70f4 *unk10;
} UnkStruct_022ca11c_2;

typedef struct UnkStruct_022ca11c_4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022ca11c_4;

extern UnkStruct_022ca11c_2 *gTaskCurrent;
extern UnkStruct_022ca11c_4 *gBoss1;

void Model_Animate(UnkStruct_022ca11c_3 *p);
void Model_Draw(UnkStruct_022ca11c_3 *p);
void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);
void NNS_G3dGetCurrentMtx(void *p, s32 a);

void BossFPartDD_Draw(void) {
    UnkStruct_022f70f4 *o = gTaskCurrent->unk10;
    UnkStruct_022ca11c_3 *p = &o->unk228;
    if (o->unk5C & 0x20)
        return;
    p->unk48 = o->unk10 << 4;
    p->unk4C = -(o->unk14 << 4);
    p->unk50 = o->unk18 << 4;
    Model_Animate(p);
    Model_Draw(p);
}
