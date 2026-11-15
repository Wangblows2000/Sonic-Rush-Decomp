#include <nitro/types.h>

typedef struct UnkStruct_022cce24_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u32 unk20;
    u8 pad2[0x28 - 0x24];
    u8 unk28;
    u8 pad3[0x2c - 0x29];
} UnkStruct_022cce24_h;

typedef struct UnkStruct_022cce24_a {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 pad1[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad2[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad3[0x90 - 0x54];
    u8 unk90[4];
} UnkStruct_022cce24_a;

typedef struct UnkStruct_022cce24 {
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
    u8 pad2[0x210 - 0x7c];
    void *unk210;
    void *unk214;
    u8 pad2b[0x21a - 0x218];
    u16 unk21A;
    u16 unk21C;
    u8 pad2c[0x22c - 0x21e];
    s32 unk22C;
    void *unk230;
    void *unk234;
    s32 unk238;
    UnkStruct_022cce24_a unk23C;
} UnkStruct_022cce24;

typedef struct UnkTask_022cce24 {
    u8 pad[0x10];
    UnkStruct_022cce24 *unk10;
} UnkTask_022cce24;

typedef struct UnkStruct_022cce24_6 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} UnkStruct_022cce24_6;

typedef struct UnkStruct_022cce24_7 {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_022cce24_7;

typedef struct UnkStruct_022cce24_8 {
    s32 unk0;
} UnkStruct_022cce24_8;

extern void *data_022c4488;
extern UnkStruct_022cce24_6 data_ov000_02369ad0;

extern UnkStruct_022cce24 *data_ov000_02369acc;
extern UnkStruct_022cce24_7 data_ov000_02369ac0;
extern UnkStruct_022cce24_8 data_022c4560;
extern u32 data_0208fe70;
extern u8 data_ov000_02365060[];
extern u8 data_ov000_02365064[];
extern u8 data_ov000_02365068[];
extern u8 data_ov000_0236507c[];
extern u8 data_ov000_02365098[];
extern u8 data_ov000_023650b4[];
extern u8 data_ov000_023650cc[];
extern u8 data_ov000_023650e4[];

UnkTask_022cce24 *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void func_0206c3d0(void);
void func_ov000_022cbcf4(void);
void func_ov000_022cb218(void);
void func_ov000_022cbcb4(void);
void func_ov000_022cbb7c(void);
u16 func_ov000_022c5f28(void);
void *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022cc3e8(s32 idx, s32 flag);
void *func_0202128c(void *a);
void func_020257f8(void *a);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}
void func_ov000_022c697c(void);
void func_ov000_022c736c(void);
void func_ov000_022c6db4(void);
void func_ov000_022c6bc0(void);
void func_ov000_022c6980(void);
void func_ov000_022c72f8(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_020212e0(void *ctx);
BOOL func_ov000_022cc6d4(void);
void func_ov000_022fba50(UnkStruct_022cce24 *s, s32 a, s32 x, s32 y);
void func_0206e2d8(UnkStruct_022cce24_h *h, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_0206e2b4(UnkStruct_022cce24_h *h, s32 a, s32 b);
void func_02031c1c(void *p, s32 a);
void func_02031bd0(void *p, s32 a, s32 b);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022cce24 *func_ov000_022cce24(u8 *a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    u32 ctx2[0x5c / 4];
    UnkStruct_022cce24 *s;
    UnkStruct_022cce24_a *p;
    if (a[0] == 0xff && a[1] == 0xff)
        return NULL;
    s = func_020372d0(func_0206c3d0, func_ov000_022cbcf4, 0, 0x500, 2, 0x374)->unk10;
    MI_CpuFill16(s, 0, 0x374);
    data_ov000_02369acc = s;
    s->unk68 = func_ov000_022cb218;
    s->unk21A = Rand() & 3;
    s->unk21C = 0xaaa;
    MI_CpuFill16(&data_ov000_02369ac0, 0, 0xc);
    data_ov000_02369ac0.unk4 = 1;
    data_ov000_02369ac0.unk8 = func_ov000_022c5f28();
    data_ov000_02369ac0.unkA = func_ov000_022c5f28();
    func_02021310(ctx, data_ov000_02365060, data_022c4488);
    if (func_ov000_022cc6d4()) {
        s->unk230 = func_02021264(ctx, 8);
        s->unk234 = func_02021264(ctx, 9);
    } else {
        s->unk230 = func_02021264(ctx, 8);
    }
    func_020212e0(ctx);
    s->unk22C = data_ov000_02369ad0.unk0;
    s->unk238 = data_ov000_02369ad0.unkC;
    func_ov000_022fba50(s, (s32)a, x, y);
    s->unk70 = func_ov000_022cbcb4;
    s->unk54 |= 0x12;
    s->unk58 |= 0xa300;
    s->unk5C |= 0x20;
    s->unk10 = 0xe100;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    p = &s->unk23C;
    func_02031c1c(p, 0);
    func_02031bd0(p, s->unk22C, 0);
    if (func_ov000_022cc6d4())
        func_02021c38(p->unk90, func_ov000_022cbb7c, 0, 6, 3);
    p->unk48 = s->unk10 << 4;
    p->unk4C = -(s->unk14 << 4);
    p->unk50 = s->unk18 << 4;
    p->unk18 = p->unk1C = p->unk20 = 0x34cc;
    s->unk210 = func_ov000_022fad3c(0xca, 0, 0, 0, 0, 0, 0, 0, 0);
    s->unk214 = func_ov000_022fad3c(0xcb, 0, 0, 0, 0, 0, 0, 0, 0);
    func_ov000_022cc3e8(data_022c4560.unk0, 0);
    func_02021310(ctx2, data_ov000_02365064, data_022c4488);
    func_020257f8(func_0202128c(data_ov000_02365068));
    func_020257f8(func_0202128c(data_ov000_0236507c));
    func_020257f8(func_0202128c(data_ov000_02365098));
    if (func_ov000_022cc6d4()) {
        func_020257f8(func_0202128c(data_ov000_023650b4));
        func_020257f8(func_0202128c(data_ov000_023650cc));
        func_020257f8(func_0202128c(data_ov000_023650e4));
    }
    func_020212e0(ctx2);
    return s;
}
