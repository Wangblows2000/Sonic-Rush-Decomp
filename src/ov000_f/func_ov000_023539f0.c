#include <nitro/types.h>

typedef struct {
    fx32 m[3][3];
} UnkMtx33_023539f0;

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_023539f0;

typedef struct {
    u8 pad0[0x8];
    u32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x28 - 0x1C];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
} UnkStruct_023539f0_Obj;

typedef struct UnkStruct_023539f0 UnkStruct_023539f0;

struct UnkStruct_023539f0 {
    u8 pad0;
    u8 unk1;
    u8 unk2;
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
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_023539f0 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_023539f0 *w);
    u8 pad344[0x388 - 0x344];
    u8 unk388[0x3E8 - 0x388];
    UnkStruct_023539f0_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    u8 pad3F8[0x420 - 0x3F8];
    u16 unk420;
    u16 unk422;
};

extern s16 data_0206e470[];

#define IDX_023539f0(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_023539f0(a) data_0206e470[(u32)(IDX_023539f0(a) << 1)]
#define COS_023539f0(a) data_0206e470[(u32)((IDX_023539f0(a) << 1) + 1)]

void func_02000dfc(UnkMtx33_023539f0 *m);
void func_02000e60(UnkMtx33_023539f0 *m, fx32 sin, fx32 cos);
void MTX_RotY33_(UnkMtx33_023539f0 *m, fx32 sin, fx32 cos);
void func_02000f34(const UnkMtx33_023539f0 *a, const UnkMtx33_023539f0 *b, UnkMtx33_023539f0 *ab);
void func_02000eb0(const UnkVec_023539f0 *v, const UnkMtx33_023539f0 *m, UnkVec_023539f0 *dst);
void func_0201c070(void *p, s32 a);
void func_ov000_0234a064(UnkStruct_023539f0 *w, u8 ang);

static inline void VecSet_023539f0(UnkVec_023539f0 *v, fx32 x, fx32 y, fx32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void func_ov000_023539f0(UnkStruct_023539f0 *w) {
    UnkMtx33_023539f0 m;
    UnkMtx33_023539f0 r;
    UnkVec_023539f0 v;

    if (w->unk3E8 != NULL) {
        w->unk1C = w->x;
        w->unk20 = w->y;
        w->unk24 = w->z;
        if (w->unk32C & 2) {
            func_02000dfc(&m);
            if (w->unk32C & 4) {
                func_02000e60(&r, SIN_023539f0((u16)-w->unk3E8->unkC), COS_023539f0((u16)-w->unk3E8->unkC));
                w->unk2 = -w->unk3E8->unkC >> 8;
            } else {
                MTX_RotY33_(&r, SIN_023539f0((u16)w->unk3E8->unk8), COS_023539f0((u16)w->unk3E8->unk8));
            }
            if (w->unk32C & 8) {
                w->unk3 = -(w->unk3E8->unk8 >> 8);
            }
            func_02000f34(&m, &r, &m);
            VecSet_023539f0(&v, -w->unk3F0 << 4, -w->unk3F4 << 4, 0);
            func_02000eb0(&v, &m, &v);
            w->x = w->unk3E8->x - (v.x >> 4);
            w->y = w->unk3E8->y - (v.y >> 4);
            w->z = w->unk3E8->z - (v.z >> 4);
        } else {
            w->x = w->unk3E8->x + w->unk3F0;
            w->y = w->unk3E8->y + w->unk3F4;
        }
        if (w->unk32C & 1) {
            w->unk1 = (w->unk28 << 1) >> 8;
        }
        if (w->unk3E8->unk54 & 0x1000) {
            w->unk2E = w->unk3E8->unk34;
            w->unk3E8 = NULL;
        } else if (w->unk3E8->unk54 & 0x2000) {
            w->unk2E = w->unk3E8->unk2E;
            w->unk3E8 = NULL;
        } else if ((w->unk32C & 1) && (w->unk3E8->unk54 & 2)) {
            w->unk2E = w->unk3E8->unk28;
            w->unk30 = w->unk3E8->unk2A;
            w->unk3E8 = NULL;
        }
    }
    if (w->unk420 & 0x20) {
        w->unk5C |= 1;
    }
    if (w->unk420 & 0x10) {
        w->unk5C &= ~1;
    }
    if (w->unk3E8 != NULL && !(w->unk422 & 3)) {
        return;
    }
    if (w->unk340 == NULL) {
        return;
    }
    if (w->unk334 == NULL) {
        return;
    }
    w->unk3E8 = NULL;
    w->unk32C &= ~0x2000;
    w->unk330 &= ~0x30;
    w->unk330 &= ~0x20000;
    w->unk5C |= 0x200;
    w->unk58 |= 0xc0;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x4000;
    w->unk32C &= ~0x100000;
    w->unk2 = 0;
    w->unk3 = 0;
    w->unk1 = 0;
    func_0201c070(w->unk388, 0);
    if (w->unk422 & 3) {
        w->unk340(w);
        return;
    }
    func_ov000_0234a064(w, 0);
    w->unk334(w);
}
