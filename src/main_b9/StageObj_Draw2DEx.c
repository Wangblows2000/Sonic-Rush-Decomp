#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2a - 8];
    s16 unk2A;
    u32 flags;
    u8 pad30[4];
    void *unk34;
} UnkSpr_0206b768;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    s8 unk6;
    s8 unk7;
    u8 pad8[0x18 - 8];
    s32 unk18;
    u8 pad1C[0x44 - 0x1c];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x5c - 0x48];
    u16 unk5C;
    u8 pad5E[0x74 - 0x5e];
    void *unk74;
    u8 pad78[0xa4 - 0x78];
    void *unkA4;
    u8 padA8[0x124 - 0xa8];
    void *unk124;
    void *unk128;
} UnkObj_0206b768;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x68 - 0xc];
    s32 unk68;
    s32 unk6C;
} UnkCam_0206b768;

extern u32 gObjManagerFlags;
extern UnkCam_0206b768 data_ov000_0236aad8;
extern s8 data_022c4eec[2];
extern s16 data_0207cf78;
extern s16 data_0207cf7c;
extern s16 data_0207cf80[2];

void AnimatorSprite_ProcessAnimation(UnkSpr_0206b768 *s, void *a, UnkObj_0206b768 *o);
void AnimatorSprite_DrawRotScale(UnkSpr_0206b768 *s, s32 sx, s32 sy, u16 r);
void AnimatorSprite_Draw(UnkSpr_0206b768 *s);
void func_0206b244(UnkSpr_0206b768 *s, void *p);
void func_0206aa38(UnkSpr_0206b768 *s, void *p, s32 sx, s32 sy);

void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, UnkObj_0206b768 *o, UnkSpr_0206b768 *s, UnkSpr_0206b768 *s2, void *p) {
    s32 ox = 0;
    s32 oy = 0;
    u16 f = 0;
    BOOL busy = FALSE;
    s16 save;
    s32 sx;
    s32 sy;

    if (o != NULL) {
        f = o->unk5C;
        if (o->unk5 != 0)
            rot = rot + o->unk5;
        if (o->unkA4 != NULL)
            f ^= 1;
        if (o->unk124 != NULL || o->unk128 != NULL)
            busy = TRUE;
    }
    s->flags |= 0x4000;
    if (o != NULL) {
        s->flags &= ~0x184;
        if (o != NULL && !busy)
            o->unk5C &= ~8;
        if (f & 4)
            s->flags |= 4;
        if (f & 1)
            s->flags |= 0x80;
        if (f & 2)
            s->flags |= 0x100;
    }
    if (s2 != NULL) {
        s2->flags &= 0x80000;
        s2->flags |= s->flags & ~0x80000;
    }
    if ((gObjManagerFlags & 4) && !(f & 0x80)) {
        ox = data_ov000_0236aad8.unk4 >> 8;
        oy = data_ov000_0236aad8.unk8 >> 8;
    }
    if (!(f & 0x2000)) {
        s->x = (x >> 8) - ox;
        s->y = (y >> 8) - oy;
        if (!(f & 0x4000)) {
            s->x += data_022c4eec[0];
            s->y += data_022c4eec[1];
        }
    }
    if (s2 != NULL) {
        if ((gObjManagerFlags & 4) && !(f & 0x80)) {
            ox = data_ov000_0236aad8.unk68 >> 8;
            oy = data_ov000_0236aad8.unk6C >> 8;
        }
        if (!(f & 0x2000)) {
            s2->x = (x >> 8) - ox;
            s2->y = (y >> 8) - oy;
            if (!(f & 0x4000)) {
                s2->x += data_022c4eec[0];
                s2->y += data_022c4eec[1];
            }
        }
    }
    if (o != NULL) {
        s->x += o->unk6;
        s->y += o->unk7;
        if (gObjManagerFlags & 0x100)
            s->y += (o->unk18 * data_0207cf78) >> 16;
        if (s2 != NULL) {
            s2->x += o->unk6;
            s2->y += o->unk7;
            if (gObjManagerFlags & 0x100)
                s2->y += (o->unk18 * data_0207cf78) >> 16;
        }
    }
    save = s->unk2A;
    s->unk2A = (save * data_0207cf7c) >> 8;
    if (s2 != NULL)
        s2->unk2A = s->unk2A;
    if (f & 0x10) {
        s->unk2A = 0;
        if (s2 != NULL)
            s2->unk2A = s->unk2A;
    }
    if (!(f & 0x1000)) {
        if (s->unk34 != NULL) {
            if (o != NULL)
                AnimatorSprite_ProcessAnimation(s, o->unk74, o);
            else
                AnimatorSprite_ProcessAnimation(s, NULL, NULL);
        }
        if (s2 != NULL && s2->unk34 != NULL)
            AnimatorSprite_ProcessAnimation(s2, NULL, NULL);
    }
    s->unk2A = save;
    if (s2 != NULL)
        s2->unk2A = save;
    sx = data_0207cf80[0] << 4;
    sy = data_0207cf80[1] << 4;
    if (o != NULL) {
        if (o->unk44 != 0)
            sx = (sx * o->unk44) >> 8;
        if (o->unk46 != 0)
            sy = (sy * o->unk46) >> 8;
    }
    if (data_0207cf80[0] != 0x100) {
        s->x = (s->x * data_0207cf80[0]) >> 8;
        if (s2 != NULL)
            s2->x = (s2->x * data_0207cf80[0]) >> 8;
    }
    if (data_0207cf80[1] != 0x100) {
        s->y = (s->y * data_0207cf80[1]) >> 8;
        if (s2 != NULL)
            s2->y = (s2->y * data_0207cf80[1]) >> 8;
    }
    if (!(f & 0x20)) {
        if ((f & 0x100) && rot != 0) {
            if (!busy)
                AnimatorSprite_DrawRotScale(s, sx, sy, rot << 8);
            if (s2 != NULL)
                AnimatorSprite_DrawRotScale(s2, sx, sy, rot << 8);
        } else {
            if (!busy) {
                if (p != NULL) {
                    if (sx == 0x1000 && sy == 0x1000)
                        func_0206b244(s, p);
                    else
                        func_0206aa38(s, p, sx, sy);
                } else {
                    if (sx == 0x1000 && sy == 0x1000)
                        AnimatorSprite_Draw(s);
                    else
                        AnimatorSprite_DrawRotScale(s, sx, sy, 0);
                }
            }
            if (s2 != NULL) {
                if (p != NULL) {
                    if (sx == 0x1000 && sy == 0x1000)
                        func_0206b244(s2, p);
                    else
                        func_0206aa38(s, p, sx, sy);
                } else {
                    if (sx == 0x1000 && sy == 0x1000)
                        AnimatorSprite_Draw(s2);
                    else
                        AnimatorSprite_DrawRotScale(s2, sx, sy, 0);
                }
            }
        }
    }
    if (busy)
        return;
    if (o == NULL)
        return;
    if (s->flags & 2)
        o->unk5C |= 8;
}
