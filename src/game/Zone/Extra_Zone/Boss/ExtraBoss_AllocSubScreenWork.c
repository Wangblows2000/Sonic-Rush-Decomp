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

extern void *gMainHeap;
extern UnkStruct_02306738 *data_ov001_0231f5a8;
extern UnkStruct_02306738_r *data_ov001_0231f5b0[];
extern UnkStruct_02306738_h *data_ov001_0231f5ac;
extern u16 data_ov001_0231df2c[];
extern UnkStruct_02306738_xy data_ov001_0231df54[];
extern u8 gGameLanguage;
extern u16 *data_ov001_0231df3c[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Background_Init(void *a, void *b, s32 c, s32 d, s32 e, u32 f, u32 g);
u32 ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(void *s, void *h, u16 idx, u32 d, u32 e, u32 f, u32 g, u32 hh, u32 i);
void AnimatorSprite_ProcessAnimation(void *s, u32 a, u32 b);

#define ENT1E(h, idx) (*(u16 *)((u8 *)(h) + (h)->unk18 + (idx) * 0x14 + 0x1e))

void ExtraBoss_AllocSubScreenWork(void) {
    vu16 zero;
    vu32 fill;
    u8 *blk;
    s32 i;
    UnkStruct_02306738_r *r;
    UnkStruct_02306738_o *o;
    u16 *p;
    u32 k;
    u32 t;

    blk = Heap_Alloc(gMainHeap, 0x620, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
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
        Background_Init(&data_ov001_0231f5a8->bgs[i], r, 0x38, 1, 1, r->unk18, r->unk1A);
        data_ov001_0231f5a8->bgs[i].unk1C += 0x20;
    }
    for (i = 0, o = data_ov001_0231f5a8->objs; i <= 3; i++, o++) {
        t = ObjVram_Alloc(1, ENT1E(data_ov001_0231f5ac, (u16)i));
        AnimatorSprite_Init(o, data_ov001_0231f5ac, (u16)i, 0x60, 1, 0, t, 0, 0x5000600);
        o->unk44 = data_ov001_0231df2c[i];
        o->unk4 = data_ov001_0231df54[i].x;
        o->unk6 = data_ov001_0231df54[i].y;
        AnimatorSprite_ProcessAnimation(o, 0, 0);
    }
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        k = gGameLanguage;
        break;
    default:
        k = 1;
        break;
    }
    p = data_ov001_0231df3c[k];
    for (i = 4, o = &data_ov001_0231f5a8->objs[4]; i <= 7; i++, o++, p++) {
        t = ObjVram_Alloc(1, ENT1E(data_ov001_0231f5ac, *p));
        AnimatorSprite_Init(o, data_ov001_0231f5ac, *p, 0x260, 1, 0, t, 0, 0x5000600);
        o->unk44 = data_ov001_0231df2c[i];
        o->unk4 = data_ov001_0231df54[i].x;
        o->unk6 = data_ov001_0231df54[i].y;
        AnimatorSprite_ProcessAnimation(o, 0, 0);
    }
}
