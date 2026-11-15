#include <nitro/types.h>

typedef struct UnkStruct_0230fae0_h {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_0230fae0_h;

typedef struct UnkStruct_0230fae0_o {
    u8 pad0[0xbc];
    u32 unkBC;
    u8 padC0[0xdc - 0xc0];
    u32 unkDC;
    u8 padE0[0xf4 - 0xe0];
} UnkStruct_0230fae0_o;

typedef struct UnkStruct_0230fae0_a {
    u8 pad0[0x10e];
    u8 unk10E;
    u8 pad10F[0x138 - 0x10f];
} UnkStruct_0230fae0_a;

typedef struct UnkStruct_0230fae0 {
    UnkStruct_0230fae0_h *unk0;
    UnkStruct_0230fae0_o objs[9];
    void *unk898[5];
    UnkStruct_0230fae0_a anims[5];
    void *unkEC4;
} UnkStruct_0230fae0;

extern void *data_0208fe80;
extern UnkStruct_0230fae0 *data_ov001_0231f658;
extern UnkStruct_0230fae0_o *data_ov001_0231f670[];
extern UnkStruct_0230fae0_a *data_ov001_0231f65c[];
extern u8 data_ov001_0231ee78[];
extern u8 data_ov001_0231ee88[];
extern void *data_ov001_0231edec[][5];

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void *func_02033fbc(void *p, s32 v);
u32 func_0203969c(u32 a, u32 b);
u32 func_02039428(u32 a, u32 b);
void func_02031878(void *dst, void *h, u32 id, u32 a, u32 b, u32 c, u32 d, u32 e);
void func_020316f4(void *p);
s32 func_ov001_0230c0cc(void);
void func_020257f8(void *p);
void func_02031c1c(void *s, u32 a);
void func_02031bd0(void *s, void *res, u32 idx);
void func_02031a68(void *p, s32 a, void *b, s32 c, u32 d);
u32 func_02025e8c(void *h);

#define ENT(h, e, off) ((u8 *)(h) + (h)->unk18 + (e) * 0x14 + (off))

#define Setup(g, o, ent, dst, id, e) \
    o = dst; \
    ent = (u8 *)g->unk0 + g->unk0->unk18; \
    a = func_0203969c(*(u32 *)(ent + (e) * 0x14 + 0x18), 0); \
    func_02031878(o, g->unk0, id, 0, a, func_02039428(*(u16 *)(ent + (e) * 0x14 + 0x16), 0), 0, 0)

void func_ov001_0230fae0(void) {
    vu32 fill;
    u8 *blk;
    UnkStruct_0230fae0 *g;
    s32 i;
    u8 *X;
    UnkStruct_0230fae0_o *po;
    u8 *Y;
    u32 a;

    blk = func_02034a40(data_0208fe80, 0xf00, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0xf00);
        *(u32 *)blk = 0xf00;
        *(u32 *)(blk + 0xefc) = 0x02467531;
        DC_StoreRange(blk, 0xf00);
        blk += 0x20;
    }
    data_ov001_0231f658 = (UnkStruct_0230fae0 *)blk;
    MI_CpuFill8(blk, 0, 0xec8);
    g = data_ov001_0231f658;
    MI_CpuFill8(g, 0, 0xec8);
    for (i = 0; i < 9; i++)
        data_ov001_0231f670[i] = &g->objs[i];
    for (i = 0; i < 5; i++)
        data_ov001_0231f65c[i] = &g->anims[i];
    g->unkEC4 = func_02033fbc(data_ov001_0231ee78, -1);
    g->unk0 = func_02033fbc(data_ov001_0231ee88, -1);
    Setup(g, X, Y, (u8 *)&g->objs[0], 0, 1);
    Setup(g, Y, X, (u8 *)&g->objs[1], 2, 3);
    Setup(g, Y, X, (u8 *)&g->objs[5], 7, 0);
    Setup(g, Y, X, (u8 *)&g->objs[6], 8, 0);
    Setup(g, Y, X, (u8 *)&g->objs[3], 4, 0);
    Setup(g, Y, X, (u8 *)&g->objs[4], 3, 0);
    Setup(g, Y, X, (u8 *)&g->objs[7], 5, 0);
    Setup(g, Y, X, (u8 *)&g->objs[8], 6, 0);
    Y = (u8 *)g->unk0 + g->unk0->unk18;
    a = func_0203969c(*(u32 *)(Y + 0x18), 0);
    func_02031878(&g->objs[2], g->unk0, 1, 0, a, func_02039428(*(u16 *)(Y + 0x16), 0), 0, 0);
    for (i = 0, po = g->objs; i < 9; i++) {
        func_020316f4(po);
        po->unkBC |= 0x14;
        po->unkDC |= 0x8000;
        po++;
    }
    for (i = 0; i < 5; i++) {
        g->unk898[i] = func_02033fbc(data_ov001_0231edec[func_ov001_0230c0cc()][i], -1);
        func_020257f8(g->unk898[i]);
        func_02031c1c(&g->anims[i], 0);
        func_02031bd0(&g->anims[i], g->unk898[i], 0);
    }
    func_02031a68(&g->anims[3], 2, g->unkEC4, 0, func_02025e8c(g->unk898[3]));
    g->anims[3].unk10E |= 4;
}
