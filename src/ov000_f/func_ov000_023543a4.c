#include <nitro/types.h>

typedef struct {
    fx32 m[3][3];
} UnkMtx33_023543a4;

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_023543a4;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x58 - 0x1C];
    u32 unk58;
} UnkStruct_023543a4_Obj;

typedef struct UnkStruct_023543a4 UnkStruct_023543a4;

struct UnkStruct_023543a4 {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x8 - 4];
    u32 unk8;
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
    void (*unk334)(UnkStruct_023543a4 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_023543a4 *w);
    u8 pad344[0x384 - 0x344];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_023543a4_Obj *unk3E8;
    u8 pad3EC[0x3F4 - 0x3EC];
    s32 unk3F4;
    u8 pad3F8[0x422 - 0x3F8];
    u16 unk422;
};

extern s16 data_0206e470[];

#define IDX_023543a4(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_023543a4(a) data_0206e470[(u32)(IDX_023543a4(a) << 1)]
#define COS_023543a4(a) data_0206e470[(u32)((IDX_023543a4(a) << 1) + 1)]

s16 func_02068df8(s16 v, s32 a, s32 b);
void func_0204b7ec(void *p, s32 a, s32 b);
void func_02000dfc(UnkMtx33_023543a4 *m);
void func_02000e98(UnkMtx33_023543a4 *m, fx32 sin, fx32 cos);
void MTX_RotY33_(UnkMtx33_023543a4 *m, fx32 sin, fx32 cos);
void func_02000f34(const UnkMtx33_023543a4 *a, const UnkMtx33_023543a4 *b, UnkMtx33_023543a4 *ab);
void func_02000eb0(const UnkVec_023543a4 *v, const UnkMtx33_023543a4 *m, UnkVec_023543a4 *dst);

static inline void VecSet_023543a4(UnkVec_023543a4 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void func_ov000_023543a4(UnkStruct_023543a4 *w) {
    UnkMtx33_023543a4 m;
    UnkMtx33_023543a4 r;
    UnkVec_023543a4 v;
    u8 prev;

    if (w->unk3E8 == NULL || (w->unk422 & 3)) {
        w->unk3E8 = NULL;
        w->unk54 &= ~2;
        w->unk58 &= ~0x2000;
        w->unk58 &= ~0x100;
        w->unk32C &= ~0x2000;
        w->unk330 &= ~0x30;
        w->unk32C &= ~0x100000;
        w->unk5C |= 0x200;
        w->unk3 = 0;
        w->unk2 = 0;
        w->unk1 = 0;
        w->unk340(w);
        return;
    }
    prev = w->unk3;
    w->unk34 = func_02068df8(w->unk34, 6, 0x280);
    w->unk3F4 -= w->unk34 * 6;
    w->unk3 = (w->unk3F4 & 0xffff) >> 8;
    if (w->unk3 > prev) {
        func_0204b7ec(w->unk384, 0, 0x4f);
    }
    if (w->unk3 < 0x80 && prev >= 0x80) {
        func_0204b7ec(w->unk384, 0, 0x4f);
    }
    w->unkC -= w->unk34;
    if (w->unk8 != 0 && w->unk8 < 0xffff) {
        w->unk8 += 0x40;
        if (w->unk8 > 0xffff) {
            w->unk8 = 0;
        }
    } else {
        w->unk8 = 0;
    }
    w->unk1C = w->x;
    w->unk20 = w->y;
    w->unk24 = w->z;
    func_02000dfc(&m);
    func_02000e98(&r, SIN_023543a4((u16)w->unk8), COS_023543a4((u16)w->unk8));
    func_02000f34(&m, &r, &m);
    MTX_RotY33_(&r, SIN_023543a4((u16)-(w->unk3 << 8)), COS_023543a4((u16)-(w->unk3 << 8)));
    func_02000f34(&m, &r, &m);
    VecSet_023543a4(&v, w->unkC << 4, 0, 0);
    func_02000eb0(&v, &m, &v);
    w->x = w->unk3E8->x - (v.x >> 4);
    w->y = w->unk3E8->y - (v.y >> 4);
    w->z = w->unk3E8->z - (v.z >> 4);
    w->unk28 = w->x - w->unk1C;
    w->unk2A = w->y - w->unk20;
    w->unk2C = w->z - w->unk24;
    if (w->unkC < 0x600) {
        w->unk34 = 0;
        w->unk3E8 = NULL;
        w->unk54 &= ~2;
        w->unk58 &= ~0x2000;
        w->unk58 &= ~0x100;
        w->unk32C &= ~0x2000;
        w->unk330 &= ~0x30;
        w->unk32C &= ~0x100000;
        w->unk5C |= 0x200;
        w->unk3 = 0;
        w->unk2 = 0;
        w->unk1 = 0;
        w->unk340(w);
    }
}
