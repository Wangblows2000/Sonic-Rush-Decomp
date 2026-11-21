#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x68 - 0x10];
    void *unk68;
} UnkStruct_022dfcbc;

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
    u8 pad210[0xd5e - 0x210];
    u16 unkD5E;
} UnkStruct_022dfcbc_g;

extern UnkStruct_022dfcbc_g *gBoss6;

void Boss6_State_PieceSpinFly(UnkStruct_022dfcbc *p);

void Boss6_State_PieceWaitSpinFly(UnkStruct_022dfcbc *p) {
    s32 v;
    UnkStruct_022dfcbc_g *g;

    p->unkC++;
    if (p->unkC < 10)
        return;
    p->unk68 = Boss6_State_PieceSpinFly;
    p->unkC = 0;
    v = -0xa00;
    g = gBoss6;
    if (g->unkD5E != 0)
        v *= -1;
    if (g->unk20C & 0x100)
        v *= -1;
    p->unk8 = v;
}
