#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 y;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x40 - 0x32];
    s16 unk40;
    s16 unk42;
    u8 pad44[0x324 - 0x44];
    u8 unk324;
    u8 unk325;
    u8 pad326;
    u8 unk327;
    u8 pad328[0x32C - 0x328];
    u32 unk32C;
    u8 pad330[0x374 - 0x330];
    s16 unk374;
    u8 pad376[0x384 - 0x376];
    u8 unk384[0x3B4 - 0x384];
    s16 unk3B4;
    u8 pad3B6[0x3CA - 0x3B6];
    s16 unk3CA;
} UnkStruct_023461f0;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
    u32 unk20;
} UnkStruct_023461f0_4560;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_023461f0_4590;

typedef struct {
    u16 v;
    u8 pad2[0x62];
} UnkStruct_023461f0_Tbl;

typedef struct {
    s16 v;
    u8 pad2[0x2E];
} UnkStruct_023461f0_Tbl2;

extern UnkStruct_023461f0_4560 data_022c4560;
extern UnkStruct_023461f0_4590 data_022c4590;
extern u32 gRandSeed;
extern UnkStruct_023461f0_Tbl data_ov000_0236ab3a[];
extern UnkStruct_023461f0_Tbl2 data_ov000_02368ba4[];
extern UnkStruct_023461f0_Tbl2 data_ov000_02368ba6[];

void func_0204c66c(s32 a);
void func_ov000_0232a11c(UnkStruct_023461f0 *w, s32 a, s32 b, u16 c);
void func_ov000_023297c0(UnkStruct_023461f0 *w);
void func_ov000_023294c8(UnkStruct_023461f0 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0232a93c(UnkStruct_023461f0 *w, s32 a, s32 b, s32 c);
void func_ov000_02342a74(UnkStruct_023461f0 *w, s32 a);
void Player_Action_Die(UnkStruct_023461f0 *w);

static inline u16 Rand_023461f0(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_023461f0(UnkStruct_023461f0 *w) {
    u16 lvl;

    if (w->unk325 != 0) {
        return;
    }
    if (data_022c4560.unk1C != 1) {
        return;
    }
    if (!(data_022c4560.unk18 & 1)) {
        return;
    }
    lvl = data_ov000_0236ab3a[w->unk327].v;
    if (lvl != 0) {
        if ((w->y >> 8) > lvl) {
            if (!(w->unk32C & 0x4000000)) {
                if (w->unk3B4 == 0) {
                    func_0204c66c(1);
                }
                func_ov000_0232a11c(w, 0, 8, data_ov000_0236ab3a[w->unk327].v);
                func_ov000_0232a11c(w, 6, 7, data_ov000_0236ab3a[w->unk327].v);
                func_ov000_0232a11c(w, -4, 6, data_ov000_0236ab3a[w->unk327].v);
                w->unk30 = 0;
                if (data_022c4590.unk18 + 1 < data_022c4560.unk20) {
                    func_ov000_023297c0(w);
                    func_ov000_023294c8(w);
                    Sound_PlaySeqArcEx(w->unk384, 0, 0x51);
                }
            }
            w->unk32C |= 0x4000000;
            if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2 && !(w->unk32C & 0x400)) {
                w->unk3CA++;
            }
        } else {
            if (w->unk32C & 0x4000000) {
                if (w->unk3B4 == 0) {
                    func_0204c66c(0);
                }
                if (data_022c4590.unk18 + 1 < data_022c4560.unk20) {
                    func_ov000_023297c0(w);
                    func_ov000_023294c8(w);
                    Sound_PlaySeqArcEx(w->unk384, 0, 0x51);
                }
                w->unk40 = data_ov000_02368ba4[w->unk324].v;
                w->unk42 = data_ov000_02368ba6[w->unk324].v;
            }
            w->unk32C &= ~0x4000000;
            w->unk3CA = 0;
        }
    } else {
        w->unk32C &= ~0x4000000;
        w->unk3CA = 0;
    }
    if ((w->unk32C & 0x4000000) && (data_022c4560.unk18 & 1) && !(data_022c4560.unk20 & 0x1f) && !(Rand_023461f0() & 3)) {
        func_ov000_0232a11c(w, 0, 0, data_ov000_0236ab3a[w->unk327].v);
    }
    if (!(w->unk32C & 0x4000000)) {
        return;
    }
    switch ((s16)(w->unk374 - w->unk3CA)) {
    case 0x78:
        func_ov000_0232a93c(w, 0, -0x18, 0);
        break;
    case 0xf0:
        func_ov000_0232a93c(w, 0, -0x18, 1);
        break;
    case 0x168:
        func_ov000_0232a93c(w, 0, -0x18, 2);
        break;
    case 0x1e0:
        func_ov000_0232a93c(w, 0, -0x18, 3);
        break;
    case 0x258:
        func_ov000_0232a93c(w, 0, -0x18, 4);
        break;
    case 0x2a8:
        func_ov000_02342a74(w, 0x17);
        break;
    case 0x2d0:
        func_ov000_0232a93c(w, 0, -0x18, 5);
        func_ov000_0232a11c(w, 0, 0, data_ov000_0236ab3a[w->unk327].v);
        break;
    }
    if (w->unk3CA <= w->unk374) {
        return;
    }
    Player_Action_Die(w);
    if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2) {
        w->unk30 = 0;
    }
    func_ov000_0232a11c(w, 0, 0, data_ov000_0236ab3a[w->unk327].v);
    func_ov000_0232a11c(w, 1, 4, data_ov000_0236ab3a[w->unk327].v);
    func_ov000_0232a11c(w, 6, 2, data_ov000_0236ab3a[w->unk327].v);
    func_ov000_0232a11c(w, -4, -6, data_ov000_0236ab3a[w->unk327].v);
}
