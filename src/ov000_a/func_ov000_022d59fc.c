#include <nitro/types.h>

typedef struct UnkStruct_022d59fc_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d59fc_v;

typedef struct UnkStruct_022d59fc UnkStruct_022d59fc;
struct UnkStruct_022d59fc {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(UnkStruct_022d59fc *s);
    u8 pad3[0x13e - 0x6c];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x20c - 0x16c];
    u32 unk20C;
    u16 unk210;
    u8 pad6[0x214 - 0x212];
    UnkStruct_022d59fc_v unk214;
    u16 unk220;
    u8 pad7[0x22a - 0x222];
    u16 unk22A;
    u16 unk22C;
    u16 unk22E;
    u8 pad8[0x234 - 0x230];
    u16 unk234;
    u8 pad9[0x2b4 - 0x236];
    u8 unk2B4[0x24];
    UnkStruct_022d59fc_v unk2D8;
};

typedef struct UnkStruct_022d59fc_q {
    u8 pad[0x25e];
    u16 unk25E;
} UnkStruct_022d59fc_q;

typedef struct UnkStruct_022d59fc_p {
    u8 pad[0x214];
    UnkStruct_022d59fc_q *unk214;
    u8 pad1[0x222 - 0x218];
    u16 unk222;
} UnkStruct_022d59fc_p;

extern UnkStruct_022d59fc_p *gBoss3;
extern s32 data_ov000_02363330[][5];

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void func_ov000_022d59e0(UnkStruct_022d59fc *s);
void func_ov000_022d58a0(UnkStruct_022d59fc *s);

void func_ov000_022d59fc(UnkStruct_022d59fc *s) {
    UnkStruct_022d59fc_p *p = gBoss3;
    s->unk214.y = data_ov000_02363330[p->unk222][p->unk214->unk25E];
    func_ov000_022c5e80(0xf42f8, 0x5fe435, s->unk10 << 4, NULL, NULL, &s->unk220);
    s->unk22E = 0;
    if (!(s->unk20C & 4))
        s->func = func_ov000_022d59e0;
    else
        s->func = func_ov000_022d58a0;
}
