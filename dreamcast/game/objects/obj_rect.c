/*
 * Hit box system: faithful translation of src/game/Stage/ObjectSystem/ObjRect_*.c
 * and StageObj_HandleCollider.
 *
 * Boxes are registered every frame into one of 4 lists ("next" lists) by
 * their mask low nibble; Stage_Main then checks the previous frame's lists
 * (ObjRect_CheckAllGroup) and swaps them (ObjRect_CheckOut).  A box's
 * callback is called when it is the defender (attacker atk vs. defender def,
 * ObjRect_FlagCheck) and the box has no flag 8.
 */
#include <string.h>
#include "obj_internal.h"

static ObjBox *s_next[4][OBJ_RECT_PER_LIST]; /* gObjRectNextList */
static ObjBox *s_cur[4][OBJ_RECT_PER_LIST];  /* gObjRectCurList */
static u8 s_next_count[4], s_cur_count[4];   /* gObjRectNextCount / CurCount */
static u8 s_next_group[4], s_cur_group[4];   /* gObjRectNextGroupFlags / CurGroupFlags */
static u16 s_prev_atk, s_prev_def;           /* gObjRectPrevAtkFlag / PrevDefFlag */
static u8 s_called_nohit;                    /* gObjRectCalledNoHit */

void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb) {
    s16 tmp;
    b->flags |= 4;
    b->l = l;
    b->r = r;
    b->t = t;
    b->b = bb;
    b->zn = -8;
    b->zf = 8;
    if (b->r < b->l) { tmp = b->l; b->l = b->r; b->r = tmp; }
    if (b->b < b->t) { tmp = b->t; b->t = b->b; b->b = tmp; }
}

void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf) {
    s16 tmp;
    b->flags |= 4;
    b->l = l;
    b->r = r;
    b->t = t;
    b->b = bb;
    b->zn = zn;
    b->zf = zf;
    if (b->r < b->l) { tmp = b->l; b->l = b->r; b->r = tmp; }
    if (b->b < b->t) { tmp = b->t; b->t = b->b; b->b = tmp; }
    if (b->zf < b->zn) { tmp = b->zn; b->zn = b->zf; b->zf = tmp; }
}

void rect_set_atkdef(ObjBox *b, u32 atk, u32 def) {
    b->atk = atk;
    b->def = def;
    b->flags &= ~0x200;
    b->flags &= ~0x400;
}

dcp_bool rect_flag_check(u32 a, u32 b) {
    if ((a & ~0x80000007u) & ~(b & ~7u)) {
        if ((a & 7) >= (b & 7))
            return DCP_TRUE;
    }
    return DCP_FALSE;
}

void rect_no_hit(ObjBox *a, ObjBox *b) {
    a->flags = s_prev_atk;
    b->flags = s_prev_def;
    s_called_nohit = 1;
}

void rect_register(ObjBox *s) {
    u8 i;
    if (!(s->flags & 4))
        return;
    for (i = 0; i < 4; i++) {
        if (s->mask & (1 << i)) {
            if (s_next_count[i] >= OBJ_RECT_PER_LIST)
                return;
            s_next[i][s_next_count[i]] = s;
            s_next_group[i] |= s->mask;
            s_next_count[i]++;
            return;
        }
    }
}

void rect_check_init(void) {
    memset(s_next, 0, sizeof(s_next));
    memset(s_cur, 0, sizeof(s_cur));
    memset(s_next_count, 0, sizeof(s_next_count));
    memset(s_cur_count, 0, sizeof(s_cur_count));
    memset(s_cur_group, 0, sizeof(s_cur_group));
    memset(s_next_group, 0, sizeof(s_next_group));
}

/* ObjRect_RegistGet (copied): the idx-th box of the current lists in mask */
void *rect_regist_get(u32 mask, s16 idx) {
    s16 i;
    for (i = 0; i < 4; i++) {
        if (mask & (1u << i)) {
            if (idx < s_cur_count[i])
                return s_cur[i][idx];
            idx -= s_cur_count[i];
        }
    }
    return NULL;
}

/* ObjRect_CheckFuncCall */
static u16 check_func_call(ObjBox *a, ObjBox *b) {
    u16 ret = 0;
    u16 f;
    s_prev_atk = a->flags;
    s_prev_def = b->flags;
    if (a == b)
        return 0;
    if ((a->flags & 0x200) && (b->flags & 0x100))
        return 0;
    if (rect_flag_check(a->atk, b->def)) {
        if (!(b->flags & 0x840) && !(a->flags & 0x840))
            a->flags |= 0x400;
        if (!(b->flags & 0x880) && !(a->flags & 0x880))
            b->flags |= 0x100;
        f = a->flags;
        if ((!(f & 0x800) || !(f & 0x80)) && (f & 8)) {
            if (f & 0x800)
                a->flags |= 0x80;
            if (a->cb != NULL)
                a->cb(a, b);
        }
        if (s_called_nohit) {
            s_called_nohit = 0;
            return 0;
        }
        { u16 t = s_prev_atk; s_prev_atk = s_prev_def; s_prev_def = t; }
        if (a->flags & 0x800)
            a->flags |= 0x40;
        f = b->flags;
        if ((!(f & 0x800) || !(f & 0x80)) && !(f & 8)) {
            if (f & 0x800)
                b->flags |= 0x80;
            if (b->cb != NULL)
                b->cb(b, a);
        }
        if (s_called_nohit) {
            s_called_nohit = 0;
            return 0;
        }
        if (b->flags & 0x800)
            b->flags |= 0x40;
        if (!(a->flags & 0x20))
            ret |= 1;
        if (!(b->flags & 0x20))
            ret |= 2;
    }
    return ret;
}

/* ObjRect_CheckGroup */
static void check_group(ObjBox **la, ObjBox **lb, u8 na, u8 nb, u8 bit) {
    u8 i, j;
    ObjBox *a, *b;
    s32 ax, ay, az, bx, by, bz;
    u16 aw, ah, ad, bw, bh, bd;
    u32 r;
    for (i = 0; i < na; i++) {
        a = la[i];
        if (a == NULL || (a->flags & 0x1000) || !(a->flags & 4) || !((a->mask >> 4) & (1 << bit)))
            continue;
        ax = (a->flags & 1) ? a->x - a->r : a->x + a->l;
        ay = (a->flags & 2) ? a->y - a->b : a->y + a->t;
        az = a->z + a->zn;
        aw = (u16)(a->r - a->l);
        ah = (u16)(a->b - a->t);
        ad = (u16)(a->zf - a->zn);
        for (j = 0; j < nb; j++) {
            b = lb[j];
            if (la[i] == NULL)
                break;
            if (b == NULL || ((b->flags | a->flags) & 0x1000) || !(b->flags & 4) || (void *)b == a->owner)
                continue;
            bx = (b->flags & 1) ? b->x - b->r : b->x + b->l;
            by = (b->flags & 2) ? b->y - b->b : b->y + b->t;
            bz = b->z + b->zn;
            bw = (u16)(b->r - b->l);
            bh = (u16)(b->b - b->t);
            bd = (u16)(b->zf - b->zn);
            if (((ax <= bx && ax + aw >= bx) || (ax >= bx && bx + bw >= ax)) &&
                ((ay <= by && ay + ah >= by) || (ay >= by && by + bh >= ay)) &&
                ((az <= bz && az + ad >= bz) || (az >= bz && bz + bd >= az))) {
                r = check_func_call(a, b);
                if (r & 1)
                    la[i] = NULL;
                if (r & 2)
                    lb[j] = NULL;
                r = check_func_call(b, a);
                if (r & 1)
                    lb[j] = NULL;
                if (r & 2)
                    la[i] = NULL;
            }
            if (b->flags & 0x400) {
                b->flags |= 0x200;
                b->flags &= ~0x400;
            }
        }
        if (a->flags & 0x400) {
            a->flags |= 0x200;
            a->flags &= ~0x400;
        }
    }
}

/* ObjRect_CheckOut */
static void check_out(void) {
    u8 i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < s_next_count[i]; j++) {
            s_cur[i][j] = s_next[i][j];
            s_next[i][j] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        s_cur_count[i] = s_next_count[i];
        s_next_count[i] = 0;
        s_cur_group[i] = s_next_group[i];
        s_next_group[i] = 0;
    }
}

/* ObjRect_CheckAllGroup */
void rect_check_all(void) {
    u8 i, j;
    ObjBox *c;
    s_prev_atk = 0;
    s_prev_def = 0;
    s_called_nohit = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < s_cur_count[i]; j++) {
            c = s_cur[i][j];
            if (c != NULL && (c->flags & 0x800))
                c->flags &= ~0x40;
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (s_cur_count[j] != 0 && (s_cur_group[i] & (0x10 << j)))
                check_group(s_cur[i], s_cur[j], s_cur_count[i], s_cur_count[j], j);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < s_cur_count[i]; j++) {
            c = s_cur[i][j];
            if (c != NULL && (c->flags & 0x800) && !(c->flags & 0x40))
                c->flags &= ~0x80;
        }
    }
    check_out();
}

/* forget a box that is being freed (native: the DS frees the task memory
 * but nothing reads it before the lists are rebuilt) */
void rect_forget(ObjBox *b) {
    u8 i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < OBJ_RECT_PER_LIST; j++) {
            if (s_cur[i][j] == b)
                s_cur[i][j] = NULL;
            if (s_next[i][j] == b)
                s_next[i][j] = NULL;
        }
}

dcp_bool rect_box_check(const ObjBox *a, s32 x, s32 y, s32 ox, s16 oy, u16 w, u16 h) {
    s32 ax, ay, px, py;
    u16 aw, ah;
    if ((a->flags & 4) && !(a->flags & 0x1000)) {
        ax = (a->flags & 1) ? a->x - a->r : a->x + a->l;
        ay = (a->flags & 2) ? a->y - a->b : a->y + a->t;
        py = y + oy;
        aw = (u16)(a->r - a->l);
        ah = (u16)(a->b - a->t);
        px = x + ox;
        if (((ax <= px && ax + aw >= px) || (ax >= px && px + w >= ax)) &&
            ((ay <= py && ay + ah >= py) || (ay >= py && py + h >= ay)))
            return DCP_TRUE;
    }
    return DCP_FALSE;
}

/* ObjRect_HitCenterX / _HitCenterY: middle of the two inner edges (24.8) */
static s32 hit_center(s32 v0, s32 v1, s32 v2, s32 v3) {
    s32 v[4];
    u8 i, k = 0, lo, hi;
    s32 m = 0, mn, d;
    v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3;
    hi = 0;
    if (v[1] > m) { m = v[1]; hi = 1; }
    if (v[2] > m) { m = v[2]; hi = 2; }
    if (v[3] > m) hi = 3;
    mn = v[0];
    lo = 0;
    if (v[1] < mn) { mn = v[1]; lo = 1; }
    if (v[2] < mn) { mn = v[2]; lo = 2; }
    if (v[3] < mn) lo = 3;
    for (i = 0; i < 4; i++) {
        if (i != hi && i != lo) {
            v[k] = v[i];
            if (k != 0)
                break;
            k++;
        }
    }
    d = (v[0] - v[1]) >> 1;
    if (d < 0)
        d = -d;
    m = v[0] > v[1] ? d + v[1] : d + v[0];
    return m << 8;
}
s32 rect_hit_center_x(const ObjBox *a, const ObjBox *b) { return hit_center(a->x + a->l, a->x + a->r, b->x + b->l, b->x + b->r); }
s32 rect_hit_center_y(const ObjBox *a, const ObjBox *b) { return hit_center(a->y + a->t, a->y + a->b, b->y + b->t, b->y + b->b); }

/* StageObj_HandleCollider: owner position -> box, flip from facing/angle, register */
void obj_handle_collider(DcPlayer *o, void *owner, int kind, ObjBox *s, int has_sprite) {
    (void)has_sprite; /* both branches of `unkA4 != NULL` are identical */
    if (o->obj_flags & 0xc)
        return;
    s->owner = owner;
    s->owner_kind = (u8)kind;
    s->x = o->x >> 8;
    s->y = o->y >> 8;
    s->z = (s16)(o->z >> 8);
    if (o->obj_flags & 2) {
        s->flags |= 0x1000;
        return;
    }
    s->flags &= ~0x1000;
    s->flags &= ~3;
    if (!(o->disp_flags & 1))
        s->flags |= 1;
    if (o->angle >= 0x60 && o->angle <= 0xa0)
        s->flags ^= 1;
    if ((o->disp_flags & 2) || o->angle == 0x80)
        s->flags |= 2;
    rect_register(s);
}

/* ObjRect_PointCheck (objdata v4: MapObj9C) */
dcp_bool rect_point_check(const ObjBox *b, s32 px, s32 py) {
    s32 left, top;
    u16 w, h;
    if ((b->flags & 4) && !(b->flags & 0x1000)) {
        if (b->flags & 1)
            left = b->x - b->r;
        else
            left = b->x + b->l;
        if (b->flags & 2)
            top = b->y - b->b;
        else
            top = b->y + b->t;
        w = (u16)(b->r - b->l);
        h = (u16)(b->b - b->t);
        if (left <= px && left + w >= px && top <= py && top + h >= py)
            return DCP_TRUE;
    }
    return DCP_FALSE;
}
