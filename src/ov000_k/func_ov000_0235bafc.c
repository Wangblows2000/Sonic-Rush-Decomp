#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s16 vx;
    s16 vy;
    u8 padC[4];
    s16 timer;
    u16 flags;
    u8 pad14[4];
    u8 *out;
} UnkStruct_0235bafc_P;

typedef struct UnkStruct_0235bafc UnkStruct_0235bafc;
struct UnkStruct_0235bafc {
    u32 flags;
    void (*cb4)(UnkStruct_0235bafc *w, UnkStruct_0235bafc_P *p);
    void (*cb8)(UnkStruct_0235bafc *w, UnkStruct_0235bafc_P *p);
    s32 (*cbC)(void *a, s32 x, s32 y, s32 b, s32 c, s32 d, s32 e);
    void (*cb10)(UnkStruct_0235bafc_P *p);
    u8 spr0[0x2C];
    u32 spr0Flags;
    u8 pad44[0x5C - 0x44];
    u8 spr1[0x2C];
    u32 spr1Flags;
    u8 pad8C[0xA4 - 0x8C];
    u8 mdl[0x160 - 0xA4];
    u32 mdlFlags;
    u8 pad164[0x1A0 - 0x164];
    UnkStruct_0235bafc_P parts[75];
    u8 count;
    u8 unk9D5;
    u8 nplayers;
};

typedef struct {
    u8 pad[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0235bafc_G;

typedef struct {
    void *pad;
    void *pad4;
    void *pad8;
    void *padC;
    UnkStruct_0235bafc *work;
} UnkStruct_0235bafc_T;

typedef struct {
    s32 x;
    s32 y;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} UnkStruct_0235bafc_Rect;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_0235bafc_Src;

extern UnkStruct_0235bafc_T *gRingManagerTask;
extern s16 data_0206e470[];

void ObjRect_SetBox2D(UnkStruct_0235bafc_Rect *r, s32 a, s32 b, s32 c, s32 d);
s32 ObjRect_BoxCheck(UnkStruct_0235bafc_Rect *r, s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);
s32 FX_Atan2(s32 y, s32 x);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 b);

#define IDX_0235bafc(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0235bafc(a) data_0206e470[(u32)(IDX_0235bafc(a) << 1)]
#define COS_0235bafc(a) data_0206e470[(u32)((IDX_0235bafc(a) << 1) + 1)]

void func_ov000_0235bafc(UnkStruct_0235bafc_Src *src) {
    UnkStruct_0235bafc_Rect r = {0};
    UnkStruct_0235bafc *w;
    UnkStruct_0235bafc_P *p;
    u8 left;
    u8 i;
    s32 ang;
    s32 v;

    if (gRingManagerTask == NULL)
        return;
    w = gRingManagerTask->work;
    w->flags |= 4;
    r.x = src->x >> 8;
    r.y = src->y >> 8;
    ObjRect_SetBox2D(&r, -0x48, -0x48, 0x48, 0x48);
    left = w->count;
    for (i = 0; i < 64; i++) {
        if (left == 0)
            return;
        p = &w->parts[i];
        if (p->timer == 0)
            continue;
        left--;
        if (!(p->flags & 0x2000)) {
            if (ObjRect_BoxCheck(&r, p->x >> 8, p->y >> 8, -6, -16, 0x14, 0x10)) {
                p->flags |= 0x200;
                p->flags |= 0x400;
                p->flags |= 0x800;
                p->flags |= 0x2000;
                p->vx = 0;
            }
        }
        if (p->flags & 0x2000) {
            ang = FX_Atan2(r.y - (p->y >> 8), r.x - (p->x >> 8));
            p->vx = ObjSpd_UpSet(p->vx, 0x40, 0x1F00);
            v = p->vx;
            p->x += (s16)((v * COS_0235bafc(ang)) >> 12);
            p->y += (s16)((v * SIN_0235bafc(ang)) >> 12);
        }
    }
}
