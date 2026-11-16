#include <nitro/types.h>

typedef struct UnkStruct_022f9eb4_v {
    s32 x, y, z;
} UnkStruct_022f9eb4_v;

typedef struct UnkStruct_022f9eb4_e {
    u8 pad[0x1e];
    u16 unk1E;
    u8 pad20[0x14 - 0x20 + 0x0c];
} UnkStruct_022f9eb4_e_unused;

typedef struct UnkStruct_022f9eb4_h {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022f9eb4_h;

typedef struct UnkStruct_022f9eb4_o {
    u8 pad[0x44];
    u16 unk44;
    u8 pad46[2];
} UnkStruct_022f9eb4_o;

typedef struct UnkStruct_022f9eb4_a {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x114 - 0x24];
    s32 unk114;
} UnkStruct_022f9eb4_a;

typedef struct UnkStruct_022f9eb4_r {
    u8 pad[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_022f9eb4_r;

typedef struct UnkStruct_022f9eb4_b {
    UnkStruct_022f9eb4_h *unk0;
    UnkStruct_022f9eb4_r *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    void *unk14;
    UnkStruct_022f9eb4_o objs[0x27];
    u8 padB10[0xb3c - (0x18 + 0x48 * 0x27)];
    u32 unkB3C;
    u8 padB40[0xe08 - 0xb40];
    void *unkE08;
    u8 padE0C[0xe24 - 0xe0c];
    s32 unkE24;
    s32 unkE28;
    s32 unkE2C;
    s32 unkE30;
} UnkStruct_022f9eb4_b;

typedef struct UnkStruct_022f9eb4_d38 {
    u8 pad[0x4c];
    u8 unk4C[0xfc - 0x4c];
    u32 unkFC;
    u8 pad100[0x240 - 0x100];
    UnkStruct_022f9eb4_v pos;
    UnkStruct_022f9eb4_v target;
    UnkStruct_022f9eb4_v up;
} UnkStruct_022f9eb4_d38;

extern UnkStruct_022f9eb4_v data_ov001_0231d544;
extern UnkStruct_022f9eb4_v data_ov001_0231d538;
extern u8 data_02084d40[];
extern s16 data_0206e470[];
extern UnkStruct_022f9eb4_d38 NNS_G3dGlb;
extern UnkStruct_022f9eb4_v data_02084f78;
extern UnkStruct_022f9eb4_v data_02084f84;
extern UnkStruct_022f9eb4_v data_02084f90;
extern u8 data_02084d84[];
extern void *gMainHeap;
extern UnkStruct_022f9eb4_b *data_ov001_0231f084;
extern s32 data_ov001_0231f08c;
extern s32 data_ov001_0231f090;
extern u16 data_ov001_02316afc[];
extern u16 data_ov001_02316b4c[];
extern u16 data_ov001_02316a94[];
extern u16 data_ov001_02316a88[];

void ExtraAct_InitDisplay(void);
void G3i_PerspectiveW_(s32 s, s32 c, s32 a, s32 n, s32 f, s32 g, s32 h, void *m);
void G3i_LookAt_(void *a, void *b, void *c, s32 d, void *e);
void func_02022270(u32 index, u32 value);
void func_02022284(u32 idx, s32 x, s32 y, s32 z);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
s32 func_ov001_022f73cc(s32 a);
void NNS_SndHandleInit(void *p);
void ExtraAct_LoadAssets(void);
u32 ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(void *s, void *h, u16 idx, u32 d, u32 e, u32 f, u32 g, u32 hh, u32 i);
void AnimatorSprite_ProcessAnimation(void *s, u32 a, u32 b);
s32 func_020480a0(s32 a);
void Model_Init(void *s, u32 a);
void Model_SetResource(void *s, void *res, u32 idx);
u32 NNS_G3dGetTex(void *h);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void Background_Init(void *a, void *b, s32 c, s32 d, s32 e, u32 f, u32 g);
void Background_Draw(void *a);
void ExtraAct_Main(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, void *e);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

typedef struct UnkStruct_022f9eb4_ent {
    u8 pad[0x1e];
    u16 unk1E;
} UnkStruct_022f9eb4_ent;
typedef struct { u8 pad[0x12]; u16 unk12; } E14;
typedef struct { u8 hdr[0xc]; E14 e[1]; } T14;
static inline u16 GetEnt1E(UnkStruct_022f9eb4_h *h, u16 idx) {
    T14 *t = (T14 *)((u8 *)h + h->unk18); return t->e[idx].unk12;
}
#define ENTRY1E(h, idx) GetEnt1E(h, idx)

void ExtraAct_Start(void) {
    vu16 zero;
    vu32 f1;
    vu32 f2;
    vu32 f3;
    vu32 f4;
    vu32 fill;
    s32 i;
    UnkStruct_022f9eb4_h *h;
    UnkStruct_022f9eb4_a *an;
    u8 *blk;
    UnkStruct_022f9eb4_o *o;
    u32 r;
    u16 idx;

    ExtraAct_InitDisplay();
    {
    UnkStruct_022f9eb4_v a = data_ov001_0231d544;
    UnkStruct_022f9eb4_v b = {0, 0, 0};
    UnkStruct_022f9eb4_v c = data_ov001_0231d538;
    UnkStruct_022f9eb4_o loc;
    G3i_PerspectiveW_(SIN(0x1555), COS(0x1555), 0x1555, 0x1000, 0x800000, 0x1000, 0, data_02084d40);
    NNS_G3dGlb.unkFC &= ~0x50;
    NNS_G3dGlb.pos = a;
    NNS_G3dGlb.target = c;
    NNS_G3dGlb.up = b;
    G3i_LookAt_(&a, &c, &b, 0, NNS_G3dGlb.unk4C);
    NNS_G3dGlb.unkFC &= ~0xe8;
    func_02022270(0, 0x7fff);
    func_02022270(1, 0x4631);
    func_02022270(2, 0x2108);
    func_02022284(0, 0x4a0, -0x4a0, 0);
    func_02022284(1, 0, -0x617, -0xecd);
    func_02022284(2, 0xec7, 0x620, 0);
    blk = Heap_Alloc(gMainHeap, 0xee0, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0xee0);
        *(u32 *)blk = 0xee0;
        *(u32 *)(blk + 0xedc) = 0x02467531;
        DC_StoreRange(blk, 0xee0);
        blk += 0x20;
    }
    {
        zero = 0;
        data_ov001_0231f084 = (UnkStruct_022f9eb4_b *)blk;
        MIi_CpuClear16(zero, blk, 0xea4);
    }
    data_ov001_0231f084->unkE24 = func_ov001_022f73cc(data_ov001_0231f08c);
    data_ov001_0231f084->unkE28 = data_ov001_0231f090 * 100;
    data_ov001_0231f084->unkE2C = data_ov001_0231f084->unkE24 + data_ov001_0231f084->unkE28;
    if (data_ov001_0231f084->unkE2C > 999999)
        data_ov001_0231f084->unkE2C = 999999;
    NNS_SndHandleInit((u8 *)data_ov001_0231f084 + 0xea0);
    ExtraAct_LoadAssets();

    h = data_ov001_0231f084->unk0;
    o = data_ov001_0231f084->objs;
    for (i = 0; i < 0x27; i++, o++) {
        idx = data_ov001_02316afc[i];
        r = ObjVram_Alloc(0, ENTRY1E(h, idx));
        AnimatorSprite_Init(o, h, idx, 0x30, 0, 0, r, 0, 0x5000200);
        o->unk44 = data_ov001_02316b4c[i];
        AnimatorSprite_ProcessAnimation(o, 0, 0);
    }
    data_ov001_0231f084->unkE30 = func_020480a0(data_ov001_0231f084->unkE2C);
    o = (UnkStruct_022f9eb4_o *)((u8 *)data_ov001_0231f084 + 0xb10);
    r = ObjVram_Alloc(0, ENTRY1E(h, (u16)(data_ov001_0231f084->unkE30 + 0x23)));
    AnimatorSprite_Init(o, h, (u16)(data_ov001_0231f084->unkE30 + 0x23), 0x60, 0, 0, r, 0, 0x5000200);
    o->unk44 = 3;
    AnimatorSprite_ProcessAnimation(o, 0, 0);
    for (i = 0; i < 5; i++) {
        AnimatorSprite_Init(&loc, h, data_ov001_02316a94[i], 0x48, 0, 0, 0, 0, 0x5000200);
        loc.unk44 = data_ov001_02316a88[i];
        AnimatorSprite_ProcessAnimation(&loc, 0, 0);
    }
    data_ov001_0231f084->unkB3C |= 0x200;

    an = (UnkStruct_022f9eb4_a *)((u8 *)data_ov001_0231f084 + 0xc90);
    Model_Init(an, 0);
    Model_SetResource(an, data_ov001_0231f084->unkC, 0);
    an->unk18 = an->unk1C = an->unk20 = 0x1000;
    r = NNS_G3dGetTex(data_ov001_0231f084->unkC);
    Model_SetAnimation(an, 0, data_ov001_0231f084->unk10, 0, r);
    Model_SetAnimation(an, 1, data_ov001_0231f084->unk14, 0, r);
    an->unk114 = 0x1000;

    an = (UnkStruct_022f9eb4_a *)((u8 *)data_ov001_0231f084 + 0xb58);
    Model_Init(an, 0);
    Model_SetResource(an, data_ov001_0231f084->unk8, 0);
    an->unk18 = an->unk1C = an->unk20 = 0x1000;

    Background_Init((u8 *)data_ov001_0231f084 + 0xdc8, data_ov001_0231f084->unk4, 0xe, 0, 1,
                  data_ov001_0231f084->unk4->unk18, data_ov001_0231f084->unk4->unk1A);
    Background_Draw((u8 *)data_ov001_0231f084 + 0xdc8);
    {
        f1 = 0x02020202;
        MIi_CpuClearFast(f1, (void *)0x6004000, 0x40);
    }
    {
        f2 = 0x12121212;
        MIi_CpuClearFast(f2, (void *)0x6004040, 0x40);
    }
    {
        f3 = 0;
        MIi_CpuClearFast(f3, (void *)0x6000800, 0x300);
    }
    {
        f4 = 0x10001;
        MIi_CpuClearFast(f4, (void *)0x6000b00, 0x300);
    }
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1100;
    data_ov001_0231f084->unkE08 = Task_Create(ExtraAct_Main, 0, 0, 0x1000, 0, 0);
    BrightnessLerp_Create(0x10, 0x10, 0, 3, (u8 *)data_ov001_0231f084 + 0xe3c);
    }
}
