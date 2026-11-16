#include <nitro/types.h>

typedef struct UnkStruct_02306db0_v {
    s32 x, y, z;
} UnkStruct_02306db0_v;

typedef struct UnkStruct_02306db0_o {
    u8 pad0[0x90];
    u8 unk90[0xbc - 0x90];
    u32 unkBC;
    u8 padC0[0xf4 - 0xc0];
} UnkStruct_02306db0_o;

typedef struct UnkStruct_02306db0_a {
    u8 pad0[0x114];
    s32 unk114;
    u8 pad118[0x138 - 0x118];
} UnkStruct_02306db0_a;

typedef struct UnkStruct_02306db0_g {
    UnkStruct_02306db0_o objs[0x20];
    u32 used1[0x20];
    UnkStruct_02306db0_a anims[0x20];
    u32 used2[0x20];
    void *unk4680;
    void *unk4684;
    void *unk4688;
    void *unk468C;
    void *unk4690;
    void *unk4694;
    void *unk4698;
    void *unk469C;
    void *unk46A0;
} UnkStruct_02306db0_g;

typedef struct UnkStruct_02306db0_e {
    UnkStruct_02306db0_v unk0;
    s32 unkC;
    u16 unk10;
    s16 unk12;
    s32 unk14;
} UnkStruct_02306db0_e;

typedef struct UnkStruct_02306db0_n UnkStruct_02306db0_n;
struct UnkStruct_02306db0_n {
    s32 unk0;
    UnkStruct_02306db0_v unk4;
    u8 pad10[0x2c - 0x10];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[4];
    s32 unk3C;
    s32 unk40;
    u8 pad44[0x60 - 0x44];
    u16 unk60;
    u8 pad62[2];
    u16 unk64;
    u8 pad66[2];
    u16 unk68;
    u8 pad6A[6];
    s32 unk70;
    u8 pad74[4];
    s32 unk78;
    u8 pad7C[0xac - 0x7c];
    UnkStruct_02306db0_a *unkAC;
    UnkStruct_02306db0_o *unkB0;
    u8 padB4[0xcc - 0xb4];
    s32 unkCC;
    u8 padD0[8];
    UnkStruct_02306db0_n *unkD8;
};

extern UnkStruct_02306db0_g *data_ov001_0231f5b8;
extern UnkStruct_02306db0_e data_ov001_02316fe4[];
extern u32 gRandSeed;

UnkStruct_02306db0_n *func_ov001_023102fc(void *g);
void AnimatorSprite_SetAnimation(void *s, u16 index);
void Model_Animate(void *p);
void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
u32 NNS_G3dGetTex(void *h);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

UnkStruct_02306db0_n *func_ov001_02306db0(s32 arg, u32 type) {
    UnkStruct_02306db0_g *g = data_ov001_0231f5b8;
    UnkStruct_02306db0_o *o = NULL;
    UnkStruct_02306db0_a *an = NULL;
    UnkStruct_02306db0_n *n;
    UnkStruct_02306db0_e *e;
    s32 i;

    if (type <= 1) {
        for (i = 0; i < 0x20; i++) {
            if (g->used2[i] == 0) {
                an = &g->anims[i];
                g->used2[i] = 1;
                an->unk114 = 0x1000;
                break;
            }
        }
    } else {
        for (i = 0; i < 0x20; i++) {
            if (g->used1[i] == 0) {
                o = &g->objs[i];
                g->used1[i] = 1;
                break;
            }
        }
    }
    if (i == 0x20)
        return NULL;
    n = func_ov001_023102fc(g->unk46A0);
    e = &data_ov001_02316fe4[type];
    n->unk0 = 1;
    n->unk4 = e->unk0;
    n->unk68 = type;
    n->unk70 = e->unk12;
    n->unk78 = arg;
    n->unkB0 = o;
    if (o != NULL) {
        AnimatorSprite_SetAnimation(o->unk90, e->unk10);
        o->unkBC &= ~0x10;
        Model_Animate(o);
        o->unkBC |= 0x10;
        if (e->unk12 > 0)
            o->unkBC |= 4;
    }
    n->unkAC = an;
    n->unk60 = 8;
    n->unk64 = e->unkC;
    n->unkCC = e->unk14;
    n->unkD8 = n;
    switch (type) {
    case 0:
        Model_SetResource(n->unkAC, g->unk4684, 0);
        Model_SetAnimation(n->unkAC, 0, g->unk468C, 0, 0);
        Model_SetAnimation(n->unkAC, 1, g->unk4688, 0, 0);
        break;
    case 1:
        Model_SetResource(n->unkAC, g->unk4690, 0);
        Model_SetAnimation(n->unkAC, 0, g->unk4694, 0, 0);
        Model_SetAnimation(n->unkAC, 1, g->unk4698, 0, 0);
        Model_SetAnimation(n->unkAC, 2, g->unk469C, 0, NNS_G3dGetTex(g->unk4690));
        break;
    case 4:
        n->unk2C = Rand() % 0x2000;
        n->unk2C -= 0x1000;
        n->unk30 = Rand() % 0x1000;
        n->unk30 += 0x800;
        n->unk3C = -0xd0;
        n->unk34 = -0x800;
        n->unk40 = 0;
        break;
    }
    return n;
}
