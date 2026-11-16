#include <nitro/types.h>
typedef float f32;

typedef struct UnkSpr_0230a58c {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[4];
    u32 unk34;
    u8 pad38[0x46 - 0x38];
    u16 unk46;
} UnkSpr_0230a58c;

typedef struct UnkObj_0230a58c {
    u8 pad[0x40];
} UnkObj_0230a58c;

typedef struct UnkWork_0230a58c {
    u8 pad0[8];
    UnkObj_0230a58c obj[4];
    UnkSpr_0230a58c spr[23];
    u8 pad780[0xaf4 - 0x780];
    f32 a[7];
    f32 b[7];
    f32 c[7];
    u16 pal0[16];
    u16 pal1[16];
    u16 pal2[16];
    u8 padba8[0xbbc - 0xba8];
    s32 unkBBC;
    u8 padbc0[8];
    s32 unkBC8[4];
    u8 padbd8[0xbfc - 0xbd8];
    UnkSpr_0230a58c spr2[3];
    u8 padcd4[0xcec - 0xcd4];
} UnkWork_0230a58c;

typedef struct UnkAnimEntry_0230a58c {
    u8 pad0[0x12];
    u16 unk12;
} UnkAnimEntry_0230a58c;

typedef struct UnkAnimHdr_0230a58c {
    u8 pad0[8];
    u16 unk8;
    u8 padA[2];
    UnkAnimEntry_0230a58c e[1];
} UnkAnimHdr_0230a58c;

typedef struct UnkAnimRes_0230a58c {
    u8 pad0[0x18];
    u32 hdrOfs;
} UnkAnimRes_0230a58c;

typedef struct UnkResSet_0230a58c {
    u8 pad0[0xc];
    u16 *res[4];
    UnkAnimRes_0230a58c *anim;
} UnkResSet_0230a58c;

#define ANIM_HDR(r) ((UnkAnimHdr_0230a58c *)((u8 *)(r) + (r)->hdrOfs))

extern void *gMainHeap;
extern UnkWork_0230a58c *data_ov001_0231f5bc;
extern UnkResSet_0230a58c *data_ov001_0231f5c4;
extern s16 data_ov001_02317098[];
extern u8 data_ov001_0231708c[];
extern u32 data_ov001_023170a8[];
extern u8 data_ov001_023170cc[];
extern u16 data_ov001_02317114[];
extern volatile u32 data_ov001_02317144[];
extern u8 data_ov001_023170e4[];
extern u8 data_ov001_023170fc[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void Background_Init(void *buf, void *res, s32 a, u32 b, u8 c, u16 d, u16 e);
u32 ObjVram_Alloc(u32 a, u16 b);
void AnimatorSprite_Init(UnkSpr_0230a58c *s, void *res, u16 idx, s32 a, u32 b, s32 c, u32 d, s32 e, u32 pal);
void AnimatorSprite_ProcessAnimation(UnkSpr_0230a58c *s, s32 a, s32 b);
void ObjVram_Free(s32 a, u32 b);
s32 func_ov001_0230c0cc(void);

void func_ov001_0230a58c(void) {
    volatile f32 f1;
    volatile f32 f2;
    vu32 fill;
    UnkSpr_0230a58c spr;
    u32 pal;
    u32 m;
    u16 idx;
    u8 *p;
    UnkWork_0230a58c *w;
    s32 i;
    s32 n;

    p = Heap_Alloc(gMainHeap, 0xd20, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, 0xd20);
        *(u32 *)p = 0xd20;
        *(u32 *)(p + 0xd1c) = 0x02467531;
        DC_StoreRange(p, 0xd20);
        p += 0x20;
    }
    data_ov001_0231f5bc = (UnkWork_0230a58c *)p;
    MI_CpuFill8(p, 0, sizeof(UnkWork_0230a58c));
    w = data_ov001_0231f5bc;
    w->unkBC8[0] = -1;
    w->unkBC8[1] = -1;
    w->unkBC8[2] = -1;
    w->unkBC8[3] = -1;
    w->unkBBC = 1;
    f1 = 34.133335f;
    n = 512;
    f2 = (0.0f - n) / 225.0f;
    for (i = 0; i < 7; i++) {
        w->a[i] = f1;
        w->b[i] = f2;
        w->c[i] = -256.0f + data_ov001_02317098[i];
    }
    for (i = 0; i < 4; i++) {
        u16 *r = data_ov001_0231f5c4->res[i];
        Background_Init(&w->obj[i], r, 0x38, data_ov001_023170a8[i], data_ov001_0231708c[i], r[0x18 / 2], r[0x1a / 2]);
    }
    for (i = 0; i < 23; i++) {
        u16 v;
        if (data_ov001_023170cc[i] == 0)
            v = ANIM_HDR(data_ov001_0231f5c4->anim)->e[data_ov001_02317114[i]].unk12;
        else
            v = ANIM_HDR(data_ov001_0231f5c4->anim)->unk8;
        pal = data_ov001_02317144[i] == 0 ? 0x05000200 : 0x05000600;
        m = data_ov001_02317144[i];
        AnimatorSprite_Init(&w->spr[i], data_ov001_0231f5c4->anim, data_ov001_02317114[i], 0x40, m, 0, ObjVram_Alloc(m, v), 0, pal);
        w->spr[i].flags |= 0x20000;
    }
    for (i = 0; i < 23; i++) {
        AnimatorSprite_ProcessAnimation(&w->spr[i], 0, 0);
        w->spr[i].flags |= 0x10;
        w->spr[i].flags &= ~0x40;
        if (data_ov001_023170e4[i] == 0)
            w->spr[i].flags |= 8;
        if (data_ov001_023170fc[i] != 0)
            w->spr[i].flags |= 4;
        w->spr[i].unk46 = 5;
    }
    for (i = 0; i < 3; i++) {
        AnimatorSprite_Init(&w->spr2[i], data_ov001_0231f5c4->anim, 0x40, 0, 1, 0, ObjVram_Alloc(1, ANIM_HDR(data_ov001_0231f5c4->anim)->e[0x40].unk12), 0, 0x05000600);
        w->spr2[i].flags |= 4;
    }
    w->spr[2].x = 0x4a;
    w->spr[2].y = 0x74;
    w->spr[3].x = 0x5e;
    w->spr[3].y = 0x74;
    w->spr[4].x = 0x72;
    w->spr[4].y = 0x74;
    w->spr[5].x = 0xa2;
    w->spr[5].y = 0x7d;
    w->spr[6].x = 0xb6;
    w->spr[6].y = 0x7d;
    w->spr[7].x = 0xca;
    w->spr[7].y = 0x7d;
    w->spr[2].flags |= 0x200;
    w->spr[3].flags |= 0x200;
    w->spr[4].flags |= 0x200;
    w->spr[5].flags |= 0x200;
    w->spr[6].flags |= 0x200;
    w->spr[7].flags |= 0x200;
    w->spr[8].x = 0x80;
    w->spr[8].y = 0xac;
    w->spr[9].x = 0x18;
    w->spr[9].y = 0xac;
    w->spr[9].unk46 = 2;
    for (i = 0; i < 7; i++) {
        UnkSpr_0230a58c *s = &w->spr[11 + i];
        s->x = -256.0f + data_ov001_02317098[i];
        s->y = 0x54;
    }
    w->spr[20].unk46 = 3;
    w->spr[18].unk46 = 8;
    w->spr[19].unk46 = 3;
    w->spr[19].flags |= 0x200;
    AnimatorSprite_Init(&spr, data_ov001_0231f5c4->anim, 0x12, 0x48, 1, 0, ObjVram_Alloc(1, ANIM_HDR(data_ov001_0231f5c4->anim)->e[0x12].unk12), 0, (u32)w->pal0);
    AnimatorSprite_ProcessAnimation(&spr, 0, 0);
    ObjVram_Free(1, spr.unk34);
    idx = func_ov001_0230c0cc() + 0x13;
    AnimatorSprite_Init(&spr, data_ov001_0231f5c4->anim, idx, 0x48, 1, 0, ObjVram_Alloc(1, ANIM_HDR(data_ov001_0231f5c4->anim)->e[idx].unk12), 0, (u32)w->pal1);
    AnimatorSprite_ProcessAnimation(&spr, 0, 0);
    ObjVram_Free(1, spr.unk34);
    MI_CpuCopy8(w->pal0, w->pal2, 0x20);
}
