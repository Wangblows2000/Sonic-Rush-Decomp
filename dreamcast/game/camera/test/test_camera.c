/*
 * Host test of the camera (camera.c) and the zone backgrounds / effects (zonefx.c)
 * on the real levels, with the real player and object modules.
 *
 * Usage: test_camera <stages dir> <build/dreamcast/player> <objdata dir> <sprites bac dir>
 *                    <zonefx.bin> <stage> [<stage> ...]
 *
 * Per stage:
 *  [run]     scripted play from PlayerStart (hold right, jump every 50 frames, a few
 *            seconds of left) for 2400 frames, then a sweep from every 40th ring:
 *              - window x == gCamera[0].x (the original horizontal camera)
 *              - window steps <= 32 px a frame (no DS screen snap), snaps only on a jump
 *              - the player inside the window (24 px from the top and the bottom)
 *              - the window inside the camera bounds and the map
 *              - the DS pair: gCamera[1] = gCamera[0] + the partner offset (+-0x110 px)
 *  [look]    standing, Up held: the window pans up 72 px (Camera_ClampLookPanY) and
 *            comes back after the release; Down the same downwards
 *  [bg]      the BG line table against the zone formulas (ZoneBg_Zone<n>_*)
 *  [fx]      zone palettes: the palette animations step from the alternate palettes,
 *            the BLDY ramp darkens, the alarm, the water tint
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../camera.h"
#include "../../zonefx/zonefx.h"
#include "../../objects/objects.h"
#include "../../objects/obj_internal.h"

float g_cam_x, g_cam_y;
void zonefx_host_palettes(const u16 *map_pal, const u16 *bg_pals, int n);
void zonefx_host_reinit_palettes(void);

static int g_fail, g_checks;
#define CHECK(c, ...)                                                   \
    do {                                                                \
        g_checks++;                                                     \
        if (!(c)) {                                                     \
            g_fail++;                                                   \
            printf("  FAIL %s:%d: ", __FILE__, __LINE__);               \
            printf(__VA_ARGS__);                                        \
            printf("\n");                                               \
        }                                                               \
    } while (0)

static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*size);
    if (fread(b, 1, *size, f) != (size_t)*size)
        exit(2);
    fclose(f);
    return b;
}

static DcPlayer P;
static rlvl_map_t M;
static u8 *g_level;
static const rlvl_header_t *g_hdr;
static u16 s_prev;
static int g_frame;
/* per-stage measurements */
static s32 m_max_step, m_max_step_x, m_min_top, m_max_top;
static u32 m_haze, m_bound_excl, m_bad_x, m_bad_step, m_out_player, m_out_bounds, m_bad_pair, m_frames;
static s32 s_last_wy;
static int s_jumped;
static u32 s_snaps;

static void camera_frame_checks(void) {
    s32 wx, wy, d, top;
    const CamLayer *l0 = camera_layer(0), *l1 = camera_layer(1);
    camera_get_view(&wx, &wy);
    m_frames++;
    {   /* Mirage Road: the top band moves row by row (ZoneBg_Zone3_BuildLineScroll) */
        const ZfxLine *zl = zonefx_lines();
        int r, n = 0;
        for (r = 1; r < 24; r++)
            n += zl[r].x != zl[r - 1].x;
        if (n > 3)
            m_haze++;
    }
    if (camera_stats()->snaps != s_snaps) {   /* a respawn (Player_Respawn) or a snap */
        s_snaps = camera_stats()->snaps;
        if (s_jumped < 2)
            s_jumped = 2;
    }
    if (wx != l0->x)
        m_bad_x++;
    d = wy - s_last_wy;
    if (!s_jumped) {
        if (DCP_ABS(d) > m_max_step)
            m_max_step = DCP_ABS(d);
        if (DCP_ABS(d) > 32 * 256) {
            m_bad_step++;
            if (getenv("CAMDBG"))
                printf("    step f%d %d px: player %d,%d vy %d window %d raw %d l0 %d pf2 %x flags %x f2 %x\n", g_frame, d >> 8,
                       P.x >> 8, P.y >> 8, P.vel_y, wy >> 8, camera_layer(2)->y >> 8, l0->y >> 8, P.pf2,
                       g_obj_camera[0].flags, camera_flags2());
        }
    }
    if (s_jumped > 0)
        s_jumped--;
    s_last_wy = wy;
    top = (P.y - wy) >> 8;
    if (top < m_min_top)
        m_min_top = top;
    if (top > m_max_top)
        m_max_top = top;
    if ((top < 23 || top > 169) && (P.y < g_obj_camera[0].bound[1] + 0x1800 || P.y > g_obj_camera[0].bound[3] - 0x1800 ||
                                    P.x < g_obj_camera[0].bound[0] || P.x > g_obj_camera[0].bound[2])) {
        m_bound_excl++;   /* the camera bounds (an enemy room left by a teleport) exclude the player */
    } else if (top < 23 || top > 169) {
        if (m_out_player < 4 && getenv("CAMDBG"))
            printf("    out f%d player %d,%d window %d,%d bounds %d..%d map %d flags %x f34 %x f2 %x lock %d %d\n", g_frame,
                   P.x >> 8, P.y >> 8, wx >> 8, wy >> 8, g_obj_camera[0].bound[1] >> 8, g_obj_camera[0].bound[3] >> 8,
                   M.bottom, g_obj_camera[0].flags, g_obj_camera[0].flags34, camera_flags2(), g_obj_camera[0].lock[1],
                   g_obj_camera[0].lock[3]);
        m_out_player++;
    }
    if (wy < g_obj_camera[0].bound[1] - 0x800 || wy + 0xc000 > g_obj_camera[0].bound[3] + 0x800 || wx < 0 ||
        wx + 0x10000 > (M.right << 8))
        m_out_bounds++;
    if (!(g_obj_camera[1].flags & 0x280) && !(camera_flags2() & 0x200) &&
        (l1->y != l0->y + camera_global()->d8 || DCP_ABS(camera_global()->d8) != 0x11000))
        m_bad_pair++;
}

static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s32 vx, vy;
    s_prev = held;
    camera_update();
    camera_frame_checks();
    camera_get_view(&vx, &vy);
    objects_set_view(0, vx, vy);
    if (getenv("CAMVIEW1_DS"))   /* experiment: view 1 = the DS partner screen (gCamera[1]) */
        objects_set_view(1, camera_layer(1)->x, camera_layer(1)->y);
    else
        objects_set_view(1, vx, vy - 0xC000);   /* main.c convention: 192 px above the window */
    objects_frame_begin();
    dcp_player_frame(&P, held, pressed, released);
    camera_player_last(&P);
    objects_update(&P);
    camera_frame_end();
    camera_apply();
    g_frame++;
}
static void place_player(s32 x, s32 y) {
    s16 rings = P.rings;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, x, y);
    objects_player_init_state(&P);
    P.rings = rings;
    P.invuln = 0x7fff;
    /* as a respawn: Player_Respawn's restart hook (the camera resets its own state) */
    if (g_dcp_hooks.restart)
        g_dcp_hooks.restart(g_dcp_hooks.ctx, &P);
    camera_snap_to_player(&P);
    objects_respawn_view();
    s_jumped = 3;   /* a teleport: the camera settles on the next frames */
}
static int floor_y(s32 x, s32 y) {
    s32 yy;
    for (yy = y - 64; yy < y + 96; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return -1;
}

/* ------------------------------------------------------------ [look] */
static void test_look(void) {
    s32 wx, wy0, wy, best = 0;
    int i, f;
    const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
    /* a flat spot with room above and below: rings on the ground */
    for (i = 0; i < (int)g_hdr->num_rings; i += 7) {
        s32 fy = floor_y(rg[i].x, rg[i].y);
        if (fy < 0 || fy < 0x200 || fy > M.bottom - 0x200)
            continue;
        place_player(rg[i].x, fy - 14);
        for (f = 0; f < 40; f++)
            frame(0);
        if (!(P.move_flags & DCP_MF_GROUND) || P.vel_x || P.gnd_vel)
            continue;
        /* a spot where the look-up animation holds (no balance / idle animation) */
        for (f = 0; f < 70; f++) {
            frame(DCP_KEY_UP);
            if (f > 4 && P.anim != 0x21 && P.anim != 0x22)
                break;
        }
        if (f < 70)
            continue;
        if (g_obj_camera[0].bound[1] != camera_global()->map_bound[1] ||
            g_obj_camera[0].bound[3] != camera_global()->map_bound[3])
            continue;   /* an enemy room / area holds the camera */
        for (f = 0; f < 120; f++)
            frame(0);
        camera_get_view(&wx, &wy0);
        for (f = 0; f < 180; f++) {
            frame(DCP_KEY_UP);
            if (getenv("CAMDBG") && f % 30 == 0)
                printf("    look f%d anim %x held %x pf2 %x f2 %x y %d st %d\n", f, P.anim, P.held, P.pf2, camera_flags2(), camera_layer(2)->y >> 8, camera_look_state());
            camera_get_view(&wx, &wy);
            if (wy0 - wy > best)
                best = wy0 - wy;
        }
        printf("  [look] up: anim 0x%x, pan %d px (DS 72; f2 %x flags %x bounds %d..%d y %d)", P.anim, best >> 8,
               camera_flags2(), g_obj_camera[0].flags, g_obj_camera[0].bound[1] >> 8, g_obj_camera[0].bound[3] >> 8, wy0 >> 8);
        CHECK(best >= 66 * 256 && best <= 84 * 256, "look up pan %d px", best >> 8);
        for (f = 0; f < 120; f++)
            frame(0);
        camera_get_view(&wx, &wy);
        printf(", back %d px\n", (wy0 - wy) >> 8);
        CHECK(DCP_ABS(wy - wy0) <= 8 * 256, "look up returns (%d px off)", (wy - wy0) >> 8);
        best = 0;
        for (f = 0; f < 180; f++) {
            frame(DCP_KEY_DOWN);
            camera_get_view(&wx, &wy);
            if (wy - wy0 > best)
                best = wy - wy0;
        }
        printf("  [look] down: anim 0x%x, pan %d px (DS 72)\n", P.anim, best >> 8);
        CHECK(best >= 60 * 256 && best <= 78 * 256, "look down pan %d px", best >> 8);
        for (f = 0; f < 120; f++)
            frame(0);
        return;
    }
    printf("  [look] no flat spot found\n");
}

/* ------------------------------------------------------------ [ship] */
/* Mirage Road sand ship (GmkTable 0x8b): Player_UpdateCamera locks the camera on the ship
 * (the lock target is the ship position before the objects ran: 2 frames of ship motion
 * behind it, as in the DS task order camera 0x80 -> player 0x100 -> objects 0x500)
 * (pf2 0x10 / 0x20, gCamera flags 0x40 / 0x80) at ship - (@0x3EC + 0x80, @0x3EE + 0x60) */
static void test_ship(void) {
    const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
    const Obj *t = NULL, *q;
    u32 i, n_lock = 0, n_far = 0, n_steps = 0;
    int k, pawns = 0;
    s32 wx, wy, py = 0, x0 = 0, maxd = 0;
    for (i = 0; i < g_hdr->num_objects && o[i].type != 0x8b; i++)
        ;
    if (i == g_hdr->num_objects)
        return;
    if (getenv("SHIP_WALK"))   /* walk onto the ship from the left (the Flycast run's start) */
        place_player(o[i].x - atoi(getenv("SHIP_WALK")), o[i].y - 60);
    else
        place_player(o[i].x, o[i].y - 60);
    for (k = 0; k < (getenv("SHIP_WALK") ? 900 : 120) && !t; k++) {
        frame(getenv("SHIP_WALK") ? DCP_KEY_RIGHT : 0);
        if (getenv("SHIP_WALK") && k % 60 == 0)
            printf("    walk k%d P(%d,%d) ang %d\n", k, P.x >> 8, P.y >> 8, P.angle);
        if (P.gimmick && ((const Obj *)P.gimmick)->prm && ((const Obj *)P.gimmick)->prm->type == 0x8b)
            t = (const Obj *)P.gimmick;
    }
    if (!t) {
        printf("  [ship] not boarded\n");
        CHECK(0, "sand ship boarded");
        return;
    }
    x0 = t->h.x;
    camera_get_view(&wx, &py);
    for (k = 0; k < 3000 && t->used && !(t->h.obj_flags & 4) && P.gimmick == t; k++) {
        ObjPlayerCtx *c = obj_player_ctx(&P);
        frame(k % 90 < 6 ? DCP_KEY_A : 0);
        if (P.invuln == 0)
            P.invuln = 0x7fff;
        camera_get_view(&wx, &wy);
        if ((P.pf2 & 0x30) && (g_obj_camera[0].flags & 0xc0)) {
            s32 tx = t->h.x - ((c->g3EC + 0x80) << 8), ty = t->h.y - ((c->g3EE + 0x60) << 8);
            n_lock++;
            if ((P.pf2 & 0x40) && DCP_ABS(wx - tx) > 0x200 + 2 * DCP_ABS(t->h.x - t->h.px)) {
                if (n_far < 3 && getenv("CAMDBG"))
                    printf("    far k%d wx %d tx %d ship x %d px %d g3EC %d flags %x pf2 %x\n", k, wx, tx, t->h.x, t->h.px, c->g3EC,
                           g_obj_camera[0].flags, P.pf2);
                n_far++;
            }
            if ((P.pf2 & 0x80) && DCP_ABS(wy - ty) > maxd)
                maxd = DCP_ABS(wy - ty);
        }
        if (DCP_ABS(wy - py) > 0x2000)
            n_steps++;
        py = wy;
        for (q = g_obj.head; q; q = q->next)
            if (q->used && q->kind == OBJK_PAWN && q->prm && q->prm->p9 != 0)
                pawns = 1;
        if (getenv("CAMDBG") && k % 300 == 0) {
            u32 e;
            printf("    ship k%d (%d,%d) P(%d,%d) window (%d,%d) ds0 y %d ds1 y %d pf2 %x:", k, t->h.x >> 8, t->h.y >> 8,
                   P.x >> 8, P.y >> 8, wx >> 8, wy >> 8, camera_layer(0)->y >> 8, camera_layer(1)->y >> 8, P.pf2);
            for (e = 0; e < g_hdr->num_objects; e++)
                if ((g_obj.prm[e].type == 0x14 || g_obj.prm[e].type == 0x15) && g_obj.prm[e].p9)
                    printf(" %x@%d,%d", g_obj.prm[e].type, g_obj.prm[e].ax, g_obj.prm[e].ay);
            printf("\n");
        }
    }
    printf("  [ship] sailed %d px in %d frames, locked %u frames (x off the ship lock %u), window y - lock target max %d px, "
           "window steps > 32 px %u, drop-in Egg Pawns seen %d\n",
           (t->h.x - x0) >> 8, k, n_lock, n_far, maxd >> 8, n_steps, pawns);
    CHECK(n_lock > 100 && n_far == 0 && maxd <= 24 * 256, "camera locked on the sand ship");
    (void)pawns;
    CHECK(n_steps == 0, "no window jump on the ship");
    if (!pawns)
        printf("  [ship] (no drop-in Egg Pawn: they spawn when a view comes within 60 px of them; view 1 is 192 px "
               "above the window, the DS top screen is 272 px above; CAMVIEW1_DS=1 tries the DS screen)\n");
}

/* ------------------------------------------------------------ [bg] */
static void test_bg(int zone, int act) {
    const ZfxLine *l = zonefx_lines();
    s32 wx, wy;
    int i, distinct = 0;
    camera_get_view(&wx, &wy);
    for (i = 1; i < 192; i++)
        distinct += l[i].x != l[i - 1].x || l[i].y != l[i - 1].y;
    printf("  [bg] window (%d,%d) row0 (%d,%d) row96 (%d,%d) row191 (%d,%d), %d offset changes, water row %d\n",
           wx >> 8, wy >> 8, l[0].x, l[0].y, l[96].x, l[96].y, l[191].x, l[191].y, distinct, zonefx_water_row());
    if (zone == 0) {   /* ZoneBg_Zone1_Update: x / 14, y * 48 / (map h - 192) */
        s32 ex = ((wx / 14) >> 8) & 0x1ff;
        s32 ey = ((s32)((((s64)wy * 0x30) << 12) / ((camera_global()->map_h - 0xc0) << 8)) >> 4 >> 8) & 0x1ff;
        CHECK(l[0].x == ex && l[0].y == ey && distinct == 0, "zone 1 BG (%d,%d) want (%d,%d)", l[0].x, l[0].y, ex, ey);
    }
    if (zone == 3 || (zone == 8 && act == 0)) {
        s32 ex = ((wx / 18) >> 8) & 0x1ff;
        CHECK(l[0].x == ex && distinct == 0, "zone 4 BG x %d want %d", l[0].x, ex);
    }
    if (zone == 5)
        CHECK(l[0].y == 0x30 && distinct == 0, "zone 6 BG y %d", l[0].y);
    if (zone == 1 && zonefx_water_row() < 150) {
        /* below the water line the wave moves every row */
        int w = 0;
        for (i = zonefx_water_row() + 2; i < 191; i++)
            w += l[i].x != l[i + 1].x || l[i].y != l[i + 1].y;
        CHECK(w > 10, "zone 2 underwater wave (%d changes)", w);
    }
    if (zone == 2 || (zone == 7 && act == 1)) {
        printf("  [bg] heat haze rows seen in %u frames\n", m_haze);
        CHECK(m_haze > 0, "zone 3 heat haze (%u frames)", m_haze);
    }
}

/* ------------------------------------------------------------ [fx] */
static u16 R5(u16 c) { return (c >> 10) & 0x1f; }
static void test_fx(int zone, const u16 *map_pal, const u16 *bg_pals, int npals) {
    int f, changed = 0, k;
    const u16 *p0 = zonefx_palette(0), *p1 = zonefx_palette(1);
    u16 a0[256], a1[256];
    if (zone != 3 && zone != 4 && zone != 6 && zone != 1 && zone != 8)
        return;
    zonefx_host_palettes(map_pal, bg_pals, npals);
    zonefx_host_reinit_palettes();
    memcpy(a0, map_pal, 512);
    memcpy(a1, bg_pals, 512);
    for (f = 0; f < 240; f++) {
        frame(0);
        for (k = 0; k < 256; k++)
            changed += (p0[k] != a0[k]) + (p1[k] != a1[k]);
        memcpy(a0, p0, 512);
        memcpy(a1, p1, 512);
    }
    printf("  [fx] zone %d: %d palette entry changes in 240 frames (%d alternate palettes)\n", zone + 1, changed, npals - 1);
    if (zone == 3 || zone == 4 || zone == 6 || zone == 8)
        CHECK(changed > 20, "palette animation runs (%d)", changed);
    if (zone == 3) {
        /* BLDY ramp up to 10 (switch darkness) and back */
        u16 c0;
        int i = 0;
        while (i < 256 && (R5(map_pal[i]) < 16))
            i++;
        if (i < 256) {
            zonefx_bldy(1, (3 << 8) | 8);
            for (f = 0; f < 10; f++)
                frame(0);
            c0 = zonefx_palette(0)[i];
            printf("  [fx] BLDY %d: entry %d red %d -> %d\n", zonefx_bldy_level(), i, R5(map_pal[i]), R5(c0));
            CHECK(zonefx_bldy_level() == 10, "BLDY ramp up reaches 10 (%d)", zonefx_bldy_level());
            zonefx_bldy(2, (3 << 8) | 8);
            for (f = 0; f < 10; f++)
                frame(0);
            CHECK(zonefx_bldy_level() == 0, "BLDY ramp down reaches 0 (%d)", zonefx_bldy_level());
            zonefx_bldy(0, 3);
            for (f = 0; f < 20; f++)
                frame(0);
            printf("  [fx] flash: BLDY %d after 20 frames\n", zonefx_bldy_level());
            for (f = 0; f < 60; f++)
                frame(0);
            CHECK(zonefx_bldy_level() == 0, "flash ends (%d)", zonefx_bldy_level());
        }
    }
    if (zone == 4) {
        /* the alarm while an enemy room locks the camera */
        g_obj_camera[0].flags34 |= 0x10000;
        for (f = 0; f < 20; f++)
            frame(0);
        printf("  [fx] alarm: BG entry 0x60 red %d -> %d\n", R5(bg_pals[0x60]), R5(zonefx_palette(1)[0x60]));
        CHECK(R5(zonefx_palette(1)[0x60]) >= R5(bg_pals[0x60]), "alarm adds red");
        g_obj_camera[0].flags34 &= ~0x10000u;
        frame(0);
        CHECK(zonefx_palette(1)[0x60] == p1[0x60], "alarm off");
    }
}

/* ------------------------------------------------------------ stage */
static int run_stage(const char *stages, const char *pdir, const char *odir, const char *sdir, const char *id) {
    char path[512];
    long size;
    s32 sx = 128, sy = 768;
    const rlvl_object_t *o;
    u32 i;
    int f, k, fail0 = g_fail;
    u16 *bgp = NULL;
    int npals = 0;
    u16 *mpal;
    (void)pdir;
    snprintf(path, sizeof(path), "%s/%s/level.bin", stages, id);
    g_level = (u8 *)load(path, &size);
    if (!g_level || rlvl_bind(&M, g_level) != 0) {
        printf("%s: no level\n", id);
        return 1;
    }
    g_hdr = (const rlvl_header_t *)g_level;
    mpal = (u16 *)rlvl_section(g_level, RLVL_SEC_PALETTE, NULL);
    snprintf(path, sizeof(path), "%s/%s/bg.bin", stages, id);
    {
        u8 *bg = (u8 *)load(path, &size);
        if (bg) {
            const rbgd_header_t *bh = (const rbgd_header_t *)bg;
            npals = bh->num_palettes;
            bgp = (u16 *)(bg + bh->palette_offset);
        }
    }
    dcp_bind_map(&M);
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    g_dcp_world.zone = g_hdr->zone;
    g_dcp_world.act = g_hdr->act;
    g_dcp_world.flags = 1;
    g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_right = M.right;
    g_dcp_world.death_y = M.bottom;
    g_dcp_world.timescale = 0x100;
    g_dcp_world.lives = 99;
    o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
    for (i = 0; i < g_hdr->num_objects; i++)
        if (o[i].type == 0x72) {
            sx = o[i].x;
            sy = o[i].y;
            break;
        }
    g_dcp_world.check_x = sx;
    g_dcp_world.check_y = sy;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, sx, sy);
    objects_shutdown();
    objects_set_paths(odir, sdir);
    objects_add_player(&P);
    objects_set_view_count(2);
    if (objects_init(&M, g_level) != 0) {
        printf("%s: objects_init failed\n", id);
        return 1;
    }
    objects_add_player(&P);
    camera_create(g_level, &P, g_hdr->zone, g_hdr->act);
    zonefx_init(NULL, g_hdr->zone, g_hdr->act);
    m_max_step = m_max_step_x = 0;
    m_min_top = 9999;
    m_max_top = -9999;
    m_haze = m_bound_excl = m_bad_x = m_bad_step = m_out_player = m_out_bounds = m_bad_pair = m_frames = 0;
    s_jumped = 1;
    camera_get_view(&sx, &s_last_wy);
    printf("== %s (zone %d act %d) %dx%d px, start screen %s\n", id, g_hdr->zone + 1, g_hdr->act + 1, M.right, M.bottom,
           (camera_flags2() & 2) ? "lower" : "upper");
    /* [run] */
    for (f = 0; f < 2400; f++) {
        u16 held = (f / 600) == 2 && f % 600 < 200 ? DCP_KEY_LEFT : DCP_KEY_RIGHT;
        if (f % 50 < 12)
            held |= DCP_KEY_A;
        if (f % 300 > 250)
            held |= DCP_KEY_X;   /* boost: the lead offset */
        frame(held);
        if (P.pf & DCP_PF_DEAD) {
            place_player(g_dcp_world.check_x, g_dcp_world.check_y);
        }
    }
    printf("  [run] player at %d,%d after 2400 frames; swaps %u, eased %u, max window step %d px (eased %d px), "
           "player rows %d..%d\n",
           P.x >> 8, P.y >> 8, camera_stats()->swaps, camera_stats()->eased, m_max_step >> 8,
           camera_stats()->max_dy >> 8, m_min_top, m_max_top);
    /* sweep */
    {
        const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
        for (i = 0; i < g_hdr->num_rings; i += 40) {
            place_player(rg[i].x, rg[i].y);
            for (k = 0; k < 200; k++)
                frame(DCP_KEY_RIGHT | ((k % 50 < 12) ? DCP_KEY_A : 0));
        }
    }
    printf("  [sweep] %u frames: x != gCamera[0].x %u, steps > 32 px %u, player outside %u, outside bounds %u, "
           "pair offset wrong %u; swaps %u, snaps %u, player clamps %u, player outside the bounds %u\n",
           m_frames, m_bad_x, m_bad_step, m_out_player, m_out_bounds, m_bad_pair, camera_stats()->swaps,
           camera_stats()->snaps, camera_stats()->clamped_player, m_bound_excl);
    CHECK(m_bad_x == 0, "window x == gCamera[0].x (%u)", m_bad_x);
    CHECK(m_bad_step == 0, "no window jump > 32 px (%u, max %d)", m_bad_step, m_max_step >> 8);
    CHECK(m_out_player == 0, "player inside the window (%u)", m_out_player);
    CHECK(m_out_bounds == 0, "window inside the bounds (%u)", m_out_bounds);
    CHECK(m_bad_pair == 0, "DS pair offset (%u)", m_bad_pair);
    test_look();
    if (g_hdr->zone == 2 && g_hdr->act == 1)
        test_ship();
    test_bg(g_hdr->zone, g_hdr->act);
    if (bgp)
        test_fx(g_hdr->zone, mpal, bgp, npals);
    zonefx_shutdown();
    camera_destroy();
    printf("  %s: %s\n", id, g_fail == fail0 ? "ok" : "FAILED");
    return 0;
}

int main(int argc, char **argv) {
    char path[512];
    long size;
    s16 *trig;
    u8 *phys, *zfx;
    int i;
    if (argc < 7) {
        printf("usage: %s <stages dir> <player dir> <objdata dir> <sprites dir> <zonefx.bin> <stage>...\n", argv[0]);
        return 2;
    }
    snprintf(path, sizeof(path), "%s/trig.bin", argv[2]);
    trig = (s16 *)load(path, &size);
    snprintf(path, sizeof(path), "%s/physics.bin", argv[2]);
    phys = (u8 *)load(path, &size);
    if (!trig || !phys)
        return 2;
    dcp_set_trig(trig, trig + 8192);
    if (dcp_set_physics(phys, (u32)size) != 2)
        return 2;
    zfx = (u8 *)load(argv[5], &size);
    CHECK(zfx && zonefx_load_data_mem(zfx, (u32)size) == 0, "zonefx.bin");
    for (i = 6; i < argc; i++)
        run_stage(argv[1], argv[2], argv[3], argv[4], argv[i]);
    printf("%d checks, %d failed\n", g_checks, g_fail);
    return g_fail != 0;
}
