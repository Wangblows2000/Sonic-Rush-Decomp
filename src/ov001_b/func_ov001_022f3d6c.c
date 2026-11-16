#include <nitro/types.h>

typedef struct UnkStruct_022f3d6c_m33 {
    s32 m[9];
} UnkStruct_022f3d6c_m33;

typedef struct UnkStruct_022f3d6c_m43 {
    s32 m[12];
} UnkStruct_022f3d6c_m43;

typedef struct UnkStruct_022f3d6c_a {
    u8 pad0[0x24];
    UnkStruct_022f3d6c_m33 mtx;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022f3d6c_a;

typedef struct UnkStruct_022f3d6c UnkStruct_022f3d6c;
struct UnkStruct_022f3d6c {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0xc];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u16 unk3C;
    u16 unk3E;
    u16 unk40;
    u8 pad42[0x7c - 0x42];
    UnkStruct_022f3d6c *unk7C;
    u8 pad80[4];
    UnkStruct_022f3d6c_a *unk84;
};

extern s32 data_ov001_0231f524;
extern s32 data_ov001_0231f520;
extern s16 data_0206e470[];

void func_02000e60(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_020316f4(void *p);
void func_020315e0(void *p);
void func_02025268(u32 op, const u32 *args, u32 n);
void func_02025a94(void *mtx, void *p);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_022f3d6c(UnkStruct_022f3d6c *w) {
    u32 c0;
    u32 c1;
    UnkStruct_022f3d6c_m33 m;
    UnkStruct_022f3d6c_m43 cur;
    UnkStruct_022f3d6c_m33 tmp;
    UnkStruct_022f3d6c *p = w->unk7C;
    UnkStruct_022f3d6c_a *a = p->unk84;

    a->unk48 = data_ov001_0231f524 + (p->unk18 + p->unk30);
    a->unk4C = -(data_ov001_0231f520 + (p->unk1C + p->unk34));
    a->unk50 = p->unk20 + p->unk38;
    func_02000e60(&a->mtx, SIN(p->unk3C), COS(p->unk3C));
    MTX_RotY33_(&m, SIN(p->unk3E), COS(p->unk3E));
    func_02000f34(&a->mtx, &m, &a->mtx);
    func_02000e98(&m, SIN(p->unk40), COS(p->unk40));
    func_02000f34(&a->mtx, &m, &a->mtx);
    if (!(w->unk14 & 0x10000))
        func_020316f4(a);
    func_020315e0(a);
    c0 = 2;
    func_02025268(0x10, &c0, 1);
    c1 = 0x1e;
    func_02025268(0x14, &c1, 1);
    func_02025a94(&cur, &tmp);
    a = w->unk84;
    w->unk18 = cur.m[9];
    w->unk1C = -cur.m[10];
    w->unk20 = cur.m[11];
    a->unk48 = w->unk18 + w->unk30;
    a->unk4C = -(w->unk1C + w->unk34);
    a->unk50 = w->unk20 + w->unk38;
    func_02000e60(&a->mtx, SIN(w->unk3C), COS(w->unk3C));
    MTX_RotY33_(&m, SIN(w->unk3E), COS(w->unk3E));
    func_02000f34(&a->mtx, &m, &a->mtx);
    func_02000e98(&m, SIN(w->unk40), COS(w->unk40));
    func_02000f34(&a->mtx, &m, &a->mtx);
    if (!(w->unk14 & 0x10000))
        func_020316f4(a);
    func_020315e0(a);
}
