#include <nitro/types.h>

typedef struct {
    u8 unk0[8];
    u16 fmtIdx;
    u8 unkA[6];
    u8 *res;
    u8 unk14[0x10];
    u32 cellOfs;
    u8 unk28[4];
    u32 flags;
    u8 unk30[4];
    u32 texAddr;
    u8 unk38[8];
    u32 plttAddr;
} UnkSub_02030a84;

typedef struct {
    u16 col : 15;
    u16 flag : 1;
} UnkColor_02030a84;

typedef struct {
    u8 unk0[0x90];
    UnkSub_02030a84 s;
    u8 unkD4[8];
    u32 polyAttr;
    UnkColor_02030a84 dif;
    UnkColor_02030a84 amb;
    UnkColor_02030a84 spe;
    UnkColor_02030a84 emi;
    u16 color;
} UnkObj_02030a84;

typedef struct {
    u16 num;
    u8 unk2[10];
    s16 x;
    s16 y;
    u16 oam[1];
} UnkCell_02030a84;

typedef struct {
    u16 w;
    u16 h;
} UnkSize_02030a84;

extern const u32 data_02072d78[];
extern const u8 data_02072e64[];
extern UnkSize_02030a84 data_02072da0[];
extern u8 data_02072e30[];
extern UnkSize_02030a84 data_02072dd0[];
extern u16 data_02072d60[];

void func_0200148c(fx32 *mtx);
void func_02025268(u32 op, const void *args, u32 n);
void func_02025364(const void *dl, u32 size);

static inline void VecSet(fx32 *v, fx32 x, fx32 y, fx32 z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}
static inline void GeMtxMode(u32 mode) {
    u32 a = mode;
    func_02025268(0x10, &a, 1);
}
static inline void GePushMtx(void) {
    func_02025268(0x11, NULL, 0);
}
static inline void GePopMtx(u32 n) {
    u32 a = n;
    func_02025268(0x12, &a, 1);
}
static inline void GeMultMtx43(const fx32 *m) {
    func_02025268(0x17, m, 12);
}
static inline void GeTranslate(fx32 x, fx32 y, fx32 z) {
    fx32 v[3];
    v[0] = x;
    v[1] = y;
    v[2] = z;
    func_02025268(0x1c, v, 3);
}
static inline void GeTranslateVec(const fx32 *v) {
    func_02025268(0x1c, v, 3);
}
static inline void GeScale(fx32 x, fx32 y, fx32 z) {
    fx32 v[3];
    v[0] = x;
    v[1] = y;
    v[2] = z;
    func_02025268(0x1b, v, 3);
}
static inline void GeTexImageParam(u32 p) {
    u32 a = p;
    func_02025268(0x2a, &a, 1);
}

void Model_DrawSprite3D(UnkObj_02030a84 *o) {
    UnkSub_02030a84 *s = &o->s;
    u8 *res = s->res;
    UnkCell_02030a84 *cell;
    u16 *oam;
    u16 flip;
    u32 texAddr;
    int fmt;
    u32 texFmt;
    u32 i;

    flip = 0;
    cell = (UnkCell_02030a84 *)(s->cellOfs + (u32)(res + *(u32 *)(res + 0xc)));
    if (cell->num == 0) return;
    texAddr = s->texAddr;
    if (s->flags & 0x80) flip |= 0x1000;
    if (s->flags & 0x100) flip |= 0x2000;
    fmt = *(u16 *)(res + *(u32 *)(res + 4) + s->fmtIdx * 8 + 8);
    {
    fx32 trans[3] = {0, 0, 0};
    fx32 mtx[12];
    oam = cell->oam;
    func_0200148c(mtx);
    func_02025268(0x29, &o->polyAttr, 1);
    texFmt = data_02072d78[fmt];
    {
        u32 a = s->plttAddr >> (4 - (texFmt == 2));
        func_02025268(0x2b, &a, 1);
    }
    {
        u32 a = o->dif.col | (o->amb.col << 16) | ((o->dif.flag ? 1 : 0) << 15);
        func_02025268(0x30, &a, 1);
    }
    {
        u32 a = o->spe.col | (o->emi.col << 16) | ((o->spe.flag ? 1 : 0) << 15);
        func_02025268(0x31, &a, 1);
    }
    {
        u32 a = o->color;
        func_02025268(0x20, &a, 1);
    }
    if (!o->dif.flag) {
        u32 a = 0x1ff00000;
        func_02025268(0x21, &a, 1);
    }
    switch (fmt) {
    case 0:
    case 1:
        for (i = 0; i < cell->num; i++, oam += 4) {
            UnkSize_02030a84 *sz = &data_02072da0[((oam[0] & 0xc000) >> 12) | ((oam[1] & 0xc000) >> 14)];
            u16 sy, sx;
            s32 dx, dy;
            u16 j;
            u16 x;
            u16 k;
            u16 y;
            u32 tf;
            if (flip & 0x1000) {
                trans[0] = (cell->x - (oam[1] & 0x1ff) - sz->w) << 12;
                mtx[9] = 0x70000;
                mtx[0] = -0x8000;
                dx = -8;
                sx = sz->w - 8;
            } else {
                trans[0] = (-cell->x + (u32)(oam[1] & 0x1ff)) << 12;
                mtx[9] = 0;
                mtx[0] = 0x8000;
                sx = 0;
                dx = 8;
            }
            if (flip & 0x2000) {
                trans[1] = (-cell->y + (u32)(oam[0] & 0xff) - sz->h) << 12;
                mtx[10] = 0x70000;
                mtx[4] = -0x8000;
                sy = sz->h - 8;
                dy = -8;
            } else {
                trans[1] = (cell->y - (oam[0] & 0xff)) << 12;
                mtx[10] = 0;
                mtx[4] = 0x8000;
                sy = 0;
                dy = 8;
            }
            GeMtxMode(3);
            GeMultMtx43(mtx);
            GeMtxMode(1);
            y = sy;
            for (j = 0; j < sz->h; j += 8, y += dy) {
                k = 0;
                x = sx;
                if (k < sz->w)
                for (tf = data_02072d78[fmt]; k < sz->w; k += 8, x += dx) {
                    GePushMtx();
                    GeTexImageParam((tf << 26) | (texAddr >> 3) | 0x60000000);
                    texAddr += 1 << data_02072e64[fmt];
                    GeTranslate(trans[0] + (x << 12), trans[1] - (y << 12), 0);
                    GeScale(0x8000, 0x8000, 0);
                    func_02025364(data_02072e30, 0x34);
                    GePopMtx(1);
                }
            }
        }
        break;
    default:
        {
        u32 idx;
        u32 n;
        UnkSize_02030a84 *sz;
        n = 0;
        if (n < cell->num)
        for (; n < cell->num; n++, oam += 4) {
            GePushMtx();
            idx = ((oam[0] & 0xc000) >> 12) | ((oam[1] & 0xc000) >> 14);
            sz = &data_02072da0[idx];
            {
            UnkSize_02030a84 *st = &data_02072dd0[idx];
            GeTexImageParam((texFmt << 26) | (texAddr >> 3) | 0x40000000 | (st->w << 20) | (st->h << 23) | 0x20000000);
            }
            texAddr += data_02072d60[idx] << data_02072e64[fmt];
            if (flip & 0x1000) {
                trans[0] = (cell->x - (oam[1] & 0x1ff) - sz->w) << 12;
                mtx[9] = (sz->w - 1) << 16;
                mtx[0] = -sz->w << 12;
            } else {
                trans[0] = (-cell->x + (u32)(oam[1] & 0x1ff)) << 12;
                mtx[9] = 0;
                mtx[0] = sz->w << 12;
            }
            if (flip & 0x2000) {
                trans[1] = (-cell->y + (u32)(oam[0] & 0xff) - sz->h) << 12;
                mtx[10] = (sz->h - 1) << 16;
                mtx[4] = -sz->h << 12;
            } else {
                trans[1] = (cell->y - (oam[0] & 0xff)) << 12;
                mtx[10] = 0;
                mtx[4] = sz->h << 12;
            }
            GeMtxMode(3);
            GeMultMtx43(mtx);
            GeMtxMode(1);
            GeTranslateVec(trans);
            GeScale((sz->w << 12) + 8, (sz->h << 12) + 8, 0);
            func_02025364(data_02072e30, 0x34);
            GePopMtx(1);
        }
        }
    }
    }
}
