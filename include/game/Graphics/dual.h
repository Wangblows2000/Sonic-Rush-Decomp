#ifndef MAIN_A2_DUAL_H
#define MAIN_A2_DUAL_H

#include <nitro/types.h>

typedef struct UnkSeqRes {
    u32 unk0;
    u32 tableOfs;
    u32 dataOfs;
} UnkSeqRes;

typedef struct UnkDualSub {
    u32 idx;
    s16 a;
    s16 b;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    s16 frame;
    UnkSeqRes *res;
    u8 unk14[8];
    u32 pos;
    u32 prevPos;
    u8 unk24[4];
    s16 wait;
    s16 unk2a;
    u32 flags;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    u32 unk3c;
    u32 unk40;
    u16 unk44;
    u16 unk46;
} UnkDualSub;

typedef struct UnkDual {
    UnkDualSub sub;
    struct {
        s16 a;
        s16 b;
    } ab[2];
    u32 mask;
    u32 unk54[2];
    u32 unk5c[2];
    u32 unk64[2];
    u32 unk6c[2];
    u16 unk74[2];
    u32 unk78;
} UnkDual;

static inline u32 UnkSeq_GetData(UnkSeqRes *r) {
    return (u32)r + r->dataOfs;
}

#define UNK_DUAL_SETUP(p, d, i)            \
    do {                                    \
        (p)->idx = (i);                     \
        (p)->a = (d)->ab[i].a;              \
        (p)->b = (d)->ab[i].b;              \
        (p)->unk30 = (d)->unk54[i];         \
        (p)->unk34 = (d)->unk5c[i];         \
        (p)->unk3c = (d)->unk64[i];         \
        (p)->unk40 = (d)->unk6c[i];         \
        (p)->unk44 = (d)->unk74[i];         \
    } while (0)

#endif
