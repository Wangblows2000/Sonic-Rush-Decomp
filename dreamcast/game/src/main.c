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
#include "../boss/boss.h"
#include "../boss/boss_final.h"
#include "../objects/obj_internal.h"
#include "../menus/menus.h"
#include "../save/save.h"
#include "../special/special.h"
#include "../cutscene/cutscene.h"
#include "../boss/boss_extra.h"
#include "../boss/boss7.h"
#include "../demo/demo.h"
#include "../engine/loading.h"
#include "../menus/extraact.h"
#include <stdlib.h>

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
static int g_demo, g_demo_spawn_x, g_demo_spawn_y;   /* attract demo running (DemoPlay) */
static uint64_t g_prof[4], g_dprof[6], g_fmax;
static int g_fslow;   /* draw: bg, layers, player model, objects, ui, boss */   /* us per 120 frames: objects update, player, draw, end_frame wait */
static int g_dbg_special = -1;   /* test builds: /cd/debug/special.txt = act frame to enter the special stage */
static void (*g_prev_stage_exit)(void *ctx, s32 kind);
static void stage_exit_hook(void *ctx, s32 kind)
{
    if(kind == OBJ_EXIT_SPECIAL_STAGE) { g_special_pending = 1; return; }   /* Player_ExitToSpecialStage */
    if(g_prev_stage_exit) g_prev_stage_exit(ctx, kind);
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
    rlvl_bind(&g_map, lv->data);
    dcp_bind_map(&g_map);
    memset(&g_player, 0, sizeof(g_player));
    dcp_player_create(&g_player, g_chara, 0, sx, sy);
    objects_shutdown();
    if(g_model_ok) dcp_model_attach(&g_dcp_model, &g_player);   /* anim hooks, chained before the objects' */
    g_obj_ok = objects_init(&g_map, lv->data) == 0;
    if(g_obj_ok) {
        if(g_ss.active && objects_restore_event_state) objects_restore_event_state();
        objects_add_player(&g_player); objects_set_view_count(2); objects_respawn_view();
        /* DCP_DF_ANIM_DONE comes from the model's joint animation (StageObj_Draw3DEx);
         * son.bac timing stands in only when the model is missing */
        objects_set_player_anim_done(!g_model_ok);
    }
    printf("objects: vram free after objects_init %lu\n", (unsigned long)render_vram_free());
    sound_load_bank(sound_stage_sdat(g_dcp_world.zone, g_dcp_world.act, g_chara));
    sound_hooks_reset_players();
    g_is_boss = 0; g_boss_started = 0;
    boss_shutdown();
    if(g_dcp_world.act == 2) {
        boss_set_paths("/cd/models", "/cd/boss");
        boss_set_difficulty(g_difficulty);
        g_is_boss = boss_init(lv->hdr->stage, lv->data, &g_player, 1) == 0;
        printf("boss: %s\n", g_is_boss ? "ok" : "FAILED");
    }
    ui_bind_player(&g_player);
    g_ui_game.language = 1;   /* English (gGameLanguage) */
    g_ui_game.difficulty = (u8)g_difficulty;
    g_ui_game.save_progress[0] = gSaveData.unkC; g_ui_game.save_progress[1] = gSaveData.unk64;
    ui_stage_start(g_dcp_world.zone, g_dcp_world.act, g_chara);
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
    ui_set_camera(g_cam_x, g_cam_y);
    printf("objects: %s\n", g_obj_ok ? "ok" : "FAILED");
    g_cam_x = sx - VIEW_W / 2; g_cam_y = sy - VIEW_H * 0.6f;
    printf("player spawned at %d,%d\n", sx, sy);
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

static void draw_player_model(pvr_list_t list)
{
    dcm_camera_t cam;
    dcm_mtx_t mdl;
    dcm_lights_t lights;
    s32 ax, ay, az, tsp, teye[3], tat[3];
    int tzoom;
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

static void start_zone_music(char zone)
{
    char path[48], line[64];
    uint32_t ls = 0, le = 0, sz;
    char *txt = load_file("/cd/music/loops.txt", &sz), *p;
    if(txt) {
        txt = realloc(txt, sz + 1); txt[sz] = 0;
        for(p = txt; p && *p; p = strchr(p, '\n') ? strchr(p, '\n') + 1 : NULL) {
            unsigned a, b; char z;
            if(sscanf(p, "%c %u %u", &z, &a, &b) == 3 && z == zone) { ls = a; le = b; break; }
        }
        free(txt);
    }
    (void)line;
    snprintf(path, sizeof(path), "/cd/music/z%c.wav", zone);
    printf("music %s loop %lu-%lu: %s\n", path, (unsigned long)ls, (unsigned long)le,
           audio_music_play(path, ls, le) == 0 ? "streaming" : "FAILED");
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
    r = level_load(lv, dir);
    if(r == 0) {
        /* gGameState.unk1C / unk1D: stage id = zone * 3 + act (0, 1, 2 = boss) */
        g_dcp_world.zone = (u8)(lv->hdr->stage / 3);
        g_dcp_world.act = (u8)(lv->hdr->stage % 3);
    }
    start_zone_music(k_stages[idx][1]);
    printf("stage %s: load %d in %llu ms, %dx%d px, %d block pages, vram used %lu, vram free %lu\n",
           k_stages[idx], r, (unsigned long long)(timer_ms_gettime64() - t0),
           r == 0 ? level_width_px(lv) : 0, r == 0 ? level_height_px(lv) : 0, lv->num_pages,
           (unsigned long)lv->vram_used, (unsigned long)render_vram_free());
    return r;
}

/* Menu sound hooks (MenuHooks): sys SDAT music streams as /cd/music/sNN.wav
 * (NN = sequence id, loop points in /cd/music/sys.txt), SE through the
 * native sequencer with the sys bank. */
static void menu_bgm(int seq)
{
    char path[32];
    uint32_t ls = 0, le = 0, sz;
    char *txt = load_file("/cd/music/sys.txt", &sz), *p;
    if(txt) {
        txt = realloc(txt, sz + 1); txt[sz] = 0;
        for(p = txt; p && *p; p = strchr(p, '\n') ? strchr(p, '\n') + 1 : NULL) {
            unsigned id, a, b;
            if(sscanf(p, "%u %u %u", &id, &a, &b) == 3 && (int)id == seq) { ls = a; le = b; break; }
        }
        free(txt);
    }
    snprintf(path, sizeof(path), "/cd/music/s%02d.wav", seq);
    if(audio_music_play(path, ls, le) != 0) printf("menu bgm %s FAILED\n", path);
}
static void menu_se(int arc, int id) { SysSound_StartSEArc(arc, id); }   /* cuts the previous menu SE */
static void menu_voice(int id) { SysSound_StartVoice(id, 0); }
static void menu_voice_alt(int id) { SysSound_StartVoice(id, 1); }
static void menu_voice_arc(int arc, int id) { Sound_PlayVoice(NULL, (uint32_t)arc, (uint32_t)id); }
static void menu_stop_all(void) { sound_stop_all(); audio_music_stop(); }
static void enter_front_end(void)
{
    g_frontend = 1;
    audio_music_stop();
    if(sound_load_bank("sys") != 0) printf("sys sound bank missing\n");
}

/* g_obj_hooks.effect: object effects that belong to the frame compositor.
 * ScreenFlash_Create: full-frame white flash fading over 16 frames.
 * Enemy-room events go to the UI (EnemyRoomHud). */
static int g_flash;
static void obj_effect_hook(void *ctx, DcPlayer *p, s32 kind, s32 a, s32 b)
{
    (void)ctx; (void)p;
    switch(kind) {
    case OBJ_FX_SCREEN_FLASH: g_flash = 16; break;
    case OBJ_FX_ENEMY_ROOM: ui_enemy_room(a, b); break;   /* EnemyRoomHud */
    default: break;
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

/* Water Palace water (OBJECTS.md 2.3): the DS darkens/tints everything below
 * the water line with the BG/OBJ window and a palette effect. Here a
 * translucent tint over the viewport below the line, plus a lighter surface band. */
static void draw_water(int line)
{
    float y = (line - g_cam_y) * VIEW_SCALE;
    if(y >= 480.0f) return;
    if(y < 0) y = 0;
    draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, y, 640, 480 - y, 0, 0, 0, 0, 9.0f, 0x502060c0);
    if(y > 0) draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, y - 2 * VIEW_SCALE, 640, 2 * VIEW_SCALE, 0, 0, 0, 0, 9.1f, 0x60c0e0ff);
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
static void boss_music_stop_hook(void *ctx) { (void)ctx; audio_music_stop(); }
static void boss_music_hook(void *ctx, s32 seq) { (void)ctx; printf("boss music seq %ld (stage SDAT stream not mapped)\n", (long)seq); }

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
        if(r->set_save_flags) SaveData_SetProgressFlags(SaveData_GetProgressFlags() | r->set_save_flags);
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
    for(;;) {
        int done;
        input_poll(&in);
        done = cutscene_run_frame(&in);
        sound_update();
        render_begin_frame(); cutscene_draw(); render_end_frame();
        audio_update();
        if(done) break;
    }
    {
        CsNext n = cs_finish();
        printf("cutscene %d/%d end, next %d\n", kind, id, n);
        return n;
    }
}
/* talk overlays: zone map (menus hook) and boss talks (Boss7 rival, final boss) */
static int menu_talk_start(int c, int step) { cs_story(0);   /* zone-map talks: the zone is not read */
    return cutscene_start(CS_TALK_MAP, cutscene_zonemap_talk_id(c, step), c); }
static int menu_talk_active(void) { return cutscene_active(); }
static void boss_talk_hook(void *ctx, s32 id)
{
    (void)ctx;
    cs_story(g_dcp_world.zone);
    cutscene_set_return_bank(NULL);
    if(cutscene_start(CS_TALK, id, g_chara) != 0) printf("talk %ld: missing data\n", (long)id);
}
static void overlay_cutscene_frame(const InputState *in)
{
    if(cutscene_active() && !cutscene_is_fullscreen() && cutscene_run_frame(in)) cs_finish();
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
    boss_extra_set_paths("/cd/models", "/cd/boss/extra");
    boss_extra_set_lives(gSaveData.unkE ? gSaveData.unkE : 3, gSaveData.unk66 ? gSaveData.unk66 : 3);
    boss_extra_set_difficulty(0);   /* gGameState.unk14: 0 normal */
    memset(&res, 0, sizeof(res));
    loading_begin(); audio_load_begin();
    r = boss_extra_init(g_chara);
    loading_end(); audio_load_end();
    if(r == 0) {
        r = BOSS_EXTRA_FAILED;
        for(;;) {
            input_poll(&in);
            r = boss_extra_run_frame(&in);
            sound_update();
            render_begin_frame();
            render_begin_list(PVR_LIST_OP_POLY); boss_extra_draw(PVR_LIST_OP_POLY); render_end_list();
            render_begin_list(PVR_LIST_PT_POLY); boss_extra_draw(PVR_LIST_PT_POLY); render_end_list();
            render_begin_list(PVR_LIST_TR_POLY); boss_extra_draw(PVR_LIST_TR_POLY); render_end_list();
            render_end_frame();
            audio_update();
            if(r != BOSS_EXTRA_RUNNING) break;
        }
        boss_extra_result(&res);
        boss_extra_shutdown();
        SaveData_SetLivesBoth(res.lives_sonic, res.lives_blaze);   /* saved after a game over too, as on the DS */
    } else printf("extra boss: init failed\n");
    printf("extra boss result %d, time %ld, rings %ld\n", r, (long)res.time, (long)res.rings);
    if(r == BOSS_EXTRA_CLEARED) {
        run_cutscene(CS_SCRIPT, 2, 8, NULL);   /* DemoScript_StartAfterBoss -> SceneChange_ToExtraAct */
        if(extraact_start((uint32_t)res.time, (uint32_t)res.rings, g_chara) == 0) {   /* the Extra Zone results */
            for(;;) {
                int done;
                input_poll(&in);
                done = extraact_run_frame(&in);
                sound_update();
                render_begin_frame(); extraact_draw(); render_end_frame();
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
    if(g_obj_ok) objects_shutdown();
    g_obj_ok = 0;
    audio_music_stop();
    ui_stage_end();
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
        for(;;) {
            input_poll(&in);
            r = special_run_frame(&in);
            render_begin_frame(); special_draw(); render_end_frame();
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
    ui_set_paths("/cd/ui", "/cd/sprites");
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
    boss7_set_talk_hook(boss_talk_hook, NULL);
    cutscene_set_paths("/cd/cutscene", "/cd/sprites", "/cd/models", "/cd/music");
    g_menu_hooks.talk_demo_start = menu_talk_start;
    g_menu_hooks.talk_demo_active = menu_talk_active;
    g_boss_hooks.music = boss_music_hook;
    g_boss_hooks.music_stop = boss_music_stop_hook;
    g_ui_hooks.voice = ui_voice_hook;
    g_ui_hooks.stage_new = ui_stage_new_hook;
    g_menu_hooks.bgm = menu_bgm; g_menu_hooks.se = menu_se; g_menu_hooks.voice = menu_voice;
    g_menu_hooks.voice_alt = menu_voice_alt; g_menu_hooks.voice_arc = menu_voice_arc; g_menu_hooks.stop_all = menu_stop_all;
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
        load_player_model();
    }
    {
        /* test builds only: /cd/debug/start.txt holds the story index to start at */
        uint32_t dsz;
        char *d = load_file("/cd/debug/start.txt", &dsz);
        char *sp = load_file("/cd/debug/special.txt", &dsz);
        if(sp) { g_dbg_special = atoi(sp) > 0 ? atoi(sp) : 300; free(sp); }
        if(d) { int v = atoi(d); if(v >= 0 && v < (int)NUM_STAGES) stage = v; free(d); dbg_start = 1; printf("debug start stage %d\n", stage); }
    }
    if(dbg_start) {
        ok = load_stage(&lv, stage) == 0;
        g_cam_x = 0; g_cam_y = 0;
        if(ok && g_player_ok) spawn_player(&lv);
    } else {
        ok = 0; enter_front_end();
        menus_init();   /* menudata, save_load() from the VMU, SEGA logo */
    }

    for(;;) {
        if(g_frontend) {
            MenuRequest r;
            input_poll(&in);
            if(in.pressed) printf("fe pad pressed %08lx\n", (unsigned long)in.pressed);
            r = menus_run_frame(&in);
            overlay_cutscene_frame(&in);   /* zone-map TalkDemo */
            sound_update();
            render_begin_frame();
            render_begin_list(PVR_LIST_OP_POLY); render_end_list();
            render_begin_list(PVR_LIST_PT_POLY); render_end_list();
            render_begin_list(PVR_LIST_TR_POLY); menus_draw_tr(); if(cutscene_active()) cutscene_draw_list(PVR_LIST_TR_POLY); render_end_list();
            render_end_frame();
            if(r.kind == MENU_REQ_STAGE && r.zone == 8 && r.act == 2) {
                /* extra story: Game_StartDemoScript3AtZone8Act2 */
                g_chara = r.character; g_dcp_world.mode = r.mode; g_emeralds = r.emeralds; g_difficulty = r.easy;
                r = run_extra_zone();
                enter_front_end();
                if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding"))
                    run_cutscene(CS_EXTRA_ENDING, 0, 8, NULL);
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
                run_cutscene(CS_ENDING, 0, 7, NULL);        /* SceneChange_ToEnding (+ staff roll, PostStaffRoll) */
            else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding"))
                run_cutscene(CS_EXTRA_ENDING, 0, 8, NULL);  /* SceneChange_ToExtraEnding */
            else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "DemoScript4"))
                run_cutscene(CS_SCRIPT, 4, 6, NULL);        /* ClearDemo_GoToNextScene zone 6 boss */
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
            float px, py, tx, ty;
            if(g_is_boss && !g_boss_started && !ui_titlecard_active()) { boss_start(); g_boss_started = 1; }
            if(g_obj_ok) {
                /* The DS spawns objects for both screens: view 1 is the area the DS
                 * top screen showed (192 px above). Spawning only; nothing is drawn
                 * for it (one continuous viewport, CAMERA.md). */
                objects_set_view(0, (s32)(g_cam_x * 256.0f), (s32)(g_cam_y * 256.0f));
                objects_set_view(1, (s32)(g_cam_x * 256.0f), (s32)((g_cam_y - 192.0f) * 256.0f));
                objects_frame_begin();
            }
            if(g_is_boss) boss_update_pre(&g_player);   /* BossF: arm colmap / ride before the player */
            { uint64_t t0 = timer_us_gettime64();
              dcp_player_frame(&g_player, pad_held, pad_pressed, pad_released);
              g_prof[1] += timer_us_gettime64() - t0; }
            if(g_model_ok) {
                s32 tsp = 0x1000;   /* title card close-up: obj unk24C slot speeds (slow motion) */
                ui_titlecard_player_draw(&tsp, NULL, NULL, NULL, NULL, NULL);
                dcp_model_set_slot_speed(&g_dcp_model, tsp);
                dcp_model_step(&g_dcp_model, &g_player, 0);   /* animation, ANIM_DONE, pose */
            }
            { uint64_t t0 = timer_us_gettime64(); if(g_obj_ok) objects_update(&g_player); g_prof[0] += timer_us_gettime64() - t0; }
            if(g_is_boss) boss_update(&g_player);
            /* Continuous camera (CAMERA.md): the DS screen-sized window follows the
             * player with damping and a modest velocity look-ahead; render-only.
             * During the title card the DS camera snaps to the player. */
            px = g_player.x / 256.0f; py = g_player.y / 256.0f;
            tx = px - VIEW_W / 2 + (ui_titlecard_active() ? 0 : g_player.vel_x / 256.0f * 8.0f);
            ty = py - VIEW_H * 0.55f;
            if(ui_titlecard_active()) { g_cam_x = tx; g_cam_y = ty; }
            else { g_cam_x += (tx - g_cam_x) * 0.25f; g_cam_y += (ty - g_cam_y) * 0.18f; }
        }
        if(ok && g_player_ok && g_dbg_special > 0 && (int)g_dcp_world.frame == g_dbg_special) { g_dbg_special = -1; g_special_pending = 1; }
        if(ok && g_player_ok && g_after_boss_pending) {
            g_after_boss_pending = 0;
            run_cutscene(CS_SCRIPT, 1, g_dcp_world.zone, sound_stage_sdat(g_dcp_world.zone, g_dcp_world.act, g_chara));
            ui_stage_exit(OBJ_EXIT_CLEAR_DEMO);   /* CS_NEXT_CLEARDEMO: the act results */
            continue;
        }
        if(ok && g_player_ok && g_special_pending) { run_special_stage(&lv, stage); continue; }
        if(ok && g_player_ok) ui_post_update(&g_player);
        if(ok && g_player_ok && ui_titlecard_active()) {
            /* StageTitleCard_HoldPlayerX / Y move the player after the world update and snap gCamera */
            g_cam_x = g_player.x / 256.0f - VIEW_W / 2; g_cam_y = g_player.y / 256.0f - VIEW_H * 0.55f;
        }
        sound_update();
        if(ok) {
            if(g_cam_x > level_width_px(&lv) - VIEW_W) g_cam_x = level_width_px(&lv) - VIEW_W;
            if(g_cam_y > level_height_px(&lv) - VIEW_H) g_cam_y = level_height_px(&lv) - VIEW_H;
        }
        if(ok && g_obj_ok && (g_obj_camera[0].flags34 & 0x10000)) {
            /* enemy room lock (MapObj8C/8F): keep the view inside the room bounds */
            float l = g_obj_camera[0].bound[0] / 256.0f, r = g_obj_camera[0].bound[2] / 256.0f;
            float b = g_obj_camera[0].bound[3] / 256.0f;
            if(r - l >= VIEW_W) {
                if(g_cam_x < l) g_cam_x = l;
                if(g_cam_x > r - VIEW_W) g_cam_x = r - VIEW_W;
            }
            if(b > VIEW_H && g_cam_y > b - VIEW_H) g_cam_y = b - VIEW_H;
        }
        if(g_cam_x < 0) g_cam_x = 0;
        if(g_cam_y < 0) g_cam_y = 0;
        ui_set_camera(g_cam_x, g_cam_y);

        { uint64_t tw = timer_us_gettime64(); pvr_wait_ready(); g_prof[3] += timer_us_gettime64() - tw; }   /* vblank / TA wait */
        uint64_t t_draw0 = timer_us_gettime64();
        {   /* frame interval: worst and over-budget count (a 60 Hz frame is 16.68 ms) */
            static uint64_t last;
            uint64_t now_f = t_draw0;
            if(last) { uint64_t d = now_f - last; if(d > g_fmax) g_fmax = d; if(d > 17500) g_fslow++; }
            last = now_f;
        }
        render_begin_frame();
        /* PVR lists must be opened in order OP, PT, TR (each once per frame).
         * Depth (1/w style, bigger = nearer): bg 0.0005 < layer B 0.001 < player model (~0.006) < layer A 10. */
        render_begin_list(PVR_LIST_OP_POLY);
        { uint64_t t0 = timer_us_gettime64(); if(ok && !g_is_boss) level_draw_bg(&lv, 0.25f, 0.125f, 0.0005f); g_dprof[0] += timer_us_gettime64() - t0; }
        if(g_is_boss) boss_draw(PVR_LIST_OP_POLY);
        if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_OP_POLY);
        render_end_list();
        render_begin_list(PVR_LIST_PT_POLY);
        { uint64_t t0 = timer_us_gettime64(); if(ok && !g_is_boss) { level_draw_layer(&lv, 1, PVR_LIST_PT_POLY, 0.001f); level_draw_layer(&lv, 0, PVR_LIST_PT_POLY, 10.0f); } g_dprof[1] += timer_us_gettime64() - t0; }
        if(g_is_boss) boss_draw(PVR_LIST_PT_POLY);
        if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_PT_POLY);
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_obj_ok) obj_draw(PVR_LIST_PT_POLY); g_dprof[3] += timer_us_gettime64() - t0; }
        render_end_list();
        render_begin_list(PVR_LIST_TR_POLY);
        if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_TR_POLY);
        if(ok && g_obj_ok) obj_draw(PVR_LIST_TR_POLY);   /* AnimatorSprite 0x400 (semi-transparent OBJ) */
        if(g_is_boss) boss_draw(PVR_LIST_TR_POLY);
        if(g_is_boss) {
            s32 bm = 0, evy = 0;
            bossf_blend(&bm, &evy);   /* BossF white-out of the 3D layer (BLDY) */
            if(bm == 2 && evy > 0)
                draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 9.5f,
                                 ((uint32_t)(evy > 16 ? 255 : evy * 255 / 16) << 24) | 0xffffff);
        }
        if(ok && g_obj_ok && !g_is_boss && k_stages[g_stage_idx][1] == '2' && (g_obj_camera[0].flags & 0x1000000))
            draw_water(g_obj_camera[0].water);
        if(g_flash > 0) {
            draw_rect_screen(NULL, PVR_LIST_TR_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 95.0f,
                             ((uint32_t)(g_flash * 15) << 24) | 0xffffff);
            if(!ui_is_paused()) g_flash--;
        }
        { uint64_t t0 = timer_us_gettime64(); if(ok && g_player_ok) ui_draw_tr(); g_dprof[4] += timer_us_gettime64() - t0; }
        if(cutscene_active() && !cutscene_is_fullscreen()) cutscene_draw_list(PVR_LIST_TR_POLY);
        if(ok && g_player_ok && !g_model_ok)
            draw_sprite_world(NULL, PVR_LIST_TR_POLY, g_player.x / 256.0f - 8, g_player.y / 256.0f - 28, 16, 28, 0, 0, 0, 0, 0, 3.0f, 0xc02060ff);
        render_end_list();
        g_prof[2] += timer_us_gettime64() - t_draw0;
        { uint64_t t1 = timer_us_gettime64(); render_end_frame(); g_prof[2] += timer_us_gettime64() - t1; }
        audio_update();
        if(ok && g_demo) {
            int dk = demo_play_frame(&real_in);   /* DemoPlay_Main */
            if(dk != DEMO_RUNNING) {
                demo_stop(); g_demo = 0; g_demo_spawn_x = g_demo_spawn_y = 0;
                if(g_obj_ok) objects_shutdown();
                boss_shutdown(); g_is_boss = 0;
                ui_stage_end();
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
            if(g_obj_ok) objects_shutdown();
            boss_shutdown(); g_is_boss = 0;
            ui_stage_end();   /* title-card / results BGs, navigator, face texture */
            level_free(&lv); ok = 0;
            if(k == UI_EXIT_NEXT_ACT || k == UI_EXIT_TRY_AGAIN)
                r = menus_stage_exit(MENU_EXIT_CLEARED, total, g_dcp_world.lives, g_emeralds, g_dcp_world.frame);
            else if(k == UI_EXIT_QUIT)
                r = menus_stage_exit(MENU_EXIT_QUIT, total, g_dcp_world.lives, g_emeralds, 0);
            else
                r = menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, g_emeralds, 0);
            if(dbg_start && k == UI_EXIT_NEXT_ACT && r.kind != MENU_REQ_STAGE) {
                /* test builds without menus: continue in story order */
                const char *nx = k_stages[(stage + 1) % NUM_STAGES];
                r.kind = MENU_REQ_STAGE; r.zone = nx[1] == 'f' ? 7 : nx[1] - '1';
                r.act = nx[2] - '1'; r.character = g_chara;
                r.mode = g_dcp_world.mode; r.lives = g_dcp_world.lives; r.emeralds = g_emeralds; r.easy = 1;
            }
            if(r.kind == MENU_REQ_STAGE && r.zone == 8 && r.act == 2) {
                g_chara = r.character;
                r = run_extra_zone();
                if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding"))
                    run_cutscene(CS_EXTRA_ENDING, 0, 8, NULL);
                enter_front_end();
            } else if(r.kind == MENU_REQ_STAGE && stage_index(r.zone, r.act) >= 0) {
                g_chara = r.character; load_player_model();
                if(r.mode == UI_MODE_STORY && r.act == 2) run_cutscene(CS_SCRIPT, 0, r.zone, NULL);   /* Game_AdvanceActWithDemo */
                stage = stage_index(r.zone, r.act);
                ok = load_stage(&lv, stage) == 0;
                if(ok && g_player_ok) spawn_player(&lv);
            } else {
                if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "Ending")) run_cutscene(CS_ENDING, 0, 7, NULL);
                else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "ExtraEnding")) run_cutscene(CS_EXTRA_ENDING, 0, 8, NULL);
                else if(r.kind == MENU_REQ_UNAVAILABLE && r.scene && !strcmp(r.scene, "DemoScript4")) run_cutscene(CS_SCRIPT, 4, 6, NULL);
                enter_front_end();
            }
        }
        if(++frame % 120 == 0 && dbg_start) {   /* frame log: test builds only (each serial line costs ms) */
            uint64_t now = timer_ms_gettime64();
            if(t_last) printf("frame %d cam %.0f,%.0f player %ld,%ld spd %d ang %d fps %.1f music %lu B loops %lu rings %d live %lu\n", frame, g_cam_x, g_cam_y, (long)(g_player.x >> 8), (long)(g_player.y >> 8), g_player.vel_x, g_player.angle, 120000.0 / (now - t_last), (unsigned long)audio_stream_bytes(), (unsigned long)audio_stream_loops(), g_obj_ok ? objects_ring_count() : -1, g_obj_ok ? (unsigned long)objects_stats()->live : 0);
            printf("prof ms/frame: objects %.2f player %.2f draw %.2f wait %.2f; worst frame %.2f ms, %d of 120 over 17.5 ms\n", g_prof[0] / 120000.0, g_prof[1] / 120000.0, g_prof[2] / 120000.0, g_prof[3] / 120000.0, g_fmax / 1000.0, g_fslow);
            g_fmax = 0; g_fslow = 0;
            printf("draw ms/frame: bg %.2f layers %.2f objects %.2f ui %.2f\n", g_dprof[0] / 120000.0, g_dprof[1] / 120000.0, g_dprof[3] / 120000.0, g_dprof[4] / 120000.0);
            memset(g_prof, 0, sizeof(g_prof)); memset(g_dprof, 0, sizeof(g_dprof));
            t_last = now;
        }
    }
    return 0;
}
