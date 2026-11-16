#include <nitro/types.h>

typedef struct {
    u16 w;
    u16 h;
} UnkObjSize_02032d2c;

typedef struct {
    u8 pad0[0x94];
    u16 shift1d;
    u16 shiftBmp;
    u8 pad98[0x8a2 - 0x98];
    s16 ofsX;
    s16 ofsY;
} UnkEngine_02032d2c;

typedef struct {
    u32 unk0;
    u32 seqOfs;
    u32 unk8;
    u32 frameOfs;
} UnkAnim_02032d2c;

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
} UnkFrame_02032d2c;

typedef struct {
    s32 engine;
    s16 x;
    s16 y;
    u16 seq;
    u16 pad0a;
    u32 unkC;
    UnkAnim_02032d2c *anim;
    u8 pad14[0x24 - 0x14];
    u32 frame;
    u32 unk28;
    u32 flags;
    u32 unk30;
    u32 vram;
    u8 pad38[0x44 - 0x38];
    u16 pltt;
    u16 oamPrio;
} UnkSprite_02032d2c;

extern UnkEngine_02032d2c *gDisplayTable[];
extern u32 gObjVramBaseTable[];
extern UnkObjSize_02032d2c data_02072da0[];
extern u16 data_02072d60[];

u16 *Oam_AllocObj(s32 engine, u16 prio);

void AnimatorSprite_Draw(UnkSprite_02032d2c *spr)
{
    s32 idx;
    UnkFrame_02032d2c *frame;
    UnkEngine_02032d2c *e;
    s32 xmin;
    u16 attr0;
    u16 attr1;
    s32 ymin;
    u32 shift;
    u32 mode;
    s32 right;
    UnkAnim_02032d2c *anim;
    s32 bottom;
    s32 x;
    s32 left;
    u32 chr;
    u16 *src;
    s32 top;
    s32 y;
    u32 i;
    u32 unit;
    s32 eng;
    UnkObjSize_02032d2c *sz;
    s32 ox;
    s32 oy;
    u16 *oam;

    anim = spr->anim;
    attr0 = 0;
    attr1 = 0;
    eng = spr->engine;
    e = gDisplayTable[eng];
    frame = (UnkFrame_02032d2c *)(spr->frame + ((u32)anim + anim->frameOfs));
    if (spr->flags & 0x80000)
        return;
    if (frame->count == 0)
        return;

    left = frame->left;
    top = frame->top;
    right = frame->right;
    bottom = frame->bottom;
    xmin = left;
    ymin = top;
    x = spr->x;
    y = spr->y;
    if (spr->flags & 0x2000) {
        x += e->ofsX;
        y += e->ofsY;
    }
    if (spr->flags & 0x80) {
        xmin = -right;
        attr1 |= 0x1000;
        right = -left;
    }
    if (spr->flags & 0x100) {
        ymin = -bottom;
        attr1 |= 0x2000;
        bottom = -top;
    }
    if (spr->flags & 0x1000)
        attr0 |= 0x1000;
    attr0 |= spr->flags & 0xc00;
    if (spr->flags & 0x4000) {
        if (x + right <= 0)
            return;
        if (x + xmin >= 256)
            return;
        if (y + bottom <= 0)
            return;
        if (y + ymin >= 192)
            return;
    }

    if ((frame->oam[0][0] & 0xc00) == 0xc00)
        chr = ((spr->vram - gObjVramBaseTable[eng]) >> (e->shiftBmp + 7)) & 0x3ff;
    else
        chr = ((spr->vram - gObjVramBaseTable[eng]) >> (e->shift1d + 5)) & 0x3ff;

    src = frame->oam[0];
    if ((frame->oam[0][0] & 0xc00) == 0xc00) {
        shift = e->shiftBmp;
        mode = 0;
    } else {
        shift = e->shift1d;
        if (*(u16 *)((u32)anim + anim->seqOfs + spr->seq * 8 + 8) == 0)
            mode = 0;
        else
            mode = 1;
    }

    for (i = 0; i < frame->count; i++, src += 4) {
        idx = ((s32)(src[0] & 0xc000) >> 12) | ((s32)(src[1] & 0xc000) >> 14);
        sz = &data_02072da0[idx];
        if (attr1 & 0x1000)
            ox = x + frame->cx - (src[1] & 0x1ff) - sz->w;
        else
            ox = (src[1] & 0x1ff) + (x - frame->cx);
        if (!(attr1 & 0x2000))
            oy = (src[0] & 0xff) + (y - frame->cy);
        else
            oy = y + frame->cy - (src[0] & 0xff) - sz->h;
        if ((spr->flags & 0x4000) && (ox + sz->w <= 0 || ox >= 256 || oy + sz->h <= 0 || oy >= 192)) {
            if (!(frame->flags & 1))
                chr += ((s32)((1 << shift) + data_02072d60[idx] - 1) >> shift) << mode;
        } else {
            oam = Oam_AllocObj(spr->engine, spr->oamPrio);
            oam[0] = (src[0] & ~0xff) | (oy & 0xff);
            oam[0] |= attr0;
            oam[1] = (src[1] & ~0x1ff) | (ox & 0x1ff);
            oam[1] ^= attr1;
            if (!(frame->flags & 1)) {
                oam[2] = (chr & 0x3ff) | (((spr->flags & 0x60000) >> 17) << 10) | ((src[2] + (spr->pltt << 12)) & 0xf000);
                chr += ((s32)((1 << shift) + data_02072d60[idx] - 1) >> shift) << mode;
            } else {
                oam[2] = ((src[2] + chr) & 0x3ff) | (((spr->flags & 0x60000) >> 17) << 10) | ((src[2] + (spr->pltt << 12)) & 0xf000);
            }
        }
    }
}
