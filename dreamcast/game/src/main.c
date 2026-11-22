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
#include <stdlib.h>

static const char *k_stages[] = { "z11", "z12", "z21", "z22", "z31", "z32", "z41", "z42",
                                  "z51", "z52", "z61", "z62", "z71", "z72" };
#define NUM_STAGES (sizeof(k_stages) / sizeof(k_stages[0]))

static DcPlayer g_player;
static rlvl_map_t g_map;
static int g_player_ok;

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
    g_cam_x = sx - VIEW_W / 2; g_cam_y = sy - VIEW_H * 0.6f;
    printf("player spawned at %d,%d\n", sx, sy);
}

static int load_stage(Level *lv, int idx)
{
    char dir[64];
    uint64_t t0 = timer_ms_gettime64();
    int r;
    snprintf(dir, sizeof(dir), "/cd/stages/%s", k_stages[idx]);
    r = level_load(lv, dir);
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
            dcp_player_frame(&g_player, ds_pad(in.held), ds_pad(in.pressed), ds_pad(in.released));
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
        render_begin_list(PVR_LIST_OP_POLY);
        if(ok) level_draw_bg(&lv, 0.25f, 0.125f, 1.0f);
        render_end_list();
        render_begin_list(PVR_LIST_PT_POLY);
        if(ok) { level_draw_layer(&lv, 1, PVR_LIST_PT_POLY, 2.0f); level_draw_layer(&lv, 0, PVR_LIST_PT_POLY, 4.0f); }
        render_end_list();
        if(ok && g_player_ok) {
            /* Placeholder player marker (the 3D player model renderer is not integrated yet). */
            float px = g_player.x / 256.0f, py = g_player.y / 256.0f;
            render_begin_list(PVR_LIST_TR_POLY);
            draw_sprite_world(NULL, PVR_LIST_TR_POLY, px - 8, py - 28, 16, 28, 0, 0, 0, 0, 0, 3.0f, 0xc02060ff);
            draw_sprite_world(NULL, PVR_LIST_TR_POLY, px - 3, py - 24, 6, 6, 0, 0, 0, 0, 0, 3.1f, 0xffffffff);
            render_end_list();
        }
        render_end_frame();
        audio_update();

        if(++frame % 120 == 0) {
            uint64_t now = timer_ms_gettime64();
            if(t_last) printf("frame %d cam %.0f,%.0f player %ld,%ld spd %d ang %d fps %.1f\n", frame, g_cam_x, g_cam_y, (long)(g_player.x >> 8), (long)(g_player.y >> 8), g_player.vel_x, g_player.angle, 120000.0 / (now - t_last));
            t_last = now;
        }
    }
    return 0;
}
