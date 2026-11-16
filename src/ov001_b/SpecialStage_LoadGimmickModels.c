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

extern void *gMainHeap;
extern UnkStruct_0230fae0 *data_ov001_0231f658;
extern UnkStruct_0230fae0_o *data_ov001_0231f670[];
extern UnkStruct_0230fae0_a *data_ov001_0231f65c[];
extern u8 data_ov001_0231ee78[];
extern u8 data_ov001_0231ee88[];
extern void *gSpecialStageGimmickModelList[][5];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void *File_Load(void *p, s32 v);
u32 VramTex_Alloc(u32 a, u32 b);
u32 VramPltt_Alloc(u32 a, u32 b);
void Model_InitSprite3D(void *dst, void *h, u32 id, u32 a, u32 b, u32 c, u32 d, u32 e);
void Model_Animate(void *p);
s32 SpecialStage_GetStageIndex(void);
void func_020257f8(void *p);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
u32 func_02025e8c(void *h);

#define ENT(h, e, off) ((u8 *)(h) + (h)->unk18 + (e) * 0x14 + (off))

#define Setup(g, o, ent, dst, id, e) \
    o = dst; \
    ent = (u8 *)g->unk0 + g->unk0->unk18; \
    a = VramTex_Alloc(*(u32 *)(ent + (e) * 0x14 + 0x18), 0); \
    Model_InitSprite3D(o, g->unk0, id, 0, a, VramPltt_Alloc(*(u16 *)(ent + (e) * 0x14 + 0x16), 0), 0, 0)

void SpecialStage_LoadGimmickModels(void) {
    vu32 fill;
    u8 *blk;
    UnkStruct_0230fae0 *g;
    s32 i;
    u8 *X;
    UnkStruct_0230fae0_o *po;
    u8 *Y;
    u32 a;

    blk = Heap_Alloc(gMainHeap, 0xf00, 0x20);
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
    g->unkEC4 = File_Load(data_ov001_0231ee78, -1);
    g->unk0 = File_Load(data_ov001_0231ee88, -1);
    Setup(g, X, Y, (u8 *)&g->objs[0], 0, 1);
    Setup(g, Y, X, (u8 *)&g->objs[1], 2, 3);
    Setup(g, Y, X, (u8 *)&g->objs[5], 7, 0);
    Setup(g, Y, X, (u8 *)&g->objs[6], 8, 0);
    Setup(g, Y, X, (u8 *)&g->objs[3], 4, 0);
    Setup(g, Y, X, (u8 *)&g->objs[4], 3, 0);
    Setup(g, Y, X, (u8 *)&g->objs[7], 5, 0);
    Setup(g, Y, X, (u8 *)&g->objs[8], 6, 0);
    Y = (u8 *)g->unk0 + g->unk0->unk18;
    a = VramTex_Alloc(*(u32 *)(Y + 0x18), 0);
    Model_InitSprite3D(&g->objs[2], g->unk0, 1, 0, a, VramPltt_Alloc(*(u16 *)(Y + 0x16), 0), 0, 0);
    for (i = 0, po = g->objs; i < 9; i++) {
        Model_Animate(po);
        po->unkBC |= 0x14;
        po->unkDC |= 0x8000;
        po++;
    }
    for (i = 0; i < 5; i++) {
        g->unk898[i] = File_Load(gSpecialStageGimmickModelList[SpecialStage_GetStageIndex()][i], -1);
        func_020257f8(g->unk898[i]);
        Model_Init(&g->anims[i], 0);
        Model_SetResource(&g->anims[i], g->unk898[i], 0);
    }
    Model_SetAnimation(&g->anims[3], 2, g->unkEC4, 0, func_02025e8c(g->unk898[3]));
    g->anims[3].unk10E |= 4;
}
