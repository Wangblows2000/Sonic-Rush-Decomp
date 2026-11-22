/* Loading indicator: the ring item sprite (act__ac_itm_ring, its 8 spin frames)
 * turning in the bottom-right corner of the 640x480 frame while a stage,
 * special stage or scene loads.
 *
 * Loads run on the main thread, and texture uploads use the same store queues
 * as the TA, so the indicator is drawn cooperatively: the engine's loaders call
 * loading_tick() between reads and uploads, and it presents at most one frame
 * per 16 ms. Nothing is drawn while no load is in progress. */
#include <kos.h>
#include <stdlib.h>
#include <string.h>
#include "loading.h"
#include "render.h"

#define RING_DIR "/cd/sprites/act__ac_itm_ring"
#define RING_FRAMES 8        /* images 0..7: the spin cycle */
#define RING_TICKS 4         /* 60 Hz frames per image */
#define RING_SCALE 2.5f      /* DS pixels -> 640x480 (same as the HUD) */
#define MARGIN 24.0f

static int s_active, s_busy, s_ring_ok, s_ring_tried;
static uint64_t s_last_us;
static unsigned s_frame;
static SprAtlas s_ring;
static uint8_t *s_dspr;

static uint8_t *read_all(const char *path)
{
    file_t f = fs_open(path, O_RDONLY);
    uint8_t *b;
    ssize_t n;
    if(f < 0) return NULL;
    n = fs_total(f);
    b = n > 0 ? malloc(n) : NULL;
    if(b && fs_read(f, b, n) != n) { free(b); b = NULL; }
    fs_close(f);
    return b;
}

static void ring_load(void)
{
    s_ring_tried = 1;
    s_dspr = read_all(RING_DIR "/frames.bin");
    if(!s_dspr) return;
    memset(&s_ring, 0, sizeof(s_ring));
    s_ring_ok = spr_atlas_load(&s_ring, RING_DIR, s_dspr, 0) > 0;   /* pages loaded */
    printf("loading: ring sprite %s (%d pages)\n", s_ring_ok ? "ok" : "FAILED", s_ring.n_pages);
}

static void draw_frame(void)
{
    const uint8_t *d = s_dspr;
    render_begin_frame();
    render_begin_list(PVR_LIST_OP_POLY);
    render_end_list();
    render_begin_list(PVR_LIST_PT_POLY);
    render_end_list();
    render_begin_list(PVR_LIST_TR_POLY);
    if(s_ring_ok && d) {
        int np = d[6] | d[7] << 8, img = (s_frame / RING_TICKS) % RING_FRAMES;
        const uint8_t *im = d + 16 + 4 * np + 16 * img;
        int page = im[0], ix = im[2] | im[3] << 8, iy = im[4] | im[5] << 8;
        int iw = im[6] | im[7] << 8, ih = im[8] | im[9] << 8;
        int ox = (int16_t)(im[10] | im[11] << 8), oy = (int16_t)(im[12] | im[13] << 8);
        int bank = spr_atlas_bank(&s_ring, img, -1);
        /* the 16x16 cell anchored at the bottom-right safe corner; each frame keeps
         * its own origin so the ring turns about its centre */
        float cx = 640.0f - MARGIN - 8.0f * RING_SCALE, by = 480.0f - MARGIN;
        if(page < s_ring.n_pages && s_ring.tex[page].ptr)
            draw_rect_screen_pal(&s_ring.tex[page], bank, PVR_LIST_TR_POLY, cx - ox * RING_SCALE, by - oy * RING_SCALE,
                                 iw * RING_SCALE, ih * RING_SCALE, ix, iy, ix + iw, iy + ih, 50.0f, 0xffffffff);
    }
    render_end_list();
    render_end_frame();
}

void loading_begin(void)
{
    if(s_active) return;
    s_active = 1;
    s_last_us = 0;
    if(!s_ring_tried) {
        s_busy = 1;          /* the ring's own load must not tick */
        ring_load();
        s_busy = 0;
    }
    loading_tick();
}

void loading_tick(void)
{
    uint64_t now;
    if(!s_active || s_busy) return;
    now = timer_us_gettime64();
    if(s_last_us && now - s_last_us < 16000) return;
    s_busy = 1;
    s_last_us = now;
    s_frame++;
    draw_frame();
    s_busy = 0;
}

void loading_end(void)
{
    s_active = 0;
}

int loading_active(void) { return s_active; }

/* fs_read in 32 KB pieces with a loading_tick between them, so long reads keep
 * the indicator turning */
ssize_t loading_read(file_t f, void *buf, size_t n)
{
    size_t done = 0;
    while(done < n) {
        size_t c = n - done > 32768 ? 32768 : n - done;
        ssize_t r = fs_read(f, (uint8_t *)buf + done, c);
        if(r <= 0) return done ? (ssize_t)done : r;
        done += (size_t)r;
        loading_tick();
    }
    return (ssize_t)done;
}
