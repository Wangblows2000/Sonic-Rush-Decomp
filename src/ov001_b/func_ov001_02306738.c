#include <nitro/types.h>

typedef struct UnkStruct_02306738_h {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_02306738_h;

typedef struct UnkStruct_02306738_r {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_02306738_r;

typedef struct UnkStruct_02306738_b {
    u8 pad0[0x1c];
    s32 unk1C;
    u8 pad20[0x20];
} UnkStruct_02306738_b;

typedef struct UnkStruct_02306738_o {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
    u8 pad8[0x3c];
    u16 unk44;
    u8 pad46[2];
} UnkStruct_02306738_o;

typedef struct UnkStruct_02306738 {
    u8 pad0[0x18];
    UnkStruct_02306738_b bgs[2];
    UnkStruct_02306738_o objs[8];
} UnkStruct_02306738;

typedef struct UnkStruct_02306738_xy {
    s16 x;
    s16 y;
} UnkStruct_02306738_xy;

extern void *data_0208fe80;
extern UnkStruct_02306738 *data_ov001_0231f5a8;
extern UnkStruct_02306738_r *data_ov001_0231f5b0[];
extern UnkStruct_02306738_h *data_ov001_0231f5ac;
extern u16 data_ov001_0231df2c[];
extern UnkStruct_02306738_xy data_ov001_0231df54[];
extern u8 data_02072cb8;
extern u16 *data_ov001_0231df3c[];

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_0203473c(void *a, void *b, s32 c, s32 d, s32 e, u32 f, u32 g);
u32 func_02039c18(u32 a, u32 b);
void func_0203363c(void *s, void *h, u16 idx, u32 d, u32 e, u32 f, u32 g, u32 hh, u32 i);
void func_0203344c(void *s, u32 a, u32 b);

#define ENT1E(h, idx) (*(u16 *)((u8 *)(h) + (h)->unk18 + (idx) * 0x14 + 0x1e))

void func_ov001_02306738(void) {
    vu16 zero;
    vu32 fill;
    u8 *blk;
    s32 i;
    UnkStruct_02306738_r *r;
    UnkStruct_02306738_o *o;
    u16 *p;
    u32 k;
    u32 t;

    blk = func_02034a40(data_0208fe80, 0x620, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x620);
        *(u32 *)blk = 0x620;
        *(u32 *)(blk + 0x61c) = 0x02467531;
        DC_StoreRange(blk, 0x620);
        blk += 0x20;
    }
    zero = 0;
    data_ov001_0231f5a8 = (UnkStruct_02306738 *)blk;
    MIi_CpuClear16(zero, blk, 0x5f0);
    for (i = 0; i < 2; i++) {
        r = data_ov001_0231f5b0[i];
        func_0203473c(&data_ov001_0231f5a8->bgs[i], r, 0x38, 1, 1, r->unk18, r->unk1A);
        data_ov001_0231f5a8->bgs[i].unk1C += 0x20;
    }
    for (i = 0, o = data_ov001_0231f5a8->objs; i <= 3; i++, o++) {
        t = func_02039c18(1, ENT1E(data_ov001_0231f5ac, (u16)i));
        func_0203363c(o, data_ov001_0231f5ac, (u16)i, 0x60, 1, 0, t, 0, 0x5000600);
        o->unk44 = data_ov001_0231df2c[i];
        o->unk4 = data_ov001_0231df54[i].x;
        o->unk6 = data_ov001_0231df54[i].y;
        func_0203344c(o, 0, 0);
    }
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        k = data_02072cb8;
        break;
    default:
        k = 1;
        break;
    }
    p = data_ov001_0231df3c[k];
    for (i = 4, o = &data_ov001_0231f5a8->objs[4]; i <= 7; i++, o++, p++) {
        t = func_02039c18(1, ENT1E(data_ov001_0231f5ac, *p));
        func_0203363c(o, data_ov001_0231f5ac, *p, 0x260, 1, 0, t, 0, 0x5000600);
        o->unk44 = data_ov001_0231df2c[i];
        o->unk4 = data_ov001_0231df54[i].x;
        o->unk6 = data_ov001_0231df54[i].y;
        func_0203344c(o, 0, 0);
    }
}
