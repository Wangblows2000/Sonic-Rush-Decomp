#include <nitro/types.h>

typedef struct UnkStruct_022dae88_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u8 pad2[0x2c - 0x20];
} UnkStruct_022dae88_h;

typedef struct UnkStruct_022dae88_a {
    u8 pad[0xa];
    u8 unkA;
    u8 pad0[0x18 - 0xb];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad1[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad2[0x16c - 0x54];
    s32 unk16C;
    void *unk170;
} UnkStruct_022dae88_a;

typedef struct UnkStruct_022dae88 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*unk68)(void);
    u8 pad1b[0x70 - 0x6c];
    void (*unk70)(void);
    u8 pad1c[0x78 - 0x74];
    void (*unk78)(void);
    u8 pad2[0x124 - 0x7c];
    void *unk124;
    u8 pad2b[0x134 - 0x128];
    UnkStruct_022dae88_h unk134;
    UnkStruct_022dae88_h unk160;
    u8 pad3[0x212 - 0x18c];
    s16 unk212;
    u8 pad4[0x290 - 0x214];
    UnkStruct_022dae88_a unk290;
    UnkStruct_022dae88_a unk404;
    s32 unk578;
} UnkStruct_022dae88;

typedef struct UnkTask_022dae88 {
    u8 pad[0x10];
    UnkStruct_022dae88 *unk10;
} UnkTask_022dae88;

typedef struct UnkStruct_022dae88_b {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_022dae88_b;

extern UnkStruct_022dae88_b *data_ov000_02369b14;
extern void *data_022c4488;
extern u8 data_ov000_02365544[];
extern s32 data_ov000_02369b18[];

UnkTask_022dae88 *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void func_0206c3d0(void);
void func_ov000_022d6380(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov000_022d5d10(void);
void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_020212e0(void *ctx);
void func_ov000_022fba50(UnkStruct_022dae88 *s, void *a, s32 x, s32 y);
void func_ov000_022d602c(void);
void func_ov000_022d5f38(void);
void func_0206e3c0(UnkStruct_022dae88_h *h, s32 a, s32 b, s32 c, s32 d);
void func_0206e2b4(UnkStruct_022dae88_h *h, s32 a, s32 b);
void func_ov000_022d5d78(void);
void func_02031c1c(void *p, s32 a);
void func_02031bd0(void *p, s32 a, s32 b);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022dae88 *func_ov000_022dae88(void *a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    UnkStruct_022dae88 *s;
    UnkStruct_022dae88_b *b = data_ov000_02369b14;
    UnkStruct_022dae88_h *h;
    UnkStruct_022dae88_a *p;
    s = func_020372d0(func_0206c3d0, func_ov000_022d6380, 0, 0x502, 2, 0x578)->unk10;
    MI_CpuFill16(s, 0, 0x578);
    s->unk68 = func_ov000_022d5d10;
    s->unk212 = 0x1000;
    func_02021310(ctx, data_ov000_02365544, data_022c4488);
    s->unk290.unk170 = func_02021264(ctx, 8);
    s->unk404.unk170 = func_02021264(ctx, 8);
    func_020212e0(ctx);
    s->unk290.unk16C = data_ov000_02369b18[2];
    s->unk404.unk16C = data_ov000_02369b18[3];
    func_ov000_022fba50(s, a, x, y);
    s->unk70 = func_ov000_022d602c;
    s->unk54 |= 0x10;
    s->unk58 |= 0x8300;
    s->unk5C |= 0x20;
    s->unk124 = &s->unk290;
    s->unk78 = func_ov000_022d5f38;
    s->unk10 = x;
    s->unk14 = b->unk14 - 0x2000;
    s->unk18 = b->unk18;
    h = &s->unk134;
    func_0206e3c0(h, -0x20, -0x20, 0x20, 0x20);
    func_0206e2b4(h, 0x11, 0);
    h->unk18 = s;
    h->unk1C = func_ov000_022d5d78;
    h->unkA &= ~4;
    h->unkA |= 0x80;
    h = &s->unk160;
    func_0206e3c0(h, -0x20, -0x20, 0x20, 0x20);
    h->unk18 = s;
    h->unkA &= ~4;
    p = &s->unk290;
    func_02031c1c(p, 0);
    func_02031bd0(p, s->unk290.unk16C, 0);
    p->unk48 = b->unk10 << 4;
    p->unk4C = -(b->unk14 << 4);
    p->unk50 = b->unk18 << 4;
    p->unk20 = 0x34cc;
    p->unk1C = p->unk20;
    p->unk18 = p->unk1C;
    p->unkA = 4;
    p = &s->unk404;
    func_02031c1c(p, 0);
    func_02031bd0(p, s->unk404.unk16C, 0);
    p->unk48 = b->unk10 << 4;
    p->unk4C = -(b->unk14 << 4);
    p->unk50 = b->unk18 << 4;
    p->unk20 = 0x34cc;
    p->unk1C = p->unk20;
    p->unk18 = p->unk1C;
    p->unkA = 4;
    return s;
}
