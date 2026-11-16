#include <nitro/types.h>

typedef struct UnkStruct_022db148_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
    void (*unk1C)(void);
    u8 pad2[0x28 - 0x20];
    u8 unk28;
    u8 pad3[0x2c - 0x29];
} UnkStruct_022db148_h;

typedef struct UnkStruct_022db148_a {
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
    u8 pad2[0x90 - 0x54];
    u8 unk90[0x16c - 0x90];
    s32 unk16C;
    void *unk170;
} UnkStruct_022db148_a;

typedef struct UnkStruct_022db148 {
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
    UnkStruct_022db148_h unk134;
    UnkStruct_022db148_h unk160;
    u8 pad3[0x2c0 - 0x18c];
    u32 *unk2C0;
    UnkStruct_022db148_a unk2C4;
} UnkStruct_022db148;

typedef struct UnkTask_022db148 {
    u8 pad[0x10];
    UnkStruct_022db148 *unk10;
} UnkTask_022db148;

typedef struct UnkStruct_022db148_b {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_022db148_b;

extern UnkStruct_022db148_b *data_ov000_02369b14;
extern void *gZoneActArchive;
extern u8 data_ov000_02365548[];
extern s32 data_ov000_02369b18[];

UnkTask_022db148 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void StageObj_Main(void);
void Boss3PartCC_Destructor(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void Boss3PartCC_State_Unk_ov000_022d8c60(void);
u32 *Sound_AllocHandle(void);
void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_020212e0(void *ctx);
void MapObject_Init(UnkStruct_022db148 *s, void *a, s32 x, s32 y);
void Boss3PartCC_Draw(void);
void ObjRect_SetBox2D(UnkStruct_022db148_h *h, s32 a, s32 b, s32 c, s32 d);
void ObjRect_SetAtkDefFlags(UnkStruct_022db148_h *h, s32 a, s32 b);
void Boss3PartCC_OnHit(void);
void func_ov000_022d91bc(void);
void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

UnkStruct_022db148 *Boss3PartCC_Create(void *a, s32 x, s32 y) {
    u32 ctx[0x5c / 4];
    UnkStruct_022db148 *s;
    UnkStruct_022db148_b *b = data_ov000_02369b14;
    UnkStruct_022db148_h *h;
    UnkStruct_022db148_a *p;
    s = Task_Create(StageObj_Main, Boss3PartCC_Destructor, 0, 0x501, 2, 0x438)->unk10;
    MI_CpuFill16(s, 0, 0x438);
    s->unk68 = Boss3PartCC_State_Unk_ov000_022d8c60;
    s->unk2C0 = Sound_AllocHandle();
    func_02021310(ctx, data_ov000_02365548, gZoneActArchive);
    s->unk2C4.unk170 = func_02021264(ctx, 8);
    func_020212e0(ctx);
    s->unk2C4.unk16C = data_ov000_02369b18[1];
    MapObject_Init(s, a, x, y);
    s->unk70 = Boss3PartCC_Draw;
    s->unk54 |= 0x10;
    s->unk58 |= 0x8300;
    s->unk5C |= 0x20;
    s->unk124 = &s->unk2C4;
    s->unk10 = x;
    s->unk14 = b->unk14;
    s->unk18 = b->unk18;
    h = &s->unk134;
    ObjRect_SetBox2D(h, -0x18, -0x20, -0x20, 0);
    ObjRect_SetAtkDefFlags(h, 0x13, 8);
    h->unk28 = 0x33;
    h->unk18 = s;
    h->unk1C = Boss3PartCC_OnHit;
    h->unkA |= 0xc4;
    h = &s->unk160;
    ObjRect_SetBox2D(h, -0x18, -0x60, 0x20, 0);
    h->unk18 = s;
    h->unkA |= 4;
    p = &s->unk2C4;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk2C4.unk16C, 0);
    func_02021c38(p->unk90, func_ov000_022d91bc, 0, 6, 3);
    p->unk48 = b->unk10 << 4;
    p->unk4C = -(b->unk14 << 4);
    p->unk50 = b->unk18 << 4;
    p->unk20 = 0x34cc;
    p->unk1C = p->unk20;
    p->unk18 = p->unk1C;
    p->unkA = 4;
    return s;
}
