/*
 * Stage camera: src/game/Camera/Camera_*.c, Player_UpdateCamera(+Lead) and
 * ScreenShake_*, translated for the Dreamcast port (camera.h, CAMERA.md).
 *
 * gCamera layout (0x140 bytes at data_ov000_0236aad8):
 *   layer i (i = 0, 1) at +i*0x64: flags @00 (g_obj_camera[i].flags), x @04, y @08,
 *   prev @0C/@10, v @14/@16, shake @18/@1A, max speed @1C/@1D, w @1E, scale @20,
 *   angle @22, BG chr buffers @24/@28 (not needed), BG scroll @2C/@30, flags34 @34,
 *   limit @38, player @3A, offset @3C/@40, lead target @44/@48, bounds @4C..@58,
 *   bound locks @5C, area objects @60, water @62 (all but the positions in g_obj_camera);
 *   globals at +0xC8: flags (g_obj_camera[2].flags), prev @CC, gap/partner @D0..@D8,
 *   split override @DC, map bounds @114 (= g_obj_camera[2].bound), map info @124.
 */
#include <string.h>
#include "camera.h"
#include "../objects/objects.h"
#include "../objects/obj_internal.h"
#include "../../../tools/dreamcast/stages/level_format.h"

extern float g_cam_x, g_cam_y;   /* render.h: drawing window (world px) */

#define C(i) (g_obj_camera[i])
#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define ABS(v) ((v) < 0 ? -(v) : (v))

static CamLayer s_l[3];          /* 0, 1 = gCamera[0..1]; 2 = the DC window (single-screen gCamera[0]) */
static CamGlobal s_g;
static CamLook s_look;
static CamStats s_stats;
static DcPlayer *s_pl[2];        /* gPlayers[] the cameras follow */
static const u8 *s_split;        /* gCamera @0x13C: split-line map (2 bits per 64 px block) */
static u16 s_split_w, s_split_h;
static u8 s_zone, s_act;
static int s_inited;
static void (*s_zone_hook)(void);
/* Player_UpdateCameraLead work @0x3D2..@0x3D8 (per player; the object module reuses
 * the same player fields for the swing, so the camera keeps its own copy) */
static struct { s16 lead_x, lead_y, tgt_x, tgt_y; } s_lead[2];
/* Altitude Limit ZoneFx_Zone6 work (gZoneFxWork: frame counter) */
static s32 s_z6_t;
static int s_z6_on;
/* the window: raw single-screen camera (s_l[2]) -> eased (s_win_*) */
static s32 s_win_x, s_win_y, s_raw_prev_y;
static int s_win_snap, s_respawn;
/* ScreenShake task work */
static struct { u16 flags; s16 t; u8 mode; s8 x, y; } s_shake;
static s8 s_shk3c[4], s_shk40[12], s_shk4c[22];

static void init_dual(DcPlayer *p, int full);
static void place_at_player(DcPlayer *p);
static u32 F2(void) { return g_obj_camera[2].flags | s_g.dual; }
static DcPlayer *player_of(int i) {
    s8 k = C(i).player;
    return (k >= 0 && k < 2) ? s_pl[(int)k] : NULL;
}

/* ================================================================ accessors */
const CamLayer *camera_layer(int i) { return &s_l[i]; }
CamLayer *camera_layer_rw(int i) { return &s_l[i]; }
const CamGlobal *camera_global(void) { return &s_g; }
u32 camera_flags2(void) { return F2(); }
const CamStats *camera_stats(void) { return &s_stats; }
void camera_set_zone_hook(void (*fn)(void)) { s_zone_hook = fn; }
void camera_get_view(s32 *x, s32 *y) { *x = s_win_x; *y = s_win_y; }
/* gCamera[1]: the DS's other screen (spawning only; never drawn) */
void camera_get_partner_view(s32 *x, s32 *y) { *x = C(1).x; *y = C(1).y; }


/* ================================================================ Stage_GetSplitLineY */
s32 Stage_GetSplitLineY(s32 x) {
    u16 i, start = 0;
    int found = 0;
    u32 idx;
    s32 v, cx;
    if (s_split == NULL)
        return 0;
    if (x < 0) {
        cx = 0;
    } else {
        cx = (s_g.map_w << 8) - 1;
        if (x <= cx)
            cx = x;
    }
    for (i = 0; i < s_split_h; i++) {
        idx = (u32)i * s_split_w + (u32)(cx >> 14);
        v = (s_split[(u16)(idx >> 2)] >> (u16)((idx & 3) << 1)) & 3;
        if (!found && v != 0) {
            if (v == 1)
                return (s32)i << 14;
            found = 1;
            start = i;
        }
        if (found && v == 1)
            return (s32)(start + i) << 13;
    }
    return 0;
}

/* Camera_GetSplitLineScreenY: the water / quicksand line in screen rows */
u8 Camera_GetSplitLineScreenY(s32 idx) {
    s16 v;
    if (C(idx).flags & 0x1000000) {
        v = (s16)((((s32)C(idx).water << 8) - s_l[idx].y) / C(idx).h);
        return (u8)CLAMP(v, 0, 0xc0);
    }
    return 0xc0;
}
/* the same for the DC window (drawing: water surface, underwater palette split) */
s32 camera_window_split_y(void) {
    s32 v;
    if (!(C(0).flags & 0x1000000))
        return 0xc0;
    v = (((s32)C(0).water << 8) - s_win_y) / C(0).h;
    return CLAMP(v, 0, 0xc0);
}

/* ================================================================ locks, area, lead */
void Camera_LockX(s32 i) { C(i).flags |= 0x40; }
void Camera_LockY(s32 i) { C(i).flags |= 0x80; }
void Camera_UnlockX(s32 i) { C(i).flags &= ~0x40u; }
void Camera_UnlockY(s32 i) { C(i).flags &= ~0x80u; }

void Camera_SetLeadTarget(s32 idx, s32 a, s32 b) {
    C(idx).def_ofs_x = a;
    C(idx).def_ofs_y = b;
    if (C(idx).flags & 0x3000)
        return;
    C(idx).ofs_x = C(idx).def_ofs_x;
    C(idx).ofs_y = C(idx).def_ofs_y;
}
void Camera_SetLeadTargetPrimary(s32 a, s32 b) { Camera_SetLeadTarget(0, a, b); }

u8 Camera_GetPlayerScreenMask(const DcPlayer *obj) {
    u8 i, r;
    if (obj == NULL)
        return 0;
    for (i = 0; i < 2; i++) {
        if (s_pl[i] == obj) {
            r = 0;
            if (C(0).player == i)
                r |= 1;
            if (C(1).player == i)
                r |= 2;
            if (r != 0)
                return r;
        }
    }
    return 0;
}

/* Camera_GetViewOrigin / Camera_WorldToScreen */
void Camera_WorldToScreen(int layer, s32 x, s32 y, s16 *ox, s16 *oy) {
    const CamLayer *l = &s_l[layer];
    s32 bx = l->x + l->vx + (l->sx << 8);
    s32 by = l->y + l->vy + (l->sy << 8);
    *ox = (s16)((x - bx) >> 8);
    *oy = (s16)((y - by) >> 8);
}

/* ================================================================ follow / clamp */
static void Camera_FollowPlayerX(int i) {
    DcPlayer *p = player_of(i);
    if ((C(i).flags & 0x10000) && p)
        s_l[i].x = p->x - C(i).ofs_x;
}
static void Camera_FollowPlayerY(int i) {
    DcPlayer *p = player_of(i);
    if ((C(i).flags & 0x20000) && p)
        s_l[i].y = p->y - C(i).ofs_y;
}
static void Camera_ClampXToBounds(int i) {
    s32 lo = C(i).bound[0], hi = C(i).bound[2] - (C(i).w << 8), x = s_l[i].x;
    s_l[i].x = x < lo ? lo : (x <= hi ? x : hi);
}
static void Camera_ClampXToMapBounds(int i) {
    s32 lo = s_g.map_bound[0], hi = s_g.map_bound[2] - (C(i).w << 8), x = s_l[i].x;
    s_l[i].x = x < lo ? lo : (x <= hi ? x : hi);
}
static void Camera_ClampYToBounds(int i, u32 flags) {
    s32 *y = &s_l[i].y;
    if (flags & 0x200)
        return;
    if (flags & 1) {
        if (!(flags & 2)) {
            *y = CLAMP(*y, C(i).bound[1], C(i).bound[3] - C(i).h * 0x1d0);
            return;
        }
        *y = CLAMP(*y, C(i).bound[1] + C(i).h * 0x110, C(i).bound[3] - C(i).h * 0xc0);
        return;
    }
    *y = CLAMP(*y, C(i).bound[1], C(i).bound[3] - C(i).h * 0xc0);
}
static void Camera_ClampYToMapBounds(int i, u32 flags) {
    s32 *y = &s_l[i].y;
    if (flags & 0x200)
        return;
    if (flags & 1) {
        if (!(flags & 2)) {
            *y = CLAMP(*y, s_g.map_bound[1], s_g.map_bound[3] - C(i).h * 0x1d0);
            return;
        }
        *y = CLAMP(*y, s_g.map_bound[1] + C(i).h * 0x110, s_g.map_bound[3] - C(i).h * 0xc0);
        return;
    }
    *y = CLAMP(*y, s_g.map_bound[1], s_g.map_bound[3] - C(i).h * 0xc0);
}
static void Camera_ClampXAroundTarget(int i) {
    CamLayer *p = &s_l[i];
    if (F2() & 0x200)
        return;
    if (!(C(i).flags & 0x100000))
        return;
    if (p->x > (p->spd_x << 8) + p->px) {
        p->x = (p->spd_x << 8) + p->px;
        return;
    }
    if (p->x < p->px - (p->spd_x << 8))
        p->x = p->px - (p->spd_x << 8);
    else
        C(i).flags |= 0x400000;
}
static void Camera_ClampYAroundTarget(int i) {
    CamLayer *p = &s_l[i];
    if (F2() & 0x200)
        return;
    if (!(C(i).flags & 0x200000))
        return;
    if (p->y > (p->spd_y << 8) + p->py) {
        p->y = (p->spd_y << 8) + p->py;
        return;
    }
    if (p->y < p->py - (p->spd_y << 8))
        p->y = p->py - (p->spd_y << 8);
    else
        C(i).flags |= 0x800000;
}
static void Camera_UpdateX(int i) {
    if (!(C(i).flags & 0x4010))
        Camera_FollowPlayerX(i);
    Camera_ClampXToBounds(i);
}

/* ================================================================ look up / down */
static void Camera_Look_Idle(CamLook *w);
static void Camera_Look_Reset(CamLook *w) {
    g_obj_camera[2].flags &= ~0x10000u;
    g_obj_camera[2].flags &= ~0x20000u;
    w->flags &= ~2u;
    w->flags &= ~4u;
    w->timer = 0;
    w->speed = 0;
    w->fn = Camera_Look_Idle;
}
static void Camera_Look_ReturnFromUp(CamLook *w) {
    g_obj_camera[2].flags |= 0x20000;
    w->speed = (C(0).h << 12) >> 8;
}
static void Camera_Look_ReturnFromDown(CamLook *w) {
    g_obj_camera[2].flags |= 0x20000;
    w->speed = -((C(0).h << 12) >> 8);
}
static void Camera_Look_PanUp(CamLook *w) {
    DcPlayer *o = player_of(0);
    if (g_obj_camera[2].flags & 0x40000) {
        w->speed = 0;
        w->fn = Camera_Look_ReturnFromUp;
        g_obj_camera[2].flags |= 0x20000;
        return;
    }
    if (o == NULL || !(o->held & 0x40) || (o->anim != 0x21 && o->anim != 0x22)) {
        w->speed = 0;
        w->fn = Camera_Look_ReturnFromUp;
        g_obj_camera[2].flags |= 0x20000;
        return;
    }
    w->speed = -((C(0).h << 10) >> 8);
}
static void Camera_Look_PanDown(CamLook *w) {
    DcPlayer *o = player_of(0);
    if (g_obj_camera[2].flags & 0x40000) {
        w->speed = 0;
        w->fn = Camera_Look_ReturnFromDown;
        g_obj_camera[2].flags |= 0x20000;
        return;
    }
    if (o == NULL || !(o->held & 0x80) || (o->anim != 3 && o->anim != 4)) {
        w->speed = 0;
        w->fn = Camera_Look_ReturnFromDown;
        g_obj_camera[2].flags |= 0x20000;
        return;
    }
    w->speed = (C(0).h << 10) >> 8;
}
static void Camera_Look_Wait(CamLook *w) {
    DcPlayer *p = player_of(0);
    u16 fl;
    if (g_obj_camera[2].flags & 0x40000) {
        Camera_Look_Reset(w);
        return;
    }
    if (p == NULL) {
        Camera_Look_Reset(w);
        return;
    }
    fl = p->held;
    if (fl & 1) {
        Camera_Look_Reset(w);
        return;
    }
    if ((w->flags & 2) && (!(fl & 0x40) || (p->anim != 0x21 && p->anim != 0x22))) {
        Camera_Look_Reset(w);
        return;
    }
    if ((w->flags & 4) && (!(fl & 0x80) || (p->anim != 3 && p->anim != 4))) {
        Camera_Look_Reset(w);
        return;
    }
    w->timer++;
    if (w->timer < 0x3c)
        return;
    w->timer = 0;
    g_obj_camera[2].flags |= 0x10000;
    w->speed = 0;
    w->fn = (w->flags & 2) ? Camera_Look_PanUp : Camera_Look_PanDown;
}
static void Camera_Look_Idle(CamLook *w) {
    DcPlayer *o = player_of(0);
    if (o == NULL)
        return;
    if ((o->held & 0x40) && (u16)(s16)(o->anim - 0x21) <= 1) {
        w->flags |= 2;
        w->timer = 0;
        w->fn = Camera_Look_Wait;
    }
    if ((o->held & 0x80) && (u16)(s16)(o->anim - 3) <= 1) {
        w->flags |= 4;
        w->timer = 0;
        w->fn = Camera_Look_Wait;
    }
}
static void Camera_RunLookCallback(CamLook *w) {
    if (g_obj_camera[2].flags & 0x100)
        return;
    if (w->fn)
        w->fn(w);
}

/* Camera_ClampLookPanY (single screen): the pan stops 0xa8 / 0x18 px from the player */
static int Camera_ClampLookPanY(int i, s32 *y) {
    DcPlayer *pl = player_of(i);
    s32 lim = pl ? pl->y : *y;
    u32 f = g_obj_camera[2].flags & 0x20000;
    if ((!f && (s_look.flags & 2)) || (f && (s_look.flags & 4))) {
        if (*y <= C(i).bound[1]) {
            *y = C(i).bound[1];
            return 1;
        }
        lim -= C(i).h * 0xa8;
        if (*y <= lim) {
            *y = lim;
            return 1;
        }
    } else {
        if (*y >= C(i).bound[3]) {
            *y = C(i).bound[3];
            return 1;
        }
        lim -= C(i).h * 0x18;
        if (*y >= lim) {
            *y = lim;
            return 1;
        }
    }
    return 0;
}
static int Camera_ClampLookPanYDual(void) {
    DcPlayer *pl = player_of(0);
    u32 fl = F2();
    s32 v, *y = &s_l[0].y;
    s32 py = pl ? pl->y : *y;
    if (fl & 0x200)
        return 0;
    if (!(fl & 2)) {
        if ((!(fl & 0x20000) && (s_look.flags & 2)) || ((fl & 0x20000) && (s_look.flags & 4))) {
            if (*y <= C(0).bound[1]) {
                *y = C(0).bound[1];
                return 1;
            }
        } else {
            v = py - C(0).h * 0x60;
            if (*y >= v) {
                *y = v;
                return 1;
            }
        }
        return 0;
    }
    if ((!(fl & 0x20000) && (s_look.flags & 2)) || ((fl & 0x20000) && (s_look.flags & 4))) {
        v = py - C(0).h * 0x58;
        if (*y <= v) {
            *y = v;
            return 1;
        }
    } else {
        v = C(0).bound[3] - C(0).h * 0xc0;
        if (*y >= v) {
            *y = v;
            return 1;
        }
    }
    return 0;
}
/* Camera_HasReturnedToPlayerY(Dual): the offset back where the lead target is */
static int has_returned(int i, s32 y) {
    DcPlayer *pl = player_of(i);
    if (pl == NULL)
        return 1;
    if (s_look.speed >= 0)
        return y + C(i).def_ofs_y >= pl->y;
    return y + C(i).def_ofs_y <= pl->y;
}

/* ================================================================ screen swap */
static int Camera_SwapScreens(int on) {
    CamLayer *p = &s_l[0], *q = &s_l[1], t;
    s16 w, h;
    if (!on) {
        if (!(F2() & 2))
            return 0;
        g_obj_camera[2].flags |= 0x200;
        g_obj_camera[2].flags &= ~2u;
        g_obj_camera[2].flags &= ~0x400u;
    } else {
        if (F2() & 2)
            return 0;
        g_obj_camera[2].flags |= 0x200;
        g_obj_camera[2].flags |= 2;
        g_obj_camera[2].flags |= 0x400;
    }
    /* a[] x y, b[] prev x y, c[] BG scroll, d[] w h, e angle */
    t = *p;
    p->x = q->x; p->y = q->y; p->px = q->px; p->py = q->py;
    p->bg_x = q->bg_x; p->bg_y = q->bg_y; p->angle = q->angle;
    q->x = t.x; q->y = t.y; q->px = t.px; q->py = t.py;
    q->bg_x = t.bg_x; q->bg_y = t.bg_y; q->angle = t.angle;
    w = C(0).w; h = C(0).h;
    C(0).w = C(1).w; C(0).h = C(1).h;
    C(1).w = w; C(1).h = h;
    s_stats.swaps++;
    return 1;
}
static int Camera_CheckPlayerCrossScreen(void) {
    DcPlayer *pl = player_of(0);
    if (pl == NULL)
        return 0;
    if (!(F2() & 2)) {
        if (pl->y > s_l[0].y + C(0).h * 0xe8) {
            Camera_SwapScreens(1);
            g_obj_camera[2].flags |= 0x200;
            s_g.d8 = -s_g.d8;
            return 1;
        }
    } else {
        if (pl->y < s_l[0].y - C(0).h * 0x28) {
            Camera_SwapScreens(0);
            g_obj_camera[2].flags |= 0x200;
            s_g.d8 = -s_g.d8;
            return 1;
        }
    }
    return 0;
}
static void look_finish(void) {
    g_obj_camera[2].flags &= ~0x10000u;
    g_obj_camera[2].flags &= ~0x20000u;
    Camera_Look_Reset(&s_look);
}

/* Camera_UpdateYDualScreen: the player's screen with the split line */
static void Camera_UpdateYDualScreen(int idx) {
    CamLayer *l = &s_l[idx];
    DcPlayer *pl = player_of(idx);
    s32 ground = Stage_GetSplitLineY(l->x);
    u32 flags = F2(), dn;
    s32 lo, hi, py;
    s32 sc = C(idx).h;
    if (pl == NULL)
        return;
    py = pl->y;
    if (flags & 8)
        ground = s_g.dc;
    if (flags & 0x10000) {
        if (!(flags & 0x20000)) {
            l->y += s_look.speed;
            Camera_CheckPlayerCrossScreen();
            Camera_ClampLookPanYDual();
        } else {
            l->y += s_look.speed;
            dn = F2() & 2;
            if ((dn && py < ground) || (!dn && py > ground)) {
                Camera_CheckPlayerCrossScreen();
                dn = F2() & 2;
                if (dn && s_look.speed < 0) {
                    if (l->y < ground + sc * 0x28) {
                        lo = ground + sc * 0x28; hi = C(idx).bound[3] - sc * 0xc0;
                        l->y = CLAMP(l->y, lo, hi);
                        look_finish();
                    }
                } else if (!dn && s_look.speed > 0) {
                    if (l->y > ground - sc * 0xe8) {
                        l->y = CLAMP(l->y, C(idx).bound[1], ground - sc * 0xe8);
                        look_finish();
                    }
                }
            } else if (!dn) {
                if (s_look.speed > 0) {
                    if (l->y > ground - sc * 0xe8) {
                        l->y = CLAMP(l->y, C(idx).bound[1], ground - sc * 0xe8);
                        look_finish();
                    } else if (has_returned(0, s_l[0].y)) {
                        l->y = py - C(idx).def_ofs_y;
                        look_finish();
                    }
                } else if (s_look.speed < 0) {
                    if (l->y < ground - sc * 0xe8 && l->y < py - C(idx).def_ofs_y) {
                        l->y = py - C(idx).def_ofs_y;
                        l->y = CLAMP(l->y, C(idx).bound[1], ground - sc * 0xe8);
                        look_finish();
                    }
                } else if (Camera_ClampLookPanYDual()) {
                    look_finish();
                    return;
                }
            } else {
                if (s_look.speed > 0) {
                    if (l->y > ground + sc * 0x28 && l->y > py - C(idx).def_ofs_y) {
                        l->y = py - C(idx).def_ofs_y;
                        lo = ground + sc * 0x28; hi = C(idx).bound[3] - sc * 0xc0;
                        l->y = CLAMP(l->y, lo, hi);
                        look_finish();
                    }
                } else if (s_look.speed < 0) {
                    if (l->y < ground + sc * 0x28) {
                        lo = ground + sc * 0x28; hi = C(idx).bound[3] - sc * 0xc0;
                        l->y = CLAMP(l->y, lo, hi);
                        look_finish();
                    } else if (has_returned(0, s_l[0].y)) {
                        l->y = py - C(idx).def_ofs_y;
                        look_finish();
                    }
                } else if (Camera_ClampLookPanYDual()) {
                    look_finish();
                    return;
                }
            }
        }
    } else if (!(flags & 4)) {
        if (!(flags & 2)) {
            if (py > ground) {
                Camera_FollowPlayerY(idx);
                Camera_SwapScreens(1);
                g_obj_camera[2].flags |= 0x200;
                s_g.d8 = -s_g.d8;
            } else if (!(C(idx).flags & 0x4020)) {
                Camera_FollowPlayerY(idx);
                l->y = CLAMP(l->y, C(idx).bound[1], ground - sc * 0xe8);
            }
        } else {
            if (py < ground) {
                Camera_FollowPlayerY(idx);
                Camera_SwapScreens(0);
                g_obj_camera[2].flags |= 0x200;
                s_g.d8 = -s_g.d8;
            } else if (!(C(idx).flags & 0x4020)) {
                Camera_FollowPlayerY(idx);
                lo = ground + sc * 0x28; hi = C(idx).bound[3] - sc * 0xc0;
                l->y = CLAMP(l->y, lo, hi);
            }
        }
    } else {
        Camera_FollowPlayerY(idx);
    }
    Camera_ClampYToBounds(idx, F2());
}

/* Camera_UpdateY (single-screen form) on the DC window: layer 2 with the flags,
 * offsets and bounds of gCamera[0]. The look state machine is shared with the pair. */
static void window_update_y(void) {
    CamLayer *v = &s_l[2];
    DcPlayer *pl = player_of(0);
    u32 f = C(0).flags;
    u32 f2 = g_obj_camera[2].flags & ~1u;   /* single-screen formulas */
    if (f & 0x280) {           /* Camera_UpdateFollow: locked (Player_UpdateCamera moved it) */
        if (!(f2 & 0x200))
            v->y = CLAMP(v->y, s_g.map_bound[1], s_g.map_bound[3] - C(0).h * 0xc0);
        return;
    }
    if (f & 0x4020) {
        v->y = CLAMP(v->y, C(0).bound[1], C(0).bound[3] - C(0).h * 0xc0);
        return;
    }
    if ((f2 & 0x10000) && pl) {
        if (!(f2 & 0x20000)) {
            v->y += s_look.speed;
            Camera_ClampLookPanY(0, &v->y);
        } else {
            v->y += s_look.speed;
            if (s_look.speed != 0 && has_returned(0, v->y)) {
                v->y = pl->y - C(0).def_ofs_y;
                look_finish();
            }
        }
    } else if ((C(0).flags & 0x20000) && pl) {
        v->y = pl->y - C(0).ofs_y;
    }
    v->y = CLAMP(v->y, C(0).bound[1], C(0).bound[3] - C(0).h * 0xc0);
}

/* Camera_UpdateFollow */
static void Camera_UpdateFollow(void) {
    int i;
    if (!(g_obj_camera[2].flags & 0x100))
        Camera_RunLookCallback(&s_look);
    /* the DC window first: its look-pan end is the one the player sees */
    s_l[2].px = s_l[2].x;
    s_l[2].py = s_l[2].y;
    window_update_y();
    if (F2() & 0x200)
        return;
    if (F2() & 1) {
        if (!(C(0).flags & 0x140)) {
            Camera_UpdateX(0);
            Camera_ClampXAroundTarget(0);
        } else {
            Camera_ClampXToMapBounds(0);
        }
        if (!(C(0).flags & 0x280)) {
            Camera_UpdateYDualScreen(0);
            Camera_ClampYAroundTarget(0);
        } else {
            Camera_ClampYToMapBounds(0, F2());
        }
        if (!(C(1).flags & 0x140))
            s_l[1].x = s_g.d4 + s_l[0].x;       /* Camera_SetXFromPartnerScreen */
        if (!(C(1).flags & 0x280))
            s_l[1].y = s_g.d8 + s_l[0].y;       /* Camera_SetYFromPartnerScreen */
        return;
    }
    for (i = 0; i < 2; i++) {
        if (!(C(i).flags & 0x140))
            Camera_UpdateX(i);
        else
            Camera_ClampXToMapBounds(i);
        if (!(C(i).flags & 0x280)) {
            if (C(i).flags & 0x4020) {
                Camera_ClampYToBounds(i, F2());
            } else if (F2() & 0x10000) {
                s_l[i].y += s_look.speed;
                Camera_ClampLookPanY(i, &s_l[i].y);
                Camera_ClampYToBounds(i, F2());
            } else {
                Camera_FollowPlayerY(i);
                Camera_ClampYToBounds(i, F2());
            }
        } else {
            Camera_ClampYToMapBounds(i, F2());
        }
    }
}

/* Camera_ClearPosInViewFlags / Camera_UpdateInViewLatch (the objects' view checks
 * run their two-camera form, so the latch runs on both cameras) */
static void Camera_ClearPosInViewFlags(void) {
    C(0).flags &= ~0x4000000u;
    C(1).flags &= ~0x4000000u;
}
static void Camera_UpdateInViewLatch(void) {
    u8 i;
    for (i = 0; i < 2; i++) {
        if (!(C(i).flags34 & 0x40) && (C(i).flags34 & 0x20))
            C(i).flags34 |= 0x40;
        else if ((C(i).flags34 & 0x40) && !(C(i).flags34 & 0x20))
            C(i).flags34 &= ~0x40u;
        C(i).flags34 &= ~0x20u;
    }
}

/* ================================================================ ZoneFx_Zone6 (camera part) */
static int z6_active(void) { return s_z6_on && s_zone == 5 && s_act != 2; }
/* ZoneFx_Zone6_Update: the Altitude Limit camera bob while y is not locked */
static void ZoneFx_Zone6_Update(void) {
    s32 d;
    int i;
    s_z6_t++;
    d = (dcp_sin((u16)((s_z6_t << 7) * 3)) << 10) >> 12;
    for (i = 0; i < 2; i++)
        if (!(C(i).flags & 0x80))
            s_l[i].y += d;
    if (!(C(0).flags & 0x80))
        s_l[2].y += d;
}
static void ZoneFx_Zone6_SetLockedCameraX(s32 x, int idx) {
    if (!z6_active())
        return;
    if (C(idx).flags & 0x40) {
        s_l[idx].x = x;
        if (idx == 0)
            s_l[2].x = x;
    }
}
static void ZoneFx_Zone6_ApplyCameraWave(s32 y, int idx) {
    s32 d;
    if (!z6_active())
        return;
    if (!(C(idx).flags & 0x80))
        return;
    d = (dcp_sin((u16)((s_z6_t << 7) * 3)) << 10) >> 12;
    s_l[idx].y = y + d;
    if (idx == 0)
        s_l[2].y = y + d;
}

/* ================================================================ the DC window */
/* the player stays inside the window: the limits of the look pans (Camera_ClampLookPanY:
 * 0x18 px from the top, 0xa8 = 0xc0 - 0x18 from the bottom) */
#define WIN_TOP 0x1800
#define WIN_BOTTOM 0x1800
static void window_finish(void) {
    CamLayer *v = &s_l[2];
    DcPlayer *pl = player_of(0);
    s32 raw = v->y, d, step, lo, hi;
    v->x = s_l[0].x;   /* horizontal: gCamera[0] exactly */
    s_win_x = v->x;
    if (s_win_snap) {
        s_win_y = raw;
        s_win_snap = 0;
        s_stats.snaps++;
    } else {
        /* move with the raw camera; a jump of the raw camera (a gimmick lock, the lead
         * offset of a screen swap, a look pan ended by the pair) is eased out */
        step = raw - s_raw_prev_y;
        if (ABS(step) > 0x2000)
            step = 0;
        s_win_y += step;
        d = raw - s_win_y;
        if (d != 0) {
            s32 e = d / 6;
            if (e == 0)
                e = d > 0 ? (d > 0x100 ? 0x100 : d) : (d < -0x100 ? -0x100 : d);
            e = CLAMP(e, -0xc00, 0xc00);
            s_win_y += e;
            s_stats.eased++;
            if (ABS(e + step) > s_stats.max_dy)
                s_stats.max_dy = ABS(e + step);
        }
    }
    s_raw_prev_y = raw;
    /* the player stays inside the window */
    if (pl) {
        lo = pl->y - (C(0).h * 0xc0 - WIN_BOTTOM * C(0).h / 0x100);
        hi = pl->y - WIN_TOP * C(0).h / 0x100;
        if (s_win_y < lo) {
            s_win_y = lo;
            s_stats.clamped_player++;
        } else if (s_win_y > hi) {
            s_win_y = hi;
            s_stats.clamped_player++;
        }
    }
    /* never outside the camera bounds (enemy rooms, area limits, the map) */
    lo = C(0).bound[1];
    hi = C(0).bound[3] - C(0).h * 0xc0;
    if (hi < lo)
        hi = lo;
    s_win_y = CLAMP(s_win_y, lo, hi);
    if (s_win_y < s_g.map_bound[1] - 0x800)
        s_win_y = s_g.map_bound[1] - 0x800;
}

/* ================================================================ Camera_Main */
void camera_update(void) {
    int i;
    if (!s_inited)
        return;
    s_stats.frames++;
    if (s_g.mode & 4) {          /* Camera_Main_Fixed (boss arenas: the boss camera is 3D) */
        s_g.prev_flags = F2();
        if (s_zone_hook)
            s_zone_hook();
        /* (DC) the 2D window still follows the player: rings, effects and the HUD
         * world positions keep an origin; the DS draws those arenas in 3D */
        if (s_pl[0]) {
            s_win_x = CLAMP(s_pl[0]->x - 0x8000, 0, (s_g.map_w - 0x100) << 8);
            s_win_y = CLAMP(s_pl[0]->y - 0x6000, 0, (s_g.map_h - 0xc0) << 8);
        }
        return;
    }
    if (s_respawn) {
        s_respawn = 0;
        if (s_g.dual)
            init_dual(s_pl[0], 0);
        place_at_player(s_pl[0]);
        objects_respawn_view();
    }
    s_g.prev_flags = F2();
    for (i = 0; i < 2; i++) {
        s_l[i].px = s_l[i].x;
        s_l[i].py = s_l[i].y;
    }
    if (F2() & 1) {
        s_g.d8 = !(F2() & 2) ? C(0).h * 0x110 : C(0).h * -0x110;
        s_g.d0 = C(0).h * 0x50;
    }
    /* Camera_HandleSwapScreenInput: single-screen (VS) only */
    Camera_UpdateFollow();
    Camera_ClearPosInViewFlags();
    Camera_UpdateInViewLatch();
    if (z6_active())
        ZoneFx_Zone6_Update();
    if (s_zone_hook)
        s_zone_hook();               /* ZoneBg_Update, ZoneFx_Update */
    for (i = 0; i < 2; i++) {
        s_l[i].x += s_l[i].vx;
        s_l[i].y += s_l[i].vy;
    }
    s_l[2].y += s_l[0].vy;
    /* Camera_ApplyLcdSwap (task 0xf00a): the swap is shown; clear the request */
    g_obj_camera[2].flags &= ~0x200u;
    for (i = 0; i < 2; i++) {
        s_l[i].vx = s_l[i].vy = 0;
        s_l[i].sx = s_l[i].sy = 0;
    }
    window_finish();
}

/* ================================================================ Player_Last (camera part) */
/* Display_IsMainScreenOnBottom: the player's screen is the lower one after a swap */
static int main_on_bottom(void) { return (F2() & 0x400) != 0; }

static void Player_UpdateCameraLead(DcPlayer *w) {
    s32 shift = 4, tx, ty, cx, cy, gx, gy;
    int k = w->pid & 1;
    if (w->pid != 0)
        return;
    if (main_on_bottom()) {
        cx = 0x80;
        cy = 0x58;
    } else {
        cx = 0x80;
        cy = 0x60;
    }
    if (w->fx_timer != 0) {
        w->fx_timer--;
        tx = s_lead[k].tgt_x + (w->move_x << 4);
        ty = s_lead[k].tgt_y + (w->move_y << 4);
        shift = 2;
    } else {
        gx = 0;
        gy = 0;
        if (w->ride != NULL) {
            gx = w->ride->vx;
            gy = w->ride->vy;
        }
        tx = (w->px - (w->x - gx)) << 4;
        ty = (w->py - (w->y - gy)) << 3;
        if (ABS(tx) < 0x1000)
            tx = 0;
        if (ABS(ty) < 0x2800)
            ty = 0;
    }
    if ((w->pf & 0x80) || w->fx_timer != 0) {
        tx = CLAMP(tx, -0x5800, 0x5800);
        ty = CLAMP(ty, -0x4800, 0x1600);
    } else {
        tx = CLAMP(tx, -0x2c00, 0x2c00);
        ty = CLAMP(ty, -0x2400, 0xb00);
    }
    s_lead[k].tgt_x = (s16)tx;
    s_lead[k].tgt_y = (s16)ty;
    if (w->pf & 0x2000) {
        s_lead[k].lead_x -= s_lead[k].lead_x >> 2;
        s_lead[k].lead_y -= s_lead[k].lead_y >> 2;
    } else {
        s_lead[k].lead_x += (s_lead[k].tgt_x - s_lead[k].lead_x) >> shift;
        s_lead[k].lead_y += (s_lead[k].tgt_y - s_lead[k].lead_y) >> shift;
    }
    Camera_SetLeadTargetPrimary(s_lead[k].lead_x + (cx << 8), s_lead[k].lead_y + (cy << 8));
}

/* the gimmick the player is held by: the object system's Obj starts with its DcPlayer header */
static const DcPlayer *gimmick_of(const DcPlayer *w) { return (const DcPlayer *)w->gimmick; }

static void Player_UpdateCamera(DcPlayer *w) {
    s32 t, d;
    u8 c = w->cam & 1;
    int win = c == 0;          /* the DC window shares gCamera[0] */
    const DcPlayer *g = gimmick_of(w);
    ObjPlayerCtx *ctx = obj_player_ctx(w);
    s16 g3ec = ctx ? ctx->g3EC : 0, g3ee = ctx ? ctx->g3EE : 0;
    int k = w->pid & 1;
    if (w->pid != 0)
        return;
    if (w->pf2 & 0x10) {
        if (!(C(c).flags & 0x40))
            Camera_LockX(c);
        if (g != NULL) {
            t = g->x - ((g3ec + 0x80) << 8);
            if (w->pf2 & 0x40) {
                s_l[c].x = t;
                if (win)
                    s_l[2].x = t;
                ZoneFx_Zone6_SetLockedCameraX(t, c);
            } else {
                s_l[c].x = ObjShift_Set(s_l[c].x, t, 4, 0, (g_dcp_world.objmgr & 0x100) ? 0x200 : 0x100);
                if (win)
                    s_l[2].x = s_l[c].x;
                if ((t & ~0x1ff) == (s_l[c].x & ~0x1ff))
                    w->pf2 |= 0x40;
            }
        } else {
            w->pf2 &= ~0x50u;
        }
    } else {
        w->pf2 &= ~0x40u;
        if (C(c).flags & 0xc0) {
            if (ABS(s_l[c].x - (w->x - 0x8000)) > 0x80)
                s_l[c].x += ((w->x - 0x8000) - s_l[c].x) >> 2;
            else
                s_l[c].x = w->x - 0x8000;
            if (win)
                s_l[2].x = s_l[c].x;
            d = ABS(s_l[c].x - (w->x - 0x8000));
            if (d <= 0x400) {
                Camera_UnlockX(c);
                s_lead[k].lead_x = 0;
            }
        }
    }
    if (w->pf2 & 0x20) {
        if (!(C(c).flags & 0x80))
            Camera_LockY(c);
        if (g != NULL) {
            t = g->y - ((g3ee + 0x60) << 8);
            if (w->pf2 & 0x80) {
                s_l[c].y = t;
                if (win)
                    s_l[2].y = t;
                ZoneFx_Zone6_ApplyCameraWave(t, c);
                return;
            }
            s_l[c].y = ObjShift_Set(s_l[c].y, t, 4, 0, 0x100);
            if (win)
                s_l[2].y = ObjShift_Set(s_l[2].y, t, 4, 0, 0x100);
            if ((t & ~0x1ff) == (s_l[c].y & ~0x1ff))
                w->pf2 |= 0x80;
        } else {
            w->pf2 &= ~0xa0u;
        }
        return;
    }
    w->pf2 &= ~0x80u;
    if (!(C(c).flags & 0x80))
        return;
    if (ABS(s_l[c].y - (w->y - 0x6000)) > 0x80)
        s_l[c].y += ((w->y - 0x6000) - s_l[c].y) >> 2;
    else
        s_l[c].y = w->y - 0x6000;
    if (win) {
        if (ABS(s_l[2].y - (w->y - 0x6000)) > 0x80)
            s_l[2].y += ((w->y - 0x6000) - s_l[2].y) >> 2;
        else
            s_l[2].y = w->y - 0x6000;
    }
    d = ABS(s_l[c].y - (w->y - 0x6000));
    if (d > 0x400)
        return;
    Camera_UnlockY(c);
    s_lead[k].lead_y = 0;
}

void camera_player_last(DcPlayer *p) {
    if (!s_inited || p == NULL || (s_g.mode & 4))
        return;
    Player_UpdateCamera(p);
    Player_UpdateCameraLead(p);
}

/* ================================================================ init / respawn */
/* Camera_InitDualScreen: the start screen comes from the split line at the start point */
static void init_dual(DcPlayer *p, int full) {
    int i;
    s32 v, lim;
    s_g.dual = 1;
    g_obj_camera[2].flags &= ~4u;
    s_g.d0 = 0x5000;
    C(0).player = 0;
    C(1).player = -1;
    C(0).ofs_x = 0x8000;
    C(0).ofs_y = 0x6000;
    v = p ? p->x >> 8 : 0;
    lim = p ? p->y >> 8 : 0;
    if ((u32)(Stage_GetSplitLineY(v << 8) >> 8) >= (u32)lim) {
        g_obj_camera[2].flags &= ~2u;
        g_obj_camera[2].flags &= ~0x400u;
        s_g.d4 = 0;
        s_g.d8 = 0x11000;
    } else {
        g_obj_camera[2].flags |= 2;
        g_obj_camera[2].flags |= 0x400;
        s_g.d4 = 0;
        s_g.d8 = -0x11000;
    }
    for (i = 0; i < 2; i++) {
        C(i).flags |= 0xF;
        C(i).flags |= 0x30000;
        if (!full)
            continue;
        C(i).w = 0x100;
        C(i).h = 0x100;
        C(i).bound[0] = s_g.map_bound[0];
        C(i).bound[1] = s_g.map_bound[1];
        C(i).bound[2] = s_g.map_bound[2];
        C(i).bound[3] = s_g.map_bound[3];
    }
}

/* the positions Camera_Main reaches in its first frame (the DS starts from 0, 0) */
static void place_at_player(DcPlayer *p) {
    int i;
    for (i = 0; i < 3; i++)
        memset(&s_l[i], 0, sizeof(s_l[i]));
    if (p) {
        s_l[0].x = s_l[1].x = s_l[2].x = p->x - C(0).ofs_x;
        s_l[0].y = s_l[2].y = p->y - C(0).ofs_y;
        s_l[1].y = s_l[0].y + s_g.d8;
    }
    Camera_ClampXToBounds(0);
    s_l[1].x = s_l[2].x = s_l[0].x;
    s_l[2].y = CLAMP(s_l[2].y, C(0).bound[1], C(0).bound[3] - C(0).h * 0xc0);
    s_win_x = s_l[2].x;
    s_win_y = s_raw_prev_y = s_l[2].y;
    s_win_snap = 1;
}

static void (*s_prev_restart)(void *ctx, DcPlayer *p);
static void (*s_prev_dcp_shake)(void *ctx, s32 s);
static void (*s_prev_obj_shake)(void *ctx, s32 s);
static void h_shake(void *ctx, s32 s) { (void)ctx; camera_shake(s); }
/* g_dcp_hooks.restart: Player_Respawn -> StageRestart_Create (a new Camera_Create on the
 * DS) or Camera_ResetOnRespawn. The objects are not recreated on the DC, so the camera
 * keeps the object-owned state (bounds, locks, water) and resets its own. */
static void h_restart(void *ctx, DcPlayer *p) {
    if (s_prev_restart)
        s_prev_restart(ctx, p);
    if (!s_inited || p == NULL)
        return;
    if (g_dcp_world.mode == 1 || (p->pf2 & 0x400000))
        Camera_ResetOnRespawn();
    s_look.flags = 0;
    Camera_Look_Reset(&s_look);
    g_obj_camera[2].flags &= ~0x30200u;
    memset(s_lead, 0, sizeof(s_lead));
    /* Camera_Create clears the area offsets (MapObj73 0x1000, enemy-room release 0x2000);
     * Camera_ResetOnRespawn does the same with ~0xf000 */
    C(0).flags &= ~0xf000u;
    C(1).flags &= ~0xf000u;
    C(0).ofs_x = C(0).def_ofs_x ? C(0).def_ofs_x : 0x8000;
    C(0).ofs_y = C(0).def_ofs_y ? C(0).def_ofs_y : 0x6000;
    C(p->cam & 1).flags |= 3;          /* Player_Respawn: gCamera[cam] |= 1 | 2 */
    Camera_UnlockX(p->cam & 1);
    Camera_UnlockY(p->cam & 1);
    /* the player is moved to the checkpoint after this hook: place the cameras on the
     * next Camera_Main (the screen of the split line again, as Camera_InitDualScreen) */
    s_respawn = 1;
}

void Camera_ResetOnRespawn(void) {
    if (F2() & 1)
        return;
    g_obj_camera[2].flags &= 0xfff8fefbu;
    C(0).flags &= ~0xf000u;
    C(0).bound[0] = g_obj_camera[2].bound[0];
    C(0).bound[1] = g_obj_camera[2].bound[1];
    C(0).bound[2] = g_obj_camera[2].bound[2];
    C(0).bound[3] = g_obj_camera[2].bound[3];
    C(0).lock[0] = C(0).lock[1] = C(0).lock[2] = C(0).lock[3] = 0;
    C(0).area_objs = 0;
    if (s_zone == 2) {
        C(0).flags &= ~0x1000000u;
        C(0).water = 0xffff;
        C(1).flags &= ~0x1000000u;
        C(1).water = 0xffff;
    }
}

void camera_create(const void *level_data, DcPlayer *p, int zone, int act) {
    const rlvl_header_t *h = (const rlvl_header_t *)level_data;
    s32 rx, ry;
    u32 rf;
    int i;
    memset(s_l, 0, sizeof(s_l));
    memset(&s_g, 0, sizeof(s_g));
    memset(&s_look, 0, sizeof(s_look));
    memset(&s_stats, 0, sizeof(s_stats));
    memset(s_lead, 0, sizeof(s_lead));
    memset(&s_shake, 0, sizeof(s_shake));
    s_pl[0] = p;
    s_pl[1] = NULL;
    s_zone = (u8)zone;
    s_act = (u8)act;
    s_split = NULL;
    if (h) {
        u32 sz = 0;
        s_split = (const u8 *)rlvl_section(level_data, RLVL_SEC_SPLIT, &sz);
        s_split_w = h->width_blocks;
        s_split_h = h->height_blocks;
        /* Camera_InitBoundsAndBuffers */
        s_g.map_w = (u16)(h->width_blocks << 6);
        s_g.map_h = (u16)(h->height_blocks << 6);
    }
    s_g.map_bound[0] = 0x800;
    s_g.map_bound[1] = 0x800;
    s_g.map_bound[2] = (s_g.map_w - 8) << 8;
    s_g.map_bound[3] = (s_g.map_h - 8) << 8;
    for (i = 0; i < 4; i++)
        g_obj_camera[2].bound[i] = s_g.map_bound[i];
    g_obj_camera[2].w = 0x100;
    g_obj_camera[2].h = 0x100;
    /* Camera_Create: boss stages (zone >= 9 || act >= 2) use the fixed camera */
    if (zone >= 9 || act >= 2) {
        s_g.mode |= 4;
    } else {
        init_dual(p, 1);
    }
    s_look.fn = (s_g.mode & 4) ? NULL : Camera_Look_Idle;
    /* ZoneFx_Zone6_Init (zone 5 acts 0/1 have a ZoneFx work) */
    s_z6_t = 0;
    s_z6_on = zone == 5 && act < 2;
    /* Stage_RestoreRestartCamera (special stage return): applied by the objects module */
    if (objects_restart_camera(&rx, &ry, &rf)) {
        g_obj_camera[2].flags = (g_obj_camera[2].flags & ~0x402u) | (rf & 2) | ((rf & 2) ? 0x400 : 0);
        s_g.d8 = (rf & 2) ? -0x11000 : 0x11000;
    }
    place_at_player(p);
    if (objects_restart_camera(&rx, &ry, &rf)) {
        s_l[0].x = s_l[1].x = s_l[2].x = rx;
        s_l[0].y = ry;
        s_l[1].y = ry + s_g.d8;
    }
    /* hooks: ScreenShake_Start for the player and the objects, Player_Respawn */
    if (g_dcp_hooks.restart != h_restart) {
        s_prev_restart = g_dcp_hooks.restart;
        g_dcp_hooks.restart = h_restart;
    }
    if (g_dcp_hooks.shake != h_shake) {
        s_prev_dcp_shake = g_dcp_hooks.shake;
        g_dcp_hooks.shake = h_shake;
    }
    if (g_obj_hooks.shake != h_shake) {
        s_prev_obj_shake = g_obj_hooks.shake;
        g_obj_hooks.shake = h_shake;
    }
    (void)s_prev_dcp_shake;
    (void)s_prev_obj_shake;
    s_inited = 1;
}

void camera_destroy(void) {
    s_inited = 0;
    s_pl[0] = s_pl[1] = NULL;
    s_split = NULL;
    memset(&s_shake, 0, sizeof(s_shake));
}

/* StageTitleCard_HoldPlayerX / Y (dual form: both screens from the held player) and
 * every jump of the player (respawn): the window snaps, the objects spawn again */
void camera_snap_to_player(DcPlayer *p) {
    if (!s_inited || p == NULL || (s_g.mode & 4))
        return;
    s_l[0].x = s_l[1].x = s_l[2].x = p->x - C(0).ofs_x;
    s_l[0].y = p->y - C(0).ofs_y;
    Camera_ClampXToBounds(0);
    s_l[1].x = s_l[2].x = s_l[0].x;
    s_l[2].y = CLAMP(s_l[0].y, C(0).bound[1], C(0).bound[3] - C(0).h * 0xc0);
    s_win_x = s_l[2].x;
    s_win_y = s_raw_prev_y = s_l[2].y;
}

/* ================================================================ ScreenShake */
void camera_set_shake_tables(const s8 *t3c, const s8 *t40, const s8 *t4c) {
    memcpy(s_shk3c, t3c, sizeof(s_shk3c));
    memcpy(s_shk40, t40, sizeof(s_shk40));
    memcpy(s_shk4c, t4c, sizeof(s_shk4c));
}
static void ScreenShake_Stop(void) { memset(&s_shake, 0, sizeof(s_shake)); }
void camera_shake(s32 v) {
    if (v == 0) {
        ScreenShake_Stop();
        return;
    }
    if (s_shake.mode <= (u8)v) {
        s_shake.mode = (u8)v;
        s_shake.flags = 1;
        s_shake.t = 0;
    }
}
s32 camera_shake_x(void) { return s_shake.x; }
s32 camera_shake_y(void) { return s_shake.y; }
/* ScreenShake_Main (task 0x3e00) */
static void ScreenShake_Main(void) {
    s_shake.x = 0;
    s_shake.y = 0;
    if (!(s_shake.flags & 1))
        return;
    switch (s_shake.mode) {
    case 1:
        if (s_shake.t > 8) { ScreenShake_Stop(); break; }
        /* fall through */
    case 5:
        if (s_shake.t >= 12) { ScreenShake_Stop(); break; }
        /* fall through */
    case 9:
        if (s_shake.t >= 12)
            s_shake.t = 0;
        s_shake.x = (s_shk40[s_shake.t] & 0xf) >> 1;
        s_shake.y = s_shk40[s_shake.t] >> 5;
        break;
    case 2:
        if (s_shake.t > 8) { ScreenShake_Stop(); break; }
        /* fall through */
    case 6:
        if (s_shake.t >= 12) { ScreenShake_Stop(); break; }
        /* fall through */
    case 10:
        if (s_shake.t >= 12)
            s_shake.t = 0;
        s_shake.x = s_shk40[s_shake.t] & 0xf;
        s_shake.y = s_shk40[s_shake.t] >> 4;
        break;
    case 3:
        if (s_shake.t >= 8) { ScreenShake_Stop(); break; }
        /* fall through */
    case 7:
        if (s_shake.t >= 16) { ScreenShake_Stop(); break; }
        /* fall through */
    case 11:
        if (s_shake.t >= 16)
            s_shake.t = 0;
        s_shake.x = s_shk3c[s_shake.t & 3] & 0xf;
        s_shake.y = s_shk3c[s_shake.t & 3] >> 4;
        break;
    case 4:
        if (s_shake.t >= 8) { ScreenShake_Stop(); break; }
        /* fall through */
    case 8:
        if (s_shake.t >= 0x16) { ScreenShake_Stop(); break; }
        /* fall through */
    case 12:
        if (s_shake.t >= 0x16)
            s_shake.t = 0;
        s_shake.x = s_shk4c[s_shake.t] & 0xf;
        s_shake.y = s_shk4c[s_shake.t] >> 4;
        break;
    }
    if (s_shake.x & 8)
        s_shake.x = (s8)-(0x10 - s_shake.x);
    s_shake.t++;
}

/* Stage_Main: gCamera.unk18 = -ScreenShake_GetOffsetX() (Camera_GetViewOrigin) and the
 * objects' ObjDraw_SetGlobalOffset: the whole picture moves by the shake offset */
void camera_frame_end(void) { ScreenShake_Main(); }
void camera_apply(void) {
    if (!s_inited)
        return;
    s_l[2].sx = (s16)-s_shake.x;
    s_l[2].sy = (s16)-s_shake.y;
    g_cam_x = s_win_x / 256.0f + s_l[2].sx;
    g_cam_y = s_win_y / 256.0f + s_l[2].sy;
}

/* the look state (tests): 0 none, 1 idle, 2 wait, 3 pan up, 4 pan down, 5 back from up, 6 back from down */
int camera_look_state(void) {
    if (s_look.fn == Camera_Look_Idle) return 1;
    if (s_look.fn == Camera_Look_Wait) return 2;
    if (s_look.fn == Camera_Look_PanUp) return 3;
    if (s_look.fn == Camera_Look_PanDown) return 4;
    if (s_look.fn == Camera_Look_ReturnFromUp) return 5;
    if (s_look.fn == Camera_Look_ReturnFromDown) return 6;
    return 0;
}
