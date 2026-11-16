#include <nitro/types.h>

typedef struct UnkNode_022ede0c {
    s32 a[3];
    s32 b[3];
    s32 pad18;
    struct UnkNode_022ede0c *next;
} UnkNode_022ede0c;

typedef struct UnkStruct_022ede0c {
    u8 pad0[0x2a4];
    UnkNode_022ede0c *head;
    s32 n;
    u8 pad2ac[4];
    u16 u2b0;
    u16 u2b2;
    s32 c0;
    s32 c1;
    s32 c2;
    s32 c2c0;
    s32 c2c4;
} UnkStruct_022ede0c;

typedef struct UnkObj_022ede0c {
    u8 pad0[0x30];
    s32 x;
    s32 y;
    u8 pad38[0x1dc - 0x38];
    UnkStruct_022ede0c *s;
} UnkObj_022ede0c;

typedef struct UnkGfx_022ede0c {
    u8 pad0[0xfc];
    u32 flags;
} UnkGfx_022ede0c;

extern s32 data_ov001_0231f524;
extern s32 data_ov001_0231f520;
extern u8 data_ov001_0231d3e0[];
extern u8 data_02084df4[];
extern UnkGfx_022ede0c NNS_G3dGlb;

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);
void NNS_G3dGlbSetBaseScale(const s32 *v);
void NNS_G3dGlbSetBaseTrans(const s32 *v);
void NNS_G3dGlbFlushP(void);
void MI_Copy36B(const void *src, void *dst);
s32 FX_DivS32(s32 a, s32 b);
void Math_LerpRGB555(u16 *out, u16 a, u16 b, s32 n, s32 i);

void ExtraBossObj_Draw_Unk_ov001_022ede0c(UnkObj_022ede0c *p) {
    UnkStruct_022ede0c *s = p->s;
    UnkNode_022ede0c *cur;
    UnkNode_022ede0c *nx;
    s32 ox, oy, dx, dy;
    s32 n, mid, i, j, c;
    u16 col;
    u32 cmd;
    s32 v[12];
    s32 scale[3];
    s32 trans[3];

    if (s->n <= 0)
        return;
    ox = s->head->b[0] + data_ov001_0231f524;
    oy = s->head->b[1] + data_ov001_0231f520;
    cmd = 2;
    NNS_G3dGeBufferOP_N(0x10, &cmd, 1);
    dx = p->x - ox;
    dy = p->y - oy;
    scale[0] = 0x1000000;
    scale[1] = 0x1000000;
    scale[2] = 0x1000000;
    trans[0] = ox - 0x10000;
    trans[1] = -(oy - 0x4000);
    trans[2] = 0x1000;
    NNS_G3dGlbSetBaseScale(scale);
    MI_Copy36B(data_ov001_0231d3e0, data_02084df4);
    NNS_G3dGlb.flags &= ~0xa4;
    NNS_G3dGlbSetBaseTrans(trans);
    NNS_G3dGlbFlushP();
    cur = s->head;
    nx = cur->next;
    mid = FX_DivS32(s->n * s->c2c4, s->c2c0);
    n = s->n;
    for (i = 0; i < n; i++) {
        if (i < mid)
            c = s->c0 + FX_DivS32(i * (s->c1 - s->c0), mid);
        else
            c = s->c1 + FX_DivS32((s->c2 - s->c1) * (i - mid), n - mid);
        *(vu32 *)0x040004a4 = ((c & 0x1f) << 16) | 0xc0;
        v[0] = cur->b[0];
        v[1] = cur->b[1];
        v[2] = cur->b[2];
        v[3] = cur->a[0];
        v[4] = cur->a[1];
        v[5] = cur->a[2];
        v[6] = nx->b[0];
        v[7] = nx->b[1];
        v[8] = nx->b[2];
        v[9] = nx->a[0];
        v[10] = nx->a[1];
        v[11] = nx->a[2];
        *(vu32 *)0x04000500 = 2;
        Math_LerpRGB555(&col, s->u2b0, s->u2b2, s->n, i);
        for (j = 0; j < 4; j++) {
            *(vu32 *)0x04000480 = col;
            *(vu32 *)0x0400048c = (u16)(s16)((data_ov001_0231f524 + (dx + v[j*3])) >> 12)
                                | ((u16)(s16)-(s16)((data_ov001_0231f520 + (dy + v[j*3+1])) >> 12) << 16);
            *(vu32 *)0x0400048c = 0;
        }
        *(vu32 *)0x04000504 = 0;
        cur = nx;
        nx = nx->next;
    }
}
