#include <nitro/types.h>

typedef struct UnkStruct_0234aec8 UnkStruct_0234aec8;

struct UnkStruct_0234aec8 {
    u8 pad0[0xC];
    u32 unkC;
    s32 x;
    u8 pad14[0x32 - 0x14];
    s16 unk32;
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x324 - 0x5E];
    u8 unk324;
    u8 pad325[0x327 - 0x325];
    u8 unk327;
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_0234aec8 *w);
    u8 pad338[0x33C - 0x338];
    void (*unk33C)(UnkStruct_0234aec8 *w);
    void (*unk340)(UnkStruct_0234aec8 *w);
    u8 pad344[0x37A - 0x344];
    s16 unk37A;
    u8 pad37C[0x384 - 0x37C];
    u8 unk384[0x420 - 0x384];
    u16 unk420;
    u16 unk422;
};

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_0234aec8_4560;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x24 - 0x20];
    s32 unk24;
} UnkStruct_0234aec8_4534;

typedef struct {
    u16 v;
    u8 pad2[0x62];
} UnkStruct_0234aec8_Tbl;

extern UnkStruct_0234aec8_4560 data_022c4560;
extern UnkStruct_0234aec8_4534 data_022c4534;
extern u32 gObjManagerFlags;
extern u32 gRandSeed;
extern UnkStruct_0234aec8_Tbl data_ov000_0236ab3a[];

void func_0201c070(void *p, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0234c4a4(UnkStruct_0234aec8 *w, s32 state);
BOOL func_ov000_023470ec(UnkStruct_0234aec8 *w);
void func_ov000_0234b6dc(UnkStruct_0234aec8 *w);
void func_ov000_0234ab34(UnkStruct_0234aec8 *w);
void func_ov000_0234b424(UnkStruct_0234aec8 *w);
void func_ov000_02347870(UnkStruct_0234aec8 *w);
void func_ov000_023470a0(UnkStruct_0234aec8 *w, s32 v);
void func_ov000_02333934(UnkStruct_0234aec8 *w);
void func_ov000_0232a11c(UnkStruct_0234aec8 *w, s32 a, s32 b, u16 c);
void func_ov000_023333a0(UnkStruct_0234aec8 *w);

static inline u16 Rand_0234aec8(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_0234aec8(UnkStruct_0234aec8 *w) {
    s32 a;
    u32 f;
    s16 st;

    if ((w->unk328 < 5 || w->unk328 > 10) &&
        (((w->unk420 & 0x20) && w->unk34 >= 0x200) || ((w->unk420 & 0x10) && w->unk34 <= -0x200))) {
        func_0201c070(w->unk384, 0);
        if (w->unk324 != 1) {
            Sound_PlaySeqArcEx(w->unk384, 0, 3);
        } else {
            Sound_PlaySeqArcEx(w->unk384, 0, 0x14);
        }
        a = w->unk34;
        if (a < 0) {
            a = -a;
        }
        if (a > 0x700) {
            func_ov000_0234c4a4(w, 8);
        } else {
            func_ov000_0234c4a4(w, 5);
        }
    }
    if (w->unk328 == 5 && (w->unk5C & 8)) {
        func_ov000_0234c4a4(w, 6);
        w->unk5C |= 4;
    }
    if (w->unk328 == 8 && (w->unk5C & 8)) {
        func_ov000_0234c4a4(w, 9);
        w->unk5C |= 4;
    }
    if ((w->unk328 == 6 || w->unk328 == 9) && (w->unk34 <= 0 || !(w->unk420 & 0x20)) &&
        (w->unk34 >= 0 || !(w->unk420 & 0x10))) {
        func_0201c070(w->unk384, 0);
        if (w->unk328 == 6) {
            func_ov000_0234c4a4(w, 7);
        } else {
            func_ov000_0234c4a4(w, 10);
        }
    }
    if (w->unk328 == 0 || (w->unk328 >= 5 && w->unk328 <= 10)) {
        if (w->unk420 & 0x20) {
            w->unk32C |= 1;
        } else if (w->unk420 & 0x10) {
            w->unk32C &= ~1;
        }
    }
    if (func_ov000_023470ec(w)) {
        func_ov000_0234b6dc(w);
        func_ov000_0234ab34(w);
        return;
    }
    if (!(w->unk330 & 0x40000) && (st = w->unk328) != 0 && (w->unk58 & 4)) {
        f = w->unk5C;
        if ((((f & 1) && (w->unk420 & 0x20)) || (!(f & 1) && (w->unk420 & 0x10))) &&
            (w->x >> 8) > data_022c4534.unk1C + 0xe && (w->x >> 8) < data_022c4534.unk24 - 0xe) {
            if (st != 0xb && st != 0xc) {
                func_ov000_0234c4a4(w, 0xb);
            } else if (st == 0xb && (f & 8)) {
                func_ov000_0234c4a4(w, 0xc);
                w->unk5C |= 4;
            }
            goto next;
        }
    }
    if (w->unk34 == 0 && w->unk32 == 0) {
        func_ov000_0234b6dc(w);
        w->unk334(w);
        return;
    }
    st = w->unk328;
    if ((st != 0 || (w->unk5C & 8)) && (st < 5 || st > 10 || ((st == 7 || st == 10) && (w->unk5C & 8)))) {
        func_ov000_0234b6dc(w);
        func_ov000_0234b424(w);
    }
next:
    func_ov000_02347870(w);
    if ((w->unk422 & 3) && w->unk340 != NULL) {
        func_ov000_0234b6dc(w);
        w->unk340(w);
        return;
    }
    if (!(w->unk330 & 0x100) && (w->unk420 & 0x80) && w->unk33C != NULL) {
        func_ov000_0234b6dc(w);
        w->unk33C(w);
        return;
    }
    func_ov000_023470a0(w, w->unk34);
    if (w->unk328 >= 5 && w->unk328 <= 10) {
        if (!(gObjManagerFlags & 0x100) && !(w->unkC & 7)) {
            func_ov000_02333934(w);
            if ((w->unk32C & 0x4000000) && !(Rand_0234aec8() & 1)) {
                func_ov000_0232a11c(w, 0, 0, data_ov000_0236ab3a[w->unk327].v);
            }
        }
        w->unkC++;
    }
    if (gObjManagerFlags & 0x100) {
        return;
    }
    if (w->unk324 != 1) {
        return;
    }
    a = w->unk34;
    if (a < 0) {
        a = -a;
    }
    if (w->unk37A >= a) {
        return;
    }
    if (w->unk32C & 0x100) {
        return;
    }
    if (!(data_022c4560.unk18 & 1)) {
        return;
    }
    if (data_022c4560.unk20 & 3) {
        return;
    }
    func_ov000_023333a0(w);
}
