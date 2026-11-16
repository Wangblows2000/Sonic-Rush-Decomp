#include <nitro/types.h>

typedef struct {
    fx32 m[3][3];
} UnkMtx33_023528d4;

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_023528d4;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x58 - 0x1C];
    u32 unk58;
} UnkStruct_023528d4_Obj;

typedef struct UnkStruct_023528d4 UnkStruct_023528d4;

struct UnkStruct_023528d4 {
    u8 pad0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 pad4[0x10 - 4];
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
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_023528d4 *w);
    u8 pad338[0x3E8 - 0x338];
    UnkStruct_023528d4_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
};

extern s16 data_0206e470[];

#define IDX_023528d4(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_023528d4(a) data_0206e470[(u32)(IDX_023528d4(a) << 1)]
#define COS_023528d4(a) data_0206e470[(u32)((IDX_023528d4(a) << 1) + 1)]

void func_02000dfc(UnkMtx33_023528d4 *m);
void MTX_RotY33_(UnkMtx33_023528d4 *m, fx32 sin, fx32 cos);
void func_02000f34(const UnkMtx33_023528d4 *a, const UnkMtx33_023528d4 *b, UnkMtx33_023528d4 *ab);
void func_02000eb0(const UnkVec_023528d4 *v, const UnkMtx33_023528d4 *m, UnkVec_023528d4 *dst);

static inline void VecSet_023528d4(UnkVec_023528d4 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void Player_State_Hand2D(UnkStruct_023528d4 *w) {
    UnkMtx33_023528d4 m;
    UnkMtx33_023528d4 r;
    UnkVec_023528d4 v;

    if (w->unk3E8 != NULL && (w->unk3E8->unk58 & 0x2000)) {
        w->unk3E8 = NULL;
    }
    if (w->unk3E8 != NULL) {
        w->unk3F4 -= (w->unk3E8->z >> 4) + 0x470;
        w->unk3 = (w->unk3F4 & 0xffff) >> 8;
        w->unk1C = w->x;
        w->unk20 = w->y;
        w->unk24 = w->z;
        func_02000dfc(&m);
        MTX_RotY33_(&r, SIN_023528d4((u16)-(w->unk3 << 8)), COS_023528d4((u16)-(w->unk3 << 8)));
        func_02000f34(&m, &r, &m);
        VecSet_023528d4(&v, w->unk3F8 << 4, w->unk3FC << 4, 0);
        func_02000eb0(&v, &m, &v);
        w->x = w->unk3E8->x - (v.x >> 4);
        w->y = w->unk3E8->y - (v.y >> 4);
        w->z = w->unk3E8->z - (v.z >> 4);
        w->unk28 = w->x - w->unk1C;
        w->unk2A = w->y - w->unk20;
        w->unk2C = w->z - w->unk24;
    }
    if (w->unk3E8 != NULL) {
        return;
    }
    if (w->unk334 == NULL) {
        return;
    }
    w->unk3E8 = NULL;
    w->unk32C &= ~0x2000;
    w->unk330 &= ~0x30;
    w->unk330 &= ~0x20000;
    w->unk58 |= 1;
    w->unk58 |= 0xc0;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x100;
    w->unk58 &= ~0x4000;
    w->unk32C &= ~0x100000;
    w->unk1 = 0;
    w->unk3 = 0;
    w->unk32 = 0;
    w->z = 0;
    w->unk34 = w->unk3F0;
    w->unk334(w);
}
