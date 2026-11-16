#include <nitro/types.h>

typedef struct UnkStruct_0231323c_v {
    s32 x, y, z;
} UnkStruct_0231323c_v;

typedef struct UnkStruct_0231323c_m33 {
    s32 m[9];
} UnkStruct_0231323c_m33;

typedef struct UnkStruct_0231323c_m43 {
    s32 m[12];
} UnkStruct_0231323c_m43;

typedef struct UnkStruct_0231323c_p {
    u8 pad[0x60];
    u16 unk60;
} UnkStruct_0231323c_p;

typedef struct UnkStruct_0231323c_t {
    u16 unk0;
    u8 pad2[0x34 - 2];
} UnkStruct_0231323c_t;

typedef struct UnkStruct_0231323c_t2 {
    s32 unk0;
    u8 pad4[0x34 - 4];
} UnkStruct_0231323c_t2;

typedef struct UnkStruct_0231323c UnkStruct_0231323c;
struct UnkStruct_0231323c {
    u8 pad0[0x10];
    u16 unk10;
    u16 unk12;
    u8 pad14[2];
    u16 unk16;
    s32 unk18;
    u8 pad1C[4];
    UnkStruct_0231323c_v pos;
    u8 pad2C[0x18];
    UnkStruct_0231323c_v vel;
    UnkStruct_0231323c_v acc;
    u8 pad5C[8];
    u16 unk64;
    u8 pad66[6];
    s32 unk6C;
    u8 pad70[4];
    s32 unk74;
    u8 pad78[0x6c];
    UnkStruct_0231323c_v unkE4;
    UnkStruct_0231323c_m33 mtx;
    u8 pad114[4];
    UnkStruct_0231323c_p *unk118;
    u8 pad11C[0x28];
    s32 unk144;
    u8 pad148[0x18];
    UnkStruct_0231323c_v unk160;
    UnkStruct_0231323c_v unk16C;
    UnkStruct_0231323c_v unk178;
    UnkStruct_0231323c_m33 unk184;
    UnkStruct_0231323c_m33 unk1A8;
    UnkStruct_0231323c_m33 unk1CC;
    u8 pad1F0[4];
    s32 unk1F4;
    u8 pad1F8[0x30];
    s32 unk228;
    void (*unk22C)(UnkStruct_0231323c *);
};

extern s16 data_0206e470[];
extern UnkStruct_0231323c_t data_ov001_0231790a[];
extern UnkStruct_0231323c_t2 data_ov001_02317918[];
extern u8 *data_ov001_0231f694;

void func_ov001_023120c8(UnkStruct_0231323c *w);
BOOL func_ov001_02310248(UnkStruct_0231323c *w, UnkStruct_0231323c_p *p);
s32 func_ov001_0230d6e0(void);
void func_ov001_02312c18(UnkStruct_0231323c *w);
void func_02000e60(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_02000e20(void *a, void *b);
void func_0200148c(void *m);
void func_02001c00(void *a, void *b, s32 x, s32 y, s32 z);
void func_0200160c(void *a, void *b, void *c);
void func_02001558(void *v, void *m, void *out);
void Model_Animate(void *p);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline void VecSet(UnkStruct_0231323c_v *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_0231323c(UnkStruct_0231323c *w) {
    UnkStruct_0231323c_p *p;
    UnkStruct_0231323c_m33 mx;
    UnkStruct_0231323c_m33 my;
    UnkStruct_0231323c_m33 mz;
    UnkStruct_0231323c_m43 m2;
    UnkStruct_0231323c_m43 m1;
    UnkStruct_0231323c_v *pv;

    w->unk6C++;
    if (w->unk144 > 0) {
        w->unk144--;
        if (w->unk144 == 0)
            func_ov001_023120c8(w);
    }
    p = w->unk118;
    if (p != NULL) {
        if (p->unk60 & 1)
            w->unk118 = NULL;
        else if (!func_ov001_02310248(w, p))
            w->unk118 = NULL;
    }
    if (w->unk74 > 0)
        w->unk74--;
    w->unk178 = w->unk16C;
    w->unk16C = w->unk160;
    w->unk160 = w->pos;
    w->unk1CC = w->unk1A8;
    w->unk1A8 = w->unk184;
    w->unk184 = w->mtx;
    w->unk1F4--;
    if (func_ov001_0230d6e0() > 0x3000) {
        w->unk178.z += func_ov001_0230d6e0();
        w->unk16C.z += func_ov001_0230d6e0();
        w->unk160.z += func_ov001_0230d6e0();
    } else if (w->unk1F4 > 0) {
        w->unk178.z += w->unk1F4 * 0x230;
        w->unk16C.z += w->unk1F4 * 0x230;
        w->unk160.z += w->unk1F4 * 0x230;
    }
    if (data_ov001_0231790a[w->unk228].unk0 & 0x400) {
        s32 v0 = w->vel.x;
        s32 n = v0 + w->acc.x;
        if (v0 == 0) {
            w->acc.x = 0;
        } else if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.x = 0;
            w->acc.x = 0;
        }
    }
    if (!(data_ov001_0231790a[w->unk228].unk0 & 0x10)) {
        w->vel.x += w->acc.x;
        w->vel.y += w->acc.y;
        w->vel.z += w->acc.z;
        w->unk16 += w->vel.x;
        w->unk18 += w->vel.y;
        w->pos.z += w->vel.z;
        if (w->unk18 < 0)
            w->unk18 = 0;
    } else {
        if (data_ov001_02317918[w->unk228].unk0 == 3 && w->unk118 != NULL) {
            func_ov001_02312c18(w);
        } else {
            w->vel.x += w->acc.x;
            w->vel.y += w->acc.y;
            w->vel.z += w->acc.z;
            w->pos.x += w->vel.x;
            w->pos.y += w->vel.y;
            w->pos.z += w->vel.z;
        }
        if (w->pos.x > 0x1e000)
            w->pos.x = 0x1e000;
        else if (w->pos.x < -0x1e000)
            w->pos.x = -0x1e000;
        if (w->pos.y < -0x1e000)
            w->pos.y = -0x1e000;
    }
    if (data_ov001_0231790a[w->unk228].unk0 & 0x100) {
        if (w->pos.z < 0) {
            w->vel.z = 0x2800;
            w->acc.z = 0;
        } else if (w->pos.z > 0) {
            w->pos.z = 0;
            w->vel.z = 0;
            w->acc.z = 0;
        }
    }
    if (data_ov001_0231790a[w->unk228].unk0 & 0x200) {
        if (w->unk16 < 0x8000) {
            if (w->vel.x > 0) {
                w->vel.x = 0;
                w->acc.x = 0;
            }
        } else {
            if (w->vel.x < 0) {
                w->vel.x = 0;
                w->acc.x = 0;
            }
        }
    }
    if (w->unk22C != NULL)
        w->unk22C(w);
    if (!(data_ov001_0231790a[w->unk228].unk0 & 0x10)) {
        if (w->unk228 == 6) {
            w->pos.x = (w->unk16 < 0x8000) ? 0x1e000 : -0x1e000;
            w->pos.y = w->unk18;
        } else {
            u16 a = w->unk16 - 0x4000;
            w->pos.x = FX_Mul(COS(a), 0x1e000);
            w->pos.y = FX_Mul(SIN(a), 0x1e000);
            if (!(w->unk64 & 0x2000)) {
                w->pos.y += w->unk18;
            } else {
                w->pos.x -= FX_Mul(w->unk18, SIN(w->unk16));
                w->pos.y += FX_Mul(w->unk18, COS(w->unk16));
            }
        }
    }
    MTX_RotY33_(&my, SIN(w->unk12), COS(w->unk12));
    func_02000e60(&mx, SIN(w->unk10), COS(w->unk10));
    func_02000e98(&mz, SIN(w->unk16), COS(w->unk16));
    func_02000f34(&my, &mx, &w->mtx);
    func_02000f34(&w->mtx, &mz, &w->mtx);
    {
    UnkStruct_0231323c_v v = {0, 0, 0};
    func_02000e20(&w->mtx, &m1);
    func_0200148c(&m2);
    func_02001c00(&m2, &m2, w->pos.x, w->pos.y, w->pos.z);
    func_0200160c(&m1, &m2, &m2);
    func_02001c00(&m2, &m2, 0, 0x4000, 0);
    func_02001558(&v, &m2, &w->unkE4);
    }
    Model_Animate(data_ov001_0231f694 + 4);
}
