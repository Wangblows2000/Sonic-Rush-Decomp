#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
} UnkPos_0206b244;

typedef struct {
    u16 w;
    u16 h;
} UnkSize_0206b244;

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
} UnkFrame_0206b244;

typedef struct {
    u8 pad0[0xc];
    u32 frameOfs;
} UnkRes_0206b244;

typedef struct {
    s32 engine;
    s16 x;
    s16 y;
    u8 pad8[8];
    UnkRes_0206b244 *res;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
    u8 pad28[4];
    u32 flags;
    u8 pad30[4];
    u32 vram;
    u8 pad38[0x44 - 0x38];
    u16 palette;
    u16 prio;
} UnkSpr_0206b244;

typedef struct {
    u8 pad0[0x94];
    u16 shift;
    u16 shift2;
    u8 pad98[0x8a2 - 0x98];
    s16 ofsX;
    s16 ofsY;
} UnkCtx_0206b244;

extern UnkCtx_0206b244 *gDisplayTable[];
extern u32 gObjVramBaseTable[];
extern UnkSize_0206b244 data_02072da0[];

u16 *Oam_AllocObj(s32 engine, u16 prio);

void StageObj_DrawSpriteAlt(UnkSpr_0206b244 *spr, UnkPos_0206b244 *pos) {
    u32 shift;
    UnkSize_0206b244 *sp_;
    u32 w;
    u32 t;
    u16 attr1x;
    s32 tw;
    s32 by;
    u16 attr0x;
    u32 sz;
    u32 k;
    u32 flags;
    u32 i;
    u16 *cell;
    s32 eng;
    u32 n;
    u16 *oam;
    u32 h;
    u32 tile;
    u32 cols;
    UnkFrame_0206b244 *frame;
    s32 bx;
    s32 y;
    u32 j;
    u16 count;
    u32 base;
    UnkCtx_0206b244 *ctx;
    s32 x;

    attr0x = 0;
    eng = spr->engine;
    ctx = gDisplayTable[eng];
    base = (u32)spr->res + spr->res->frameOfs;
    frame = (UnkFrame_0206b244 *)(spr->unk24 + base);
    count = frame->count;
    attr1x = attr0x;
    if (count == 0)
        return;
    shift = ctx->shift;
    bx = spr->x;
    by = spr->y;
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
    if ((frame->cells[0] & 0xc00) == 0xc00)
        tile = ((spr->vram - gObjVramBaseTable[eng]) >> (ctx->shift2 + 7)) & 0x3ff;
    else
        tile = ((spr->vram - gObjVramBaseTable[eng]) >> (*(u16 *)((u8 *)ctx + 0x94) + 5)) & 0x3ff;
    if ((frame->cells[0] & 0xc00) == 0xc00)
        shift = ctx->shift2;
    cell = frame->cells;
    for (i = 0; i < frame->count; i++, cell += 4) {
        sz = ((cell[0] & 0xc000) >> 12) | ((cell[1] & 0xc000) >> 14);
        sp_ = &data_02072da0[sz];
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
            for (k = 0; k < w; k++) {
                if (attr1x & 0x1000) {
                    tw = 8 << gDisplayTable[spr->engine]->shift;
                    x = tw * (-1 - k) + (frame->unkC + (bx + pos[n].x) - ((s32)(cell[1] << 23) >> 23));
                } else {
                    tw = 8 << gDisplayTable[spr->engine]->shift;
                    x = k * tw + ((bx + pos[n].x) - frame->unkC + ((s32)(cell[1] << 23) >> 23));
                }
                if (attr1x & 0x2000)
                    y = frame->unkE + (by + pos[n].y) - (s8)cell[0] - (s32)(j * 8) - 8;
                else
                    y = (s32)(j * 8) + ((by + pos[n].y) - frame->unkE + (s8)cell[0]);
                if ((spr->flags & 0x4000) && !(x + tw > 0 && x < 0x100 && y + 8 > 0 && y < 0xc0)) {
                    t = (u8)(t + 1);
                    n++;
                    continue;
                }
                oam = Oam_AllocObj(*(s32 *)spr, spr->prio);
                oam[0] = (y & 0xff) | 0x4000;
                oam[0] ^= attr0x;
                oam[1] = x & 0x1ff;
                oam[1] ^= attr1x;
                oam[2] = ((spr->palette << 12) & 0xf000)
                       | (((t + (tile + (((cell[2] & 0x3ff) + (1 << shift) - 1) >> shift))) & 0x3ff)
                          | (((spr->flags & 0x60000) >> 17) << 10));
                n++;
                t = (u8)(t + 1);
            }
        }
    }
}
