/* Sonic Rush DC — native game entry. Stage viewer/bring-up stage:
 * loads a converted stage package from /cd/stages/<id>, renders background,
 * back layer, front layer with the 2.5x DS-framing camera. */
#include <kos.h>
#include <stdio.h>
#include <string.h>
#include "../engine/render.h"
#include "../engine/input.h"
#include "../engine/level.h"
#include "../engine/audio.h"
#include "../player/dcp_player.h"
#include "../model/model.h"
#include "../player/dcp_model.h"
#include "../objects/objects.h"
#include "../ui/ui.h"
#include "../sound/sound.h"
#include "../sound/bgm.h"
#include "../sound/snd_nns.h"
#include "../boss/boss.h"
#include "../boss/boss_final.h"
#include "../objects/obj_internal.h"
#include "../menus/menus.h"
#include "../save/save.h"
#include "../special/special.h"
#include "../cutscene/cutscene.h"
#include "../boss/boss_extra.h"
#include "../boss/boss7.h"
/* test builds (bosshp.txt): gBossStatus (boss_internal.h, not included here: its renaming macros
 * would turn BossHelp_Create, ScreenShake_Start, ... into the boss module's own functions) */
extern struct { u16 unk0; u32 unk4; u16 unk8; u16 unkA; } boss_gBossStatus;
#define gBossStatus boss_gBossStatus
#include "../demo/demo.h"
#include "../engine/loading.h"
#include "../engine/frameprof.h"
#include "../menus/extraact.h"
#include "../camera/camera.h"
#include "../zonefx/zonefx.h"
#include "../vs/vs.h"
#include "../vs/vs_cpu.h"
#include <stdlib.h>
#include <math.h>

/* test builds (/cd/debug/frameprof.txt): frame pacing in RAM, summaries at scene changes */
#define FRAME_BEGIN() do { frameprof_work_end(); render_begin_frame(); frameprof_frame(); } while(0)
/* test builds (/cd/debug/sweep.txt = frames per act, boss acts twice): every act in story order, the
 * special stage after each act 2 of zone 1, the Extra Zone after zf3, again and again; A pressed
 * every 30 frames in the cutscene / special-stage / Extra loops, 99 lives */
static int g_sweep, g_sweep_tick;
static void sweep_input(InputState *in) { if(g_sweep && ++g_sweep_tick % 30 == 0) in->pressed |= IN_JUMP; }

/* Story order (Sonic): zone N acts 1, 2 and the boss act 3. */
static const char *k_stages[] = { "z11", "z12", "z13", "z21", "z22", "z23", "z31", "z32", "z33",
                                  "z41", "z42", "z43", "z51", "z52", "z53", "z61", "z62", "z63",
                                  "z71", "z72", "z73",
                                  /* zone 8 (final story: zf3 = final boss arena), zone 9 (extra) */
                                  "z81", "z82", "zf3", "z91", "z92" };
#define NUM_STAGES (sizeof(k_stages) / sizeof(k_stages[0]))

static DcPlayer g_player;
/* Player 3D model (son.nsbmd from the owner's ROM, converted by nsbmd2dc.py). */
static int g_model_ok;
static rlvl_map_t g_map;
static int g_player_ok;
static int g_chara;   /* 0 Sonic, 1 Blaze (hold Y while booting until the menus exist) */
static int g_obj_ok;
static int g_is_boss, g_boss_started;
static int g_stage_idx;
static Level *g_lv;
static int g_difficulty = 1;   /* gGameState.unk14: 1 normal, 0 easy (options) */
static int g_frontend;   /* 1: SEGA logo / title / menus / zone map are running */
static int g_emeralds;
static int g_model_chara = -1;

static void *load_file(const char *path, uint32_t *size)
{
    file_t f = fs_open(path, O_RDONLY);
    void *b;
    if(f == FILEHND_INVALID) return NULL;
    *size = fs_total(f);
    b = malloc(*size);
    if(b && loading_read(f, b, *size) != (ssize_t)*size) { free(b); b = NULL; }
    fs_close(f);
    return b;
}

/* DS pad bits the translated player code expects (Player_ReadInput). */
static uint16_t ds_pad(uint32_t b)
{
    uint16_t o = 0;
    if(b & IN_JUMP) o |= 0x001;   /* A */
    if(b & IN_TRICK) o |= 0x002;  /* B */
    if(b & IN_RIGHT) o |= 0x010;
    if(b & IN_LEFT) o |= 0x020;
    if(b & IN_UP) o |= 0x040;
    if(b & IN_DOWN) o |= 0x080;
    if(b & IN_BOOST) o |= 0x400;  /* X */
    if(b & IN_ALT) o |= 0x800;    /* Y */
    if(b & IN_R) o |= 0x100;      /* R: Player_State_InAir tricks / air action, grind tricks */
    if(b & IN_L) o |= 0x200;      /* L */
    return o;
}

/* Special stage round trip (SPECIAL.md 5.1/5.2). Player_Destructor with
 * gGameState.unk18 & 0x40 saves these into data_022c4590; Player_Create
 * restores them; Stage_SaveEventState keeps collected rings / destroyed
 * objects (objects module, weak until it provides the API). */
void objects_save_event_state(void) __attribute__((weak));
void objects_restore_event_state(void) __attribute__((weak));
static struct {
    int active;
    s16 rings, rings_total, tension;
    u32 shield, water;
    int x, y;
    u32 frame, score;
} g_ss;
static int g_special_pending;
static int g_demo, g_demo_spawn_x, g_demo_spawn_y;
static int g_dbg_pos_set, g_dbg_pos_x, g_dbg_pos_y;   /* test builds: /cd/debug/startpos.txt, first spawn only */   /* attract demo running (DemoPlay) */
static uint64_t g_prof[4], g_dprof[6], g_fmax;
static int g_fslow;   /* draw: bg, layers, player model, objects, ui, boss */   /* us per 120 frames: objects update, player, draw, end_frame wait */
/* test builds: /cd/debug/bosshp.txt "N": the boss's hit points are held at N or below once the
 * fight starts (a real fight reaches its late phases: Boss7 clash and talk, BossF sky charge and talk) */
static int g_dbg_bosshp = -1;
static void dbg_boss_log(void *ctx, const char *ev, s32 a, s32 b)
{
    (void)ctx;
    if(strcmp(ev, "idle") && strcmp(ev, "draw")) printf("boss: %s %ld %ld (frame %lu, brightness %ld)\n", ev, (long)a, (long)b,
                                                        (unsigned long)g_dcp_world.frame, (long)boss_brightness());
}
static int g_dbg_special = -1;   /* test builds: /cd/debug/special.txt = act frame to enter the special stage */
/* test builds: /cd/debug/autoclear.txt "N": the act is cleared at stage frame N (the goal's
 * StageShutdown_Create(ClearDemo_Create)); in a boss act Stage_OnBossCleared runs instead (the
 * fight is skipped), and the Extra boss counts as beaten after N frames of its fight */
static int g_dbg_autoclear = -1;
/* test builds: /cd/debug/autodie.txt "N L": at stage frame N the lives are set to L and the
 * player is put below the map (pit death; L 1: game over) */
static int g_dbg_autodie = -1, g_dbg_autodie_lives = 1, g_dbg_autodie_armed;   /* once per stage start */
/* test builds: /cd/debug/spawn.txt "x y [tension]": the first stage starts the player there (px),
 * with that tension (0..0x12c0), e.g. at a special ring */
static int g_dbg_spawn_x, g_dbg_spawn_y, g_dbg_spawn_tension = -1;
static int g_dbg_spawn_late_x, g_dbg_spawn_late_y;   /* the title card moves the player: placed again after it */
static int g_dbg_spawn_trace;   /* frames left of the player trace after the debug spawn */
int ui_exit_cleared(void);   /* ui/ui_state.c: the stage ended through the act results */
void ui_extra_pause_open(void);                   /* ui/ui_pause.c: the pause menu over the Extra boss */
int ui_extra_pause_frame(const InputState *in);
static void (*g_prev_stage_exit)(void *ctx, s32 kind);
/* g_dcp_hooks.restart in a boss act: Player_Respawn -> StageRestart_Create destroys every task and
 * creates the stage again (StageRestart_State_Release / _CreateStage), so the boss starts over
 * with full health after the title card. The boss is created again in the main loop. */
static void (*g_prev_boss_restart)(void *ctx, DcPlayer *p);
static int g_boss_restart;
/* the same StageRestart in an act: the DS makes the act's objects and rings again from the level's
 * lists (Stage_SetupZoneResources + Stage_Create), so a death brings back the enemies, broken
 * platforms and rings of the last attempt; the player is moved to the checkpoint by Player_Respawn.
 * Before this the objects of the last attempt were kept: what was destroyed stayed gone, and a
 * section that needs them (enemies to bounce on, platforms that fall) could no longer be passed. */
static int g_stage_restart;
static void boss_restart_hook(void *ctx, DcPlayer *p)
{
    if(g_prev_boss_restart) g_prev_boss_restart(ctx, p);
    if(g_is_boss && g_dcp_world.mode != 1 && !(p->pf2 & 0x400000)) g_boss_restart = 1;
    else if(!g_is_boss && g_dcp_world.mode != 1 && !(p->pf2 & 0x400000)) g_stage_restart = 1;
    printf("restart: boss %d, lives %d, rings %d, frame %lu\n", g_is_boss, (int)g_dcp_world.lives, (int)p->rings, (unsigned long)g_dcp_world.frame);
}

static void stage_exit_hook(void *ctx, s32 kind)
{
    if(kind == OBJ_EXIT_SPECIAL_STAGE) { g_special_pending = 1; return; }   /* Player_ExitToSpecialStage */
    if(g_prev_stage_exit) g_prev_stage_exit(ctx, kind);
}

/* g_dcp_hooks.gravity_flipped = Stage_IsGravityFlipped (src/game/Stage): Dead Line acts flip
 * below (act 1) / above (act 2) the split line (camera module's Stage_GetSplitLineY) */
s32 Stage_GetSplitLineY(s32 x);
static dcp_bool gravity_flipped_hook(void *ctx, s32 x, s32 y)
{
    (void)ctx;
    if(g_dcp_world.zone == 6 && g_dcp_world.act < 2) {
        if(g_dcp_world.act == 1) { if(y < Stage_GetSplitLineY(x)) return DCP_TRUE; }
        else if(y > Stage_GetSplitLineY(x)) return DCP_TRUE;
    }
    return DCP_FALSE;
}

/* g_boss_hooks.fx3d: a boss's StageObj_Load3dBAC one-shot effect (boss-arena world coordinates) */
static int boss_fx3d_hook(void *ctx, const char *id, s32 anim, s32 x, s32 y, s32 z, u32 flags, s32 scale)
{
    (void)ctx;
    return objects_fx3d_spawn(id, (int)anim, x, y, z, flags, scale);
}

static void spawn_player(Level *lv)
{
    uint32_t i;
    const rlvl_object_t *obj = (const rlvl_object_t *)(lv->data + lv->hdr->sec[RLVL_SEC_OBJECTS].offset);
    int sx = 128, sy = 128;
    for(i = 0; i < lv->hdr->num_objects; i++)
        if(obj[i].type == 114) { sx = obj[i].x; sy = obj[i].y; break; }   /* PlayerStart */
    if(g_ss.active) { sx = g_ss.x; sy = g_ss.y; }   /* 280 px below the special ring */
    else if(g_demo_spawn_x || g_demo_spawn_y) { sx = g_demo_spawn_x; sy = g_demo_spawn_y; }   /* DemoPlay start point */
    else if(g_dbg_pos_set) { sx = g_dbg_pos_x; sy = g_dbg_pos_y; g_dbg_pos_set = 0; printf("debug start position %d,%d\n", sx, sy); }
    else if(g_dbg_spawn_x || g_dbg_spawn_y) {
        sx = g_dbg_spawn_late_x = g_dbg_spawn_x; sy = g_dbg_spawn_late_y = g_dbg_spawn_y; g_dbg_spawn_x = g_dbg_spawn_y = 0;
    }
    rlvl_bind(&g_map, lv->data);
    dcp_bind_map(&g_map);
    memset(&g_player, 0, sizeof(g_player));
    dcp_player_create(&g_player, g_chara, 0, sx, sy);
    if(g_dbg_spawn_tension >= 0) { g_player.tension = (s16)g_dbg_spawn_tension; g_dbg_spawn_tension = -1; }
    g_dbg_autodie_armed = 1;
    objects_shutdown(); objects_models_free();
    if(g_model_ok) dcp_model_attach(&g_dcp_model, &g_player);   /* anim hooks, chained before the objects' */
    g_obj_ok = objects_init(&g_map, lv->data) == 0;
    if(g_obj_ok) {
        if(g_ss.active && objects_restore_event_state) objects_restore_event_state();
        objects_add_player(&g_player); objects_set_view_count(3); objects_respawn_view();
        /* DCP_DF_ANIM_DONE comes from the model's joint animation (StageObj_Draw3DEx);
         * son.bac timing stands in only when the model is missing */
        objects_set_player_anim_done(!g_model_ok);
        /* object models (RopeC, Wheel2D, Hand2d, GmkTable, slide, crane, glider) are drawn in the
         * 3D layer (objects_draw_models); their 2D stand-ins only while a model is not loaded */
        objects_set_model_sprites(0);
    }
    printf("objects: vram free after objects_init %lu\n", (unsigned long)render_vram_free());
    sound_load_bank(sound_stage_sdat(g_dcp_world.zone, g_dcp_world.act, g_chara));
    sound_hooks_reset_players();
    sound_obj_reset();
    bgm_bind_player(&g_player);   /* StageJingle / gravity-flip layer (sound/bgm.c) */
    g_is_boss = 0; g_boss_started = 0;
    boss_shutdown();
    if(g_dcp_world.act == 2) {
        boss_set_paths("/cd/models", "/cd/boss");
        boss_set_difficulty(g_difficulty);
        sound_boss_reset();   /* the previous fight's sound handles */
        g_is_boss = boss_init(lv->hdr->stage, lv->data, &g_player, 1) == 0;
        printf("boss: %s\n", g_is_boss ? "ok" : "FAILED");
    }
    ui_bind_player(&g_player);
    g_ui_game.language = 1;   /* English (gGameLanguage) */
    g_ui_game.difficulty = (u8)g_difficulty;
    g_ui_game.save_progress[0] = gSaveData.unkC; g_ui_game.save_progress[1] = gSaveData.unk64;
    ui_stage_start(g_dcp_world.zone, g_dcp_world.act, g_chara);
    if(g_obj_ok) objects_models_prepare();   /* the stage's object models and 3D BACs, under the loading ring */
    loading_end();
    audio_load_end();
    if(g_obj_hooks.stage_exit != stage_exit_hook) { g_prev_stage_exit = g_obj_hooks.stage_exit; g_obj_hooks.stage_exit = stage_exit_hook; }
    if(g_ss.active) {
        /* Player_Create with unk18 & 0x40: rings, shield, tension, water flag, blinking invulnerability */
        g_player.rings = g_ss.rings; g_player.rings_total = g_ss.rings_total; g_player.tension = g_ss.tension;
        g_player.pf |= g_ss.shield;
        if(g_ss.water) g_player.pf2 |= 0x8000;
        g_player.pf2 |= 0x200000;
        g_player.invuln = g_player.invuln_len;
        g_dcp_world.frame = g_ss.frame; g_ui_game.total = g_ss.score;
        g_ss.active = 0;
    }
    printf("objects: %s\n", g_obj_ok ? "ok" : "FAILED");
    /* Camera_Create (+ ZoneBg_Init / ZoneFx_Init): the DS camera pair and the DC window */
    camera_create(lv->data, &g_player, g_dcp_world.zone, g_dcp_world.act);
    zonefx_init(lv, g_dcp_world.zone, g_dcp_world.act);
    camera_apply();
    ui_set_camera(g_cam_x, g_cam_y);
    printf("player spawned at %d,%d\n", sx, sy);
}

/* StageRestart_Create in a boss act (Player_Respawn after a death): the DS destroys every task and
 * creates the stage again, so the fight starts over from nothing of the last attempt: a new player at
 * PlayerStart (Player_Create: no ride, no crush, no 3D depth, no flags of the death), the rings and
 * the objects again (RingManager_Create: the arena's rings are back), the water level and the boss.
 * Lives, score and the stage clock are the UI's (ui_stage_restart). Before this only the boss was made
 * again: a crush by the arena left the player in the floor at the respawn, and it fell to its death
 * again and again until the game was over; and the rings taken stayed gone. */
static void boss_stage_restart(Level *lv)
{
    uint32_t i;
    const rlvl_object_t *obj = (const rlvl_object_t *)(lv->data + lv->hdr->sec[RLVL_SEC_OBJECTS].offset);
    int sx = 128, sy = 128;
    for(i = 0; i < lv->hdr->num_objects; i++)
        if(obj[i].type == 114) { sx = obj[i].x; sy = obj[i].y; break; }   /* PlayerStart */
    memset(&g_player, 0, sizeof(g_player));
    dcp_player_create(&g_player, g_chara, 0, sx, sy);
    objects_shutdown();
    if(g_model_ok) dcp_model_attach(&g_dcp_model, &g_player);
    g_obj_ok = objects_init(&g_map, lv->data) == 0;
    if(g_obj_ok) {
        objects_add_player(&g_player); objects_set_view_count(3); objects_respawn_view();
        objects_set_player_anim_done(!g_model_ok);
        objects_set_model_sprites(0);
        objects_models_prepare();   /* (the same stage: the models stay loaded) */
    }
    sound_obj_reset();
    ui_bind_player(&g_player);
    printf("boss: stage restart, player at %d,%d, objects %s, lives %d\n", sx, sy, g_obj_ok ? "ok" : "FAILED", (int)g_dcp_world.lives);
}

static void load_player_model(void)
{
    if(g_model_ok && g_model_chara == g_chara) return;
    g_model_chara = g_chara;
    /* body + ball model of the character, son.dca / son.dcv, Player_SetAnimation tables
     * (dcp_model_load frees the previous character's textures first) */
    g_model_ok = dcp_model_load(&g_dcp_model, g_chara, "/cd/player", 1) == 0;
    printf("player model: %s (vram free %lu)\n", g_model_ok ? "ok" : "MISSING", (unsigned long)render_vram_free());
}

/* StageObj_Draw3DEx for the player: (x, y, z) in DS 1/256 px */
static void place_player_model(s32 x, s32 y, s32 z, dcm_camera_t *cam, dcm_mtx_t *mdl, dcm_lights_t *lights, int ghost)
{
    if(g_is_boss) {
        s32 bt[3], br[9];
        /* boss arenas are 3D: the player is placed in world space under the boss camera */
        boss_camera(cam);
        if(boss_player_model(bt, br)) {
            /* circular arenas (Boss2+): the boss module builds the player transform */
            int i;
            dcm_mtx_t ry, rot;
            for(i = 0; i < 9; i++) rot.m[i / 3][i % 3] = br[i] / 4096.0f;
            rot.m[3][0] = bt[0] / 4096.0f; rot.m[3][1] = bt[1] / 4096.0f; rot.m[3][2] = bt[2] / 4096.0f;
            dcm_stage_model_matrix(&ry, 1.0f, 0x4000, 0, 0, 0, 0);
            dcm_mtx_mul(&ry, &ry, &rot);
            dcp_model_matrix_ext(&ry, mdl);
            if(!boss_lights(lights)) dcp_model_lights(&g_player, ghost, lights);   /* Boss3+: stage lights */
            return;
        }
        dcp_model_matrix(&g_player, x / 256.0f, -y / 256.0f, z / 256.0f, mdl);
    } else {
        float tx = x / 256.0f - g_cam_x, ty = -(y / 256.0f - g_cam_y);
        dcm_camera_stage(cam);
        dcm_camera_stage_offaxis(cam, &tx, &ty);
        dcp_model_matrix(&g_player, tx, ty, 0, mdl);
    }
    dcp_model_lights(&g_player, ghost, lights);
}

/* ClearDemo_DrawModel (act / boss results, ui_results_model): the player's body model posed with
 * the results animation (ClearDemo_InitModel: joint 0x26 / 0x27, Blaze 0x48 / 0x49; visibility 2,
 * Blaze 1 / 5; no loop) under the ClearDemo camera and lights (ClearDemo_Create), scaled, rotated
 * and moved by ClearDemoModel_*, in the depth band between the results BG and the tally (ui.h).
 * Replaces the stage player's draw while the results show it, as the title card close-up does. */
static int draw_results_model(pvr_list_t list)
{
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static uint8_t visb[DCM_MAX_JOINTS];
    static const u16 col[3] = {0x6f7b, 0x2108, 0x2108};
    static const s16 vec[3][3] = {{0, -0x4a0, -0xf40}, {0xec7, -0x600, 0}, {0, 0x620, 0xec7}};
    const dcm_model_t *mdl = &g_dcp_model.res[0];
    s32 pos[3], scale, jnt, vis, frame, mx, my;
    /* eye z = FX_Div(COS(0x1555), SIN(0x1555)) * 0x60 (ClearDemo_DrawModel) */
    s32 eye[3] = {0x80000, -0x86666, 0x1bb6 * 0x60}, at[3] = {0x80000, -0x60000, 0};
    u16 ry;
    dcm_camera_t cam;
    dcm_mtx_t mm;
    dcm_lights_t l;
    dcm_draw_opts_t o;
    float c[3], d, k, k0, r;
    int i, j, nf;
    if(!ui_results_model(pos, &scale, &ry, &jnt, &vis, &frame)) return 0;
    if(!g_dcp_model.ok || !mdl->hdr || !g_dcp_model.jnt.hdr || jnt >= g_dcp_model.jnt.hdr->num_anims) return 1;
    nf = g_dcp_model.jnt.anims[jnt].num_frames;
    dca_sample(&g_dcp_model.jnt, jnt, (float)((frame >> 12) < nf ? (frame >> 12) : nf - 1), mdl, loc);
    dcm_pose(mdl, loc, pal);
    if(g_dcp_model.vis.hdr && vis < g_dcp_model.vis.hdr->num_anims) {
        nf = g_dcp_model.vis.anims[vis].num_frames;
        dca_visibility(&g_dcp_model.vis, vis, (float)((frame >> 12) < nf ? (frame >> 12) : nf - 1), mdl, visb);
    } else memset(visb, 1, mdl->hdr->num_joints);
    /* the camera of the title card close-up (same LookAt / perspective code and values) */
    mx = pos[0]; my = pos[1];
    dcp_model_titlecard_view(&g_player, eye, at, mx, my, 0, UI_TC_MODEL_Z0, UI_TC_MODEL_Z1, &cam, &mm);
    /* depth band for this model: about 5x the stage player (scale 0x107fc against 0x34cc), feet at
     * pos, so its centre is ~80 px above pos and every vertex lies within ~110 px of it */
    r = 110.0f * scale / 0x107fc;
    c[0] = pos[0] / 4096.0f - eye[0] / 4096.0f; c[1] = pos[1] / 4096.0f + 80.0f * scale / 0x107fc - eye[1] / 4096.0f;
    c[2] = pos[2] / 4096.0f - eye[2] / 4096.0f;
    d = sqrtf(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
    k = 1.0f / (UI_TC_MODEL_Z1 * (d - r > 2.0f ? d - r : 2.0f));
    if(UI_TC_MODEL_Z0 * (d + r) * k > 1.0f) k = 0.999f / (UI_TC_MODEL_Z0 * (d + r));
    k0 = cam.near_w * 2.0f;
    if(k0 > 0.0f) {
        for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) cam.proj[i][j] *= k / k0;
        cam.near_w = 0.5f * k;
    }
    /* Model_Draw: v * S(scale) * RotY(rot) + translation */
    dcm_stage_model_matrix(&mm, scale / 4096.0f, ry, 0, pos[0] / 4096.0f, pos[1] / 4096.0f, pos[2] / 4096.0f);
    /* NNS_G3dGlbLightColor / LightVector of ClearDemo_Create, turned by the camera */
    memset(&l, 0, sizeof(l));
    l.count = 3;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++)
            l.dir[i][j] = (vec[i][0] * cam.view.m[0][j] + vec[i][1] * cam.view.m[1][j] + vec[i][2] * cam.view.m[2][j]) / 4096.0f;
        l.col[i][0] = (col[i] & 31) / 31.0f; l.col[i][1] = (col[i] >> 5 & 31) / 31.0f; l.col[i][2] = (col[i] >> 10 & 31) / 31.0f;
    }
    o.list = list; o.pt_list = PVR_LIST_PT_POLY; o.alpha_scale = 255;
    dcm_draw(mdl, pal, &mm, &cam, visb, &l, &o);
    return 1;
}

static void draw_player_model(pvr_list_t list)
{
    dcm_camera_t cam;
    dcm_mtx_t mdl;
    dcm_lights_t lights;
    s32 ax, ay, az, tsp, teye[3], tat[3];
    int tzoom;
    if(draw_results_model(list)) return;
    /* StageTitleCardPlayer_Draw replaces the player's draw during the title card: close-up camera,
     * depth between the title card BG and its sprites (UI_TC_MODEL_Z0..Z1) */
    if(ui_titlecard_player_draw(&tsp, teye, tat, &ax, &ay, &tzoom)) {
        dcp_model_titlecard_view(&g_player, teye, tat, ax, ay, tzoom, UI_TC_MODEL_Z0, UI_TC_MODEL_Z1, &cam, &mdl);
        dcp_model_lights(&g_player, 0, &lights);
        dcp_model_draw(&g_dcp_model, &g_player, list, &mdl, &cam, &lights);
        return;
    }
    /* Player_Draw: high-speed / homing after-image at the previous position */
    if(!g_is_boss && dcp_model_afterimage(&g_player, &ax, &ay, &az)) {
        place_player_model(ax, ay, az, &cam, &mdl, &lights, 1);
        dcp_model_draw(&g_dcp_model, &g_player, list, &mdl, &cam, &lights);
    }
    place_player_model(g_player.x, g_player.y, g_player.z, &cam, &mdl, &lights, 0);
    dcp_model_draw(&g_dcp_model, &g_player, list, &mdl, &cam, &lights);
}

static int load_stage(Level *lv, int idx)
{
    loading_begin();   /* spinning ring until spawn_player has finished */
    audio_load_begin(); /* music/effects fade out over the load (AUDIO.md 13) */
    g_stage_idx = idx;
    char dir[64];
    uint64_t t0 = timer_ms_gettime64();
    int r;
    snprintf(dir, sizeof(dir), "/cd/stages/%s", k_stages[idx]);
    {   /* frame pacing: one scene per act (and character) */
        char sn[24];
        snprintf(sn, sizeof(sn), "%s %s", k_stages[idx], g_chara ? "blaze" : "sonic");
        frameprof_scene(sn, 1);
    }
    r = level_load(lv, dir);
    if(r == 0) {
        /* gGameState.unk1C / unk1D: stage id = zone * 3 + act (0, 1, 2 = boss) */
        g_dcp_world.zone = (u8)(lv->hdr->stage / 3);
        g_dcp_world.act = (u8)(lv->hdr->stage % 3);
        zonefx_stage_loaded(lv, g_dcp_world.zone, g_dcp_world.act);   /* tinted banks before the sprites take rows */
    }
    /* Stage_SetupZoneResources -> Stage_PlayBGM(0): act / boss music of the zone SDAT,
     * character and act (sound/bgm.c, /cd/music/streams.txt); jingles preloaded here */
    if(r == 0) bgm_stage_start(g_dcp_world.zone, g_dcp_world.act, g_chara, &g_player);
    if(g_sweep) { g_dcp_world.lives = 99; g_ui_game.save_lives[0] = g_ui_game.save_lives[1] = 99; }
    printf("stage %s: load %d in %llu ms, %dx%d px, %d block pages, vram used %lu, vram free %lu\n",
           k_stages[idx], r, (unsigned long long)(timer_ms_gettime64() - t0),
           r == 0 ? level_width_px(lv) : 0, r == 0 ? level_height_px(lv) : 0, lv->num_pages,
           (unsigned long)lv->vram_used, (unsigned long)render_vram_free());
    return r;
}

/* Menu sound hooks (MenuHooks): sys SDAT music streams as /cd/music/sNN.wav
 * (NN = sequence id, loop points in /cd/music/sys.txt), SE through the
 * native sequencer with the sys bank. */
static void menu_bgm(int seq) { bgm_set_context("sys"); if(bgm_play("sys", seq) != 0) printf("menu bgm %d: no stream\n", seq); }
/* the front end plays from the sys bank; the sound test may have loaded another one */
static void menu_sys_bank(void)
{
    if(strcmp(sound_bank_name(), "sys") != 0 && sound_load_bank("sys") != 0) printf("sys sound bank missing\n");
}
static void menu_se(int arc, int id) { menu_sys_bank(); SysSound_StartSEArc(arc, id); }   /* cuts the previous menu SE */
static void menu_voice(int id) { menu_sys_bank(); SysSound_StartVoice(id, 0); }
static void menu_voice_alt(int id) { menu_sys_bank(); SysSound_StartVoice(id, 1); }
static void menu_voice_arc(int arc, int id) { menu_sys_bank(); Sound_PlayVoice(NULL, (uint32_t)arc, (uint32_t)id); }
/* g_menu_hooks.sound_test: SoundTest_LoadTrack / SoundTest_PlayTrack. archive = gSoundTestArchiveNameTable
 * index (snd/<name>/sound_data.sdat); type 0 BGM (archive 0 group 0xe = STRM), 1 SE, 2 voice. The music is the
 * stream of (sdat, seq) (musictab.py), STRM 0 / 1 the staff-roll recordings; effects and voices load the
 * archive's bank: a zone SDAT's own bank, for sys the SSAR's own bank sys_aNN (csbank.py / sfxbank.py) unless
 * the sys bank keeps that SSAR (0, 1, 0x11) */
static const char *const k_sound_test_sdat[] = {
    "sys", "extra_end", "z1s", "z1b", "z1es", "z1eb", "z2s", "z2b", "z2es", "z2eb", "z3s", "z3b", "z3es", "z3eb",
    "z4s", "z4b", "z4es", "z4eb", "z5s", "z5b", "z5es", "z5eb", "z6s", "z6b", "z6es", "z6eb", "z7s", "z7b", "z7es",
    "z7eb", "zfes", "zfeb", "ze"};
static void menu_sound_test_stop(int arc, int id, int fade) { NNS_SndPlayerStopSeqBySeqArcIdx(arc, id, fade); }
static void menu_sound_test(int index, int archive, int type, int group, int arc, int id)
{
    const char *sd = archive >= 0 && archive < (int)(sizeof k_sound_test_sdat / sizeof k_sound_test_sdat[0]) ?
                     k_sound_test_sdat[archive] : "sys";
    char bank[16];
    (void)index;
    bgm_stop_sound_all();
    if(type == 0) {
        if(archive == 0 && group == 0xe) {   /* NNS_SndArcStrmStart(strm, id): Final_Roll / Extra_Roll */
            if(audio_music_play(id == 1 ? "/cd/music/Extra_Roll.wav" : "/cd/music/Final_Roll.wav", 0, 0) != 0)
                printf("sound test: stream %d missing\n", id);
        } else if(bgm_play(sd, id) != 0) {
            printf("sound test: no stream for %s seq %d\n", sd, id);
        }
        return;
    }
    if(!strcmp(sd, "sys") && arc != 0 && arc != 1 && arc != 0x11) snprintf(bank, sizeof bank, "sys_a%02d", arc);
    else snprintf(bank, sizeof bank, "%s", sd);
    if(strcmp(sound_bank_name(), bank) != 0 && sound_load_bank(bank) != 0) {
        printf("sound test: bank %s missing\n", bank);
        return;
    }
    if(type == 1) Sound_PlaySeqArc(NULL, (uint32_t)arc, (uint32_t)id);
    else Sound_PlayVoice(NULL, (uint32_t)arc, (uint32_t)id);
}
static void menu_stop_all(void) { bgm_stop_sound_all(); }
static void enter_front_end(void)
{
    frameprof_scene("front end", 0);
    g_frontend = 1;
    bgm_stage_end();
    bgm_set_context("sys");
    audio_music_stop();
    if(sound_load_bank("sys") != 0) printf("sys sound bank missing\n");
}

/* g_obj_hooks.effect: object effects that belong to the frame compositor.
 * ScreenFlash_Create: full-frame white flash fading over 16 frames.
 * Enemy-room events go to the UI (EnemyRoomHud). */
static int g_flash;
static int g_vs_script = -1;   /* test builds: /cd/debug/vs_p2.txt (scripted pads, see vs_script_pad) */
static void obj_effect_hook(void *ctx, DcPlayer *p, s32 kind, s32 a, s32 b)
{
    (void)ctx; (void)p;
    switch(kind) {
    case OBJ_FX_SCREEN_FLASH: g_flash = 16; break;
    case OBJ_FX_ENEMY_ROOM: ui_enemy_room(a, b); break;   /* EnemyRoomHud */
    case OBJ_FX_ZONE4_BLDY: zonefx_bldy(a, b); break;     /* ZoneFx_Zone4_StartBldy* (dark areas) */
    default: bgm_obj_effect(p, kind, a); break;          /* OBJ_FX_BGM_LAYER / OBJ_FX_JINGLE */
    }
}
static void ui_voice_hook(void *ctx, s32 arc, s32 id) { (void)ctx; Sound_PlayVoice(NULL, (uint32_t)arc, (uint32_t)id); }
int StoryProgress_IsStageNew(void);   /* menus/mn_story.c (BOOL) */
static int ui_stage_new_hook(void *ctx) { (void)ctx; return StoryProgress_IsStageNew(); }

/* g_boss_hooks.player_box_flags (Boss4_Action_Defeat): the player's hit boxes
 * belong to the objects module (box 0 = body coll2A0, 1 = attack coll2CC). */
static void boss_player_box_flags(void *ctx, DcPlayer *p, s32 box, u16 clear, u16 set)
{
    ObjPlayerCtx *c = obj_player_ctx(p);
    ObjBox *b;
    (void)ctx;
    if(!c) return;
    b = box == 0 ? &c->body : &c->atk;
    b->flags = (u16)((b->flags & ~clear) | set);
}

/* g_boss_hooks.cleared: StageShutdown_Create(Stage_OnBossCleared) -> the act
 * results (ClearDemo), handled by the UI like the goal of an act. */
static int g_after_boss_pending;
static void boss_cleared_hook(void *ctx)
{
    (void)ctx;
    /* Stage_OnBossCleared: story bosses of zones 0..6 run DemoScript_StartAfterBoss
     * first; the final zone (7) and time attack go straight to ClearDemo */
    if(g_dcp_world.mode == UI_MODE_STORY && g_dcp_world.zone != 7) g_after_boss_pending = 1;
    else ui_stage_exit(OBJ_EXIT_CLEAR_DEMO);
}

/* g_boss_hooks.help: BossHelp_Create / BossHelp_Destroy drawn by the UI module */
static void boss_help_hook(void *ctx, s32 kind, s32 a, s32 b)
{
    (void)ctx; (void)b;
    if(kind == BOSS_HELP_CREATE) BossHelp_Create((u32)a); else BossHelp_Destroy();
}

/* ---- cutscenes (CUTSCENES.md section 2) ---- */
int StoryProgress_IsStageNew(void);
static void cs_story(int zone)
{
    CsStory st;
    memset(&st, 0, sizeof(st));
    st.character = g_chara; st.zone = zone; st.stage_new = StoryProgress_IsStageNew();
    st.save_flags = gSaveData.unk8; st.sonic_progress = gSaveData.unkC; st.blaze_progress = gSaveData.unk64;
    st.language = menus_language(); st.sonic_emeralds = gSaveData.unkF & 0x7f;
    cutscene_set_story(&st);
}
/* DemoScript_Finish / TalkDemo_MarkSeen / StaffRoll_State_Finish -> save */
static CsNext cs_finish(void)
{
    const CsResult *r = cutscene_result();
    CsNext next = r ? r->next : CS_NEXT_NONE;
    if(r) {
        if(r->progress >= 0) {
            if(g_chara) SaveData_SetBlazeProgress((uint16_t)r->progress); else SaveData_SetSonicProgress((uint16_t)r->progress);
            SaveData_StoreLives(g_chara, (uint8_t)g_dcp_world.lives, (uint16_t)g_emeralds);
        }
        if(r->set_save_flags) {
            SaveData_SetProgressFlags(SaveData_GetProgressFlags() | r->set_save_flags);
            printf("save flags |= %#lx -> %#lx\n", (unsigned long)r->set_save_flags, (unsigned long)SaveData_GetProgressFlags());
        }
        if(r->set_sonic_progress >= 0) SaveData_SetSonicProgress((uint16_t)r->set_sonic_progress);
        if(r->set_blaze_progress >= 0) SaveData_SetBlazeProgress((uint16_t)r->set_blaze_progress);
        if(r->game_cleared) SaveData_SetGameCleared();
        if(r->progress >= 0 || r->set_save_flags || r->set_sonic_progress >= 0 || r->set_blaze_progress >= 0 || r->game_cleared)
            SaveData_Commit();
    }
    cutscene_shutdown();
    return next;
}
/* full-screen cutscene (PlayerDemo, TransformDemo, DemoScript, Ending, StaffRoll) run to its end */
static CsNext run_cutscene(CsKind kind, int id, int zone, const char *return_bank)
{
    InputState in = {0};
    cs_story(zone);
    cutscene_set_return_bank(return_bank);
    loading_begin(); audio_load_begin();
    if(cutscene_start(kind, id, g_chara) != 0) { loading_end(); audio_load_end(); printf("cutscene %d/%d: missing data\n", kind, id); return CS_NEXT_NONE; }
    loading_end(); audio_load_end();
    printf("cutscene %d/%d start\n", kind, id);
    {
        int pg;
        char prev[32], sn[24];
        snprintf(prev, sizeof(prev), "%s", frameprof_scene_name(&pg));
        snprintf(sn, sizeof(sn), "cutscene %d/%d", kind, id);
        frameprof_scene(sn, 0);
        for(;;) {
            int done;
            input_poll(&in);
            sweep_input(&in);
            done = cutscene_run_frame(&in);
            sound_update();
            FRAME_BEGIN(); cutscene_draw(); render_end_frame();
            audio_update();
            if(done) break;
        }
        frameprof_scene(prev, pg);   /* back to the scene that started it */
    }
    {
        CsNext n = cs_finish();
        printf("cutscene %d/%d end, next %d\n", kind, id, n);
        return n;
    }
}
/* SceneChange_ToEnding / ToExtraEnding: the ending, the staff roll, PostStaffRoll; then
 * PostStaffRoll_ShowUnlockNotices (menus) and the SEGA logo */
static void run_ending(CsKind kind, int zone)
{
    CsNext n = run_cutscene(kind, 0, zone, "sys");   /* the menus follow: their bank back */
    if(n == CS_NEXT_UNLOCK_NOTICES) menus_post_staff_roll();
}
/* talk overlays: zone map (menus hook) and boss talks (Boss7 rival, final boss) */
static int menu_talk_start(int c, int step) { cs_story(0);   /* zone-map talks: the zone is not read */
    cutscene_set_return_bank("sys");   /* the talk loads sys_a04: the menus' bank afterwards */
    return cutscene_start(CS_TALK_MAP, cutscene_zonemap_talk_id(c, step), c); }
static int menu_talk_active(void) { return cutscene_active(); }
static void boss_talk_hook(void *ctx, s32 id)
{
    (void)ctx;
    cs_story(g_dcp_world.zone);
    cutscene_set_return_bank(NULL);
    if(cutscene_start(CS_TALK, id, g_chara) != 0) printf("talk %ld: missing data\n", (long)id);
    else printf("boss talk %#lx start: stage frame %lu, player %ld,%ld, boss phase %ld\n", (long)id, (unsigned long)g_dcp_world.frame,
                (long)(g_player.x >> 8), (long)(g_player.y >> 8), (long)boss_phase());
}
static void overlay_cutscene_frame(const InputState *in)
{
    if(cutscene_active() && !cutscene_is_fullscreen() && cutscene_run_frame(in)) {
        if(!g_frontend)   /* System_Freeze: nothing in the stage moved while the talk ran */
            printf("boss talk end: stage frame %lu, player %ld,%ld, boss phase %ld\n", (unsigned long)g_dcp_world.frame,
                   (long)(g_player.x >> 8), (long)(g_player.y >> 8), (long)boss_phase());
        cs_finish();
    }
}

/* ---- Extra Zone (zone 8 act 2): script 3, the Extra boss, script 2, ExtraAct ---- */
static MenuRequest run_extra_zone(void)
{
    InputState in = {0};
    BossExtraResult res;
    int r = BOSS_EXTRA_FAILED;
    CsNext n = run_cutscene(CS_SCRIPT, 3, 8, NULL);   /* Game_StartDemoScript3AtZone8Act2 */
    (void)n;
    sound_load_bank("ze");
    bgm_stage_end();
    bgm_set_context("ze");   /* ExtraBoss_ResetRound / _AdvancePhase: Sound_PlaySeq 0x14 / 0x15 in /snd/ze */
    boss_extra_set_paths("/cd/models", "/cd/boss/extra");
    boss_extra_set_lives(gSaveData.unkE ? gSaveData.unkE : 3, gSaveData.unk66 ? gSaveData.unk66 : 3);
    boss_extra_set_difficulty(0);   /* gGameState.unk14: 0 normal */
    memset(&res, 0, sizeof(res));
    loading_begin(); audio_load_begin();
    r = boss_extra_init(g_chara);
    loading_end(); audio_load_end();
    if(r == 0) {
        r = BOSS_EXTRA_FAILED;
        frameprof_scene(g_chara ? "extra boss blaze" : "extra boss sonic", 1);
        int ef = 0;
        int fight = 0, pause_open = 0, quit = 0;
        for(;;) {
            input_poll(&in);
            sweep_input(&in);
            if(g_sweep && ++ef > 2 * g_sweep) { r = BOSS_EXTRA_FAILED; break; }
            if(pause_open) {
                /* PauseMenu_Create(0) of ExtraBoss_State_Battle: CONTINUE resumes; GO BACK (shown once the
                 * extra story was cleared) -> PauseMenu_Main_Quit: ExtraBoss_Shutdown, player select */
                int pr = ui_extra_pause_frame(&in);
                if(pr == 1) { boss_extra_resume(); pause_open = 0; }
                else if(pr == 2) quit = 1;
                in.pressed = 0;
            }
            if(quit) { r = BOSS_EXTRA_FAILED; break; }
            r = boss_extra_run_frame(&in);
            if(!pause_open && boss_extra_paused()) { ui_extra_pause_open(); pause_open = 1; }
            if(g_dbg_autoclear > 0 && r == BOSS_EXTRA_RUNNING && ++fight >= g_dbg_autoclear) {
                printf("debug: extra boss cleared at fight frame %d\n", fight);
                r = BOSS_EXTRA_CLEARED;
            }
            sound_update();
            FRAME_BEGIN();
            render_begin_list(PVR_LIST_OP_POLY); boss_extra_draw(PVR_LIST_OP_POLY); render_end_list();
            render_begin_list(PVR_LIST_PT_POLY); boss_extra_draw(PVR_LIST_PT_POLY); render_end_list();
            render_begin_list(PVR_LIST_TR_POLY); boss_extra_draw(PVR_LIST_TR_POLY); if(pause_open) ui_draw_tr(); render_end_list();
            render_end_frame();
            audio_update();
            if(r != BOSS_EXTRA_RUNNING) break;
        }
        boss_extra_result(&res);
        if(r == BOSS_EXTRA_CLEARED && !res.cleared) {   /* debug autoclear */
            res.cleared = 1; res.time = boss_extra_time(); res.rings = boss_extra_rings();
            if(res.lives_current < 2) res.lives_current = 2;
        }
        boss_extra_shutdown();
        SaveData_SetLivesBoth(res.lives_sonic, res.lives_blaze);   /* saved after a game over too, as on the DS */
        if(quit) {
            printf("extra boss: pause GO BACK\n");
            return menus_stage_exit(MENU_EXIT_EXTRA_QUIT, 0, res.lives_current, g_emeralds, 0);
        }
    } else printf("extra boss: init failed\n");
    printf("extra boss result %d, time %ld, rings %ld\n", r, (long)res.time, (long)res.rings);
    if(r == BOSS_EXTRA_CLEARED) {
        run_cutscene(CS_SCRIPT, 2, 8, NULL);   /* DemoScript_StartAfterBoss -> SceneChange_ToExtraAct */
        if(extraact_start((uint32_t)res.time, (uint32_t)res.rings, g_chara) == 0) {   /* the Extra Zone results */
            frameprof_scene("extraact", 0);
            for(;;) {
                int done;
                input_poll(&in);
                sweep_input(&in);
                done = extraact_run_frame(&in);
                sound_update();
                FRAME_BEGIN(); extraact_draw(); render_end_frame();
                audio_update();
                if(done) break;
            }
        }
        extraact_shutdown();
        return menus_stage_exit(MENU_EXIT_CLEARED, (uint32_t)res.rings, res.lives_current, g_emeralds, (uint32_t)res.time);
    }
    return menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, g_emeralds, 0);
}

/* SceneChange_ToSpecialStage ... SceneChange_ToStageFromSpecialStage */
static void run_special_stage(Level *lv, int stage)
{
    InputState in = {0};
    SpecialResult res;
    int r;
    g_special_pending = 0;
    audio_load_begin();
    g_ss.rings = g_player.rings; g_ss.rings_total = g_player.rings_total; g_ss.tension = g_player.tension;
    g_ss.shield = g_player.pf & 0x30000000; g_ss.water = g_player.pf2 & 0x8000;
    g_ss.x = g_player.x >> 8; g_ss.y = (g_player.y >> 8) + 0x118;
    g_ss.frame = g_dcp_world.frame; g_ss.score = g_ui_game.total;
    if(objects_save_event_state) objects_save_event_state();
    bgm_stage_end();   /* Stage_DestroyTasks (StageJingle) */
    if(g_obj_ok) { objects_shutdown(); objects_models_free(); }
    g_obj_ok = 0;
    audio_music_stop();
    ui_stage_end();
    zonefx_shutdown(); camera_destroy();
    level_free(lv);
    special_set_path("/cd/special");
    special_set_language(1);
    special_set_emerald_flags(g_emeralds);
    printf("special: vram free before %lu\n", (unsigned long)render_vram_free());
    loading_begin();
    r = special_init(g_dcp_world.zone, 0);
    loading_end();
    audio_load_end();
    printf("special stage %d: init %d\n", g_dcp_world.zone, r);
    if(r == 0) {
        char sn[24];
        snprintf(sn, sizeof(sn), "special %d %s", g_dcp_world.zone, g_chara ? "blaze" : "sonic");
        frameprof_scene(sn, 1);
        for(;;) {
            input_poll(&in);
            sweep_input(&in);
            r = special_run_frame(&in);
            FRAME_BEGIN(); special_draw(); render_end_frame();
            sound_update(); audio_update();
            if(r) break;
        }
        audio_load_begin();   /* the act reload: fade the special-stage music out */
        special_result(&res);
        g_emeralds = res.emerald_flags;   /* SpecialStage_Shutdown: unk28 |= 1 << stage when cleared */
        printf("special stage result %d, emeralds %02x\n", r, g_emeralds);
        special_shutdown();
        printf("special: vram free after %lu\n", (unsigned long)render_vram_free());
    }
    /* StageEntry_StartFromSpecialStage: the act again, title card, saved state */
    g_ss.active = 1;
    if(load_stage(lv, stage) == 0) spawn_player(lv);
    if(r != 0) audio_load_end();   /* the init failed: no frame-loop bracket was opened */
}

/* MENU_REQ_STAGE -> index into k_stages (story table), -1 if the stage is not in the port */
static int stage_index(int zone, int act)
{
    char name[8];
    unsigned i;
    if(zone == 7 && act == 2) snprintf(name, sizeof(name), "zf3");
    else snprintf(name, sizeof(name), "z%d%d", zone + 1, act + 1);
    for(i = 0; i < NUM_STAGES; i++) if(!strcmp(k_stages[i], name)) return i;
    return -1;
}

/* ===================================================================== Battle Play
 * Two players on one Dreamcast (controller ports A / B), split screen: player 1 above, player 2
 * below, one 640x224 viewport each (the world at 7/6: 548x192 world px, the DS screen's height),
 * a 32-px bar between (PVR user clip is tile-granular). Rules: vs/vs.c. Per player: camera,
 * zone-BG lines, HUD and checkpoint contexts; shared: stage, objects, rings, music. */
#define VS_VIEW_H 224.0f
#define VS_SCALE (224.0f / 192.0f)
static DcPlayer g_player2;
static DcpModel g_dcp_model2;
static int g_model2_ok, g_model2_chara = -1;
static void *g_vs_cam[2], *g_vs_zfx[2], *g_vs_hud[2];

static void vs_ctx_alloc(void)
{
    int i;
    for(i = 0; i < 2; i++) {
        if(!g_vs_cam[i]) g_vs_cam[i] = malloc(camera_ctx_size());
        if(!g_vs_zfx[i]) g_vs_zfx[i] = malloc(zonefx_view_size());
        if(!g_vs_hud[i]) g_vs_hud[i] = malloc(hud_ctx_size());
    }
}
static int g_vs_loaded;
/* vs.c: an object's ScreenShake_Start on player i's camera (the loaded context is kept) */
static void vs_cam_shake(int i, s32 s)
{
    int l = g_vs_loaded;
    if(i == l) { camera_shake(s); return; }
    camera_ctx_save(g_vs_cam[l]);
    camera_ctx_load(g_vs_cam[i]);
    camera_shake(s);
    camera_ctx_save(g_vs_cam[i]);
    camera_ctx_load(g_vs_cam[l]);
}
static void vs_load(int i) { g_vs_loaded = i; camera_ctx_load(g_vs_cam[i]); zonefx_view_load(g_vs_zfx[i]); hud_ctx_load(g_vs_hud[i]); zonefx_set_view_only(i == 1); }
static void vs_save(int i) { camera_ctx_save(g_vs_cam[i]); zonefx_view_save(g_vs_zfx[i]); hud_ctx_save(g_vs_hud[i]); }

/* the viewport of player i, and the camera window centred on the wider world window */
static float s_dbg_cam[2][2];
static void vs_view(int i)
{
    float ww;
    render_set_view(0.0f, i ? 256.0f : 0.0f, 640.0f, VS_VIEW_H, VS_SCALE, 1);
    camera_apply();
    ww = VIEW_W;
    g_cam_x -= (ww - 256.0f) * 0.5f;
    if(g_cam_x < 0) g_cam_x = 0;
    s_dbg_cam[i][0] = g_cam_x; s_dbg_cam[i][1] = g_cam_y;
}

/* StageObj_Draw3DEx for a player in the current viewport */
static void vs_draw_player(pvr_list_t list, DcPlayer *p, DcpModel *m, int ok)
{
    dcm_camera_t cam;
    dcm_mtx_t mdl;
    dcm_lights_t lights;
    s32 ax, ay, az;
    float tx, ty;
    if(!ok || (p->disp_flags & 0x20)) return;
    if(dcp_model_afterimage(p, &ax, &ay, &az)) {
        tx = ax / 256.0f - g_cam_x; ty = -(ay / 256.0f - g_cam_y);
        dcm_camera_stage(&cam); dcm_camera_stage_offaxis(&cam, &tx, &ty);
        dcp_model_matrix(p, tx, ty, 0, &mdl); dcp_model_lights(p, 1, &lights);
        dcp_model_draw(m, p, list, &mdl, &cam, &lights);
    }
    tx = p->x / 256.0f - g_cam_x; ty = -(p->y / 256.0f - g_cam_y);
    dcm_camera_stage(&cam); dcm_camera_stage_offaxis(&cam, &tx, &ty);
    dcp_model_matrix(p, tx, ty, p->z / 256.0f, &mdl); dcp_model_lights(p, 0, &lights);
    dcp_model_draw(m, p, list, &mdl, &cam, &lights);
}

/* test builds: /cd/debug/vs_p2.txt "1" = player 2's pad is scripted (run right, jump every 50 frames,
 * boost every 4 s), for emulator runs with one keyboard; "2" = the same for player 1 too */
static int g_vs_dbg_x, g_vs_dbg_y, g_vs_dbg_clock;   /* test builds (vs.txt): start point, race clock */
static void vs_script_pad(InputState *in, int frame, int who)
{
    static uint32_t prev[2];
    uint32_t real = in->held & (IN_START | IN_TRICK);   /* keep the real pad's pause / quit buttons */
    uint32_t h = IN_RIGHT | real;
    if(frame % 50 < 6) h |= IN_JUMP;
    if((frame + who * 37) % 240 < 20) h |= IN_BOOST;
    in->pressed = h & ~prev[who]; in->released = prev[who] & ~h; in->held = h; prev[who] = h;
}
/* Vs CPU: player 2's racing lines (/cd/vs/zNA_C.rte, made from the owner's stages at disc build time) */
static void *g_cpu_route;
static uint32_t g_cpu_route_size;
static u32 g_vs_races;   /* races run (the CPU's seed: reproducible, varied from race to race) */
/* test builds: /cd/debug/vsseq.txt, one race a line "zone act chara1 chara2 p1cpu" (0-based): the races
 * run one after another from the boot, each to its end (the results scene is skipped, its numbers
 * logged); p1cpu 1: player 1 is a CPU too (its own character's lines), a rival for the CPU under test */
static int g_vs_seq, g_vs_p1cpu;
static unsigned g_vs_tr0 = 1, g_vs_tr1;   /* vsseq fields 6, 7: player 2 traced every frame of this clock range */
static void *g_cpu_route1;
static uint32_t g_cpu_route1_size;
/* ---------------------------------------------------------------- single-player gameplay view
 * Ordinary Story Mode levels (zones 1..7 acts 1 and 2, not a boss arena, the attract demo, time attack,
 * Battle Play, a special stage or an in-level cutscene) show SP_VIEW_K times the DS window per axis:
 * 320 x 240 world px at 2.0x instead of 256 x 192 at 2.5x. The camera itself is unchanged (tracking,
 * lead, smoothing, bounds): the wider window is centred on its DS window and kept inside the camera's
 * bounds; where the bounds leave less room (a locked area, a narrow room) the widening shrinks so no
 * more than the bounds is shown. Set SP_VIEW_K to 1.0f for the original view. */
#define SP_VIEW_K 1.25f
static float g_sp_k = 1.0f;          /* current widening (eased by the bounds, cut by cutscenes) */
static int sp_view_eligible(int ok, int stage)
{
    return ok && g_player_ok && !g_is_boss && !g_demo && g_dcp_world.mode == UI_MODE_STORY && !cutscene_active() &&
           stage >= 0 && stage < 21 && k_stages[stage][2] != '3';
}
/* after camera_apply: the window to draw (g_cam_x / g_cam_y, render view, UI scale) */
static void sp_view_begin(int eligible, int new_stage)
{
    s32 l, t, r, b, i;
    float k, kmax, w, h, x, y, dsx = g_cam_x, dsy = g_cam_y;
    if(!eligible || SP_VIEW_K <= 1.0f) { g_sp_k = 1.0f; return; }
    /* the camera's limits (its window stays inside them): area bounds and the map */
    l = g_obj_camera[0].bound[0] >> 8; t = g_obj_camera[0].bound[1] >> 8;
    r = g_obj_camera[0].bound[2] >> 8; b = g_obj_camera[0].bound[3] >> 8;
    i = g_obj_camera[2].bound[0] >> 8; if(i > l) l = i;
    i = g_obj_camera[2].bound[1] >> 8; if(i > t) t = i;
    i = g_obj_camera[2].bound[2] >> 8; if(r <= l || (i > 0 && i < r)) r = i;
    i = g_obj_camera[2].bound[3] >> 8; if(b <= t || (i > 0 && i < b)) b = i;
    /* the DS window may sit outside them (a shake, a look pan): those edges do not limit it */
    if(dsx < l) l = (s32)dsx;
    if(dsy < t) t = (s32)dsy;
    if(dsx + 256 > r) r = (s32)dsx + 256;
    if(dsy + 192 > b) b = (s32)dsy + 192;
    kmax = (float)(r - l) / 256.0f;
    if((float)(b - t) / 192.0f < kmax) kmax = (float)(b - t) / 192.0f;
    k = SP_VIEW_K < kmax ? SP_VIEW_K : kmax;
    if(k < 1.0f) k = 1.0f;
    if(new_stage) g_sp_k = k;
    else if(g_sp_k < k) g_sp_k = g_sp_k + 0.01f > k ? k : g_sp_k + 0.01f;   /* eased: no jump at an area edge */
    else if(g_sp_k > k) g_sp_k = g_sp_k - 0.01f < k ? k : g_sp_k - 0.01f;
    if(g_sp_k > kmax) g_sp_k = kmax < 1.0f ? 1.0f : kmax;              /* never past the bounds */
    w = 256.0f * g_sp_k; h = 192.0f * g_sp_k;
    x = dsx - (w - 256.0f) * 0.5f; y = dsy - (h - 192.0f) * 0.5f;
    if(x + w > r) x = r - w;
    if(x < l) x = l;
    if(y + h > b) y = b - h;
    if(y < t) y = t;
    g_cam_x = x; g_cam_y = y;
    g_view_row_ofs = (int)(dsy - y + 0.5f);
    g_bg_zoom = g_sp_k > 1.001f ? g_sp_k : 0.0f;   /* the zone BG: the DS screen's, scaled to the window (level.c) */
    render_set_view(0.0f, 0.0f, 640.0f, 480.0f, 2.5f / g_sp_k, 0);
    ui_set_world_scale(2.5f / g_sp_k);
}
/* after the frame: everything else draws with the original view */
static void sp_view_end(void)
{
    render_reset_view();
    g_view_row_ofs = 0;
    g_bg_zoom = 0.0f;
    ui_set_world_scale(2.5f);
}

static int vs_cpu_skill(void)
{
    uint32_t sz;
    char *t = load_file("/cd/debug/cpu_skill.txt", &sz);   /* test builds: 0 easy, 1 normal, 2 hard */
    int k = t ? atoi(t) : VSC_NORMAL;
    free(t);
    return k < VSC_EASY || k > VSC_HARD ? VSC_NORMAL : k;
}
/* the "CPU" tag of player 2 in the bar between the viewports: the name plates' font (font8.bin) */
static Texture g_cpu_tag;
static int g_cpu_tag_w;
static void vs_cpu_tag_load(void)
{
    unsigned char mask[32 * 8];
    uint16_t px[32 * 8];
    int i;
    if(g_cpu_tag.ptr) return;
    g_cpu_tag_w = menus_text_mask("CPU", mask, 32, 8);
    if(g_cpu_tag_w <= 0) return;
    for(i = 0; i < 32 * 8; i++) px[i] = mask[i] ? 0xffff : 0x0000;   /* ARGB4444: white ink */
    g_cpu_tag.ptr = pvr_mem_malloc(sizeof px);
    if(!g_cpu_tag.ptr) return;
    pvr_txr_load(px, g_cpu_tag.ptr, sizeof px);
    g_cpu_tag.w = 32; g_cpu_tag.h = 8; g_cpu_tag.fmt = PVR_TXRFMT_ARGB4444; g_cpu_tag.bytes = sizeof px; g_cpu_tag.twiddled = 0;
}
static void vs_cpu_tag_free(void)
{
    if(g_cpu_tag.ptr) pvr_mem_free(g_cpu_tag.ptr);
    memset(&g_cpu_tag, 0, sizeof g_cpu_tag);
}
/* player 2's own water level (objects/obj_player.c obj_water_of) into its camera: the viewport's water
 * line and zone effects (the objects run with player 1's camera) */
static void vs_sync_water(void)
{
    u16 wl;
    u32 wf;
    int k;
    if(!g_obj_ok || !objects_player_water(1, &wl, &wf)) return;
    vs_load(1);
    for(k = 0; k < 2; k++) {
        g_obj_camera[k].water = wl;
        g_obj_camera[k].flags = (g_obj_camera[k].flags & ~0x3000000u) | wf;
    }
    vs_save(1);
    vs_load(0);
}
static MenuRequest run_vs_stage(MenuRequest r)
{
    for(;;) {
        Level lv;
        InputState in0, in1;
        int idx = stage_index(r.zone, r.act), frame = 0, go = 0, paused = 0, i;
        u32 clock0 = 0;
        VsRaceResult res;
        MenuRequest next;
        memset(&lv, 0, sizeof lv);
        memset(&in0, 0, sizeof in0); memset(&in1, 0, sizeof in1);
        if(r.character2 == r.character) r.character2 = !r.character;   /* PlayerSelectMenu: never the same character */
        if(idx < 0 || r.act >= 2) { printf("vs: stage z%d%d not available\n", r.zone + 1, r.act + 1); memset(&next, 0, sizeof next); return next; }
        printf("vs: race z%d%d, P1 %s, P2 %s%s\n", r.zone + 1, r.act + 1, r.character ? "Blaze" : "Sonic", r.character2 ? "Blaze" : "Sonic",
               r.cpu ? " (CPU)" : "");
        vs_ctx_alloc();
        g_chara = r.character;
        load_player_model();
        if(!g_model2_ok || g_model2_chara != r.character2) {
            g_model2_chara = r.character2;
            g_model2_ok = dcp_model_load(&g_dcp_model2, r.character2, "/cd/player", 1) == 0;
        }
        g_dcp_world.mode = 1;                 /* gGameState.unk10: VS */
        /* gGameState.unk18 & 0x20: RingManager 2 players, VS objects. Assigned with the clock and the object
         * manager flags (and player 2 cleared), so nothing of the stage before (a story death's restart bit 2:
         * PlayerStart keeps the old respawn point; a boss's pause bit; the clock the objects created in
         * objects_init see) reaches the race: it starts as the host simulation's do (vs_sim_init) */
        g_dcp_world.flags = 0x20;
        g_dcp_world.frame = 0;
        g_dcp_world.objmgr = 0x18;
        memset(&g_player2, 0, sizeof g_player2);
        g_dcp_world.lives = 0;
        g_emeralds = 0; g_difficulty = r.easy;
        g_frontend = 0;
        if(load_stage(&lv, idx) != 0 || !g_player_ok) { level_free(&lv); memset(&next, 0, sizeof next); return next; }
        spawn_player(&lv);                     /* player 1, objects, sound, ui resources, camera 1 */
        g_dcp_world.flags |= 0x20;             /* (ui_stage_start clears gGameState.unk18) */
        g_dcp_world.mode = 1;
        /* player 2 at the same start */
        memset(&g_player2, 0, sizeof g_player2);
        dcp_player_create(&g_player2, (u8)r.character2, 0, g_player.x >> 8, g_player.y >> 8);
        g_player.rings = g_player2.rings = 1;  /* Player_Respawn / Player_Create, VS: unk3AE = 1 */
        if(g_vs_dbg_x || g_vs_dbg_y) {
            g_player.x = g_player2.x = g_vs_dbg_x << 8; g_player.y = g_player2.y = g_vs_dbg_y << 8;
            g_player2.x += 0x2000;
            g_dcp_world.check_x = g_vs_dbg_x; g_dcp_world.check_y = g_vs_dbg_y;
            camera_snap_to_player(&g_player);
            printf("debug: vs start %d,%d\n", g_vs_dbg_x, g_vs_dbg_y);
        }
        if(g_vs_dbg_clock) g_dcp_world.frame = (u32)g_vs_dbg_clock;
        if(g_model2_ok) dcp_model_attach_second(&g_dcp_model2, &g_player2);
        /* player 2's character sounds: its own zone SDAT resident beside player 1's */
        if(r.character2 != r.character) {
            sound_load_bank2(sound_stage_sdat(g_dcp_world.zone, g_dcp_world.act, r.character2));
            sound_hooks_set_bank2_player(&g_player2);
        }
        if(g_obj_ok) { objects_add_player(&g_player2); objects_set_view_count(8); objects_respawn_view(); }
        /* the cameras: player 1's from spawn_player; player 2's built the same way */
        hud_ctx_save(g_vs_hud[0]); hud_ctx_save(g_vs_hud[1]);
        camera_set_rival(&g_player2);
        camera_ctx_save(g_vs_cam[0]); zonefx_view_save(g_vs_zfx[0]);
        camera_create(lv.data, &g_player2, g_dcp_world.zone, g_dcp_world.act);
        camera_set_rival(&g_player);
        camera_ctx_save(g_vs_cam[1]); zonefx_view_save(g_vs_zfx[1]);
        ui_vs_hud_reset();
        vs_begin(&g_player, &g_player2);
        vs_set_camera_shake_fn(vs_cam_shake);
        if(r.cpu) {   /* Vs CPU: player 2's racing lines for this stage and character */
            char path[64];
            u32 seed = 0x5eed1u + g_vs_races * 7919u + (u32)(r.zone * 16 + r.act) * 131u;
            int skill = vs_cpu_skill();
            free(g_cpu_route); g_cpu_route = NULL; g_cpu_route_size = 0;
            snprintf(path, sizeof path, "/cd/vs/z%d%d_%d.rte", r.zone + 1, r.act + 1, r.character2);
            g_cpu_route = load_file(path, &g_cpu_route_size);
            if(vs_cpu_begin(1, g_cpu_route, g_cpu_route_size, seed, skill) != 0)
                printf("vs: CPU has no racing line (%s), it drives by sight\n", path);
            else
                printf("vs: CPU line %s (%lu bytes), seed %lu, skill %d\n", path, (unsigned long)g_cpu_route_size, (unsigned long)seed, skill);
            vs_cpu_tag_load();
            if(g_vs_p1cpu) {   /* test builds: player 1 a CPU too */
                free(g_cpu_route1); g_cpu_route1 = NULL; g_cpu_route1_size = 0;
                snprintf(path, sizeof path, "/cd/vs/z%d%d_%d.rte", r.zone + 1, r.act + 1, r.character);
                g_cpu_route1 = load_file(path, &g_cpu_route1_size);
                printf("vs: player 1 CPU %s\n", vs_cpu_begin(0, g_cpu_route1, g_cpu_route1_size, seed * 31u + 7u, skill) ? "without a line" : path);
            }
        }
        g_vs_races++;
        vs_load(0);
        while(vs_info()->exit == VS_EXIT_NONE) {
            uint16_t h[2], pr[2], rl[2];
            int run;
            input_poll_port(0, &in0);
            input_poll_port(1, &in1);
            if(g_vs_script < 0) {
                uint32_t sz;
                char *t = load_file("/cd/debug/vs_p2.txt", &sz);
                g_vs_script = t ? atoi(t) : 0;
                free(t);
                printf("vs: controllers A %d B %d, scripted pads %d\n", input_port_present(0), input_port_present(1), g_vs_script);
            }
            if(r.cpu) memset(&in1, 0, sizeof in1);   /* Vs CPU: no controller for player 2 */
            else if(g_vs_script >= 1) vs_script_pad(&in1, frame, 1);
            if(g_vs_script >= 2) vs_script_pad(&in0, frame, 0);
            if(go && ((in0.pressed | in1.pressed) & IN_START)) paused = !paused;
            if(paused && ((in0.pressed | in1.pressed) & IN_TRICK)) vs_request_exit(VS_EXIT_QUIT);
            if(!go && ui_titlecard_started()) {   /* StageTitleCard "GO!": the race clock runs */
                go = 1;
                g_dcp_world.flags |= 1;
                /* the title card held player 1 (NOINPUT | NOTENSION); player 2 the same */
                g_player.pf &= ~0x600000u;
                g_player2.pf &= ~0x600000u;
            }
            run = !paused && go;
            h[0] = ds_pad(in0.held); pr[0] = ds_pad(in0.pressed); rl[0] = ds_pad(in0.released);
            h[1] = ds_pad(in1.held); pr[1] = ds_pad(in1.pressed); rl[1] = ds_pad(in1.released);
            if(!run) { h[0] = h[1] = pr[0] = pr[1] = rl[0] = rl[1] = 0; }
            else if(r.cpu) {
                vs_cpu_pad(1, &g_player2, &g_player, &h[1], &pr[1], &rl[1]);   /* the CPU's pad */
                if(g_vs_p1cpu) vs_cpu_pad(0, &g_player, &g_player2, &h[0], &pr[0], &rl[0]);
            }
            uint64_t tp0 = timer_us_gettime64(), tp1, tp2, tp3;
            static uint64_t vs_prof[4];
            if(!paused) {
                /* Camera_Main for each camera */
                for(i = 0; i < 2; i++) {
                    s32 vx, vy, px2, py2;
                    vs_load(i);
                    camera_update();
                    camera_get_view(&vx, &vy);
                    camera_get_partner_view(&px2, &py2);
                    if(g_obj_ok) {   /* the DS pair and the DC window (two halves: 548 world px) */
                        int b = i * 4;
                        if(i == 0) objects_set_view(0, camera_layer(0)->x, camera_layer(0)->y);
                        else objects_set_view(b, camera_layer(0)->x, camera_layer(0)->y);
                        objects_set_view(b + 1, px2, py2);
                        objects_set_view(b + 2, vx - 0x9200, vy);
                        objects_set_view(b + 3, vx + 0x9200, vy);
                    }
                    vs_save(i);
                }
                vs_load(0);
                if(g_obj_ok) objects_frame_begin();
                clock0 = g_dcp_world.frame;   /* dcp_player_frame ticks it: Stage_Main does, once */
                for(i = 0; i < 2; i++) {
                    DcPlayer *p = i ? &g_player2 : &g_player;
                    vs_load(i);
                    vs_player_begin(i);
                    vs_filter_input(i, &h[i], &pr[i], &rl[i]);
                    if(run) dcp_player_frame(p, h[i], pr[i], rl[i]);
                    camera_player_last(p);
                    vs_player_end(i);
                    vs_save(i);
                    g_dcp_world.frame = clock0;
                }
                if(g_model_ok) dcp_model_step(&g_dcp_model, &g_player, !run);
                if(g_model2_ok) dcp_model_step(&g_dcp_model2, &g_player2, !run);
                vs_load(0);
                if(g_obj_ok && run) objects_update(&g_player);
                if(run) vs_after_players();
                vs_save(0);   /* objects_update ran with player 1's camera loaded (shakes, gCamera) */
                vs_sync_water();
                if(g_dcp_world.flags & 1) g_dcp_world.frame++;   /* Stage_Main */
                for(i = 0; i < 2; i++) { vs_load(i); camera_frame_end(); vs_save(i); }
                vs_frame_end();
                if(run && clock0 >= g_vs_tr0 && clock0 <= g_vs_tr1)   /* test builds: test_cpu -dump's format */
                    printf("  dump %lu pad %04x -> %ld,%ld vx %d anim %x df %x pf %lx rings %d\n", (unsigned long)clock0, h[1],
                           (long)(g_player2.x >> 8), (long)(g_player2.y >> 8), g_player2.gnd_vel, g_player2.anim,
                           g_player2.disp_flags, (unsigned long)g_player2.pf, g_player2.rings);
            }
            tp1 = timer_us_gettime64();
            sound_update();
            if(g_obj_ok) objects_models_prepare();
            pvr_wait_ready();
            tp2 = timer_us_gettime64();
            zonefx_commit_palettes();
            render_begin_frame();
            /* OP, PT, TR: player 1's viewport, then player 2's */
            render_begin_list(PVR_LIST_OP_POLY);
            for(i = 0; i < 2; i++) {
                vs_load(i); vs_view(i); render_emit_clip();
                zonefx_draw_bg(0.0005f);
                vs_draw_player(PVR_LIST_OP_POLY, &g_player, &g_dcp_model, g_model_ok);
                vs_draw_player(PVR_LIST_OP_POLY, &g_player2, &g_dcp_model2, g_model2_ok);
                if(g_obj_ok) objects_draw_models(PVR_LIST_OP_POLY);
            }
            render_reset_view();
            render_end_list();
            render_begin_list(PVR_LIST_PT_POLY);
            for(i = 0; i < 2; i++) {
                vs_load(i); vs_view(i); render_emit_clip();
                zonefx_draw_layer(1, PVR_LIST_PT_POLY, 0.001f); zonefx_draw_layer(0, PVR_LIST_PT_POLY, 10.0f);
                vs_draw_player(PVR_LIST_PT_POLY, &g_player, &g_dcp_model, g_model_ok);
                vs_draw_player(PVR_LIST_PT_POLY, &g_player2, &g_dcp_model2, g_model2_ok);
                if(g_obj_ok) { objects_draw_models(PVR_LIST_PT_POLY); obj_draw(PVR_LIST_PT_POLY); }
            }
            render_reset_view();
            render_end_list();
            render_begin_list(PVR_LIST_TR_POLY);
            for(i = 0; i < 2; i++) {
                DcPlayer *p = i ? &g_player2 : &g_player;
                int fl;
                vs_load(i); vs_view(i); render_emit_clip();
                vs_draw_player(PVR_LIST_TR_POLY, &g_player, &g_dcp_model, g_model_ok);
                vs_draw_player(PVR_LIST_TR_POLY, &g_player2, &g_dcp_model2, g_model2_ok);
                if(g_obj_ok) { objects_draw_models(PVR_LIST_TR_POLY); obj_draw(PVR_LIST_TR_POLY); }
                zonefx_draw_tr();
                /* the player's HUD in its viewport (the DS HUD layout at 224 / 480) */
                if(go) {   /* hidden under the title card; FixVsHud_Draw: ahead (x) = 1 */
                    const DcPlayer *o = i ? &g_player : &g_player2;
                    ui_vs_hud_draw(p, g_dcp_world.frame, i ? 256 : 0, (int)VS_VIEW_H, p->x > o->x ? 1 : 2);
                }
                fl = vs_fade_level(i);
                if(fl > 0)
                    draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, i ? 256.0f : 0.0f, 640, VS_VIEW_H, 0, 0, 0, 0, 98.0f,
                                     ((uint32_t)(fl >= 16 ? 255 : fl * 16) << 24));
            }
            render_reset_view();
            /* the bar between the viewports: player colours (1P left, 2P right), the race time */
            draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 224, 640, 32, 0, 0, 0, 0, 97.0f, 0xff101828);
            draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 224, 8, 32, 0, 0, 0, 0, 97.5f, g_player.chara ? 0xffb050e0 : 0xff2060ff);
            draw_rect_screen(NULL, PVR_LIST_TR_POLY, 632, 224, 8, 32, 0, 0, 0, 0, 97.5f, g_player2.chara ? 0xffb050e0 : 0xff2060ff);
            if(r.cpu && g_cpu_tag.ptr)   /* player 2 is the CPU: "CPU" at 2x by its colour */
                draw_rect_screen(&g_cpu_tag, PVR_LIST_TR_POLY, 624 - g_cpu_tag_w * 2, 232, (float)(g_cpu_tag_w * 2), 16, 0, 0,
                                 (float)g_cpu_tag_w, 8, 97.6f, 0xffffffff);
            ui_vs_titlecard_draw();   /* StageTitleCard: the VS card, "Ready? / GO!" */
            if(paused) draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 96.0f, 0x80000000);
            render_end_list();
            render_end_frame();
            tp3 = timer_us_gettime64();
            vs_prof[0] += tp1 - tp0; vs_prof[1] += tp2 - tp1; vs_prof[2] += tp3 - tp2;
            audio_update();
            if(++frame % 120 == 0 && g_vs_script > 0) {   /* test builds (vs_p2.txt): frame log */
                static uint64_t t_prev;
                uint64_t now = timer_ms_gettime64();
                if(t_prev) printf("vs fps %.1f: update %.2f wait %.2f draw %.2f ms/frame\n", 120000.0 / (double)(now - t_prev),
                                  vs_prof[0] / 120000.0, vs_prof[1] / 120000.0, vs_prof[2] / 120000.0);
                memset(vs_prof, 0, sizeof vs_prof);
                t_prev = now;
            }
            if(frame % 120 == 0 && g_vs_script > 0 && r.cpu) {
                const VsCpuStats *cs = vs_cpu_stats(1);
                printf("vs cpu: line point %d/%d, on %d off %d, recover %d, relocate %d, jumps %d boosts %d attacks %d\n", cs->idx,
                       cs->line_len, cs->on_line, cs->off_line, cs->recover, cs->relocate, cs->jumps, cs->boosts, cs->rival_attacks);
            }
            if(frame % 120 == 0 && g_vs_script > 0)
                printf("vs frame %d: p1 %ld,%ld rings %d t %d | p2 %ld,%ld rings %d t %d | clock %lu\n", frame,
                       (long)(g_player.x >> 8), (long)(g_player.y >> 8), g_player.rings, g_player.tension,
                       (long)(g_player2.x >> 8), (long)(g_player2.y >> 8), g_player2.rings, g_player2.tension,
                       (unsigned long)g_dcp_world.frame);
            if(frame % 120 == 0 && g_vs_script > 0)
                printf("vs dbg: p1 pf %08lx pf2 %08lx mf %08lx st %p held %04x | p2 pf %08lx mf %08lx | cam0 %.0f,%.0f cam1 %.0f,%.0f\n",
                       (unsigned long)g_player.pf, (unsigned long)g_player.pf2, (unsigned long)g_player.move_flags, (void *)g_player.state,
                       (unsigned)g_player.held, (unsigned long)g_player2.pf, (unsigned long)g_player2.move_flags, s_dbg_cam[0][0], s_dbg_cam[0][1], s_dbg_cam[1][0], s_dbg_cam[1][1]);
        }
        /* the race is over: ResultsScreen_Create */
        memset(&res, 0, sizeof res);
        res.winner = vs_info()->winner < 0 ? 2 : vs_info()->winner;
        for(i = 0; i < 2; i++) {
            const DcPlayer *p = i ? &g_player2 : &g_player;
            res.time[i] = vs_info()->finished[i] ? vs_info()->time[i] : g_dcp_world.frame;
            res.rings[i] = p->rings;
            res.finished[i] = vs_info()->finished[i];
        }
        printf("vs: race over, winner %d, times %lu / %lu\n", res.winner, (unsigned long)res.time[0], (unsigned long)res.time[1]);
        vs_end();
        if(r.cpu) {
            const VsCpuStats *cs = vs_cpu_stats(1);
            printf("vs cpu result: z%d%d chara %d %s time %lu, p1 %s, recover %d relocate %d on %d off %d jumps %d boosts %d attacks %d\n",
                   r.zone + 1, r.act + 1, r.character2, res.finished[1] ? "GOAL" : "no goal", (unsigned long)res.time[1], res.finished[0] ? "goal" : "-", cs->recover,
                   cs->relocate, cs->on_line, cs->off_line, cs->jumps, cs->boosts, cs->rival_attacks);
            vs_cpu_end(1); free(g_cpu_route); g_cpu_route = NULL; vs_cpu_tag_free();
            if(g_vs_p1cpu) { vs_cpu_end(0); free(g_cpu_route1); g_cpu_route1 = NULL; }
        }
        sound_hooks_set_bank2_player(NULL);
        sound_load_bank2(NULL);
        dcp_model_attach_second(NULL, NULL);
        if(g_obj_ok) { objects_shutdown(); objects_models_free(); }
        ui_stage_end();
        zonefx_set_view_only(0);
        zonefx_shutdown(); camera_destroy();
        level_free(&lv);
        render_reset_view();
        g_dcp_world.mode = 0;
        g_dcp_world.flags = 0;
        enter_front_end();
        if(g_vs_seq) { MenuRequest q; memset(&q, 0, sizeof q); return q; }   /* test builds: the next race of the list */
        next = menus_vs_exit(&res);
        if(next.kind != MENU_REQ_VS_STAGE) return next;
        r = next;   /* rematch */
    }
}

int main(int argc, char **argv)
{
    Level lv;
    InputState in = {0};
    int stage = 0, frame = 0, ok, dbg_start = 0;
    uint64_t t_last = 0;

    {
        /* OP, TR and PT (punch-through for tile layers / sprites) lists. */
        pvr_init_params_t params = {
            .opb_sizes = { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16 },
            .vertex_buf_size = 512 * 1024,
            .opb_overflow_count = 3
        };
        pvr_init(&params);
    }
    audio_init();
    sound_init();
    sound_hooks_install();
    if(g_dcp_hooks.restart != boss_restart_hook) { g_prev_boss_restart = g_dcp_hooks.restart; g_dcp_hooks.restart = boss_restart_hook; }
    g_dcp_hooks.gravity_flipped = gravity_flipped_hook;
    ui_set_paths("/cd/ui", "/cd/sprites");
    printf("zonefx: %s\n", zonefx_load_data("/cd/zonefx/zonefx.bin") == 0 ? "ok" : "MISSING");
    printf("ui: %s\n", ui_init() == 0 ? "ok" : "FAILED");
    g_dcp_world.mode = UI_MODE_STORY;
    save_vmu_install();
    menus_set_paths("/cd/menus", "/cd/sprites");
    g_obj_hooks.effect = obj_effect_hook;
    g_boss_hooks.player_box_flags = boss_player_box_flags;
    g_boss_hooks.navi = ui_boss_navi;
    g_boss_hooks.help = boss_help_hook;
    g_boss_hooks.cleared = boss_cleared_hook;
    g_boss_hooks.talk = boss_talk_hook;
    g_boss_hooks.water_fx = objects_fx3d_water;          /* EffWater3D_Create: 3D BAC water spray (Water Palace boss) */
    g_boss_hooks.fx3d = boss_fx3d_hook;                 /* Boss7: EffBoostA(2)_3D, EffFire01_3D, EffKemuriSpin3D (3D BAC one-shots) */
    boss7_set_talk_hook(boss_talk_hook, NULL);
    cutscene_set_paths("/cd/cutscene", "/cd/sprites", "/cd/models", "/cd/music");
    g_menu_hooks.talk_demo_start = menu_talk_start;
    g_menu_hooks.talk_demo_active = menu_talk_active;
    g_boss_hooks.music = bgm_boss_music_hook;          /* pinch 0xf, Boss7 0x11, BossF talk 0x13, Extra 0x14 / 0x15 */
    g_boss_hooks.music_stop = bgm_boss_music_stop_hook;
    g_boss_hooks.music_fade = bgm_boss_music_fade_hook;     /* Boss7 ClashFadeOutBGM, BossF SkyChargeStartFadeOut: 0x3c */
    g_boss_hooks.music_volume = bgm_boss_music_volume_hook; /* Boss7 defeat scene: 0x20 over 0xb4 */
    g_boss_hooks.sfx = sound_boss_sfx;                 /* Sound_PlaySeqArc(NULL, 0, id): boss sound effects */
    g_boss_hooks.voice = sound_boss_voice;             /* Sound_PlayVoice(NULL, 0, id): Eggman's voice lines */
    g_boss_hooks.sfx_handle = sound_boss_sfx_handle;   /* Sound_PlaySeqArc(handle, 0, id): looping boss sounds */
    g_boss_hooks.sfx_stop = sound_boss_sfx_stop;       /* NNS_SndPlayerStopSeq(handle, 0) */
    g_boss_hooks.sfx_volume = sound_boss_sfx_volume;   /* NNS_SndPlayerSetVolume(gSESoundHandle, vol) */
    g_ui_hooks.music = bgm_ui_music_hook;              /* ClearDemo / new record / game over (sys) */
    g_ui_hooks.music_pause = bgm_ui_music_pause_hook;  /* pause menu: players 3..9 */
    g_ui_hooks.sfx = bgm_ui_sfx_hook;                  /* results rank SE, pause menu, enemy room */
    special_hooks.music = bgm_special_music;           /* SEQ_SS_GAME / SEQ_SS_END */
    special_hooks.music_stop = bgm_special_music_stop;
    g_ui_hooks.voice = ui_voice_hook;
    g_ui_hooks.stage_new = ui_stage_new_hook;
    g_menu_hooks.bgm = menu_bgm; g_menu_hooks.se = menu_se; g_menu_hooks.voice = menu_voice;
    g_menu_hooks.voice_alt = menu_voice_alt; g_menu_hooks.voice_arc = menu_voice_arc; g_menu_hooks.stop_all = menu_stop_all;
    g_menu_hooks.sound_test = menu_sound_test;
    g_menu_hooks.bgm_stop = bgm_stop;
    g_menu_hooks.sound_test_stop = menu_sound_test_stop;
    g_ui_game.save_lives[0] = g_ui_game.save_lives[1] = 3;
    printf("Sonic Rush DC: %dx%d\n", vid_mode->width, vid_mode->height);
    {
        uint32_t ts, ps;
        int16_t *trig = load_file("/cd/player/trig.bin", &ts);
        uint8_t *phys = load_file("/cd/player/physics.bin", &ps);
        if(trig && phys) {
            dcp_set_trig(trig, trig + 8192);
            g_player_ok = dcp_set_physics(phys, ps) == 2;
        }
        printf("player tables: %s\n", g_player_ok ? "ok" : "MISSING");
        input_poll(&in);
        g_chara = (in.held & IN_ALT) ? 1 : 0;   /* test builds (debug start) only */
        {   /* test builds: /cd/debug/chara.txt "1" = Blaze */
            uint32_t csz;
            char *c = load_file("/cd/debug/chara.txt", &csz);
            if(c) { g_chara = atoi(c) ? 1 : 0; free(c); }
        }
        load_player_model();
    }
    frameprof_init();   /* test builds: /cd/debug/frameprof.txt */
    {
        /* test builds only: /cd/debug/start.txt holds the story index to start at */
        uint32_t dsz;
        char *d = load_file("/cd/debug/start.txt", &dsz);
        char *sp = load_file("/cd/debug/special.txt", &dsz);
        char *bh = load_file("/cd/debug/bosshp.txt", &dsz);
        char *ac = load_file("/cd/debug/autoclear.txt", &dsz);
        if(bh) { g_dbg_bosshp = atoi(bh); free(bh); printf("debug: boss hit points held at %d\n", g_dbg_bosshp); g_boss_hooks.log = dbg_boss_log; }
        char *ad = load_file("/cd/debug/autodie.txt", &dsz);
        char *sw = load_file("/cd/debug/spawn.txt", &dsz);
        if(sw) { sw = realloc(sw, dsz + 1); sw[dsz] = 0; sscanf(sw, "%d %d %d", &g_dbg_spawn_x, &g_dbg_spawn_y, &g_dbg_spawn_tension); free(sw);
                 printf("debug: spawn at %d,%d tension %d\n", g_dbg_spawn_x, g_dbg_spawn_y, g_dbg_spawn_tension); }
        if(ac) { g_dbg_autoclear = atoi(ac) > 0 ? atoi(ac) : 240; free(ac); printf("debug: autoclear at frame %d\n", g_dbg_autoclear); }
        if(ad) { ad = realloc(ad, dsz + 1); ad[dsz] = 0; if(sscanf(ad, "%d %d", &g_dbg_autodie, &g_dbg_autodie_lives) < 1) g_dbg_autodie = 300; free(ad);
                 printf("debug: autodie at frame %d, lives %d\n", g_dbg_autodie, g_dbg_autodie_lives); }
        char *spos = d ? load_file("/cd/debug/startpos.txt", &dsz) : NULL;   /* "x y" px: replaces PlayerStart */
        if(spos) { spos = realloc(spos, dsz + 1); spos[dsz] = 0; g_dbg_pos_set = sscanf(spos, "%d %d", &g_dbg_pos_x, &g_dbg_pos_y) == 2; free(spos); }
        if(sp) { g_dbg_special = atoi(sp) > 0 ? atoi(sp) : 300; free(sp); }
        {
            char *sw = d ? load_file("/cd/debug/sweep.txt", &dsz) : NULL;
            if(sw) { g_sweep = atoi(sw) > 0 ? atoi(sw) : 600; free(sw); printf("sweep: %d frames per act\n", g_sweep); }
        }
        if(d) { int v = atoi(d); if(v >= 0 && v < (int)NUM_STAGES) stage = v; free(d); dbg_start = 1; printf("debug start stage %d\n", stage); }
    }
    if(dbg_start) {
        ok = load_stage(&lv, stage) == 0;
        if(ok && g_player_ok) spawn_player(&lv);
    } else {
        ok = 0; enter_front_end();
        menus_init();   /* menudata, save_load() from the VMU, SEGA logo */
        {   /* test builds: /cd/debug/extra.txt = start with the Extra Zone (script 3, Extra boss) */
            uint32_t esz;
            char *e = load_file("/cd/debug/extra.txt", &esz);
            if(e) { free(e); printf("debug start: Extra Zone\n"); run_extra_zone(); enter_front_end(); }
        }
        {   /* test builds: /cd/debug/vsseq.txt: Vs CPU races one after another (see g_vs_seq) */
            uint32_t qsz;
            char *q = load_file("/cd/debug/vsseq.txt", &qsz), *line;
            if(q) {
                int n = 0;
                q = realloc(q, qsz + 1); q[qsz] = 0;
                g_vs_seq = 1;
                for(line = q; line && *line; line = strchr(line, '\n') ? strchr(line, '\n') + 1 : NULL) {
                    MenuRequest vr;
                    int p1c = 0;
                    char one[80];
                    g_vs_tr0 = 1; g_vs_tr1 = 0;
                    size_t ln = strcspn(line, "\n");
                    if(ln >= sizeof one) ln = sizeof one - 1;
                    memcpy(one, line, ln); one[ln] = 0;
                    memset(&vr, 0, sizeof vr);
                    if(sscanf(one, "%d %d %d %d %d %u %u", &vr.zone, &vr.act, &vr.character, &vr.character2, &p1c, &g_vs_tr0, &g_vs_tr1) < 4) continue;
                    vr.kind = MENU_REQ_VS_STAGE; vr.mode = 1; vr.easy = 1; vr.cpu = 1;
                    if(vr.character2 == vr.character) vr.character2 = !vr.character;
                    g_vs_p1cpu = p1c;
                    printf("vs seq: race %d z%d%d p1 %d%s p2 %d (CPU)\n", n++, vr.zone + 1, vr.act + 1, vr.character, p1c ? " (CPU)" : "", vr.character2);
                    menus_vs_set_cpu(1);
                    menus_vs_prepare(vr.zone, vr.act, vr.character, vr.character2);
                    run_vs_stage(vr);
                }
                printf("vs seq: done, %d races\n", n);
                free(q);
                g_vs_seq = 0; g_vs_p1cpu = 0;
            }
        }
        {   /* test builds: /cd/debug/vs.txt "zone act chara1 chara2" = start a Battle Play race */
            uint32_t vsz;
            char *v = load_file("/cd/debug/vs.txt", &vsz);
            if(v) {
                MenuRequest vr;
                memset(&vr, 0, sizeof vr);
                vr.kind = MENU_REQ_VS_STAGE; vr.mode = 1; vr.easy = 1; vr.character2 = 1;
                sscanf(v, "%d %d %d %d %d %d %d %d", &vr.zone, &vr.act, &vr.character, &vr.character2,
                       &g_vs_dbg_x, &g_vs_dbg_y, &g_vs_dbg_clock, &vr.cpu);
                free(v);
                printf("debug start: Battle Play z%d%d\n", vr.zone + 1, vr.act + 1);
                if(vr.character2 == vr.character) vr.character2 = !vr.character;
                menus_vs_set_cpu(vr.cpu);
                menus_vs_prepare(vr.zone, vr.act, vr.character, vr.character2);   /* the selects' state (results, rematch) */
                run_vs_stage(vr);
            }
        }
    }

    for(;;) {
        if(g_frontend) {
            MenuRequest r;
            input_poll(&in);
            if(in.pressed) printf("fe pad pressed %08lx\n", (unsigned long)in.pressed);
            {   /* Battle Play: player 2's controller (port B) for the menus */
                static InputState in2;
                static int fe_frame;
                input_poll_port(1, &in2);
                if(g_vs_script < 0) {
                    uint32_t sz;
                    char *t = load_file("/cd/debug/vs_p2.txt", &sz);
                    g_vs_script = t ? atoi(t) : 0;
                    free(t);
                }
                if(g_vs_script >= 1) {   /* test builds: player 2 presses RIGHT, then A, every second */
                    uint32_t hh = 0;
                    fe_frame++;
                    if(fe_frame % 60 < 4) hh = IN_RIGHT;
                    else if(fe_frame % 60 >= 40 && fe_frame % 60 < 44) hh = IN_JUMP;   /* after the 32-frame scroll */
                    in2.pressed = hh & ~in2.held; in2.released = in2.held & ~hh; in2.held = hh;
                }
                menus_set_pad2(&in2);
            }
            r = menus_run_frame(&in);
            overlay_cutscene_frame(&in);   /* zone-map TalkDemo */
            sound_update();
            FRAME_BEGIN();
            /* the menus' 3D layer (zone-map character model, mn_model.c) in all three lists */
            render_begin_list(PVR_LIST_OP_POLY); menus_draw_list(PVR_LIST_OP_POLY); render_end_list();
            render_begin_list(PVR_LIST_PT_POLY); menus_draw_list(PVR_LIST_PT_POLY); render_end_list();
            render_begin_list(PVR_LIST_TR_POLY); menus_draw_tr(); menus_draw_list(PVR_LIST_TR_POLY); if(cutscene_active()) cutscene_draw_list(PVR_LIST_TR_POLY); render_end_list();
            render_end_frame();
            if(r.kind == MENU_REQ_VS_STAGE) {   /* Battle Play race (+ results, rematches) */
                MenuRequest n = run_vs_stage(r);
                (void)n;   /* back in the menus (the results scene chose quit) */
                continue;
            }
            if(r.kind == MENU_REQ_STAGE && r.zone == 8 && r.act == 2) {
                /* extra story: Game_StartDemoScript3AtZone8Act2 */
                g_chara = r.character; g_dcp_world.mode = r.mode; g_emeralds = r.emeralds; g_difficulty = r.easy;
                r = run_extra_zone();
                enter_front_end();
                if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding"))
                    run_ending(CS_EXTRA_ENDING, 8);
                continue;
            }
            if(r.kind == MENU_REQ_STAGE) {
                int idx = stage_index(r.zone, r.act);
                if(idx < 0) { printf("menus: stage z%d%d not in the port\n", r.zone + 1, r.act + 1); continue; }
                /* Game_StartStoryStage: the boss act of a story zone starts with DemoScript_StartScript0 */
                g_chara = r.character;
                if(r.mode == UI_MODE_STORY && r.act == 2) run_cutscene(CS_SCRIPT, 0, r.zone, NULL);
                g_dcp_world.mode = r.mode;
                g_dcp_world.lives = r.lives;
                g_ui_game.save_lives[r.character] = r.lives;
                g_ui_game.save_flags = gSaveData.unk8;
                g_emeralds = r.emeralds;
                g_difficulty = r.easy;
                load_player_model();
                stage = idx; g_frontend = 0;
                ok = load_stage(&lv, stage) == 0;
                if(ok && g_player_ok) spawn_player(&lv);
            } else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "Ending"))
                run_ending(CS_ENDING, 7);                   /* SceneChange_ToEnding (+ staff roll, PostStaffRoll) */
            else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding"))
                run_ending(CS_EXTRA_ENDING, 8);             /* SceneChange_ToExtraEnding */
            else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "DemoScript4")) {
                run_cutscene(CS_SCRIPT, 4, 6, "sys");        /* ClearDemo_GoToNextScene zone 6 boss */
                menus_restart_bgm();                        /* the zone map's BGM after the script's music */
            }
            else if(r.kind == MENU_REQ_UNAVAILABLE)
                printf("menus: %s not in the port\n", r.scene ? r.scene : "?");
            else if(r.kind == MENU_REQ_DEMO) {
                /* SceneChange_ToDemoPlay: DemoPlay_Create + the demo's stage */
                DemoInfo di;
                int idx;
                if(demo_start(r.demo, &di) != 0 || (idx = stage_index(di.zone, di.act)) < 0) {
                    printf("demo %d: not available\n", r.demo);
                    menus_stage_exit(MENU_EXIT_DEMO_END, 0, 0, g_emeralds, 0);
                } else {
                    g_chara = di.character; load_player_model();
                    g_demo_spawn_x = di.start_x; g_demo_spawn_y = di.start_y;
                    stage = idx; g_frontend = 0;
                    ok = load_stage(&lv, stage) == 0;
                    if(ok && g_player_ok) spawn_player(&lv);
                    demo_after_stage_load();
                    g_demo = 1;
                }
            }
            continue;
        }
        input_poll(&in);
        /* attract demo: DemoInput_Main replaces gPad with the recorded keys */
        uint16_t pad_held = ds_pad(in.held), pad_pressed = ds_pad(in.pressed), pad_released = ds_pad(in.released);
        InputState real_in = in;
        if(g_demo) {
            DemoPad dp;
            demo_input_frame(&real_in, &dp);
            in = dp.in; pad_held = dp.held; pad_pressed = dp.pressed; pad_released = dp.released;
        }
        if(ok && g_player_ok && g_is_boss) ui_set_boss_health(boss_health());
        if(ok && g_player_ok) ui_update(&in, &g_player);
        if(ok && g_player_ok) overlay_cutscene_frame(&in);   /* boss talks: System_Freeze while active */
        if(ok && g_player_ok && !ui_is_paused() && !ui_world_frozen() && !cutscene_active()) {
            /* StageTitleCard_State_StartStage: gBossStatus.flags |= 1 at "GO!" (under the white flash), not
             * when the title card task ends: the arena (BossF camera) and the boss start there */
            if(g_boss_restart) {   /* StageRestart: the stage and the boss are created again (full health, waits for the title card) */
                g_boss_restart = 0;
                boss_shutdown();
                boss_stage_restart(&lv);
                boss_set_paths("/cd/models", "/cd/boss");
                boss_set_difficulty(g_difficulty);
                sound_boss_reset();   /* the previous fight's sound handles */
                g_is_boss = boss_init(lv.hdr->stage, lv.data, &g_player, 1) == 0;
                g_boss_started = 0;
                printf("boss: restart %s\n", g_is_boss ? "ok" : "FAILED");
            }
            if(g_stage_restart) {   /* StageRestart in an act: the objects and rings again */
                g_stage_restart = 0;
                if(g_obj_ok) objects_stage_restart();
                sound_obj_reset();   /* the previous attempt's object sound handles */
                printf("stage restart: player at %ld,%ld, lives %d\n", (long)(g_player.x >> 8), (long)(g_player.y >> 8), (int)g_dcp_world.lives);
            }
            if(g_is_boss && !g_boss_started && ui_titlecard_started()) { boss_start(); g_boss_started = 1; }
            if(g_is_boss && g_boss_started && g_dbg_bosshp > 0 && gBossStatus.unk8 > g_dbg_bosshp) {
                printf("debug: boss hit points %d -> %d\n", gBossStatus.unk8, g_dbg_bosshp);
                gBossStatus.unk8 = (u16)g_dbg_bosshp;
            }
            camera_update();   /* Camera_Main (task 0x80): the DS screen pair, the DC window, ZoneBg / ZoneFx */
            if(g_obj_ok) {
                /* The DS spawns objects for both screens: view 1 is the area the DS
                 * top screen showed (192 px above). Spawning only; nothing is drawn
                 * for it (one continuous viewport, CAMERA.md). */
                s32 vx, vy, px2, py2;
                camera_get_view(&vx, &vy);
                camera_get_partner_view(&px2, &py2);   /* the DS's other screen, as the original spawns */
                /* views 0/1 = the DS screen pair (gCamera[0]/[1]): spawning and the in-view
                 * checks follow the original (a gimmick held by gCamera[0], e.g. the special
                 * ring, stays alive: DEFECTS F10); view 2 = the DC window, so nothing visible
                 * is ever missing */
                objects_set_view(0, camera_layer(0)->x, camera_layer(0)->y);
                objects_set_view(1, px2, py2);
                objects_set_view(2, vx, vy);
                objects_frame_begin();
            }
            if(g_is_boss) boss_update_pre(&g_player);   /* BossF: arm colmap / ride before the player */
            { uint64_t t0 = timer_us_gettime64();
              dcp_player_frame(&g_player, pad_held, pad_pressed, pad_released);
              camera_player_last(&g_player);   /* Player_Last: Player_UpdateCamera + Player_UpdateCameraLead */
              g_prof[1] += timer_us_gettime64() - t0; frameprof_add(FP_PLAYER, (uint32_t)(timer_us_gettime64() - t0)); }
            if(g_model_ok) {
                s32 tsp = 0x1000;   /* title card close-up: obj unk24C slot speeds (slow motion) */
                ui_titlecard_player_draw(&tsp, NULL, NULL, NULL, NULL, NULL);
                dcp_model_set_slot_speed(&g_dcp_model, tsp);
                dcp_model_step(&g_dcp_model, &g_player, 0);   /* animation, ANIM_DONE, pose */
            }
            { uint64_t t0 = timer_us_gettime64(); if(g_obj_ok) objects_update(&g_player); g_prof[0] += timer_us_gettime64() - t0;
              frameprof_add(FP_OBJECTS, (uint32_t)(timer_us_gettime64() - t0)); }
            if(g_is_boss) boss_update(&g_player);
            camera_frame_end();   /* ScreenShake_Main */
        }
        if(ok && g_player_ok && g_dbg_special > 0 && (int)g_dcp_world.frame == g_dbg_special) { g_dbg_special = -1; g_special_pending = 1; }
        if(ok && g_player_ok && !g_demo && ui_state() == UI_PLAYING && !ui_titlecard_active() && !cutscene_active()) {
            if(g_dbg_spawn_late_x || g_dbg_spawn_late_y) {   /* debug spawn: the place again, the title card is over */
                g_player.x = g_dbg_spawn_late_x << 8; g_player.y = g_dbg_spawn_late_y << 8;
                g_player.vel_x = g_player.vel_y = 0; g_player.gnd_vel = 0;
                camera_snap_to_player(&g_player);   /* the objects of that place spawn now */
                if(g_obj_ok) objects_respawn_view();
                printf("debug: player placed at %d,%d after the title card\n", g_dbg_spawn_late_x, g_dbg_spawn_late_y);
                g_dbg_spawn_late_x = g_dbg_spawn_late_y = 0;
                g_dbg_spawn_trace = 1200;
            }
            if(g_dbg_spawn_trace > 0 && (g_dbg_spawn_trace-- % 30) == 0) {
                ObjPlayerCtx *pc = obj_player_ctx(&g_player);
                s32 vx = 0, vy = 0;
                camera_get_view(&vx, &vy);
                printf("debug: player %ld,%ld pf %08lx pf2 %08lx gimmick %p held %04x boost frames %d timer %d tension %d view %ld,%ld despawned %lu live %lu\n",
                       (long)(g_player.x >> 8), (long)(g_player.y >> 8), (unsigned long)g_player.pf, (unsigned long)g_player.pf2,
                       g_player.gimmick, (unsigned)g_player.held, pc ? (int)pc->g3F4 : -1, (int)g_player.timer, (int)g_player.tension,
                       (long)(vx >> 8), (long)(vy >> 8), (unsigned long)objects_stats()->despawned, (unsigned long)objects_stats()->live);
            }
            if(g_dbg_autoclear > 0 && (int)g_dcp_world.frame == g_dbg_autoclear) {
                printf("debug: autoclear z%d a%d (%s)\n", g_dcp_world.zone, g_dcp_world.act, g_is_boss ? "boss" : "goal");
                if(g_is_boss) { if(!boss_cleared()) boss_cleared_hook(NULL); }
                else if(g_obj_hooks.stage_exit) g_obj_hooks.stage_exit(g_obj_hooks.ctx, OBJ_EXIT_CLEAR_DEMO);
            }
            if(g_dbg_autodie > 0 && g_dbg_autodie_armed && g_dbg_autodie_lives >= 10 && (int)g_dcp_world.frame < g_dbg_autodie)
                g_player.invuln = 0x10;   /* test builds: autodie lives >= 10 (taken off 10): unhurt until the death */
            if(g_dbg_autodie > 0 && g_dbg_autodie_armed && (int)g_dcp_world.frame == g_dbg_autodie) {
                if(g_dbg_autodie_lives >= 10) g_dbg_autodie_lives -= 10;
                g_dbg_autodie_armed = 0;   /* the timer stops while the player dies */
                printf("debug: autodie, lives %d\n", g_dbg_autodie_lives);
                g_dcp_world.lives = (u8)g_dbg_autodie_lives;
                g_player.y = g_dcp_world.death_y << 8;   /* below the map: the pit death (Player_Main) */
            }
        }
        if(ok && g_player_ok && g_after_boss_pending) {
            g_after_boss_pending = 0;
            run_cutscene(CS_SCRIPT, 1, g_dcp_world.zone, sound_stage_sdat(g_dcp_world.zone, g_dcp_world.act, g_chara));
            ui_stage_exit(OBJ_EXIT_CLEAR_DEMO);   /* CS_NEXT_CLEARDEMO: the act results */
            continue;
        }
        if(ok && g_player_ok && g_special_pending) { run_special_stage(&lv, stage); continue; }
        if(g_sweep && ok) {   /* test builds: next act */
            static int n, sp_done;
            if(++n >= (g_dcp_world.act == 2 ? 2 * g_sweep : g_sweep)) {
                int next = (stage + 1) % (int)NUM_STAGES;
                n = 0;
                if(stage == 1 && !sp_done) { sp_done = 1; run_special_stage(&lv, stage); continue; }   /* round trip */
                sp_done = 0;
                if(g_obj_ok) { objects_shutdown(); objects_models_free(); }
                boss_shutdown(); g_is_boss = 0;
                ui_stage_end();
                zonefx_shutdown(); camera_destroy();
                level_free(&lv); ok = 0;
                if(!strcmp(k_stages[stage], "zf3")) { run_extra_zone(); }
                if(k_stages[next][2] == '3') run_cutscene(CS_SCRIPT, 0, k_stages[next][1] == 'f' ? 7 : k_stages[next][1] - '1', NULL);
                stage = next;
                ok = load_stage(&lv, stage) == 0;
                if(ok && g_player_ok) spawn_player(&lv);
                continue;
            }
        }
        if(ok && g_player_ok) ui_post_update(&g_player);
        /* StageTitleCard_HoldPlayerX / Y move the player after the world update and snap gCamera */
        if(ok && g_player_ok && ui_titlecard_active()) camera_snap_to_player(&g_player);
        sound_update();
        if(ok) camera_apply();   /* g_cam_x / g_cam_y: the DC window + the screen shake */
        {   /* the single-player gameplay view (wider window in ordinary story levels) */
            static int sp_last_stage = -1;
            sp_view_begin(sp_view_eligible(ok, stage), stage != sp_last_stage || !ok);
            sp_last_stage = ok ? stage : -1;
            if(ok && g_dcp_world.frame == 300)   /* one log line a stage: the view in use */
                printf("sp view: %s k %.3f (eligible %d, mode %d, boss %d)\n", k_stages[stage], (double)g_sp_k,
                       sp_view_eligible(ok, stage), (int)g_dcp_world.mode, g_is_boss);
        }
        ui_set_camera(g_cam_x, g_cam_y);

        { uint64_t t0 = timer_us_gettime64(); if(ok && g_obj_ok) objects_models_prepare(); frameprof_add(FP_D_OMODEL, (uint32_t)(timer_us_gettime64() - t0)); }   /* object model / 3D BAC loads, texture uploads, outside the lists */
        frameprof_work_end();
        { uint64_t tw = timer_us_gettime64(); pvr_wait_ready(); g_prof[3] += timer_us_gettime64() - tw; }   /* vblank / TA wait */
        frameprof_frame();
        /* zone palettes at the frame start: zonefx waits for the render in progress before a write */
        { uint64_t tz = timer_us_gettime64(); if(ok && !g_is_boss) zonefx_commit_palettes();
          frameprof_add(FP_WAIT, (uint32_t)(timer_us_gettime64() - tz)); }   /* may wait for the render in progress */
        uint64_t t_draw0 = timer_us_gettime64();
        {   /* frame interval: worst and over-budget count (a 60 Hz frame is 16.68 ms) */
            static uint64_t last;
            uint64_t now_f = t_draw0;
            if(last) { uint64_t d = now_f - last; if(d > g_fmax) g_fmax = d; if(d > 17500) g_fslow++; }
            last = now_f;
        }
        render_begin_frame();
        /* PVR lists must be opened in order OP, PT, TR (each once per frame).
         * Depth (1/w style, bigger = nearer): bg 0.0005 < OBJ prio 3 < layer B 0.001 < OBJ prio 2 < 3D layer
         * (player and object models ~0.0035..0.03) < OBJ prio 1 (2..2.64) < layer A 10 < OBJ prio 0 (11..11.64) < HUD 100.
         * The OBJ bands are obj_draw.c's (Stage_InitDisplay BG priorities: A 0, 3D 1, B 2, zone BG 3). */
        render_begin_list(PVR_LIST_OP_POLY);
        { uint64_t t0 = timer_us_gettime64(); if(ok && !g_is_boss) zonefx_draw_bg(0.0005f); g_dprof[0] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 0, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(g_is_boss) boss_draw(PVR_LIST_OP_POLY); g_dprof[5] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 5, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_OP_POLY); g_dprof[2] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 2, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_obj_ok && !g_is_boss) objects_draw_models(PVR_LIST_OP_POLY); frameprof_add(FP_D_OMODEL, (uint32_t)(timer_us_gettime64() - t0)); }   /* object models, 3D layer */
        render_end_list();
        render_begin_list(PVR_LIST_PT_POLY);
        { uint64_t t0 = timer_us_gettime64(); if(ok && !g_is_boss) { zonefx_draw_layer(1, PVR_LIST_PT_POLY, 0.001f); zonefx_draw_layer(0, PVR_LIST_PT_POLY, 10.0f); } g_dprof[1] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 1, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(g_is_boss) boss_draw(PVR_LIST_PT_POLY); g_dprof[5] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 5, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_PT_POLY); g_dprof[2] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 2, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_obj_ok && !g_is_boss) objects_draw_models(PVR_LIST_PT_POLY); frameprof_add(FP_D_OMODEL, (uint32_t)(timer_us_gettime64() - t0)); }   /* object models, 3D layer */
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_obj_ok) obj_draw(PVR_LIST_PT_POLY); g_dprof[3] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 3, (uint32_t)(timer_us_gettime64() - t0)); }
        render_end_list();
        render_begin_list(PVR_LIST_TR_POLY);
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_TR_POLY); g_dprof[2] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 2, (uint32_t)(timer_us_gettime64() - t0)); }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_obj_ok && !g_is_boss) objects_draw_models(PVR_LIST_TR_POLY); frameprof_add(FP_D_OMODEL, (uint32_t)(timer_us_gettime64() - t0)); }   /* object models, 3D layer */
        if(ok && g_obj_ok) obj_draw(PVR_LIST_TR_POLY);   /* AnimatorSprite 0x400 (semi-transparent OBJ) */
        { uint64_t t0 = timer_us_gettime64(); if(g_is_boss) boss_draw(PVR_LIST_TR_POLY); g_dprof[5] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 5, (uint32_t)(timer_us_gettime64() - t0)); }
        if(g_is_boss) {
            s32 bm = 0, evy = 0;
            bossf_blend(&bm, &evy);   /* BossF white-out of the 3D layer (BLDY) */
            if(bm == 2 && evy > 0)
                draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 9.5f,
                                 ((uint32_t)(evy > 16 ? 255 : evy * 255 / 16) << 24) | 0xffffff);
        }
        if(ok && !g_is_boss) zonefx_draw_tr();   /* Water Palace: surface sprites (ZoneFx_Zone2_DrawSprites) */
        if(g_flash > 0) {
            draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 95.0f,
                             ((uint32_t)(g_flash * 15) << 24) | 0xffffff);
            if(!ui_is_paused()) g_flash--;
        }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_player_ok) ui_draw_tr(); g_dprof[4] += timer_us_gettime64() - t0; frameprof_add(FP_D_BG + 4, (uint32_t)(timer_us_gettime64() - t0)); }
        if(cutscene_active() && !cutscene_is_fullscreen()) cutscene_draw_list(PVR_LIST_TR_POLY);
        if(ok && g_player_ok && !g_model_ok)
            draw_sprite_world(NULL, PVR_LIST_TR_POLY, g_player.x / 256.0f - 8, g_player.y / 256.0f - 28, 16, 28, 0, 0, 0, 0, 0, 3.0f, 0xc02060ff);
        render_end_list();
        g_prof[2] += timer_us_gettime64() - t_draw0;
        { uint64_t t1 = timer_us_gettime64(); render_end_frame(); g_prof[2] += timer_us_gettime64() - t1; sp_view_end();
          frameprof_add(FP_DRAW, (uint32_t)(timer_us_gettime64() - t_draw0)); }
        audio_update();
        if(ok && g_demo) {
            int dk = demo_play_frame(&real_in);   /* DemoPlay_Main */
            if(dk != DEMO_RUNNING) {
                demo_stop(); g_demo = 0; g_demo_spawn_x = g_demo_spawn_y = 0;
                if(g_obj_ok) { objects_shutdown(); objects_models_free(); }
                boss_shutdown(); g_is_boss = 0;
                ui_stage_end();
                zonefx_shutdown(); camera_destroy();
                level_free(&lv); ok = 0;
                menus_stage_exit(dk == DEMO_EXIT_TITLE ? MENU_EXIT_DEMO_END : MENU_EXIT_DEMO_TIMEOUT, 0, 0, g_emeralds, 0);
                enter_front_end();
                continue;
            }
        }
        if(ok && g_player_ok && ui_state() == UI_EXIT) {
            int k = ui_exit_kind();
            MenuRequest r;
            uint32_t total = g_ui_game.total;
            printf("stage exit kind %d\n", k);
            if(g_obj_ok) { objects_shutdown(); objects_models_free(); }
            boss_shutdown(); g_is_boss = 0;
            ui_stage_end();   /* title-card / results BGs, navigator, face texture */
            zonefx_shutdown(); camera_destroy();
            level_free(&lv); ok = 0;
            if(k == UI_EXIT_NEXT_ACT || (k == UI_EXIT_TRY_AGAIN && ui_exit_cleared()))
                r = menus_stage_exit(MENU_EXIT_CLEARED, total, g_dcp_world.lives, g_emeralds, g_dcp_world.frame);
            else if(k == UI_EXIT_TRY_AGAIN)   /* time attack: pause quit / Stage_ExitAfterFail, no record */
                r = menus_stage_exit(MENU_EXIT_TRY_AGAIN, total, g_dcp_world.lives, g_emeralds, 0);
            else if(k == UI_EXIT_QUIT)
                r = menus_stage_exit(MENU_EXIT_QUIT, total, g_dcp_world.lives, g_emeralds, 0);
            else
                r = menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, g_emeralds, 0);
            if(dbg_start && (k == UI_EXIT_NEXT_ACT || g_sweep) && r.kind != MENU_REQ_STAGE) {   /* sweep: past a game over too */
                /* test builds without menus: continue in story order */
                const char *nx = k_stages[(stage + 1) % NUM_STAGES];
                r.kind = MENU_REQ_STAGE; r.zone = nx[1] == 'f' ? 7 : nx[1] - '1';
                r.act = nx[2] - '1'; r.character = g_chara;
                r.mode = g_dcp_world.mode; r.lives = g_dcp_world.lives; r.emeralds = g_emeralds; r.easy = 1;
            }
            if(r.kind == MENU_REQ_STAGE && r.zone == 8 && r.act == 2) {
                g_chara = r.character;
                r = run_extra_zone();
                enter_front_end();
                if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding"))
                    run_ending(CS_EXTRA_ENDING, 8);
            } else if(r.kind == MENU_REQ_STAGE && stage_index(r.zone, r.act) >= 0) {
                g_chara = r.character; load_player_model();
                if(r.mode == UI_MODE_STORY && r.act == 2) run_cutscene(CS_SCRIPT, 0, r.zone, NULL);   /* Game_AdvanceActWithDemo */
                stage = stage_index(r.zone, r.act);
                ok = load_stage(&lv, stage) == 0;
                if(ok && g_player_ok) spawn_player(&lv);
            } else {
                enter_front_end();
                if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "Ending")) run_ending(CS_ENDING, 7);
                else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding")) run_ending(CS_EXTRA_ENDING, 8);
                else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "DemoScript4")) {
                    run_cutscene(CS_SCRIPT, 4, 6, "sys");
                    menus_restart_bgm();   /* the zone map's BGM after the script's music */
                }
            }
        }
        if(g_dbg_bosshp > 0 && g_is_boss && frame % 60 == 0 && g_dcp_world.zone == 6) {
            Boss7Info bi;
            boss7_info(&bi);
            printf("debug boss7: rival %ld vx %ld anim %ld move %s player %ld gnd %d\n", (long)(bi.rival_x >> 8), (long)bi.rival_vx, (long)bi.rival_anim,
                   bi.rival_move ? bi.rival_move : "-", (long)(g_player.x >> 8), g_player.gnd_vel);
            printf("debug boss7: clash %d balance %ld power %ld/%ld won %d lost %d blend %ld/%ld pad %04x frozen %d cs %d paused %d hp %d\n",
                   bi.clash, (long)bi.balance, (long)bi.power_player, (long)bi.power_rival, bi.won, bi.lost, (long)bi.blend_mode,
                   (long)bi.blend_evy, (unsigned)g_player.pad_raw[1], ui_world_frozen(), cutscene_active(), ui_is_paused(), gBossStatus.unk8);
        }
        if(++frame % 120 == 0 && dbg_start && !frameprof_enabled()) {   /* frame log: test builds only (each serial line costs ms) */
            uint64_t now = timer_ms_gettime64();
            if(t_last) printf("frame %d cam %.0f,%.0f player %ld,%ld spd %d ang %d fps %.1f music %lu B loops %lu rings %d live %lu | player rings %d lives %d\n", frame, g_cam_x, g_cam_y, (long)(g_player.x >> 8), (long)(g_player.y >> 8), g_player.vel_x, g_player.angle, 120000.0 / (now - t_last), (unsigned long)audio_stream_bytes(), (unsigned long)audio_stream_loops(), g_obj_ok ? objects_ring_count() : -1, g_obj_ok ? (unsigned long)objects_stats()->live : 0, (int)g_player.rings, (int)g_dcp_world.lives);
            printf("prof ms/frame: objects %.2f player %.2f draw %.2f wait %.2f; worst frame %.2f ms, %d of 120 over 17.5 ms\n", g_prof[0] / 120000.0, g_prof[1] / 120000.0, g_prof[2] / 120000.0, g_prof[3] / 120000.0, g_fmax / 1000.0, g_fslow);
            if(g_obj_ok) {   /* object models in view, PVR triangles; palette writes that waited for a frame start */
                u32 mv = 0, mt = 0;
                objects_models_stats(&mv, &mt);
                u32 q3 = 0, fl = 0, fs = 0;
                objects_spr3d_stats(&q3, &fl, &fs);
                printf("objmodels: views %lu tris %lu; palette queued writes %lu; 3d sprites %lu quads, fx %lu live %lu spawned\n", (unsigned long)mv, (unsigned long)mt, (unsigned long)pal_queued_writes(), (unsigned long)q3, (unsigned long)fl, (unsigned long)fs);
            }
            g_fmax = 0; g_fslow = 0;
            {   /* camera / zone effects: DS pair y (gCamera[0], [1]), swap flag, window, BG row 0 / 191, BLDY, water row */
                const CamLayer *c0 = camera_layer(0), *c1 = camera_layer(1);
                const CamStats *cs = camera_stats();
                const ZfxLine *zl = zonefx_lines();
                printf("cam ds %ld,%ld / %ld swap %d window %.0f,%.0f swaps %lu eased %lu clamps %lu bg %u,%u %u,%u bldy %d water %d pal %lu\n",
                       (long)(c0->x >> 8), (long)(c0->y >> 8), (long)(c1->y >> 8), (int)((camera_flags2() >> 1) & 1), g_cam_x, g_cam_y,
                       (unsigned long)cs->swaps, (unsigned long)cs->eased, (unsigned long)cs->clamped_player, zl[0].x, zl[0].y, zl[191].x, zl[191].y,
                       zonefx_bldy_level(), zonefx_water_row(), (unsigned long)zonefx_palette_updates());
            }
            printf("draw ms/frame: bg %.2f layers %.2f player %.2f objects %.2f ui %.2f boss %.2f\n", g_dprof[0] / 120000.0, g_dprof[1] / 120000.0, g_dprof[2] / 120000.0, g_dprof[3] / 120000.0, g_dprof[4] / 120000.0, g_dprof[5] / 120000.0);
            memset(g_prof, 0, sizeof(g_prof)); memset(g_dprof, 0, sizeof(g_dprof));
            t_last = now;
        }
    }
    return 0;
}
