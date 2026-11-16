#include <nitro/types.h>

typedef struct UnkStruct_022c736c_3 {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x48 - 0xd];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022c736c_3;

typedef struct UnkStruct_022c736c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x2d0 - 0x5e];
    u8 unk2D0[0x2f4 - 0x2d0];
    s32 unk2F4;
    s32 unk2F8;
    s32 unk2FC;
    u8 pad3[0x30c - 0x300];
    UnkStruct_022c736c_3 unk30C;
} UnkStruct_022c736c;

typedef struct UnkStruct_022c736c_2 {
    u8 pad[0x10];
    UnkStruct_022c736c *unk10;
} UnkStruct_022c736c_2;

typedef struct UnkStruct_022c736c_4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022c736c_4;

extern UnkStruct_022c736c_2 *gTaskCurrent;
extern UnkStruct_022c736c_4 *data_ov000_02369acc;

void Model_Animate(UnkStruct_022c736c_3 *p);
void Model_Draw(UnkStruct_022c736c_3 *p);
void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);
void NNS_G3dGetCurrentMtx(void *p, s32 a);

void Boss1Hammer_Draw(void) {
    UnkStruct_022c736c *o = gTaskCurrent->unk10;
    UnkStruct_022c736c_3 *p = &o->unk30C;
    if (o->unk5C & 0x20)
        return;
    if (data_ov000_02369acc->unk20C & 1) {
        p->unkA = 3;
        p->unkB = 0;
        p->unk48 = o->unk10 << 4;
        p->unk4C = -(o->unk14 << 4);
        p->unk50 = o->unk18 << 4;
        Model_Animate(p);
        Model_Draw(p);
        return;
    }
    p->unkA = 4;
    p->unkB = 2;
    p->unkC = 0;
    p->unk48 = 0;
    p->unk4C = 0;
    p->unk50 = 0;
    Model_Animate(p);
    Model_Draw(p);
    {
        u32 v = 2;
        NNS_G3dGeBufferOP_N(0x10, &v, 1);
    }
    {
        u32 v = 0x1e;
        NNS_G3dGeBufferOP_N(0x14, &v, 1);
    }
    NNS_G3dGetCurrentMtx(o->unk2D0, 0);
    o->unk10 = o->unk2F4 >> 4;
    o->unk14 = -o->unk2F8 >> 4;
    o->unk18 = o->unk2FC >> 4;
}
