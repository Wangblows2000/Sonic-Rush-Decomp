#include <nitro/types.h>

typedef struct UnkStruct_0231066c_v {
    s32 x, y, z;
} UnkStruct_0231066c_v;

typedef struct UnkStruct_0231066c_m43 {
    s32 m[12];
} UnkStruct_0231066c_m43;

typedef struct UnkStruct_0231066c_r {
    void *unk0;
    u8 unk4[0x34 - 4];
    u16 unk34;
} UnkStruct_0231066c_r;

typedef struct UnkStruct_0231066c UnkStruct_0231066c;
struct UnkStruct_0231066c {
    u8 pad0[0x16];
    u16 unk16;
    s32 unk18;
    s32 unk1C;
    UnkStruct_0231066c_v pos;
    u8 pad2C[0x18];
    UnkStruct_0231066c_v vel;
    UnkStruct_0231066c_v acc;
    u8 pad5C[4];
    u16 unk60;
    u8 pad62[2];
    u16 unk64;
    u8 pad66[6];
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    UnkStruct_0231066c_r *unk78;
    u8 unk7C[0xac - 0x7c];
    void *unkAC;
    void *unkB0;
    u8 padB4[0xcc - 0xb4];
    void (*unkCC)(UnkStruct_0231066c *);
    s32 unkD0;
    s32 unkD4;
    s32 unkD8;
};

typedef struct UnkStruct_0231066c_c {
    u8 pad0[0x1d8];
    void (*unk1D8)(UnkStruct_0231066c *);
    void (*unk1DC)(UnkStruct_0231066c *);
} UnkStruct_0231066c_c;

extern s16 data_0206e470[];

void func_ov001_023102ec(UnkStruct_0231066c *w);
void func_020316f4(void *p);
s32 func_ov001_02310144(UnkStruct_0231066c *w);
void func_ov001_0230d28c(void *a, void *b, s32 c, s32 d, u16 e, s32 f, s32 g);
void func_0200160c(void *a, void *b, void *c);
void func_02001558(void *v, void *m, void *out);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_0231066c(UnkStruct_0231066c_c *c, UnkStruct_0231066c *w) {
    s32 v0;
    s32 n;

    if (w->unk60 & 1)
        return;
    if (w->unk60 & 2)
        return;
    if (w->unk70 == 0) {
        func_ov001_023102ec(w);
        return;
    }
    if (c != NULL && c->unk1D8 != NULL)
        c->unk1D8(w);
    if (w->unk74 > 0) {
        w->unk74--;
        w->unk60 |= 0x100;
    } else {
        w->unk60 &= ~0x100;
    }
    if (w->unkCC != NULL)
        w->unkCC(w);
    if (w->unk64 & 0x100) {
        v0 = w->vel.x;
        n = v0 + w->acc.x;
        if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.x = 0;
            w->acc.x = 0;
        }
    }
    if (w->unk64 & 0x200) {
        v0 = w->vel.y;
        n = v0 + w->acc.y;
        if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.y = 0;
            w->acc.y = 0;
        }
    }
    if (w->unk64 & 0x400) {
        v0 = w->vel.z;
        n = v0 + w->acc.z;
        if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.z = 0;
            w->acc.z = 0;
        }
    }
    if (!(w->unk64 & 0x1000)) {
        w->vel.x += w->acc.x;
        w->vel.y += w->acc.y;
        w->vel.z += w->acc.z;
        if ((w->unk64 & 1) && w->unkD4 != 0) {
        } else if (w->unkD8 == w->unkD4 && (w->unk64 & 1)) {
        } else if ((w->unk64 & 0x10) && w->unkD8 != 0) {
        } else {
            w->unk16 += w->vel.x;
            w->unk18 += w->vel.y;
            w->unk1C += w->vel.z;
        }
    }
    if (!(w->unk60 & 0x10)) {
        if (w->unkAC != NULL)
            func_020316f4(w->unkAC);
        if (w->unkB0 != NULL)
            func_020316f4(w->unkB0);
    }
    if (c != NULL && c->unk1DC != NULL)
        c->unk1DC(w);
    w->unk6C++;
    w->unkD0++;
    w->unk70--;
    if (w->unk78 != NULL && (w->unk78->unk34 & 1))
        func_ov001_023102ec(w);
    if (w->unk60 & 0x20)
        return;
    if (w->unk78 != NULL) {
        UnkStruct_0231066c_v v = {0, 0, 0};
        UnkStruct_0231066c_m43 m;
        s32 flag = 0;
        if (!(w->unk64 & 0x1000)) {
            if (w->unk64 & 0x2000)
                flag = 1;
            func_ov001_0230d28c(w->unk7C, w->unk78->unk0, flag, func_ov001_02310144(w), w->unk16, w->unk18, w->unk1C);
        }
        func_0200160c(w->unk7C, w->unk78->unk4, &m);
        func_02001558(&v, &m, &v);
        w->pos = v;
    } else {
        u16 a = w->unk16 - 0x4000;
        w->pos.x = FX_Mul(COS(a), 0x1e000);
        w->pos.y = FX_Mul(SIN(a), 0x1e000);
        w->pos.x -= FX_Mul(w->unk18, SIN(w->unk16));
        w->pos.y += FX_Mul(w->unk18, COS(w->unk16));
    }
}
