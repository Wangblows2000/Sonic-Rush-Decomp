#include <nitro/types.h>

typedef struct UnkStruct_022ca4e8_3 {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x48 - 0xd];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022ca4e8_3;

typedef struct UnkStruct_022ca4e8 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x20c - 0x5e];
    UnkStruct_022ca4e8_3 unk20C;
} UnkStruct_022ca4e8;

typedef struct UnkStruct_022ca4e8_2 {
    u8 pad[0x10];
    UnkStruct_022ca4e8 *unk10;
} UnkStruct_022ca4e8_2;

typedef struct UnkStruct_022ca4e8_4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022ca4e8_4;

extern UnkStruct_022ca4e8_2 *gTaskCurrent;
extern UnkStruct_022ca4e8_4 *data_ov000_02369acc;

void Model_Animate(UnkStruct_022ca4e8_3 *p);
void Model_Draw(UnkStruct_022ca4e8_3 *p);
void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);
void NNS_G3dGetCurrentMtx(void *p, s32 a);

void Boss5PartD3_Draw(void) {
    UnkStruct_022ca4e8 *o = gTaskCurrent->unk10;
    UnkStruct_022ca4e8_3 *p = &o->unk20C;
    if (o->unk5C & 0x20)
        return;
    p->unk48 = o->unk10 << 4;
    p->unk4C = -(o->unk14 << 4);
    p->unk50 = o->unk18 << 4;
    Model_Animate(p);
    Model_Draw(p);
}
