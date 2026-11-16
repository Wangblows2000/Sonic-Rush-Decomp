#include <nitro/types.h>

typedef struct {
    u16 w;
    u16 h;
    u16 data[1];
} UnkStruct_0233b684_M;

typedef struct {
    u32 flags;
    s32 x;
    s32 y;
    u8 padC[0x18 - 0xC];
    s16 ofsX;
    s16 ofsY;
    u8 pad1C[2];
    s16 scaleX;
    s16 scaleY;
    u16 angle;
    u8 *chr0;
    u8 *chr1;
    u8 pad2C[0x64 - 0x2C];
} UnkStruct_0233b684_L;

typedef struct {
    UnkStruct_0233b684_L layer[2];
    u8 padC8[0x10E - 0xC8];
    u16 width;
    u16 height;
    u8 pad112[0x130 - 0x112];
    u8 *tiles;
    UnkStruct_0233b684_M *map0;
    UnkStruct_0233b684_M *map1;
} UnkStruct_0233b684;

typedef struct {
    fx32 m[4];
} UnkMtx22_0233b684;

extern UnkStruct_0233b684 gCamera;
extern s16 data_0206e470[];
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];

void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void ScreenTransfer_QueueRectRaw(void *data, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h);
void MTX_Rot22_(UnkMtx22_0233b684 *m, s32 sin, s32 cos);
void MTX_ScaleApply22(const UnkMtx22_0233b684 *src, UnkMtx22_0233b684 *dst, fx32 x, fx32 y);
fx32 func_0200225c(fx32 a, fx32 b);
void G2x_SetBGyAffine_(void *p, UnkMtx22_0233b684 *m, s32 cx, s32 cy, s32 x1, s32 y1);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov000_0233b684(void) {
    UnkStruct_0233b684 *g = &gCamera;
    u32 i;
    u32 j;
    u8 *chr;
    u16 ty;
    u16 w;
    u16 h;
    u16 by;
    UnkStruct_0233b684_L *l;
    u16 bx;
    UnkStruct_0233b684_M *map;
    u16 tx;
    u16 k;
    u8 *dst;
    u8 *src;
    s32 yy;
    u8 *row;
    s32 mode;
    u16 sx;
    u16 sy;
    u16 x1;
    u16 y1;
    s32 cw;
    s32 ch;
    UnkMtx22_0233b684 mtx;

    l = g->layer;
    for (i = 0; i < 2; i++, l++) {
        if (l->flags & 0x10000000) {
            continue;
        }
        w = g->width >> 6;
        h = g->height >> 6;
        bx = ((l->x >> 8) + l->ofsX) >> 6;
        by = ((l->y >> 8) + l->ofsY) >> 6;
        if ((s16)bx < 0) {
            bx = 0;
        }
        if ((s16)by < 0) {
            by = 0;
        }
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                map = g->map0;
                chr = l->chr0;
            } else {
                map = g->map1;
                chr = l->chr1;
            }
            for (ty = 0; ty < 8; ty++) {
                yy = by + ty;
                if (yy >= h) {
                    break;
                }
                row = chr + (ty << 10);
                for (tx = 0; tx < 8; tx++) {
                    if (bx + tx >= w) {
                        break;
                    }
                    dst = row + (tx << 4);
                    src = g->tiles + (map->data[map->w * yy + (bx + tx)] << 7);
                    for (k = 0; k < 8; k++) {
                        MIi_CpuCopy32(src, dst, 0x10);
                        src += 0x10;
                        dst += 0x80;
                    }
                }
            }
        }
    }

    l = g->layer;
    for (i = 0; i < 2; i++, l++) {
        if (i == 0) {
            mode = 10;
        } else {
            mode = 22;
        }
        if (l->flags & 0x10000000) {
            continue;
        }
        sx = (((l->x >> 8) + l->ofsX) >> 3) & 7;
        sy = (((l->y >> 8) + l->ofsY) >> 3) & 7;
        if (l->flags & 1) {
            ch = ((l->scaleY + 7) >> 3) + 1;
            if (ch > 0x40) {
                ch = 0x40;
            }
            cw = ((l->scaleX + 7) >> 3) + 1;
            if (cw > 0x40) {
                cw = 0x40;
            }
            ScreenTransfer_QueueRectRaw(l->chr0, sx, sy, 0x40, 0, mode, 0, 0, 0, 0, cw, ch);
        }
        if (l->flags & 2) {
            ch = ((l->scaleY + 7) >> 3) + 1;
            if (ch > 0x40) {
                ch = 0x40;
            }
            cw = ((l->scaleX + 7) >> 3) + 1;
            if (cw > 0x40) {
                cw = 0x40;
            }
            ScreenTransfer_QueueRectRaw(l->chr1, sx, sy, 0x40, 0, mode, 0, 4, 0, 0, cw, ch);
        }
    }

    l = &g->layer[0];
    MTX_Rot22_(&mtx, SIN(l->angle), COS(l->angle));
    MTX_ScaleApply22(&mtx, &mtx, l->scaleX << 4, l->scaleY << 4);
    x1 = (u32)(func_0200225c((((l->x >> 8) + l->ofsX) & 7) << 8, l->scaleX) >> 4) >> 8;
    y1 = (u32)(func_0200225c((((l->y >> 8) + l->ofsY) & 7) << 8, l->scaleY) >> 4) >> 8;
    if (l->flags & 1) {
        G2x_SetBGyAffine_(data_02089254, &mtx, 0, 0, x1, y1);
    }
    if (l->flags & 2) {
        G2x_SetBGyAffine_(data_02089264, &mtx, 0, 0, x1, y1);
    }
    l = &g->layer[1];
    MTX_Rot22_(&mtx, SIN(l->angle), COS(l->angle));
    MTX_ScaleApply22(&mtx, &mtx, l->scaleX << 4, l->scaleY << 4);
    x1 = (u32)(func_0200225c((((l->x >> 8) + l->ofsX) & 7) << 8, l->scaleX) >> 4) >> 8;
    y1 = (u32)(func_0200225c((((l->y >> 8) + l->ofsY) & 7) << 8, l->scaleY) >> 4) >> 8;
    if (l->flags & 1) {
        G2x_SetBGyAffine_(data_02087d2c, &mtx, 0, 0, x1, y1);
    }
    if (l->flags & 2) {
        G2x_SetBGyAffine_(data_02087d3c, &mtx, 0, 0, x1, y1);
    }
}
