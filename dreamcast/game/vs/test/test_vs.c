/* Battle Play rules (vs.c) on a real stage with the real player and objects modules:
 * items 8..11 on the rival, the warp respawn, the push box, the boost knock-back, the goal
 * (first player wins, the other stops), time over (draw), checkpoints per player, hooks restored. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../vs.h"
#include "../../camera/camera.h"
#include "../../objects/objects.h"
#include "../../objects/obj_internal.h"

static int g_fail, g_checks;
#define CHECK(c, ...) do { g_checks++; if (!(c)) { g_fail++; printf("FAIL: "); printf(__VA_ARGS__); printf("\n"); } } while (0)
static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END); *size = ftell(f); fseek(f, 0, SEEK_SET);
    b = malloc((size_t)*size);
    if (fread(b, 1, (size_t)*size, f) != (size_t)*size) { free(b); b = NULL; }
    fclose(f);
    return b;
}
float g_cam_x, g_cam_y;   /* engine/render.c (not linked) */
static DcPlayer A, B;
extern Obj *g_obj_cur;
static int s_shook[2];
static void cam_shake(int i, s32 s) { (void)s; s_shook[i & 1]++; }
static rlvl_map_t M;
static u8 *g_level;

static void frames(int n, u16 ha, u16 hb) {
    while (n-- > 0) {
        u32 c = g_dcp_world.frame;
        objects_frame_begin();
        vs_player_begin(0); dcp_player_frame(&A, ha, 0, 0); vs_player_end(0); g_dcp_world.frame = c;
        vs_player_begin(1); dcp_player_frame(&B, hb, 0, 0); vs_player_end(1); g_dcp_world.frame = c;
        objects_update(&A);
        vs_after_players();
        if (g_dcp_world.flags & 1) g_dcp_world.frame++;
        vs_frame_end();
    }
}
static void reset_players(s32 x, s32 y) {
    A.x = B.x = x << 8; A.y = B.y = y << 8;
    A.vel_x = B.vel_x = A.gnd_vel = B.gnd_vel = 0;
}

int main(int argc, char **argv) {
    char path[512];
    long size;
    s16 *trig;
    u8 *phys;
    const rlvl_header_t *h;
    const rlvl_object_t *o;
    s32 sx = 128, sy = 768;
    u32 i;
    if (argc < 5) { printf("usage: %s <stages> <player dir> <objdata dir> <sprites dir>\n", argv[0]); return 2; }
    snprintf(path, sizeof path, "%s/trig.bin", argv[2]); trig = (s16 *)load(path, &size);
    snprintf(path, sizeof path, "%s/physics.bin", argv[2]); phys = (u8 *)load(path, &size);
    if (!trig || !phys) return 2;
    dcp_set_trig(trig, trig + 8192);
    if (dcp_set_physics(phys, (u32)size) != 2) return 2;
    snprintf(path, sizeof path, "%s/z11/level.bin", argv[1]);
    g_level = (u8 *)load(path, &size);
    if (!g_level || rlvl_bind(&M, g_level) != 0) return 2;
    h = (const rlvl_header_t *)g_level;
    dcp_bind_map(&M);
    memset(&g_dcp_world, 0, sizeof g_dcp_world);
    g_dcp_world.zone = h->zone; g_dcp_world.act = h->act;
    g_dcp_world.mode = 1; g_dcp_world.flags = 1 | 0x20; g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_right = M.right; g_dcp_world.death_y = M.bottom; g_dcp_world.timescale = 0x100;
    o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
    for (i = 0; i < h->num_objects; i++) if (o[i].type == 0x72) { sx = o[i].x; sy = o[i].y; break; }
    g_dcp_world.check_x = sx; g_dcp_world.check_y = sy;
    dcp_player_create(&A, 0, 0, sx, sy);
    dcp_player_create(&B, 1, 0, sx, sy);
    objects_set_paths(argv[3], argv[4]);
    objects_add_player(&A); objects_add_player(&B);
    objects_set_view_count(8);
    CHECK(objects_init(&M, g_level) == 0, "objects_init");
    objects_add_player(&A); objects_add_player(&B);
    camera_create(g_level, &A, h->zone, h->act);
    {
        ObjHooks before = g_obj_hooks;
        vs_begin(&A, &B);
        CHECK(vs_active() && g_obj_hooks.item != before.item, "vs hooks installed");
        frames(30, 0, 0);
        /* items: Stage_NetProcessRivalEvents 7..10 on the rival of the collector */
        g_obj_hooks.item(g_obj_hooks.ctx, &A, OBJ_ITEM_VS_SLOW);
        CHECK(B.slowdown == 0x200 && A.slowdown == 0, "slowdown on the rival: %d / %d", B.slowdown, A.slowdown);
        g_obj_hooks.item(g_obj_hooks.ctx, &B, OBJ_ITEM_VS_CONFUSE);
        CHECK(A.confuse == 0x258 && B.confuse == 0, "confusion on the rival: %d / %d", A.confuse, B.confuse);
        frames(2, 0, 0);
        CHECK(B.slowdown == 0x200 - 2 && A.confuse == 0x258 - 2, "timers run down (%d, %d)", B.slowdown, A.confuse);
        B.tension = 0x1000;
        g_obj_hooks.item(g_obj_hooks.ctx, &A, OBJ_ITEM_VS_DRAIN);
        CHECK(B.tension == 0, "tension drained to %d", B.tension);
        /* Attract: the rival warps to the collector */
        reset_players(sx, sy);
        A.x += 0x6000;
        frames(1, 0, 0);
        g_obj_hooks.item(g_obj_hooks.ctx, &A, OBJ_ITEM_VS_11);
        CHECK(B.state == dcp_state_warp && B.warp_x == A.x, "warp state, target %ld", (long)(B.warp_x >> 8));
        {
            s32 tx = B.warp_x;
            frames(0x40, 0, 0);
            CHECK(B.state != dcp_state_warp && (B.x >> 8) == (tx >> 8) && !(B.pf2 & 0x400000), "warped to %ld (target %ld)", (long)(B.x >> 8), (long)(tx >> 8));
        }
        /* push box: B runs into A from the left */
        reset_players(sx, sy);
        frames(4, 0, 0);
        A.x = B.x + 0x1400;
        A.vs_timer = B.vs_timer = 0;
        frames(1, 0, 0x10);
        CHECK((B.pf & 0x10000) || (A.pf & 0x10000) || B.push_x != 0 || A.push_x != 0, "push contact: pf %lx / %lx", (unsigned long)A.pf, (unsigned long)B.pf);
        /* knock-back: A attacked (pf 0x20000 this frame) moving right */
        reset_players(sx, sy);
        frames(4, 0, 0);
        A.move_x = 0x200; A.vs_attack = 1; B.vs_timer = 0;
        vs_after_players();
        CHECK(B.vs_timer == 0xc && B.hitstop == 0x44 && (B.gnd_vel >= 0x480 || B.vel_x >= 0x480), "knock-back: timer %d hitstop %x vel %d/%d", B.vs_timer, B.hitstop, B.gnd_vel, B.vel_x);
        /* checkpoint per player */
        g_dcp_world.check_x = 1234; g_dcp_world.check_y = 567;
        g_obj_stage.check_player = &B;
        vs_after_players();
        vs_player_begin(0);
        CHECK(g_dcp_world.check_x != 1234, "player 1's checkpoint unchanged (%ld)", (long)g_dcp_world.check_x);
        vs_player_end(0);
        vs_player_begin(1);
        CHECK(g_dcp_world.check_x == 1234 && g_dcp_world.check_y == 567, "player 2's checkpoint taken");
        vs_player_end(1);
        /* camera effects of an object belong to the screens it is on */
        {
            static Obj fake;
            vs_set_camera_shake_fn(cam_shake);
            memset(&fake, 0, sizeof fake);
            fake.used = 1;
            reset_players(sx, sy);
            B.x = A.x + (0x800 << 8);          /* 2048 px apart */
            fake.h.x = B.x + 0x4000; fake.h.y = B.y;
            g_obj_cur = &fake;
            g_obj_hooks.shake(g_obj_hooks.ctx, 3);
            CHECK(s_shook[1] == 1 && s_shook[0] == 0, "shake near player 2 only on its camera (%d / %d)", s_shook[0], s_shook[1]);
            fake.h.x = A.x; fake.h.y = A.y;
            g_obj_hooks.shake(g_obj_hooks.ctx, 3);
            CHECK(s_shook[0] == 1 && s_shook[1] == 1, "shake near player 1 on its camera");
            B.x = A.x + 0x2000;
            g_obj_hooks.shake(g_obj_hooks.ctx, 3);
            CHECK(s_shook[0] == 2 && s_shook[1] == 2, "both players near: both cameras");
            B.x = A.x + (0x800 << 8);
            fake.h.x = B.x; fake.h.y = B.y;
            g_obj_hooks.fade(g_obj_hooks.ctx, 4, 0x100);
            g_obj_cur = NULL;
            { int k; for (k = 0; k < 20; k++) vs_frame_end(); }
            CHECK(vs_fade_level(1) == 16 && vs_fade_level(0) == 0, "object fade on player 2's screen only (%d / %d)", vs_fade_level(0), vs_fade_level(1));
            g_obj_cur = &fake;
            g_obj_hooks.fade(g_obj_hooks.ctx, 2, 0x100);
            g_obj_cur = NULL;
            { int k; for (k = 0; k < 20; k++) vs_frame_end(); }
            CHECK(vs_fade_level(1) == 0, "back from the fade");
        }
        /* goal: B first, then A */
        g_obj_hooks.goal(g_obj_hooks.ctx, &B, NULL);
        g_obj_hooks.goal(g_obj_hooks.ctx, &A, NULL);
        CHECK(vs_info()->winner == 1 && (A.pf & 0x1000000) && (B.pf & 0x200000), "goal: winner %d", vs_info()->winner);
        {
            u16 hh = 0xff, pp = 0xff, rr = 0xff;
            vs_filter_input(0, &hh, &pp, &rr);
            CHECK(hh == 0 && pp == 0, "the loser has no input");
            hh = 0xff; pp = 0xff;
            vs_filter_input(1, &hh, &pp, &rr);
            CHECK(pp == 0 && (hh & 0x30), "the winner runs on (held %04x)", hh);
        }
        g_obj_hooks.stage_exit(g_obj_hooks.ctx, OBJ_EXIT_RESULTS);
        CHECK(vs_info()->exit == VS_EXIT_RESULTS, "goal ring -> results");
        vs_end();
        CHECK(!vs_active() && g_obj_hooks.item == before.item && g_obj_hooks.goal == before.goal, "hooks restored");
        /* time over: nobody at the goal at 0x8c9f -> fade, draw */
        memset(&A, 0, sizeof A); memset(&B, 0, sizeof B);
        dcp_player_create(&A, 0, 0, sx, sy); dcp_player_create(&B, 1, 0, sx, sy);
        g_dcp_world.flags = 1 | 0x20;
        vs_begin(&A, &B);
        g_dcp_world.frame = 0x8c9f - 2;
        frames(4, 0, 0);
        CHECK(vs_info()->winner == 2 && (A.pf & 0x1000000) && (B.pf & 0x1000000), "time over: draw (%d)", vs_info()->winner);
        frames(40, 0, 0);
        CHECK(vs_info()->exit == VS_EXIT_RESULTS && vs_fade_level(0) == 16, "time over -> fade -> results (%d, fade %d)", vs_info()->exit, vs_fade_level(0));
        vs_end();
    }
    printf("vs rules: %d checks, %d failed\n", g_checks, g_fail);
    return g_fail != 0;
}
