/* Sonic Rush DC — native game entry. Stage viewer/bring-up stage:
 * loads a converted stage package from /cd/stages/<id>, renders background,
 * back layer, front layer with the 2.5x DS-framing camera. */
#include <kos.h>
#include <stdio.h>
#include "../engine/render.h"
#include "../engine/input.h"
#include "../engine/level.h"
#include "../engine/audio.h"

static const char *k_stages[] = { "z11", "z12", "z21", "z22", "z31", "z32", "z41", "z42",
                                  "z51", "z52", "z61", "z62", "z71", "z72" };
#define NUM_STAGES (sizeof(k_stages) / sizeof(k_stages[0]))

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
    ok = load_stage(&lv, stage) == 0;
    g_cam_x = 0; g_cam_y = 0;

    for(;;) {
        input_poll(&in);
        if(in.pressed & IN_START) break;
        if(in.pressed & (IN_TRICK | IN_ALT)) {
            level_free(&lv);
            stage = (stage + ((in.pressed & IN_TRICK) ? 1 : NUM_STAGES - 1)) % NUM_STAGES;
            ok = load_stage(&lv, stage) == 0;
            g_cam_x = g_cam_y = 0;
        }
        g_cam_x += in.ax / 16.0f + ((in.held & IN_RIGHT) ? 4 : 0) - ((in.held & IN_LEFT) ? 4 : 0);
        g_cam_y += in.ay / 16.0f + ((in.held & IN_DOWN) ? 4 : 0) - ((in.held & IN_UP) ? 4 : 0);
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
        render_end_frame();
        audio_update();

        if(++frame % 120 == 0) {
            uint64_t now = timer_ms_gettime64();
            if(t_last) printf("frame %d cam %.0f,%.0f fps %.1f\n", frame, g_cam_x, g_cam_y, 120000.0 / (now - t_last));
            t_last = now;
        }
    }
    return 0;
}
