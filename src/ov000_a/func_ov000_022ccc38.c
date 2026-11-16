#include <nitro/types.h>

typedef struct UnkStruct_022ccc38_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u32 unk20;
    u8 pad2[0x28 - 0x24];
    u8 unk28;
    u8 pad3[0x2c - 0x29];
} UnkStruct_022ccc38_h;

typedef struct UnkStruct_022ccc38_a {
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
} UnkStruct_022ccc38_a;

typedef struct UnkStruct_022ccc38 {
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
    u8 pad2[0x21c - 0x7c];
    s32 unk21C;
    void *unk220;
    void *unk224;
    UnkStruct_022ccc38_a unk228;
} UnkStruct_022ccc38;

typedef struct UnkTask_022ccc38 {
    u8 pad[0x10];
    UnkStruct_022ccc38 *unk10;
} UnkTask_022ccc38;

typedef struct UnkStruct_022ccc38_6 {
    u8 pad[4];
    s32 unk4;
    s32 unk8;
} UnkStruct_022ccc38_6;

extern void *gZoneActArchive;
extern u8 data_ov000_0236505c[];
extern UnkStruct_022ccc38_6 data_ov000_02369ad0;

typedef struct UnkStruct_022ccc38_g {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_022ccc38_g;

extern UnkStruct_022ccc38_g *data_ov000_02369acc;

UnkTask_022ccc38 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void func_ov000_022ca184(void);
void func_ov000_022c9db8(void);
void func_ov000_022ca11c(void);
void func_ov000_022ca0a8(void);
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
void func_ov000_022fba50(UnkStruct_022ccc38 *s, s32 a, s32 x, s32 y);
void ObjRect_SetBox3D(UnkStruct_022ccc38_h *h, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjRect_SetAtkDefFlags(UnkStruct_022ccc38_h *h, s32 a, s32 b);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022ccc38 *func_ov000_022ccc38(s32 a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    UnkStruct_022ccc38 *s;
    UnkStruct_022ccc38_g *g = data_ov000_02369acc;
    UnkStruct_022ccc38_a *p;
    s = Task_Create(StageObj_Main, func_ov000_022ca184, 0, 0x501, 2, 0x360)->unk10;
    MI_CpuFill16(s, 0, 0x360);
    s->unk68 = func_ov000_022c9db8;
    func_02021310(ctx, data_ov000_0236505c, gZoneActArchive);
    if (func_ov000_022cc6d4()) {
        s->unk220 = func_02021264(ctx, 8);
        s->unk224 = func_02021264(ctx, 9);
    } else {
        s->unk220 = func_02021264(ctx, 8);
    }
    func_020212e0(ctx);
    s->unk21C = data_ov000_02369ad0.unk4;
    func_ov000_022fba50(s, a, x, y);
    s->unk70 = func_ov000_022ca11c;
    s->unk54 |= 0x12;
    s->unk58 |= 0x8300;
    s->unk5C |= 0x20;
    s->unk10 = g->unk10;
    s->unk14 = g->unk14;
    s->unk18 = g->unk18 - 0x8e00;
    p = &s->unk228;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk21C, 0);
    func_02021c38(p->unk90, func_ov000_022ca0a8, 0, 6, 3);
    p->unk48 = g->unk10 << 4;
    p->unk4C = -(g->unk14 << 4);
    p->unk50 = (g->unk18 - 0x8e00) << 4;
    p->unk18 = p->unk1C = p->unk20 = 0x34cc;
    p->unkA = 4;
    return s;
}
