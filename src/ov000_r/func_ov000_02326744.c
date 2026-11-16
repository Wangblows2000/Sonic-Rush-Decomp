#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_02326744;

typedef struct UnkNode_02326744 {
    UnkVec_02326744 a;
    UnkVec_02326744 b;
    u32 unk18;
    struct UnkNode_02326744 *next;
} UnkNode_02326744;

typedef struct {
    u8 unk0;
    u8 pad1[0x32f];
    u32 unk330;
} UnkObj_02326744;

typedef struct {
    u8 pad0[6];
    s8 unk6;
    s8 unk7;
    u8 pad8[0x12c];
    UnkObj_02326744 *unk134;
    u8 pad138[0x164];
    UnkNode_02326744 *unk29c;
    s32 unk2a0;
    u8 pad2a4[4];
    u16 unk2a8;
    u16 unk2aa;
    s32 unk2ac;
    s32 unk2b0;
    s32 unk2b4;
    s32 unk2b8;
    s32 unk2bc;
} UnkWork_02326744;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02326744 *work;
} UnkTask_02326744;

typedef struct {
    u8 pad0[0xfc];
    u32 unkfc;
} UnkStruct_02084d38;

extern UnkTask_02326744 *gTaskCurrent;
extern s8 data_022c4eec[];
extern u8 data_ov000_02367ae8[];
extern u8 data_02084df4[];
extern UnkStruct_02084d38 NNS_G3dGlb;

void Camera_WorldToScreen(void *m, s32 x, s32 y, s16 *ox, s16 *oy);
void *Camera_GetPrimary(void);
void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
void NNS_G3dGlbSetBaseScale(const fx32 *v);
void NNS_G3dGlbSetBaseTrans(const fx32 *v);
void NNS_G3dGlbFlushP(void);
void MI_Copy36B(const void *src, void *dst);
void func_020490b8(u16 *out, u16 a, u16 b, s32 n, s32 i);

#define REG_DISP3DCNT (*(vu16 *)0x04000060)
#define REG_POLYGON_ATTR (*(vu32 *)0x040004a4)
#define REG_BEGIN_VTXS (*(vu32 *)0x04000500)
#define REG_END_VTXS (*(vu32 *)0x04000504)
#define REG_COLOR (*(vu32 *)0x04000480)
#define REG_VTX_XY (*(vu32 *)0x0400048c)

static inline void VtxXY(s16 x, s16 y) {
    REG_VTX_XY = (u16)x | ((u16)y << 16);
}

void func_ov000_02326744(void) {
    s32 j;
    s32 mid;
    s32 i;
    UnkNode_02326744 *next;
    s32 dx;
    UnkWork_02326744 *w;
    s32 dy;
    s32 a;
    UnkVec_02326744 *p;
    UnkNode_02326744 *node;
    s32 rem;
    s32 cnt;

    s16 sx;
    s16 sy;
    u16 col;
    s16 v[3];
    u32 arg;
    UnkVec_02326744 pts[4];
    fx32 scale[3];
    fx32 trans[3];

    w = gTaskCurrent->work;

    if (w->unk134->unk0 == 1 && (w->unk134->unk330 & 0x20000))
        return;

    node = w->unk29c;
    Camera_WorldToScreen(Camera_GetPrimary(), node->b.x, node->b.y, &v[0], &v[1]);
    v[2] = 0;
    arg = 2;
    NNS_G3dGeBufferOP_N(0x10, &arg, 1);
    REG_DISP3DCNT &= 0xcfdf;

    dx = data_022c4eec[0] + w->unk6 - v[0];
    dy = data_022c4eec[1] + w->unk7 - v[1];
    if (w != NULL) {
        dx += w->unk6;
        dy += w->unk7;
    }

    scale[0] = 0x1000000;
    scale[1] = 0x1000000;
    scale[2] = 0x1000000;
    trans[0] = v[0] << 12;
    trans[1] = -(v[1] << 12);
    trans[2] = 0;
    NNS_G3dGlbSetBaseScale(scale);
    MI_Copy36B(data_ov000_02367ae8, data_02084df4);
    NNS_G3dGlb.unkfc &= ~0xa4;
    NNS_G3dGlbSetBaseTrans(trans);
    NNS_G3dGlbFlushP();

    node = w->unk29c;
    cnt = w->unk2a0;
    next = node->next;
    mid = cnt * w->unk2bc / w->unk2b8;
    i = 0;
    if (i < cnt) {
    rem = cnt - mid;
    do {
        if (i <= mid)
            a = w->unk2ac - i * (w->unk2ac - w->unk2b0) / mid;
        else
            a = w->unk2b0 - (w->unk2b0 - w->unk2b4) * (i - mid) / rem;
        REG_POLYGON_ATTR = ((a & 0x1f) << 16) | 0xc0;
        pts[0].x = node->b.x;
        pts[0].y = node->b.y;
        pts[0].z = node->b.z;
        pts[1].x = node->a.x;
        pts[1].y = node->a.y;
        pts[1].z = node->a.z;
        pts[2].x = next->b.x;
        pts[2].y = next->b.y;
        pts[2].z = next->b.z;
        pts[3].x = next->a.x;
        pts[3].y = next->a.y;
        pts[3].z = next->a.z;
        REG_BEGIN_VTXS = 2;
        func_020490b8(&col, w->unk2a8, w->unk2aa, w->unk2a0, i);
        p = pts;
        for (j = 0; j < 4; j++) {
            REG_COLOR = col;
            Camera_WorldToScreen(Camera_GetPrimary(), p->x, p->y, &sx, &sy);
            sx += dx;
            sy += dy;
            VtxXY(sx, -sy);
            REG_VTX_XY = 0;
            p++;
        }
        REG_END_VTXS = 0;
        node = next;
        next = next->next;
    } while (++i < cnt);
    }
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x20;
}
