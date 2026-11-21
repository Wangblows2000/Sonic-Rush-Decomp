#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022e4b5c;

typedef struct {
    s32 x;
    s32 y;
    s32 idx;
    s32 type;
} UnkPt_022e4b5c;

typedef struct {
    s32 x;
    s32 y;
} UnkPos_022e4b5c;

typedef struct {
    s32 count;
    s32 type[8];
    UnkPos_022e4b5c pos[8];
} UnkGrp_022e4b5c;

typedef struct {
    s32 x;
    s32 y;
    s16 w;
    s16 h;
    s32 idx;
} UnkEnt_022e4b5c;

typedef struct {
    s32 x;
    s32 y;
} UnkCam_022e4b5c;

typedef struct {
    UnkPt_022e4b5c pts[17];
    UnkGrp_022e4b5c grp[22];
    UnkCam_022e4b5c *cam;
    UnkSpr_022e4b5c spr[21];
    UnkEnt_022e4b5c ent[7];
    UnkSpr_022e4b5c spr2[3];
    UnkSpr_022e4b5c spr3[8];
    s32 counter;
} UnkStruct_022e4b5c;

void AnimatorSprite_ProcessAnimation(UnkSpr_022e4b5c *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022e4b5c *s);
void ZoneMap_DrawNodeIcons(UnkStruct_022e4b5c *p);
s32 ZoneMap_EaseInterp(s32 a, s32 b, s32 n, s32 m, s32 t);

#pragma thumb on
void ZoneMap_DrawNodeLayer(UnkStruct_022e4b5c *p) {
    UnkSpr_022e4b5c *s0 = &p->spr[0];
    UnkSpr_022e4b5c *s1 = &p->spr[1];
    UnkSpr_022e4b5c *s;
    UnkPt_022e4b5c *pt;
    UnkGrp_022e4b5c *g;
    UnkEnt_022e4b5c *e;
    s32 i;
    s32 k;
    s32 t;
    s32 v;
    s32 sgn;
    s16 d;

    AnimatorSprite_ProcessAnimation(s0, 0, 0);
    AnimatorSprite_ProcessAnimation(s1, 0, 0);
    ZoneMap_DrawNodeIcons(p);
    for (i = 0, pt = p->pts; i < 17; pt++, i++) {
        if (pt->type == 7 || pt->type == 4 || pt->type == 5)
            continue;
        if (pt->type == 0) {
            s = &p->spr[pt->idx];
        } else if (pt->type == 1) {
            s = &p->spr[19];
        } else if (pt->type == 3) {
            s = &p->spr[19];
        } else if (pt->type == 2) {
            s = &p->spr[0];
        }
        s->x = pt->x - p->cam->x;
        s->y = pt->y - p->cam->y;
        if (s->x >= -0x20 && s->x < 0x120 && s->y >= -0x20 && s->y < 0xe0) {
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
            if (pt->type == 0 && (i == 4 || i == 5 || (u32)(i - 9) <= 2)) {
                s = &p->spr[19];
                s->x = pt->x - p->cam->x;
                s->y = pt->y - p->cam->y;
                AnimatorSprite_ProcessAnimation(s, 0, 0);
                AnimatorSprite_Draw(s);
            }
        }
    }
    for (i = 0; i < 22; i++) {
        g = &p->grp[i];
        for (k = 0; k < g->count; k++) {
            if (g->type[k] == 2)
                continue;
            if (g->type[k] == 0) {
                s = s0;
            } else if (g->type[k] == 1) {
                s = s1;
            }
            s->x = g->pos[k].x - p->cam->x;
            s->y = g->pos[k].y - p->cam->y;
            if (s->x >= -8 && s->x < 0x108 && s->y >= -8 && s->y < 200)
                AnimatorSprite_Draw(s);
        }
    }
    for (i = 0; i < 3; i++)
        AnimatorSprite_ProcessAnimation(&p->spr2[i], 0, 0);
    for (i = 0, e = p->ent; i < 7; e++, i++) {
        s = &p->spr2[e->idx];
        s->x = e->x - p->cam->x;
        s->y = e->y - p->cam->y;
        t = i * 240 / 7;
        t = (t + p->counter) % 240;
        switch (t / 60) {
        case 0:
            t = t % 60;
            sgn = t;
            break;
        case 1:
            t = t % 60;
            t = 59 - t;
            sgn = t;
            break;
        case 2:
            t = t % 60;
            sgn = -t;
            break;
        case 3:
            t = t % 60;
            t = 59 - t;
            sgn = -t;
            break;
        default:
            t = 0;
            break;
        }
        v = ZoneMap_EaseInterp(0, 6, 60, t, 0x2000);
        if (sgn < 0)
            v = -v;
        s->x += (s16)v;
        if (s->x >= -e->w && s->x < 0x100 && s->y >= -e->h && s->y < 0xc0)
            AnimatorSprite_Draw(s);
    }
    p->counter++;
}
#pragma thumb reset
