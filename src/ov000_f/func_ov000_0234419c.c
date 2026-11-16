#include <nitro/types.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
} UnkStruct_0234419c_Obj;

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkStruct_0234419c_Pal;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x3E - 0x18];
    s16 unk3E;
    u8 pad40[0x54 - 0x40];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x2C0 - 0x5E];
    u32 unk2C0;
    u32 unk2C4;
    u8 pad2C8[0x324 - 0x2C8];
    u8 unk324;
    u8 pad325[0x328 - 0x325];
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x352 - 0x334];
    s16 unk352;
    u8 pad354[0x358 - 0x354];
    s16 unk358;
    u8 pad35A[0x368 - 0x35A];
    s16 unk368;
    u8 pad36A[0x36C - 0x36A];
    s16 unk36C;
    u8 pad36E[0x3B2 - 0x36E];
    s16 unk3B2;
    s16 unk3B4;
    s16 unk3B6;
    s16 unk3B8;
    u8 pad3BA[0x3BE - 0x3BA];
    s16 unk3BE;
    s16 unk3C0;
    s16 unk3C2;
    u8 pad3C4[0x3C8 - 0x3C4];
    s16 unk3C8;
    s16 unk3CA;
    u8 pad3CC[0x3E8 - 0x3CC];
    UnkStruct_0234419c_Obj *unk3E8;
    u8 pad3EC[0x410 - 0x3EC];
    u8 unk410;
    u8 unk411;
    u8 pad412[0x414 - 0x412];
    UnkStruct_0234419c_Pal pal;
    u8 pad420[0x42C - 0x420];
    u16 unk42C;
    u16 unk42E;
    u16 unk430;
    u16 unk432;
} UnkStruct_0234419c;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0234419c *ply;
} UnkStruct_0234419c_Task;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
    u32 unk20;
} UnkStruct_0234419c_4560;

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
} UnkStruct_0234419c_abd8;

typedef struct {
    u16 v[4];
} UnkStruct_0234419c_Col;

typedef struct {
    s16 v;
    u8 pad2[0x2E];
} UnkStruct_0234419c_Tbl;

extern UnkStruct_0234419c_Task *gTaskCurrent;
extern UnkStruct_0234419c_4560 data_022c4560;
extern UnkStruct_0234419c_abd8 data_ov000_0236abd8;
extern u32 gRandSeed;
extern UnkStruct_0234419c_Col data_ov000_02368b74;
extern UnkStruct_0234419c_Tbl data_ov000_02368b7e[];
extern UnkStruct_0234419c_Tbl data_ov000_02368b8e[];
extern UnkStruct_0234419c_Tbl data_ov000_02368b9e[];
extern UnkStruct_0234419c_Tbl data_ov000_02368b92[];

void func_ov000_02344f68(UnkStruct_0234419c *w);
void func_ov000_02345278(UnkStruct_0234419c *w);
void func_ov000_02344a74(UnkStruct_0234419c *w);
s32 func_ov000_0233b078(s32 x);
void func_ov000_023355e0(u16 a, u16 b, u16 c, u16 d);
void func_ov000_02344940(UnkStruct_0234419c *w);
void func_ov000_023455ec(UnkStruct_0234419c *w);
void func_ov000_02343a38(void);
BOOL func_ov000_02345758(s32 x, s32 y);
u32 func_ov000_023376a4(void);
void func_ov000_02337688(u32 v);
void func_0206cdf4(UnkStruct_0234419c_Pal *p, s32 a, s32 b, s32 c);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_02349038(UnkStruct_0234419c *w);
void func_ov000_023461f0(UnkStruct_0234419c *w);
void func_ov000_02346e38(UnkStruct_0234419c *w);
void func_ov000_02346f34(UnkStruct_0234419c *w);
void func_ov000_023466e8(UnkStruct_0234419c *w);
void func_ov000_02346ddc(UnkStruct_0234419c *w);
void func_ov000_02346178(UnkStruct_0234419c *w);
void func_ov000_023460ec(UnkStruct_0234419c *w);
void func_ov000_02346040(UnkStruct_0234419c *w);
void func_ov000_02345e98(UnkStruct_0234419c *w);

static inline u16 Rand_0234419c(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_0234419c(void) {
    UnkStruct_0234419c *w = gTaskCurrent->ply;
    s32 bottom;
    s32 right;
    s32 top;
    s32 left;

    if (data_022c4560.unk10 == 2) {
        func_ov000_02344f68(w);
    }
    func_ov000_02345278(w);
    func_ov000_02344a74(w);
    if (w->unk330 & 0x200000) {
        w->unk330 &= ~0x200000;
        if (w->y > func_ov000_0233b078(w->x)) {
            left = (w->x >> 8) - 0x100;
            top = (w->y >> 8) - 0x1e0;
            right = (w->x >> 8) + 0x100;
            bottom = (w->y >> 8) + 0x120;
        } else {
            left = (w->x >> 8) - 0x100;
            top = (w->y >> 8) - 0x120;
            right = (w->x >> 8) + 0x100;
            bottom = (w->y >> 8) + 0x1e0;
        }
        if (left < 0) {
            left = 0;
        } else if (left > 0xffff) {
            left = 0xffff;
        }
        if (top < 0) {
            top = 0;
        } else if (top > 0xffff) {
            top = 0xffff;
        }
        if (right < 0) {
            right = 0;
        } else if (right > 0xffff) {
            right = 0xffff;
        }
        if (bottom < 0) {
            bottom = 0;
        } else if (bottom > 0xffff) {
            bottom = 0xffff;
        }
        func_ov000_023355e0(left, top, right, bottom);
    }
    func_ov000_02344940(w);
    func_ov000_023455ec(w);
    func_ov000_02343a38();
    if (w->unk3E8 != NULL && (w->unk3E8->unk54 & 4)) {
        w->unk3E8 = NULL;
    }
    if (w->unk410 != 0) {
        if ((w->unk328 < 0x1a || w->unk328 > 0x20) && !(w->unk330 & 4) && (w->unk411 & 2)) {
            w->unk54 &= ~1;
            w->unk54 |= w->unk411 & 1;
            w->unk411 = 0;
            if (!(w->unk410 & 0x80)) {
                w->unk410 = 0;
            }
        }
        w->unk330 &= ~4;
    }
    if (w->unk352 != 0) {
        w->unk352--;
    }
    if (w->unk3BE != 0) {
        w->unk3BE--;
    }
    if (w->unk3B8 != 0) {
        w->unk3B8--;
    }
    if (w->unk3C0 != 0) {
        if (!(w->unk3C0 & 0x7f)) {
            UnkStruct_0234419c_Col col = data_ov000_02368b74;
            u16 r = Rand_0234419c() & 3;
            w->unk430 = col.v[(u16)(r + 1) & 3];
            w->unk432 = col.v[(u16)(r + 2) & 3];
            w->unk42E = col.v[(u16)(r + 3) & 3];
            w->unk42C = col.v[(u16)(r + 4) & 3];
        }
        w->unk3C0--;
        if (w->unk3C0 == 0) {
            w->unk430 = 0x40;
            w->unk432 = 0x80;
            if (func_ov000_02345758(w->x, w->y) != 0) {
                w->unk42E = 0x10;
                w->unk42C = 0x20;
            } else {
                w->unk42E = 0x20;
                w->unk42C = 0x10;
            }
        }
    }
    if (w->unk3C2 != 0) {
        w->unk3C2--;
        w->unk358 = data_ov000_02368b7e[w->unk324].v >> 2;
        w->unk368 = data_ov000_02368b8e[w->unk324].v >> 1;
        w->unk3E = data_ov000_02368b9e[w->unk324].v >> 2;
        w->unk36C = data_ov000_02368b92[w->unk324].v >> 2;
        if (w->unk3C2 == 0) {
            w->unk358 = data_ov000_02368b7e[w->unk324].v;
            w->unk368 = data_ov000_02368b8e[w->unk324].v;
            w->unk3E = data_ov000_02368b9e[w->unk324].v;
            w->unk36C = data_ov000_02368b92[w->unk324].v;
        }
    }
    if (w->unk3C8 != 0) {
        w->unk3C8--;
    }
    if (w->unk3B2 != 0) {
        w->unk3B2--;
        if (w->unk3B2 & 2) {
            w->unk5C |= 0x20;
        } else {
            w->unk5C &= ~0x20;
        }
        w->unk2C4 |= 5;
        if (w->unk3B2 == 0) {
            w->unk5C &= ~0x20;
            w->unk2C4 = 2;
        }
    }
    if (w->unk3B6 != 0) {
        if (data_022c4560.unk18 & 1) {
            w->unk3B6--;
        }
        if (!(w->unk32C & 0x1000000)) {
            func_ov000_02337688(func_ov000_023376a4() | 0x20);
            if (w->unk3B6 == 0) {
                func_ov000_02337688(func_ov000_023376a4() & ~0x20);
                func_0206cdf4(&w->pal, 0, 0, 0);
                Sound_PlaySeqArcEx(NULL, 0, 0x3c);
            } else {
                u32 v = data_022c4560.unk20;
                u32 a;
                u32 b;
                if (v & 0x40) {
                    a = (v >> 1) & 0x1f;
                    b = a >> 1;
                    func_0206cdf4(&w->pal, (s8)a, (s8)b, (s8)-b);
                } else {
                    a = 0x1f - ((v >> 1) & 0x1f);
                    b = a >> 1;
                    func_0206cdf4(&w->pal, (s8)a, (s8)b, (s8)-b);
                }
            }
        }
    }
    if (w->unk3B4 != 0) {
        w->unk3CA = 0;
        w->unk3B4--;
        w->unk2C4 |= 5;
        w->unk2C0 |= 0x15;
        if (!(w->unk32C & 0x1000000)) {
            if (!(data_022c4560.unk20 & 0xe) && (data_022c4560.unk18 & 1)) {
                func_0206cdf4(&w->pal, 0x1f, 0x1f, 0x1f);
            } else if (w->unk3B6 == 0) {
                func_0206cdf4(&w->pal, 0, 0, 0);
            }
        }
        if (w->unk3B4 == 0) {
            w->unk5C &= ~0x20;
            w->unk2C0 = 10;
            w->unk2C4 = 2;
            func_0206cdf4(&w->pal, 0, 0, 0);
        }
    }
    if (!(w->unk58 & 0x100) && !(w->unk32C & 0x1000000) &&
        (data_022c4560.unk1C != 6 || data_022c4560.unk1D != 0)) {
        if (data_ov000_0236abd8.unk10 - 0x1a <= (w->y >> 8)) {
            func_ov000_02349038(w);
        }
        if (data_022c4560.unk1C == 6 && data_022c4560.unk1D == 1 && (w->y >> 8) <= 0x1a) {
            func_ov000_02349038(w);
        }
    }
    func_ov000_023461f0(w);
    func_ov000_02346e38(w);
    func_ov000_02346f34(w);
    func_ov000_023466e8(w);
    func_ov000_02346ddc(w);
    func_ov000_02346178(w);
    func_ov000_023460ec(w);
    func_ov000_02346040(w);
    func_ov000_02345e98(w);
}
