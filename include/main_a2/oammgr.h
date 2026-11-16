#ifndef MAIN_A2_OAMMGR_H
#define MAIN_A2_OAMMGR_H

#include <nitro/types.h>

typedef struct UnkOam {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 attr3;
} UnkOam;

typedef struct UnkOamAffine {
    u16 p0[3];
    s16 pa;
    u16 p1[3];
    s16 pb;
    u16 p2[3];
    s16 pc;
    u16 p3[3];
    s16 pd;
} UnkOamAffine;

typedef struct UnkOamObj {
    u16 attr[3];
    u16 next;
} UnkOamObj;

typedef struct UnkOamMgr {
    struct {
        u16 x;
        u16 y;
    } ofs[0x227];
    u16 unk89c;
    u16 numObj;
    u16 numAffine;
    u16 unk8a2;
    u16 unk8a4;
    u16 unk8a6;
    union {
        UnkOam oam[128];
        UnkOamAffine affine[32];
    } buf;
    u8 unkca8[0x400];
    UnkOamObj objs[128];
    u16 heads[32];
    u16 tails[32];
} UnkOamMgr;

extern UnkOamMgr *data_02072c78[];

#endif
