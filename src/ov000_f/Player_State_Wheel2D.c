#include <nitro/types.h>

typedef struct {
    fx32 m[3][3];
} UnkMtx33_023530c0;

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_023530c0;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x58 - 0x1C];
    u32 unk58;
} UnkStruct_023530c0_Obj;

typedef struct UnkStruct_023530c0 UnkStruct_023530c0;

struct UnkStruct_023530c0 {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0xC - 4];
    s32 unkC;
    s32 x;
    s32 y;
    s32 z;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    u8 pad2E[0x32 - 0x2E];
    s16 unk32;
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x328 - 0x5E];
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_023530c0 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_023530c0 *w);
    u8 pad344[0x384 - 0x344];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_023530c0_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
};

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023530c0_4560;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_023530c0_aad8;

extern s16 data_0206e470[];
extern u32 gRandSeed;
extern UnkStruct_023530c0_4560 data_022c4560;
extern UnkStruct_023530c0_aad8 data_ov000_0236aad8;

#define IDX_023530c0(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_023530c0(a) data_0206e470[(u32)(IDX_023530c0(a) << 1)]
#define COS_023530c0(a) data_0206e470[(u32)((IDX_023530c0(a) << 1) + 1)]

void func_ov000_02329984(UnkStruct_023530c0 *w, s32 x, s32 y, s32 a, s32 b, u16 c);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Player_SetAnimation(UnkStruct_023530c0 *w, s32 state);

static inline u16 Rand_023530c0(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void MTX_Identity33_(UnkMtx33_023530c0 *m);
void MTX_RotY33_(UnkMtx33_023530c0 *m, fx32 sin, fx32 cos);
void MTX_Concat33(const UnkMtx33_023530c0 *a, const UnkMtx33_023530c0 *b, UnkMtx33_023530c0 *ab);
void MTX_MultVec33(const UnkVec_023530c0 *v, const UnkMtx33_023530c0 *m, UnkVec_023530c0 *dst);

static inline void VecSet_023530c0(UnkVec_023530c0 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void Player_State_Wheel2D(UnkStruct_023530c0 *w) {
    UnkMtx33_023530c0 m;
    UnkMtx33_023530c0 r;
    UnkVec_023530c0 v;
    u8 prev;

    if (w->unk3E8 == NULL) {
        w->unk34 = 0;
        w->unk3E8 = NULL;
        w->unk54 &= ~2;
        w->unk58 &= ~0x2000;
        w->unk58 &= ~0x4000;
        w->unk32C &= ~0x2000;
        w->unk330 &= ~0x30;
        w->unk330 &= ~0x20000;
        w->unk32C &= ~0x100000;
        w->unk5C |= 0x200;
        w->unk3 = 0;
        w->unk2 = 0;
        w->unk1 = 0;
        w->unk340(w);
        return;
    }
    if (!(data_022c4560.unk20 & 3) && !(Rand_023530c0() & 3)) {
        func_ov000_02329984(w, w->unk3E8->x, w->unk3E8->y, 0, 0, data_ov000_0236aad8.unk62);
    }
    if (w->unk34 < 0x40) {
        w->unk34 = 0x40;
    }
    prev = w->unk3;
    w->unk3F4 -= w->unk34 * 6;
    w->unk3 = (w->unk3F4 & 0xffff) >> 8;
    if (w->unk3 > prev) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x58);
    }
    if (w->unk3 < 0x80 && prev >= 0x80) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x58);
    }
    w->unk3F8 -= (w->unk3F8 - 0x5000) >> 4;
    w->unkC++;
    if (w->unkC < 0x40) {
        w->unk3FC -= (w->unk3FC - 0x1800) >> 4;
    } else {
        if (w->unkC == 0x70) {
            w->unk32C |= 2;
        }
        if (w->unk328 == 0x28) {
            Player_SetAnimation(w, 0x29);
            w->unk5C |= 4;
        }
        if (w->unk32C & 1) {
            w->unk3FC -= 0x80;
        } else {
            w->unk3FC += 0x180;
        }
    }
    w->unk1C = w->x;
    w->unk20 = w->y;
    w->unk24 = w->z;
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, SIN_023530c0((u16)-(w->unk3 << 8)), COS_023530c0((u16)-(w->unk3 << 8)));
    MTX_Concat33(&m, &r, &m);
    VecSet_023530c0(&v, w->unk3F8 << 4, w->unk3FC << 4, 0);
    MTX_MultVec33(&v, &m, &v);
    w->x = w->unk3E8->x - (v.x >> 4);
    w->y = w->unk3E8->y - (v.y >> 4);
    w->z = w->unk3E8->z - (v.z >> 4);
    w->unk28 = w->x - w->unk1C;
    w->unk2A = w->y - w->unk20;
    w->unk2C = w->z - w->unk24;
}
