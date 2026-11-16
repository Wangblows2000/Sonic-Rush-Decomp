#include <nitro/types.h>

typedef union {
    u32 v;
    struct {
        u16 w;
        u16 h;
    } s;
} UnkObjSize_02032480;

typedef struct {
    u16 p0[3];
    s16 pa;
    u16 p1[3];
    s16 pb;
    u16 p2[3];
    s16 pc;
    u16 p3[3];
    s16 pd;
} UnkAffine_02032480;

typedef struct {
    u8 pad0[0x94];
    u16 shift1d;
    u16 shiftBmp;
    u8 pad98[0x89e - 0x98];
    u16 numObj;
    u16 numAffine;
    s16 ofsX;
    s16 ofsY;
    u16 unk8a6;
    UnkAffine_02032480 affine[32];
} UnkEngine_02032480;

typedef struct {
    u32 unk0;
    u32 seqOfs;
    u32 unk8;
    u32 frameOfs;
} UnkAnim_02032480;

typedef struct {
    u16 count;
    u16 flags;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    s16 cx;
    s16 cy;
    u16 oam[1][4];
} UnkFrame_02032480;

typedef struct {
    s32 engine;
    s16 x;
    s16 y;
    u16 seq;
    u16 pad0a;
    u32 unkC;
    UnkAnim_02032480 *anim;
    u8 pad14[0x24 - 0x14];
    u32 frame;
    u32 unk28;
    u32 flags;
    u32 unk30;
    u32 vram;
    u8 pad38[0x44 - 0x38];
    u16 pltt;
    u16 oamPrio;
} UnkSprite_02032480;

typedef struct {
    fx32 m[4];
} UnkMtx22_02032480;

extern UnkEngine_02032480 *data_02072c78[];
extern u32 data_02072c70[];
extern UnkObjSize_02032480 data_02072da0[];
extern u16 data_02072d60[];
extern s16 data_0206e470[];
extern u16 data_02078034[];

void func_02032d2c(UnkSprite_02032480 *spr);
fx32 func_02002240(fx32 x);
void func_02000cd8(UnkMtx22_02032480 *m, fx32 s, fx32 c);
void func_02000d98(const UnkMtx22_02032480 *src, UnkMtx22_02032480 *dst, fx32 x, fx32 y);
u16 func_02036aec(s32 engine, const UnkMtx22_02032480 *m);
u16 *func_02036bfc(s32 engine, u16 prio);

static inline s16 FX_SinIdx(int idx) {
    return data_0206e470[idx << 1];
}
static inline s16 FX_CosIdx(int idx) {
    return data_0206e470[(idx << 1) + 1];
}
static inline fx32 FX_Mul(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800LL) >> 12);
}

void AnimatorSprite_DrawRotScale(UnkSprite_02032480 *spr, fx32 sx, fx32 sy, s32 rot) {
    UnkObjSize_02032480 sz;
    u16 affIdx[4];
    UnkMtx22_02032480 pos[4];
    UnkMtx22_02032480 aff[4];
    UnkAnim_02032480 *anim;
    UnkFrame_02032480 *frame;
    UnkEngine_02032480 *e;
    s32 eng;
    u16 attr0;
    u16 attr1;
    u32 shift;
    u32 mode;
    u32 i;
    u32 flags;
    fx32 ix;
    fx32 iy;
    fx32 px;
    fx32 py;
    u32 chr;
    u16 *src;
    u32 unit;
    u32 affFlag;
    u32 szIdx;
    u32 fi;
    fx32 x;
    fx32 y;
    fx32 ox;
    fx32 oy;
    UnkMtx22_02032480 *m;
    u32 mi;
    u16 *oam;

    anim = spr->anim;
    attr1 = attr0 = 0;
    frame = (UnkFrame_02032480 *)(spr->frame + ((u32)anim + anim->frameOfs));
    flags = spr->flags;
    eng = spr->engine;
    e = data_02072c78[eng];
    if (flags & 0x80000) return;
    if (frame->count == 0) return;
    if ((sx < 0 ? -sx : sx) < 0x22) return;
    if ((sy < 0 ? -sy : sy) < 0x22) return;
    if (!(flags & 0x10000)) {
        u16 n = frame->count;
        if (n >= 4) n = 4;
        if (n + (e->numAffine + e->numObj) >= 32) {
            func_02032d2c(spr);
            return;
        }
    }
    if (rot != 0 || sx != 0x1000 || sy != 0x1000) {
        ix = func_02002240(sx > 0 ? sx + 0xd0 : sx - 0xd0);
        iy = func_02002240(sy > 0 ? sy + 0xd0 : sy - 0xd0);
    } else {
        ix = func_02002240(sx);
        iy = func_02002240(sy);
    }
    {
        s32 idx = (rot & 0xffff) >> 4;
        func_02000cd8(&pos[0], FX_SinIdx(idx), FX_CosIdx(idx));
    }
    func_02000d98(&pos[0], &aff[1], -ix, iy);
    func_02000d98(&pos[0], &pos[1], -sx, sy);
    func_02000d98(&pos[0], &aff[2], ix, -iy);
    func_02000d98(&pos[0], &pos[2], sx, -sy);
    func_02000d98(&pos[0], &aff[3], -ix, -iy);
    func_02000d98(&pos[0], &pos[3], -sx, -sy);
    func_02000d98(&pos[0], &aff[0], ix, iy);
    func_02000d98(&pos[0], &pos[0], sx, sy);
    px = spr->x << 12;
    py = spr->y << 12;
    flags = spr->flags;
    if (flags & 0x2000) {
        px += e->ofsX << 12;
        py += e->ofsY << 12;
    }
    if (flags & 0x80) attr1 |= 0x1000;
    if (flags & 0x100) attr1 |= 0x2000;
    if (flags & 0x1000) attr0 |= 0x1000;
    attr0 |= flags & 0xc00;
    if (flags & 0x200) attr0 |= 0x200;
    if ((frame->oam[0][0] & 0xc00) == 0xc00)
        chr = ((spr->vram - data_02072c70[spr->engine]) >> (e->shiftBmp + 7)) & 0x3ff;
    else
        chr = ((spr->vram - data_02072c70[spr->engine]) >> (e->shift1d + 5)) & 0x3ff;
    affIdx[0] = data_02078034[0];
    affIdx[1] = data_02078034[1];
    affIdx[2] = data_02078034[2];
    affIdx[3] = data_02078034[3];
    src = frame->oam[0];
    if ((frame->oam[0][0] & 0xc00) == 0xc00) {
        shift = data_02072c78[spr->engine]->shiftBmp;
        mode = 0;
    } else {
        shift = data_02072c78[spr->engine]->shift1d;
        if (*(u16 *)((u32)spr->anim + spr->anim->seqOfs + spr->seq * 8 + 8) == 0)
            mode = 0;
        else
            mode = 1;
    }
    i = 0;
    if (i >= frame->count) return;
    unit = 1 << shift;
    affFlag = attr0 & 0x200;
    do {
        szIdx = ((s32)(src[0] & 0xc000) >> 12) | ((s32)(src[1] & 0xc000) >> 14);
        sz.v = data_02072da0[szIdx].v;
        fi = (s32)((attr1 ^ src[1]) & 0x3000) >> 12;
        y = ((src[0] & 0xff) - frame->cy + (sz.s.h >> 1)) << 12;
        x = ((src[1] & 0x1ff) - frame->cx + (sz.s.w >> 1)) << 12;
        m = &pos[fi];
        ox = (px - (sz.s.w << 11)) + (FX_Mul(m->m[0], x) + FX_Mul(m->m[2], y));
        oy = (py - (sz.s.h << 11)) + (FX_Mul(m->m[1], x) + FX_Mul(m->m[3], y));
        if (affFlag) {
            ox -= sz.s.w << 11;
            oy -= sz.s.h << 11;
            *(u32 *)&sz <<= 1;
        }
        if ((spr->flags & 0x4000) && !(ox + (sz.s.w << 12) > 0 && ox < 0x100000 && oy + (sz.s.h << 12) > 0 && oy < 0xc0000)) {
            if (!(frame->flags & 1))
                chr += ((s32)(unit + data_02072d60[szIdx] - 1) >> shift) << mode;
            goto next;
        }
        mi = affIdx[fi];
        if ((s16)mi < 0) {
            affIdx[fi] = func_02036aec(spr->engine, &aff[fi]);
            mi = affIdx[fi];
            if ((s16)mi >= 0) {
                UnkAffine_02032480 *p = &e->affine[mi];
                p->pa = aff[fi].m[0] >> 4;
                p->pb = aff[fi].m[1] >> 4;
                p->pc = aff[fi].m[2] >> 4;
                p->pd = aff[fi].m[3] >> 4;
            } else {
                if (!(frame->flags & 1))
                    chr += ((s32)(unit + data_02072d60[szIdx] - 1) >> shift) << mode;
                goto next;
            }
        }
        oam = func_02036bfc(spr->engine, spr->oamPrio);
        oam[0] = (src[0] & ~0xff) | ((oy >> 12) & 0xff) | 0x100;
        oam[0] |= attr0;
        oam[1] = (src[1] & ~0x1ff) | ((ox >> 12) & 0x1ff);
        oam[1] = (oam[1] & ~0x3e00) | (mi << 9);
        if (!(frame->flags & 1)) {
            oam[2] = (chr & 0x3ff) | (((spr->flags & 0x60000) >> 17) << 10) | ((src[2] + (spr->pltt << 12)) & 0xf000);
            chr += ((s32)(unit + data_02072d60[szIdx] - 1) >> shift) << mode;
        } else {
            oam[2] = ((src[2] + chr) & 0x3ff) | (((spr->flags & 0x60000) >> 17) << 10) | ((src[2] + (spr->pltt << 12)) & 0xf000);
        }
    next:
        i++;
        src += 4;
    } while (i < frame->count);
}
