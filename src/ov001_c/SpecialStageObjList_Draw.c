#include <nitro/types.h>

typedef struct UnkStruct_0231039c_v {
    s32 x, y, z;
} UnkStruct_0231039c_v;

typedef struct UnkStruct_0231039c_m {
    s32 m[9];
} UnkStruct_0231039c_m;

typedef struct UnkStruct_0231039c_r {
    u8 pad0[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 padD[0x18 - 0xd];
    UnkStruct_0231039c_v pos;
    UnkStruct_0231039c_m rot;
    UnkStruct_0231039c_v scale;
} UnkStruct_0231039c_r;

typedef struct UnkStruct_0231039c_e {
    u8 pad0[4];
    UnkStruct_0231039c_v pos;
    u16 rx;
    u16 ry;
    u16 rz;
    u16 rz2;
    u8 pad18[0x20 - 0x18];
    UnkStruct_0231039c_v scale;
    u8 pad2C[0x60 - 0x2c];
    u16 unk60;
    u8 pad62[2];
    u16 unk64;
    u8 pad66[0xac - 0x66];
    UnkStruct_0231039c_r *unkAC;
    UnkStruct_0231039c_r *unkB0;
    u8 padB4[0xdc - 0xb4];
    struct UnkStruct_0231039c_e *next;
} UnkStruct_0231039c_e;

typedef struct UnkStruct_0231039c {
    u8 pad0[0xec];
    UnkStruct_0231039c_e *head;
    u8 padF0[4];
    u8 sentinel[4];
} UnkStruct_0231039c;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

static inline void VecSet(UnkStruct_0231039c_v *d, s32 x, s32 y, s32 z) {
    d->x = x;
    d->y = y;
    d->z = z;
}

void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
s32 func_ov001_02310054(UnkStruct_0231039c_e *e);
void MTX_RotY33_(UnkStruct_0231039c_m *m, s32 s, s32 c);
void MTX_RotX33_(UnkStruct_0231039c_m *m, s32 s, s32 c);
void MTX_RotZ33_(UnkStruct_0231039c_m *m, s32 s, s32 c);
void MTX_Concat33(UnkStruct_0231039c_m *a, UnkStruct_0231039c_m *b, UnkStruct_0231039c_m *c);
void Model_Draw(UnkStruct_0231039c_r *o);

void SpecialStageObjList_Draw(UnkStruct_0231039c *p) {
    u32 one;
    UnkStruct_0231039c_m b;
    UnkStruct_0231039c_m a;
    UnkStruct_0231039c_m c;
    UnkStruct_0231039c_e *e;
    s32 idx;

    NNS_G3dGeBufferOP_N(0x11, 0, 0);
    for (e = p->head; e != (UnkStruct_0231039c_e *)p->sentinel; e = e->next) {
        if (e->unk60 & 1)
            continue;
        if (e->unk60 & 4)
            continue;
        if (func_ov001_02310054(e) == 1)
            continue;
        if (e->unkAC) {
            if (!(e->unk60 & 0x40)) {
                VecSet(&e->unkAC->scale, e->scale.x, e->scale.y, e->scale.z);
                idx = (s32)(e->ry & 0xffff) >> 4;
                MTX_RotY33_(&a, SIN_IDX(idx), COS_IDX(idx));
                idx = (s32)(e->rx & 0xffff) >> 4;
                MTX_RotX33_(&b, SIN_IDX(idx), COS_IDX(idx));
                if (e->unk64 & 0x2000) {
                    idx = (s32)(e->rz2 & 0xffff) >> 4;
                    MTX_RotZ33_(&c, SIN_IDX(idx), COS_IDX(idx));
                } else {
                    idx = (s32)(e->rz & 0xffff) >> 4;
                    MTX_RotZ33_(&c, SIN_IDX(idx), COS_IDX(idx));
                }
                MTX_Concat33(&a, &b, &e->unkAC->rot);
                MTX_Concat33(&e->unkAC->rot, &c, &e->unkAC->rot);
            }
            VecSet(&e->unkAC->pos, e->pos.x, e->pos.y, e->pos.z);
            Model_Draw(e->unkAC);
        }
        if (e->unkB0) {
            if (e->unk64 & 0x4000) {
                e->unkB0->unkA = 0x13;
                e->unkB0->unkB = 3;
                e->unkB0->unkC = 0;
            }
            if (!(e->unk60 & 0x40)) {
                VecSet(&e->unkB0->scale, e->scale.x, e->scale.y, e->scale.z);
                if (e->unk64 & 0x2000) {
                    idx = (s32)(e->rz2 & 0xffff) >> 4;
                    MTX_RotZ33_(&e->unkB0->rot, SIN_IDX(idx), COS_IDX(idx));
                } else {
                    idx = (s32)(e->rz & 0xffff) >> 4;
                    MTX_RotZ33_(&e->unkB0->rot, SIN_IDX(idx), COS_IDX(idx));
                }
            }
            VecSet(&e->unkB0->pos, e->pos.x, e->pos.y, e->pos.z);
            Model_Draw(e->unkB0);
        }
    }
    one = 1;
    NNS_G3dGeBufferOP_N(0x12, &one, 1);
}
