#include <nitro/types.h>

typedef struct UnkStruct_022cc8cc_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u32 unk20;
    u8 pad2[0x28 - 0x24];
    u8 unk28;
    u8 pad3[0x2c - 0x29];
} UnkStruct_022cc8cc_h;

typedef struct UnkStruct_022cc8cc_a {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
    u8 pad1[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad2[0x90 - 0x24];
    u8 unk90[4];
} UnkStruct_022cc8cc_a;

typedef struct UnkStruct_022cc8cc {
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
    u8 pad2[0x134 - 0x7c];
    UnkStruct_022cc8cc_h unk134;
    UnkStruct_022cc8cc_h unk160;
    u8 pad3[0x20c - 0x18c];
    UnkStruct_022cc8cc_h unk20C;
    UnkStruct_022cc8cc_h unk238;
    UnkStruct_022cc8cc_h unk264;
    UnkStruct_022cc8cc_h unk290;
    u8 pad4[0x300 - 0x2bc];
    s32 unk300;
    void *unk304;
    void *unk308;
    UnkStruct_022cc8cc_a unk30C;
} UnkStruct_022cc8cc;

typedef struct UnkTask_022cc8cc {
    u8 pad[0x10];
    UnkStruct_022cc8cc *unk10;
} UnkTask_022cc8cc;

typedef struct UnkStruct_022cc8cc_6 {
    u8 pad[8];
    s32 unk8;
} UnkStruct_022cc8cc_6;

extern void *gZoneActArchive;
extern u8 data_ov000_02365058[];
extern UnkStruct_022cc8cc_6 data_ov000_02369ad0;

UnkTask_022cc8cc *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void Boss1Hammer_Destructor(void);
void Boss1Hammer_State_Unk_ov000_022c697c(void);
void Boss1Hammer_Draw(void);
void Boss1Hammer_Collide(void);
void func_ov000_022c6bc0(void);
void func_ov000_022c6980(void);
void func_ov000_022c72f8(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_020212e0(void *ctx);
BOOL func_ov000_022cc6d4(void);
void MapObject_Init(UnkStruct_022cc8cc *s, s32 a, s32 x, s32 y);
void ObjRect_SetBox3D(UnkStruct_022cc8cc_h *h, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjRect_SetAtkDefFlags(UnkStruct_022cc8cc_h *h, s32 a, s32 b);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022cc8cc *Boss1Hammer_Create(s32 a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    UnkStruct_022cc8cc *s;
    UnkStruct_022cc8cc_h *h;
    UnkStruct_022cc8cc_a *p;
    s = Task_Create(StageObj_Main, Boss1Hammer_Destructor, 0, 0x502, 2, 0x444)->unk10;
    MI_CpuFill16(s, 0, 0x444);
    s->unk68 = Boss1Hammer_State_Unk_ov000_022c697c;
    func_02021310(ctx, data_ov000_02365058, gZoneActArchive);
    if (func_ov000_022cc6d4()) {
        s->unk304 = func_02021264(ctx, 8);
        s->unk308 = func_02021264(ctx, 9);
    } else {
        s->unk304 = func_02021264(ctx, 8);
    }
    func_020212e0(ctx);
    s->unk300 = data_ov000_02369ad0.unk8;
    MapObject_Init(s, a, x, y);
    s->unk70 = Boss1Hammer_Draw;
    s->unk54 |= 0x10;
    s->unk58 |= 0x8300;
    s->unk5C |= 0x20;
    s->unk78 = Boss1Hammer_Collide;
    h = &s->unk20C;
    *h = s->unk134;
    ObjRect_SetBox3D(h, -0x14, -0x30, 0x14, -0x20, -0x7d00, 0x7d00);
    h->unk18 = s;
    h->unk1C = func_ov000_022c6bc0;
    h->unkA &= ~4;
    h = &s->unk238;
    *h = s->unk160;
    ObjRect_SetBox3D(h, -0x1c, -0x78, 0x1c, 0x20, -0x7d00, 0x7d00);
    h->unk18 = s;
    h->unkA &= ~4;
    h = &s->unk264;
    *h = s->unk160;
    ObjRect_SetBox3D(h, -0x1c, -0x78, 0x1c, 0x20, -0x7d00, 0x7d00);
    h->unk20 |= 0x20;
    h->unk18 = s;
    h->unkA &= ~4;
    h = &s->unk290;
    ObjRect_SetBox3D(h, -0x1c, -0x20, 0x1c, 0x20, -0x7d00, 0x7d00);
    ObjRect_SetAtkDefFlags(h, 0, -0x10);
    h->unkA |= 0xc0;
    h->unk28 = 0x12;
    h->unk18 = s;
    h->unk1C = func_ov000_022c6980;
    h->unkA &= ~4;
    s->unk10 = 0;
    s->unk14 = 0;
    s->unk18 = 0;
    p = &s->unk30C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk300, 0);
    func_02021c38(p->unk90, func_ov000_022c72f8, 0, 6, 3);
    p->unk18 = p->unk1C = p->unk20 = 0x34cc;
    p->unk8 = 0x1e;
    p->unkA = 4;
    p->unkB = 2;
    return s;
}
