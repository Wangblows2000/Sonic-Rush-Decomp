/*
 * Rings: faithful translation of src/game/Objects/RingManager_*.c (2D stages)
 * and src/game/Effects/RingEffect2D_*.c (collect sparkle).
 *
 * One manager holds up to 64 ring parts (placed .rg rings and scattered
 * rings).  All rings share one animated sprite (ac_itm_ring.bac anim 0).
 */
#include <string.h>
#include "obj_internal.h"

#define ZABS_R(x) ((x) < 0 ? -(x) : (x))

typedef struct RingPart {  /* 0x1C bytes */
    s32 x, y;              /* +0x00 */
    s16 vx, vy;            /* +0x08 */
    s16 unkC, unkE;        /* +0x0C */
    s16 timer;             /* +0x10 */
    u16 flags;             /* +0x12 0x10 layer 1, 0x20 no timer, 0x100 no pickup, 0x200 no gravity,
                              0x400 static, 0x800 no map collision, 0x1000 scattered, 0x2000,
                              0x8000 gravity flipped */
    s32 unk14;             /* +0x14 */
    u8 *out;               /* +0x18 .rg entry byte 0 */
    const void *sucker;    /* +0x14 (objdata v4): the duct sucking this ring (RingManager_SuckScatteredRings) */
    u8 drawn;              /* native: drawn this frame (cb4) */
} RingPart;

typedef struct RingFx {    /* RingEffect2D work */
    s32 x, y;
    ObjSprite spr;
    u8 used;
} RingFx;

#define RING_FX_MAX 32

static struct {
    u32 flags;             /* +0x000 bit0 pause, bit1 scatter active, bit2 */
    int collide_obj;       /* cb10 = CollideObjMap (zone 2) */
    ObjSprite spr;         /* +0x014 spr0 */
    RingPart parts[OBJ_RING_MAX]; /* +0x1A0 */
    u8 count;              /* +0x9D4 */
    u8 scatter;            /* +0x9D5 */
    u8 nplayers;           /* +0x9D6 */
    RingFx fx[RING_FX_MAX];
    int active;
} R;

int ring_count(void) { return R.count; }
u8 *ring_scatter_level(void) { return &R.scatter; }
void ring_mark_scatter(void) { R.flags |= 2; }

/* RingManager_Create (normal 2D stage branch) */
void ring_init(void) {
    memset(&R, 0, sizeof(R));
    R.nplayers = 1;
    if (W.flags & 0x20)
        R.nplayers = 2;
    spr_init(&R.spr, &g_obj.res[OBJ_BAC_RING], 0, 0);
    R.spr.flags |= 0x4010;
    R.spr.order = 8;
    R.spr.flags |= 2u << 17; /* SpritePair_SetOAMPriority(.., 2) */
    R.active = 1;
}

/* RingManager_Destructor: live placed rings give their entry back to the
 * ring list (collected ones were freed with timer 0 and stay 0xff) */
void ring_destroy(void) {
    u32 i;
    RingPart *e = R.parts;
    for (i = 0; i < OBJ_RING_MAX; i++, e++)
        if (e->timer != 0 && e->out != NULL)
            *e->out = (u8)(e->x >> 8);
    R.active = 0;
}

static void place_common(RingPart *e, s32 a, s32 b) {
    if (obj_gravity_flipped(a, b))
        e->flags |= 0x8000;
    else
        e->flags &= ~0x8000;
    R.count++;
}

/* RingManager_PlaceRing */
void ring_place(u8 *flag, s32 a, s32 b) {
    u8 i;
    RingPart *e;
    if (!R.active || R.count >= OBJ_RING_MAX)
        return;
    for (i = 0; i < OBJ_RING_MAX; i++) {
        e = &R.parts[i];
        if (e->timer == 0) {
            memset(e, 0, sizeof(*e));
            e->x = a;
            e->y = b;
            e->flags = 0x620;
            e->timer = 0x80;
            *flag = 0xff;
            e->out = flag;
            place_common(e, a, b);
            g_obj.stats.rings_placed++;
            return;
        }
    }
}

/* RingManager_SpawnRing */
void ring_spawn(s32 a, s32 b, s16 c, s16 d, u16 f) {
    u8 i;
    RingPart *e;
    if (!R.active || R.count >= OBJ_RING_MAX)
        return;
    for (i = 0; i < OBJ_RING_MAX; i++) {
        e = &R.parts[i];
        if (e->timer == 0) {
            memset(e, 0, sizeof(*e));
            e->x = a;
            e->y = b;
            e->vx = c;
            e->vy = d;
            e->flags = f;
            e->timer = 0x100;
            e->timer += RAND() & 0x1f;
            e->out = NULL;
            place_common(e, a, b);
            return;
        }
    }
}

/* RingManager_CollideMap / RingManager_CollideObjMap */
static s32 ring_probe(int obj, s32 x, s32 y, u8 flag, u32 dir) {
    if (obj)
        return colmap_probe_distance(x, y, flag, dir, NULL, NULL);
    return rlvl_probe_distance(g_obj.map, x, y, flag, dir, NULL, NULL);
}
static void ring_collide(RingPart *s, int obj) {
    s32 r = 0;
    u8 flag;
    s16 vy = s->vy;
    if (s->flags & 0x800)
        return;
    flag = (s->flags & 0x10) ? 1 : 0;
    if (vy > 0) {
        r = ring_probe(obj, s->x >> 8, s->y >> 8, flag, 2);
        if (r < 0) {
            if (s->flags & 0x8000)
                s->y -= r << 8;
            else
                s->y += r << 8;
        }
    } else if (vy < 0) {
        r = ring_probe(obj, s->x >> 8, s->y >> 8, flag, 3);
        if (r < 0) {
            if (s->flags & 0x8000)
                s->y += r << 8;
            else
                s->y -= r << 8;
        }
    }
    if (r < 0) {
        s->vy -= s->vy >> 2;
        s->vy = (s16)-s->vy;
    }
    r = 0;
    if (s->vx > 0) {
        r = ring_probe(obj, s->x >> 8, s->y >> 8, flag, 0);
        if (r < 0)
            s->x += r << 8;
    } else if (s->vx < 0) {
        r = ring_probe(obj, s->x >> 8, s->y >> 8, flag, 1);
        if (r < 0)
            s->x -= r << 8;
    }
    if (r >= 0)
        return;
    s->vx -= s->vx >> 2;
    s->vx = (s16)-s->vx;
}

/* RingEffect2D_Create (RingManager_SpawnEffect2D) */
static void ring_fx_create(s32 x, s32 y) {
    int i;
    for (i = 0; i < RING_FX_MAX; i++)
        if (!R.fx[i].used) {
            RingFx *f = &R.fx[i];
            memset(f, 0, sizeof(*f));
            f->used = 1;
            f->x = x;
            f->y = y;
            spr_init(&f->spr, &g_obj.res[OBJ_BAC_RING], 1, 0);
            f->spr.flags |= 0x24010;
            spr_set_anim(&f->spr, 1);
            return;
        }
}

/* RingManager_Main, then the RingEffect2D tasks (same id, created later) */
void ring_main(void) {
    u8 i, j, left;
    dcp_bool ok = DCP_TRUE;
    RingPart *p;
    u16 save;
    int k;
    if (!R.active)
        return;
    for (i = 0; i < OBJ_RING_MAX; i++)
        R.parts[i].drawn = 0;
    if (R.count == 0)
        goto fx;
    if (W.zone == 2 && g_obj.n_players > 0) {
        if (g_obj.players[0].p->pf2 & 0x40000) {
            R.spr.flags = (R.spr.flags & ~0x60000u) | (1u << 17);
            R.collide_obj = 1;
        } else {
            R.spr.flags = (R.spr.flags & ~0x60000u) | (2u << 17);
            R.collide_obj = 0;
        }
    }
    if (R.flags & 1)
        goto fx;
    left = R.count;
    R.spr.flags &= ~SPR_STOPPED;
    spr_process(&R.spr, NULL, NULL);
    R.spr.flags |= SPR_STOPPED;
    for (i = 0; i < OBJ_RING_MAX; i++) {
        if (left == 0)
            break;
        p = &R.parts[i];
        if (p->timer == 0)
            continue;
        left--;
        if (obj_view_out(p->x, p->y, (p->flags & 0x400) ? 0x10 : 0xC4, 0, 0, 0, 0)) {
            p->timer = 1;
            p->flags &= ~0x20;
            if (p->out != NULL)
                *p->out = (u8)(p->x >> 8);
        } else if (!(p->flags & 0x100) && p->timer < 0xD8) {
            for (j = 0; j < R.nplayers && j < g_obj.n_players; j++) {
                ObjPlayerCtx *pc = &g_obj.players[j];
                save = pc->body.flags;
                pc->body.flags = save & ~0x1000;
                if (rect_box_check(&pc->body, p->x >> 8, p->y >> 8, -6, -16, 0x14, 0x10)) { /* RingManager_BoxCheck */
                    p->timer = 1;
                    if (p->flags & 0x1000) {
                        R.flags &= ~2u;
                        g_obj.stats.rings_scatter_collected++;
                    }
                    p->flags &= ~0x20;
                    dcp_add_rings(pc->p, 1);
                    g_obj.stats.rings_collected++;
                    obj_log("ring", p->x >> 8, p->y >> 8);
                    ring_fx_create(p->x, p->y); /* RingManager_SpawnEffect2D */
                }
                pc->body.flags = save;
            }
        }
        if (!(p->flags & 0x20))
            p->timer--;
        if ((p->flags & 0x2000) && !(R.flags & 4)) {
            p->flags &= ~0x2E00;
            p->vx = 0;
            p->vy = -0x100;
        }
        if (p->timer == 0) {
            R.count--;
            continue;
        }
        if (p->flags & 0x1000)
            ok = DCP_FALSE;
        if (!(p->flags & 0x400)) {
            p->x += p->vx;
            if (p->flags & 0x8000)
                p->y -= p->vy;
            else
                p->y += p->vy;
        }
        if (!(p->flags & 0x200))
            p->vy += g_obj.data.ring_gravity;
        ring_collide(p, R.collide_obj);
        if (p->timer > 0x20 || (p->timer & 2))
            p->drawn = 1; /* RingManager_DrawRing2D */
    }
    if ((R.flags & 2) && ok) {
        R.scatter = 0;
        R.flags &= ~2u;
    }
    R.flags &= ~4u;
fx:
    for (k = 0; k < RING_FX_MAX; k++) {
        RingFx *f = &R.fx[k];
        if (!f->used)
            continue;
        obj_draw2d_ex(f->x, f->y, NULL, &f->spr); /* RingEffect2D_Main */
        if (f->spr.flags & SPR_ENDED)
            f->used = 0;
    }
}

/* drawing helpers for obj_draw.c */
int ring_draw_count(void) { return OBJ_RING_MAX; }
int ring_draw_get(int i, s32 *x, s32 *y, int *vflip) {
    RingPart *p = &R.parts[i];
    if (!R.active || p->timer == 0 || !p->drawn)
        return 0;
    *x = p->x;
    *y = p->y;
    *vflip = (p->flags & 0x8000) != 0;
    return 1;
}
const ObjSprite *ring_draw_sprite(void) { return &R.spr; }
int ring_fx_get(int i, const ObjSprite **s) {
    if (i >= RING_FX_MAX || !R.fx[i].used)
        return 0;
    *s = &R.fx[i].spr;
    return 1;
}
int ring_fx_max(void) { return RING_FX_MAX; }

/* RingManager_AttractRings (copied): magnet barrier, called from Player_Last
 * (objects_update, after Player_Collide) while pf & 0x20000000 */
void RingManager_AttractRings(DcPlayer *src) {
    ObjBox r;
    RingPart *p;
    u8 left, i;
    s32 ang, v;
    if (!R.active)
        return;
    memset(&r, 0, sizeof(r));
    R.flags |= 4;
    r.x = src->x >> 8;
    r.y = src->y >> 8;
    rect_set_box2d(&r, -0x48, -0x48, 0x48, 0x48);
    left = R.count;
    for (i = 0; i < 64; i++) {
        if (left == 0)
            return;
        p = &R.parts[i];
        if (p->timer == 0)
            continue;
        left--;
        if (!(p->flags & 0x2000)) {
            if (rect_box_check(&r, p->x >> 8, p->y >> 8, -6, -16, 0x14, 0x10)) {
                p->flags |= 0x200;
                p->flags |= 0x400;
                p->flags |= 0x800;
                p->flags |= 0x2000;
                p->vx = 0;
                g_obj.stats.rings_attracted++;
            }
        }
        if (p->flags & 0x2000) {
            ang = dcp_atan2(r.y - (p->y >> 8), r.x - (p->x >> 8));
            p->vx = (s16)dcp_spd_up(p->vx, 0x40, 0x1F00);
            v = p->vx;
            p->x += (s16)((v * dcp_cos((u16)ang)) >> 12);
            p->y += (s16)((v * dcp_sin((u16)ang)) >> 12);
        }
    }
}

/* RingManager_SuckScatteredRings (objdata v4, Duct 0x97): scattered rings
 * within 0x80 px fly into the duct and vanish (Duct_State_CheckPlayer) */
dcp_bool RingManager_SuckScatteredRings(const DcPlayer *w) {
    ObjBox box;
    RingPart *ring;
    dcp_bool result = DCP_FALSE;
    u16 count, i, ang;
    s32 spd;
    if (!R.active)
        return DCP_FALSE;
    memset(&box, 0, sizeof(box));
    R.flags |= 4;
    box.x = w->x >> 8;
    box.y = w->y >> 8;
    rect_set_box2d(&box, -0x80, -0x80, 0x80, 0x80);
    count = R.count;
    for (i = 0; i < 0x40; i++) {
        if (count == 0)
            break;
        ring = &R.parts[i];
        if (ring->timer == 0)
            continue;
        count--;
        if ((ring->flags & 0x1000) && !(ring->flags & 0x4000) && ring->timer < 0xd8 &&
            rect_box_check(&box, ring->x >> 8, ring->y >> 8, -6, -0x10, 0x14, 0x10)) {
            result = DCP_TRUE;
            ring->flags |= 0x200;
            ring->flags |= 0x400;
            ring->flags |= 0x800;
            ring->flags |= 0x100;
            ring->flags |= 0x4000;
            ring->vx = 0;
            ring->unkC = 0x100;
            ring->unkE = 0x100;
            ring->sucker = w;
        }
        if ((ring->flags & 0x4000) && ring->sucker == w) {
            ang = (u16)dcp_atan2(box.y - (ring->y >> 8), box.x - (ring->x >> 8));
            ring->vx = (s16)dcp_spd_up(ring->vx, 0x10, 0x800);
            spd = ring->vx;
            ring->x += (s16)((spd * dcp_cos((u16)(ang + 0x1000))) >> 12);
            ring->y += (s16)((spd * dcp_sin((u16)(ang + 0x1000))) >> 12);
            ring->unkC -= 2;
            if (ring->unkC < 0x40)
                ring->unkC = 0x40;
            ring->unkE -= 2;
            if (ring->unkE < 0x40)
                ring->unkE = 0x40;
            if (ZABS_R(ring->x - w->x) < 0x400 && ZABS_R(ring->y - w->y) < 0x400)
                ring->timer = 0;
            result = DCP_TRUE;
        }
    }
    return result;
}
