#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
} UnkPos_0206aa38;

typedef struct {
    u16 w;
    u16 h;
} UnkSize_0206aa38;

typedef struct {
    fx32 _00, _01, _10, _11;
} UnkMtx22_0206aa38;

typedef struct {
    u16 v[4];
} UnkAff4_0206aa38;

typedef struct {
    u16 count;
    u16 pad2;
    s16 unk4;
    s16 pad6;
    s16 unk8;
    s16 padA;
    s16 unkC;
    s16 unkE;
    u16 cells[1];
} UnkFrame_0206aa38;

typedef struct {
    u8 pad0[0xc];
    u32 frameOfs;
} UnkRes_0206aa38;

typedef struct {
    s32 engine;
    s16 x;
    s16 y;
    u8 pad8[8];
    UnkRes_0206aa38 *res;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
    u8 pad28[4];
    u32 flags;
    u8 pad30[4];
    u32 vram;
    u8 pad38[0x44 - 0x38];
    u16 palette;
    u16 prio;
} UnkSpr_0206aa38;

typedef struct {
    u16 pad0[3];
    s16 p;
} UnkOamAff_0206aa38;

typedef struct {
    u8 pad0[0x94];
    u16 shift;
    u16 shift2;
    u8 pad98[0x89e - 0x98];
    u16 unk89e;
    u16 unk8a0;
    s16 ofsX;
    s16 ofsY;
    u8 pad8a6[2];
    UnkOamAff_0206aa38 oam[128];
} UnkCtx_0206aa38;

extern UnkCtx_0206aa38 *gDisplayTable[];
extern u32 gObjVramBaseTable[];
extern UnkSize_0206aa38 data_02072da0[];
extern const UnkAff4_0206aa38 data_0207cf84;

u16 *Oam_AllocObj(s32 engine, u16 prio);
u16 Oam_GetAffineIndex(u32 engine, UnkMtx22_0206aa38 *mtx);
void AnimatorSprite_Draw(UnkSpr_0206aa38 *spr);
fx32 FX_Inv(fx32 x);
void MTX_Identity22_(UnkMtx22_0206aa38 *m);
void MTX_ScaleApply22(const UnkMtx22_0206aa38 *pSrc, UnkMtx22_0206aa38 *pDst, fx32 x, fx32 y);

static inline fx32 FX_Mul_0206aa38(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

void StageObj_DrawSpriteRotScaleAlt(UnkSpr_0206aa38 *spr, UnkPos_0206aa38 *pos, fx32 sx, fx32 sy) {
    u32 shift;
    UnkCtx_0206aa38 *ctx;
    UnkFrame_0206aa38 *frame;
    u32 tile;
    u16 attr0x;
    u16 attr1x;
    u32 cols;
    UnkSize_0206aa38 *sp_;
    u32 k;
    u32 j;
    u32 i;
    u32 w;
    u32 h;
    s32 bx;
    s32 by;
    UnkAff4_0206aa38 aff;
    UnkMtx22_0206aa38 mtx[4];
    UnkMtx22_0206aa38 inv[4];
    u32 count;
    u32 base;
    s32 ix;
    s32 iy;
    u32 flags;
    u16 *cell;
    u32 n;
    u32 flip;
    s32 tw;
    s32 tx;
    s32 ty;
    u32 t;
    s32 x;
    s32 y;
    u32 idx;
    u16 *oam;
    UnkOamAff_0206aa38 *o;

    attr0x = 0;
    ctx = gDisplayTable[spr->engine];
    base = (u32)spr->res + spr->res->frameOfs;
    frame = (UnkFrame_0206aa38 *)(spr->unk24 + base);
    count = frame->count;
    attr1x = attr0x;
    if (count == 0)
        return;
    if ((sx < 0 ? -sx : sx) < 0x22)
        return;
    if ((sy < 0 ? -sy : sy) < 0x22)
        return;
    if (count >= 4)
        count = 4;
    if ((s32)(count + (ctx->unk8a0 + ctx->unk89e)) >= 0x20) {
        AnimatorSprite_Draw(spr);
        return;
    }
    ix = FX_Inv(sx);
    iy = FX_Inv(sy);
    MTX_Identity22_(&mtx[0]);
    MTX_ScaleApply22(&mtx[0], &inv[1], -ix, iy);
    MTX_ScaleApply22(&mtx[0], &mtx[1], -sx, sy);
    MTX_ScaleApply22(&mtx[0], &inv[2], ix, -iy);
    MTX_ScaleApply22(&mtx[0], &mtx[2], sx, -sy);
    MTX_ScaleApply22(&mtx[0], &inv[3], -ix, -iy);
    MTX_ScaleApply22(&mtx[0], &mtx[3], -sx, -sy);
    MTX_ScaleApply22(&mtx[0], &inv[0], ix, iy);
    MTX_ScaleApply22(&mtx[0], &mtx[0], sx, sy);
    bx = spr->x;
    by = spr->y;
    shift = gDisplayTable[spr->engine]->shift;
    cols = (u8)((frame->unk8 - frame->unk4) / (8 << shift));
    if (cols == 0)
        cols = 1;
    flags = spr->flags;
    if (flags & 0x2000) {
        bx += ctx->ofsX;
        by += ctx->ofsY;
    }
    if (flags & 0x80)
        attr1x |= 0x1000;
    if (flags & 0x100)
        attr1x |= 0x2000;
    if (flags & 0x1000)
        attr0x |= 0x1000;
    if (flags & 0x200)
        attr0x |= 0x200;
    if ((frame->cells[0] & 0xc00) == 0xc00)
        tile = ((spr->vram - gObjVramBaseTable[spr->engine]) >> (ctx->shift2 + 7)) & 0x3ff;
    else
        tile = ((spr->vram - gObjVramBaseTable[spr->engine]) >> (ctx->shift + 5)) & 0x3ff;
    cell = frame->cells;
    aff = data_0207cf84;
    if ((frame->cells[0] & 0xc00) == 0xc00)
        shift = gDisplayTable[spr->engine]->shift2;
    for (i = 0; i < frame->count; i++, cell += 4) {
        sp_ = &data_02072da0[((cell[0] & 0xc000) >> 12) | ((cell[1] & 0xc000) >> 14)];
        w = (u8)(sp_->w / (8 << gDisplayTable[spr->engine]->shift));
        h = (u8)(sp_->h >> 3);
        if (w == 0)
            w = 1;
        if (h == 0)
            h = 1;
        t = 0;
        for (j = 0; j < h; j++) {
            n = (u8)((((s32)(cell[1] << 23) >> 26) >> gDisplayTable[spr->engine]->shift))
              + (u8)(cols * (j + ((s32)(cell[0] << 23) >> 26)));
            flip = ((attr1x ^ cell[1]) & 0x3000) >> 12;
            for (k = 0; k < w; k++) {
                tw = 8 << gDisplayTable[spr->engine]->shift;
                ty = ((((s32)(cell[0] << 24) >> 24) - frame->unkE + pos[n].y) + (s32)(j * 8) + 4) << 12;
                tx = (k * tw + ((((s32)(cell[1] << 23) >> 23) - frame->unkC) + pos[n].x) + (tw >> 1)) << 12;
                x = (bx - (tw >> 1)) + ((FX_Mul_0206aa38(mtx[flip]._00, tx) + FX_Mul_0206aa38(mtx[flip]._10, ty)) >> 12);
                y = (by - 4) + ((FX_Mul_0206aa38(mtx[flip]._01, tx) + FX_Mul_0206aa38(mtx[flip]._11, ty)) >> 12);
                if (attr0x & 0x200) {
                    x -= sp_->w << 11;
                    y -= sp_->h << 11;
                    *(u32 *)sp_ <<= 1;
                }
                if ((spr->flags & 0x4000) && !(x + (8 << gDisplayTable[spr->engine]->shift) > 0 && x < 0x100 && y + 8 > 0 && y < 0xc0)) {
                    t = (u8)(t + 1);
                    n++;
                    continue;
                }
                idx = aff.v[flip];
                if ((s16)idx < 0) {
                    aff.v[flip] = Oam_GetAffineIndex(spr->engine, &inv[flip]);
                    idx = aff.v[flip];
                    if ((s16)idx >= 0) {
                        o = &ctx->oam[idx * 4];
                        o[0].p = inv[flip]._00 >> 4;
                        o[1].p = inv[flip]._01 >> 4;
                        o[2].p = inv[flip]._10 >> 4;
                        o[3].p = inv[flip]._11 >> 4;
                    } else {
                        tile++;
                        continue;
                    }
                }
                oam = Oam_AllocObj(spr->engine, spr->prio);
                oam[0] = (y & 0xff) | 0x4100;
                oam[0] ^= attr0x;
                oam[1] = x & 0x1ff;
                oam[1] = (oam[1] & ~0x3e00) | (idx << 9);
                oam[2] = ((spr->palette << 12) & 0xf000)
                       | (((t + (tile + (((cell[2] & 0x3ff) + (1 << shift) - 1) >> shift))) & 0x3ff)
                          | (((spr->flags & 0x60000) >> 17) << 10));
                n++;
                t = (u8)(t + 1);
            }
        }
    }
}
