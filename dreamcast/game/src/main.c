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
#include "../objects/objects.h"
#include <stdlib.h>

static const char *k_stages[] = { "z11", "z12", "z21", "z22", "z31", "z32", "z41", "z42",
                                  "z51", "z52", "z61", "z62", "z71", "z72" };
#define NUM_STAGES (sizeof(k_stages) / sizeof(k_stages[0]))

static DcPlayer g_player;
/* Player 3D model (son.nsbmd from the owner's ROM, converted by nsbmd2dc.py). */
static dcm_model_t g_pmodel;
static dca_set_t g_panims, g_pvis;
static uint8_t g_animmap[0x68];       /* Player_SetAnimation table ov000 0x02368bdc */
static int g_model_ok;
static float g_anim_frame;
static int g_anim_cur = -1;
static dcm_local_t g_locals[DCM_MAX_JOINTS];
static dcm_mtx_t g_palette[DCM_MAX_PALETTE];
static uint8_t g_visbits[DCM_MAX_JOINTS];
static rlvl_map_t g_map;
static int g_player_ok;
static int g_obj_ok;

static void *load_file(const char *path, uint32_t *size)
{
    file_t f = fs_open(path, O_RDONLY);
    void *b;
    if(f == FILEHND_INVALID) return NULL;
    *size = fs_total(f);
    b = malloc(*size);
    if(b && fs_read(f, b, *size) != (ssize_t)*size) { free(b); b = NULL; }
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

static void spawn_player(Level *lv)
{
    uint32_t i;
    const rlvl_object_t *obj = (const rlvl_object_t *)(lv->data + lv->hdr->sec[RLVL_SEC_OBJECTS].offset);
    int sx = 128, sy = 128;
    for(i = 0; i < lv->hdr->num_objects; i++)
        if(obj[i].type == 114) { sx = obj[i].x; sy = obj[i].y; break; }   /* PlayerStart */
    rlvl_bind(&g_map, lv->data);
    dcp_bind_map(&g_map);
    memset(&g_player, 0, sizeof(g_player));
    dcp_player_create(&g_player, 0, 0, sx, sy);
    objects_shutdown();
    g_obj_ok = objects_init(&g_map, lv->data) == 0;
    if(g_obj_ok) { objects_add_player(&g_player); objects_set_view_count(1); objects_respawn_view(); }
    printf("objects: %s\n", g_obj_ok ? "ok" : "FAILED");
    g_cam_x = sx - VIEW_W / 2; g_cam_y = sy - VIEW_H * 0.6f;
    printf("player spawned at %d,%d\n", sx, sy);
}

static void load_player_model(void)
{
    uint32_t sz;
    void *m = load_file("/cd/player/son.dcm", &sz);
    void *a, *v, *t;
    uint32_t asz, vsz, tsz;
    a = load_file("/cd/player/son.dca", &asz);
    v = load_file("/cd/player/son.dcv", &vsz);
    t = load_file("/cd/player/animmap.bin", &tsz);
    if(m && a && t && tsz == sizeof(g_animmap) && dcm_model_load(&g_pmodel, m, sz, 1) == 0 &&
       dca_set_load(&g_panims, a, asz) == 0) {
        memcpy(g_animmap, t, sizeof(g_animmap));
        if(v) dca_set_load(&g_pvis, v, vsz);
        g_model_ok = 1;
    }
    printf("player model: %s (vram free %lu)\n", g_model_ok ? "ok" : "MISSING", (unsigned long)render_vram_free());
}

static void draw_player_model(pvr_list_t list)
{
    dcm_camera_t cam;
    dcm_mtx_t mdl;
    dcm_lights_t lights;
    dcm_draw_opts_t opts;
    float tx = g_player.x / 256.0f - g_cam_x, ty = -(g_player.y / 256.0f - g_cam_y);
    int a = g_animmap[(uint16_t)g_player.anim < sizeof(g_animmap) ? g_player.anim : 0];
    int left = g_player.pf & DCP_PF_FACE_L;
    if(a != g_anim_cur) { g_anim_cur = a; g_anim_frame = 0; }
    dca_sample(&g_panims, a, g_anim_frame, &g_pmodel, g_locals);
    dcm_pose(&g_pmodel, g_locals, g_palette);
    if(g_pvis.hdr) dca_visibility(&g_pvis, 0, 0, &g_pmodel, g_visbits);
    dcm_camera_stage(&cam);
    dcm_camera_stage_offaxis(&cam, &tx, &ty);
    dcm_stage_model_matrix(&mdl, 0x34cc / 4096.0f, left ? 0xC000 : 0x4000, g_player.angle, tx, ty, 0);
    dcm_lights_stage(&lights, left);
    opts.list = list; opts.pt_list = PVR_LIST_PT_POLY; opts.alpha_scale = 255;
    dcm_draw(&g_pmodel, g_palette, &mdl, &cam, g_pvis.hdr ? g_visbits : NULL, &lights, &opts);
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
    char dir[64];
    uint64_t t0 = timer_ms_gettime64();
    int r;
    snprintf(dir, sizeof(dir), "/cd/stages/%s", k_stages[idx]);
    r = level_load(lv, dir);
    start_zone_music(k_stages[idx][1]);
    printf("stage %s: load %d in %llu ms, %dx%d px, %d block pages, vram used %lu, vram free %lu\n",
           k_stages[idx], r, (unsigned long long)(timer_ms_gettime64() - t0),
           r == 0 ? level_width_px(lv) : 0, r == 0 ? level_height_px(lv) : 0, lv->num_pages,
           (unsigned long)lv->vram_used, (unsigned long)render_vram_free());
    return r;
}

int main(int argc, char **argv)
{
    Level lv;
    InputState in = {0};
    int stage = 0, frame = 0, ok;
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
        load_player_model();
    }
    ok = load_stage(&lv, stage) == 0;
    g_cam_x = 0; g_cam_y = 0;
    if(ok && g_player_ok) spawn_player(&lv);

    for(;;) {
        input_poll(&in);
        if(in.pressed & IN_START) break;
        if(in.pressed & (IN_TRICK | IN_ALT)) {
            level_free(&lv);
            stage = (stage + ((in.pressed & IN_TRICK) ? 1 : NUM_STAGES - 1)) % NUM_STAGES;
            ok = load_stage(&lv, stage) == 0;
            g_cam_x = g_cam_y = 0;
            if(ok && g_player_ok) spawn_player(&lv);
        }
        if(ok && g_player_ok) {
            float px, py, tx, ty;
            if(g_obj_ok) {
                objects_set_view(0, (s32)(g_cam_x * 256.0f), (s32)(g_cam_y * 256.0f));
                objects_frame_begin();
            }
            dcp_player_frame(&g_player, ds_pad(in.held), ds_pad(in.pressed), ds_pad(in.released));
            if(g_obj_ok) objects_update(&g_player);
            /* Continuous camera (CAMERA.md): the DS screen-sized window follows the
             * player with damping and a modest velocity look-ahead; render-only. */
            px = g_player.x / 256.0f; py = g_player.y / 256.0f;
            tx = px - VIEW_W / 2 + g_player.vel_x / 256.0f * 8.0f;
            ty = py - VIEW_H * 0.55f;
            g_cam_x += (tx - g_cam_x) * 0.25f;
            g_cam_y += (ty - g_cam_y) * 0.18f;
        } else {
            g_cam_x += in.ax / 16.0f + ((in.held & IN_RIGHT) ? 4 : 0) - ((in.held & IN_LEFT) ? 4 : 0);
            g_cam_y += in.ay / 16.0f + ((in.held & IN_DOWN) ? 4 : 0) - ((in.held & IN_UP) ? 4 : 0);
        }
        if(ok) {
            if(g_cam_x > level_width_px(&lv) - VIEW_W) g_cam_x = level_width_px(&lv) - VIEW_W;
            if(g_cam_y > level_height_px(&lv) - VIEW_H) g_cam_y = level_height_px(&lv) - VIEW_H;
        }
        if(g_cam_x < 0) g_cam_x = 0;
        if(g_cam_y < 0) g_cam_y = 0;

        render_begin_frame();
        if(ok && g_player_ok && g_model_ok) {
            g_anim_frame += 1.0f;
            if(g_anim_cur >= 0 && g_anim_frame >= g_panims.anims[g_anim_cur].num_frames)
                g_anim_frame = 0;   /* loop; one-shot ends are not signalled yet */
        }
        /* PVR lists must be opened in order OP, PT, TR (each once per frame).
         * Depth (1/w style, bigger = nearer): bg 0.0005 < layer B 0.001 < player model (~0.006) < layer A 10. */
        render_begin_list(PVR_LIST_OP_POLY);
        if(ok) level_draw_bg(&lv, 0.25f, 0.125f, 0.0005f);
        if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_OP_POLY);
        render_end_list();
        render_begin_list(PVR_LIST_PT_POLY);
        if(ok) { level_draw_layer(&lv, 1, PVR_LIST_PT_POLY, 0.001f); level_draw_layer(&lv, 0, PVR_LIST_PT_POLY, 10.0f); }
        if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_PT_POLY);
        if(ok && g_obj_ok) obj_draw(PVR_LIST_PT_POLY);
        render_end_list();
        render_begin_list(PVR_LIST_TR_POLY);
        if(ok && g_player_ok && g_model_ok) draw_player_model(PVR_LIST_TR_POLY);
        if(ok && g_player_ok && !g_model_ok)
            draw_sprite_world(NULL, PVR_LIST_TR_POLY, g_player.x / 256.0f - 8, g_player.y / 256.0f - 28, 16, 28, 0, 0, 0, 0, 0, 3.0f, 0xc02060ff);
        render_end_list();
        render_end_frame();
        audio_update();

        if(++frame % 120 == 0) {
            uint64_t now = timer_ms_gettime64();
            if(t_last) printf("frame %d cam %.0f,%.0f player %ld,%ld spd %d ang %d fps %.1f music %lu B loops %lu rings %d live %lu\n", frame, g_cam_x, g_cam_y, (long)(g_player.x >> 8), (long)(g_player.y >> 8), g_player.vel_x, g_player.angle, 120000.0 / (now - t_last), (unsigned long)audio_stream_bytes(), (unsigned long)audio_stream_loops(), g_obj_ok ? objects_ring_count() : -1, g_obj_ok ? (unsigned long)objects_stats()->live : 0);
            t_last = now;
        }
    }
    return 0;
}
