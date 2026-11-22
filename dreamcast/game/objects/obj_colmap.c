/*
 * Solid objects: faithful translation of src/game/Stage/ObjectSystem/
 * ObjCollisionMap_*.c.  An object with a collision map (StageObj @0x1B8)
 * registers it every frame (MapObject_Collide_Default when in view);
 * Stage_Main commits the list (ObjCollisionMap_CommitFrame) and the player's
 * probes (StageObj_ProbeCollision -> g_dcp_col.obj_probe) scan it.
 */
#include <string.h>
#include "obj_internal.h"

static ObjColMap *s_next[OBJ_COLMAP_MAX]; /* data_022c4de4 */
static ObjColMap *s_cur[OBJ_COLMAP_MAX];  /* data_022c4e64 */
static u8 s_next_n;                       /* data_022c4de0 */
static u8 s_cur_n;                        /* data_022c4ddc */

void colmap_reset(void) {
    memset(s_next, 0, sizeof(s_next));
    memset(s_cur, 0, sizeof(s_cur));
    s_next_n = s_cur_n = 0;
}

void colmap_register(ObjColMap *e) {
    if (s_cur_n >= OBJ_COLMAP_MAX) /* (the original compares the committed count) */
        return;
    if (e->obj->h.obj_flags & 0xc)
        return;
    if (e->rider != NULL && e->rider->ride != &e->obj->ref)
        e->rider = NULL;
    if (e->toucher != NULL && e->toucher->touch != &e->obj->ref)
        e->toucher = NULL;
    if (s_next_n >= OBJ_COLMAP_MAX)
        return; /* native guard: the DS array would overflow */
    s_next[s_next_n++] = e;
}

void colmap_commit(void) {
    u8 i = 0, n = s_next_n;
    for (; i < n; i++)
        s_cur[i] = s_next[i];
    for (; i < OBJ_COLMAP_MAX; i++)
        s_next[i] = NULL;
    s_cur_n = n;
    s_next_n = 0;
}

void colmap_forget(ObjColMap *e) {
    u8 i;
    for (i = 0; i < OBJ_COLMAP_MAX; i++) {
        if (s_cur[i] == e)
            s_cur[i] = NULL;
        if (s_next[i] == e)
            s_next[i] = NULL;
    }
}

/* ObjCollisionMap_GetOffset */
static void get_offset(const ObjColMap *e, s16 *ox, s16 *oy) {
    *ox = e->ox;
    *oy = e->oy;
    if (e->obj == NULL)
        return;
    if (e->obj->h.disp_flags & 1)
        *ox = (s16)-(e->ox + (e->w << 3));
    if (e->obj->h.disp_flags & 2)
        *oy = (s16)-(e->oy + (e->h << 3));
}

/* shared body of ObjCollisionMap_GetTileHeight (vert=1) / _GetTileWidth (vert=0) */
static s8 get_tile(const ObjColMap *e, s32 x, s32 y, u32 flags, u8 *out_ang, u8 *out_attr, int vert) {
    s8 v;
    u16 col, row, idx;
    s16 ox = 0, oy = 0;
    u32 sub;
    u8 a;
    const Obj *o = e->obj;
    s32 obx = o->h.x >> 8, oby = o->h.y >> 8;
    u16 df = o->h.disp_flags;
    get_offset(e, &ox, &oy);
    if (x < obx + ox || x >= obx + ox + e->w * 8)
        return 0;
    if (y < oby + oy || y >= oby + oy + e->h * 8)
        return 0;
    col = (u16)((u32)(x - obx - ox) >> 3);
    row = (u16)((u32)(y - oby - oy) >> 3);
    if (df & 1)
        col = (u16)(e->w - 1 - col);
    if (df & 2)
        row = (u16)(e->h - 1 - row);
    idx = (u16)(row * e->w + col);
    if (vert) {
        sub = (u32)(x - obx - ox) & 7;
        if (df & 1)
            sub = 7 - sub;
        v = (s8)((u32)e->map[idx][sub] >> 4);
    } else {
        sub = (u32)(y - oby - oy) & 7;
        if (df & 2)
            sub = 7 - sub;
        v = (s8)(e->map[idx][sub] & 0xf);
    }
    if (v & 8)
        v = (s8)(v | ~0xf);
    if (v == -8)
        v = 8;
    if (e->attr != NULL && (flags & 0x80) && (*e->attr & 1))
        v = 0;
    if ((df & (vert ? 2 : 1)) && v != 8 && v != 0)
        v = (s8)(v > 0 ? v - 8 : v + 8);
    if (out_ang != NULL && v != 0) {
        int has_ang = e->angles != NULL || e->angles_ff;
        a = (e->angles != NULL && !e->angles_ff) ? e->angles[idx] : 0;
        if (e->flags & 1)
            a = (u8)(a + o->h.angle);
        if (has_ang) {
            if (vert) {
                if (!(df & 1))
                    a = (u8)-(s8)a;
                if (df & 2)
                    a = (u8)(-0x80 - (s8)a);
            } else {
                if (!(df & 1)) /* (same order as GetTileHeight in the original) */
                    a = (u8)-(s8)a;
                if (df & 2)
                    a = (u8)(-0x80 - (s8)a);
            }
        }
        *out_ang = a;
    }
    if (out_attr == NULL || v == 0)
        return v;
    if (e->attr != NULL)
        *out_attr = *e->attr;
    return v;
}
static s32 tile_h(const ObjColMap *e, s32 y, s32 x, u32 f, u8 *a, u8 *b) { return get_tile(e, x, y, f, a, b, 1); }
static s32 tile_w(const ObjColMap *e, s32 x, s32 y, u32 f, u8 *a, u8 *b) { return get_tile(e, x, y, f, a, b, 0); }

static s32 calc(s32 h, s32 step, s32 sub) {
    if (h > 0)
        return step > 0 ? h - (sub + 1) : 8 - sub;
    return step > 0 ? -(sub + 1) : h + sub;
}

/* ObjCollisionMap_ProbeEntry */
static s32 probe_entry(const ObjColMap *c, s32 x, s32 y, u32 flags, u8 mode, u8 *oa, u8 *ob) {
    u8 sa = 0, sb = 0;
    s32 dir = 8, v;
    s8 sub;
    s16 ox = 0, oy = 0;
    get_offset(c, &ox, &oy);
    if (oa) sa = *oa;
    if (ob) sb = *ob;
    if (mode & 1)
        dir = -8;
    if (mode & 2) {
        v = tile_h(c, y, x, flags, oa, ob);
        sub = (s8)((y - ((c->obj->h.y >> 8) + oy)) & 7);
    } else {
        v = tile_w(c, x, y, flags, oa, ob);
        sub = (s8)((x - ((c->obj->h.x >> 8) + ox)) & 7);
    }
    if (v == 0) {
        if (oa) *oa = sa;
        if (ob) *ob = sb;
        return dir > 0 ? 8 - sub : sub + 1;
    }
    if (v == 8)
        return dir > 0 ? -(sub + 1) : sub - 8;
    if (v > 0)
        return dir > 0 ? v - (sub + 1) : 8 - sub;
    return dir > 0 ? -(sub + 1) : v + sub;
}

typedef s32 (*TileFn)(const ObjColMap *, s32, s32, u32, u8 *, u8 *);

/* ObjCollisionMap_ProbeEntryLong */
static s32 probe_entry_long(const ObjColMap *e, s32 x, s32 y, u32 flags, u8 dir, u8 *oa, u8 *ob) {
    s32 sa = 0, sb = 0, sub, off = 0, step = 8, h, t;
    s16 ox = 0, oy = 0;
    TileFn fn;
    get_offset(e, &ox, &oy);
    if (oa) sa = *oa;
    if (ob) sb = *ob;
    if (dir & 1)
        step = -8;
    if (dir & 2) {
        sub = (s8)((y - ((e->obj->h.y >> 8) + oy)) & 7);
        fn = tile_h;
        t = x; x = y; y = t;
        h = tile_h(e, x, y, flags, oa, ob);
    } else {
        fn = tile_w;
        h = tile_w(e, x, y, flags, oa, ob);
        sub = (s8)((x - ((e->obj->h.x >> 8) + ox)) & 7);
    }
    if (h == 0) {
        off += step;
        h = fn(e, x + off, y, flags, oa, ob);
        if (h == 0) {
            h = fn(e, x + (off + step), y, flags, oa, ob);
            if (h == 0) {
                if (oa) *oa = (u8)sa;
                if (ob) *ob = (u8)sb;
                return (step > 0 ? 8 - sub : sub + 1) + 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) + 16;
            return calc(h, step, sub) + 16;
        }
        if (h == 8)
            return (step > 0 ? -(sub + 1) : sub - 8) + 8;
        return calc(h, step, sub) + 8;
    } else if (h == 8) {
        if (oa) sa = *oa;
        if (ob) sb = *ob;
        off -= step;
        h = fn(e, x + off, y, flags, oa, ob);
        if (h == 8) {
            if (oa) sa = *oa;
            if (ob) sb = *ob;
            h = fn(e, x + (off - step), y, flags, oa, ob);
            if (h == 0) {
                if (oa) *oa = (u8)sa;
                if (ob) *ob = (u8)sb;
                if (step > 0)
                    sub = 8 - (sub + 1);
                return sub - 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) - 16;
            return calc(h, step, sub) - 16;
        }
        if (h == 0) {
            if (oa) *oa = (u8)sa;
            if (ob) *ob = (u8)sb;
            if (step > 0)
                sub = 8 - (sub + 1);
            return sub - 8;
        }
        return calc(h, step, sub) - 8;
    }
    return calc(h, step, sub);
}

/* rotate (x,y) px into the object's frame (MTX_RotZ33_ with -angle) */
static void rotate_into(const Obj *o, s32 *x, s32 *y) {
    s32 vx = ((*x << 8) - o->h.x) << 4, vy = ((*y << 8) - o->h.y) << 4;
    u16 a = (u16)(-(o->h.angle << 8));
    s32 s = dcp_sin(a), c = dcp_cos(a);
    /* MTX_RotZ33_(m, s, c): [c s; -s c] row vectors, MTX_MultVec33: v * m */
    s32 nx = (s32)(((s64)vx * c - (s64)vy * s) >> 12);
    s32 ny = (s32)(((s64)vx * s + (s64)vy * c) >> 12);
    *x = ((nx >> 4) + o->h.x) >> 8;
    *y = ((ny >> 4) + o->h.y) >> 8;
}

/* ObjCollisionMap_ProbeForObj: probe for actor a (the player) */
s32 colmap_probe_for_obj(DcPlayer *a, s32 x, s32 y, u32 p3, u32 dir, u8 *out, u8 *p6) {
    const ObjColMap *e;
    ObjColMap *em;
    u32 f;
    s32 nf = 0, best = 0x18, d, sx = x, sy = y;
    s16 ox = 0, oy = 0;
    u8 i;
    dcp_bool flag = DCP_FALSE;
    if (s_cur_n == 0)
        return best;
    f = a->move_flags & 0x10;
    if (f == 0)
        nf = 1;
    switch ((s32)((a->grav_rot + 0x20) & 0xc0) >> 6) {
    case 0: if (dir == 2 && (f == 0 || a->move_y >= 0)) flag = DCP_TRUE; break;
    case 1: if (dir == 1 && (f == 0 || a->move_x < 0)) flag = DCP_TRUE; break;
    case 2: if (dir == 3 && (f == 0 || a->move_y < 0)) flag = DCP_TRUE; break;
    case 3: if (dir == 0 && (f == 0 || a->move_x > 0)) flag = DCP_TRUE; break;
    }
    for (i = 0; i < s_cur_n; i++) {
        em = s_cur[i];
        e = em;
        if (e == NULL || e->obj == NULL || &e->obj->h == a || e->w == 0 || e->h == 0)
            continue;
        x = sx;
        y = sy;
        get_offset(e, &ox, &oy);
        if ((e->flags & 1) && e->obj->h.angle != 0)
            rotate_into(e->obj, &x, &y);
        if (x < (e->obj->h.x >> 8) - 0x18 + ox || x > ox + ((e->obj->h.x >> 8) + e->w * 8 + 0x18))
            continue;
        if (y < (e->obj->h.y >> 8) - 0x18 + oy || y > oy + ((e->obj->h.y >> 8) + e->h * 8 + 0x18))
            continue;
        d = probe_entry_long(e, x, y, p3, (u8)dir, out, p6);
        if (best <= d)
            continue;
        best = d;
        if (d <= nf) {
            a->touch = &e->obj->ref;
            em->toucher = a;
        }
        if (d <= nf && flag) {
            a->ride = &e->obj->ref;
            em->rider = a;
            if (out != NULL && a->grav_rot != 0)
                *out = (u8)(*out + a->grav_rot);
        } else if (out != NULL && a->grav_rot != 0) {
            *out = a->grav_rot;
        }
    }
    return best;
}

/* ObjCollisionMap_ProbeDistance (no actor; RingManager_CollideObjMap) */
s32 colmap_probe_distance(s32 x, s32 y, u32 layer, u32 dir, u8 *oa, u8 *ob) {
    s32 best = 0x18, h, sx = x, sy = y;
    s16 ox = 0, oy = 0;
    u8 i;
    const ObjColMap *l;
    for (i = 0; i < s_cur_n; i++) {
        l = s_cur[i];
        if (l == NULL || l->obj == NULL || l->w == 0 || l->h == 0)
            continue;
        x = sx;
        y = sy;
        get_offset(l, &ox, &oy);
        if ((l->flags & 1) && l->obj->h.angle != 0)
            rotate_into(l->obj, &x, &y);
        if (x < (l->obj->h.x >> 8) - 0x18 + ox || x > ox + ((l->obj->h.x >> 8) + (l->w << 3) + 0x18))
            continue;
        if (y < (l->obj->h.y >> 8) - 0x18 + oy || y > oy + ((l->obj->h.y >> 8) + (l->h << 3) + 0x18))
            continue;
        h = probe_entry(l, x, y, layer, (u8)dir, oa, ob);
        if (best > h)
            best = h;
    }
    return best;
}
