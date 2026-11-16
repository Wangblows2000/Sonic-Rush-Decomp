#include <nitro/types.h>

typedef struct UnkStruct_022cc6f0_a {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad2[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022cc6f0_a;

typedef struct UnkStruct_022cc6f0_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
} UnkStruct_022cc6f0_h;

typedef struct UnkStruct_022cc6f0 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u32 unk58;
    u8 pad1[0x70 - 0x5c];
    void (*unk70)(void);
    u8 pad2[0x134 - 0x74];
    UnkStruct_022cc6f0_h unk134;
    u8 pad3[0x160 - 0x134 - sizeof(UnkStruct_022cc6f0_h)];
    UnkStruct_022cc6f0_h unk160;
    u8 pad4[0x20c - 0x160 - sizeof(UnkStruct_022cc6f0_h)];
    UnkStruct_022cc6f0_a unk20C;
    u8 pad5[0x394 - 0x20c - sizeof(UnkStruct_022cc6f0_a)];
    void *unk394;
    void *unk398;
    void *unk39C;
    void *unk3A0;
    void *unk3A4;
    void *unk3A8;
} UnkStruct_022cc6f0;

typedef struct UnkTask_022cc6f0 {
    u8 pad[0x10];
    UnkStruct_022cc6f0 *unk10;
} UnkTask_022cc6f0;

extern void *gZoneActArchive;
extern u8 data_ov000_02365054[];

UnkTask_022cc6f0 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void Boss5PartD3_Destructor(void);
void Boss5PartD3_Draw(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_020212e0(void *ctx);
void MapObject_Init(UnkStruct_022cc6f0 *s, s32 a, s32 x, s32 y);
void ObjRect_SetBox2D(UnkStruct_022cc6f0_h *h, s32 a, s32 b, s32 c, s32 d);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, void *a, s32 b);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022cc6f0 *Boss5PartD3_Create(s32 a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    UnkStruct_022cc6f0 *s;
    UnkStruct_022cc6f0_a *p;
    UnkStruct_022cc6f0_h *h;
    s = Task_Create(StageObj_Main, Boss5PartD3_Destructor, 0, 0x502, 2, 0x3ac)->unk10;
    MI_CpuFill16(s, 0, 0x3ac);
    func_02021310(ctx, data_ov000_02365054, gZoneActArchive);
    s->unk394 = func_02021264(ctx, 0x3d);
    s->unk398 = func_02021264(ctx, 0x41);
    s->unk39C = func_02021264(ctx, 0x42);
    s->unk3A0 = func_02021264(ctx, 0x43);
    s->unk3A4 = func_02021264(ctx, 0x44);
    s->unk3A8 = func_02021264(ctx, 0x45);
    func_020212e0(ctx);
    MapObject_Init(s, a, x, y);
    s->unk70 = Boss5PartD3_Draw;
    s->unk54 |= 0x10;
    s->unk58 |= 0x3300;
    s->unk10 = x;
    s->unk14 = y;
    s->unk18 = 0;
    h = &s->unk134;
    h->unkA &= ~4;
    h = &s->unk160;
    ObjRect_SetBox2D(h, -0x10, -0x20, 0x10, 0);
    h->unk18 = s;
    h->unkA &= ~4;
    p = &s->unk20C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk394, 0);
    p->unk48 = s->unk10 << 4;
    p->unk4C = -(s->unk14 << 4);
    p->unk50 = s->unk18 << 4;
    p->unk18 = p->unk1C = p->unk20 = 0x34cc;
    return s;
}
