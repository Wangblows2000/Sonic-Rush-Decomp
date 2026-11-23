/*
 * Host test of the PVR palette RAM allocator and its write queue (engine/render.c):
 * a write to a row that the frame being built or the frame being rendered drew with is
 * queued; the next render_begin_frame hands it to the render-done interrupt, which writes it
 * when the render in progress ends (before the next render starts), without a CPU wait;
 * other writes go straight to the palette RAM.  pal_write_rows (zone palettes) the same.
 * Build: make -C dreamcast/game/engine/test
 */
#include <stdio.h>
#include <string.h>
#include <kos.h>
#include "../render.h"

static uint16_t s_ram[1024];          /* palette RAM as the PVR reads it */
static int s_render_busy;             /* a frame is being rendered */
static int s_bad_writes;              /* writes while the PVR renders a frame that uses the row */
static int s_waits;
static uint8_t s_row_in_flight[64];   /* rows the frame in flight (being rendered) drew with */
static int g_fail, g_checks;
#define CHECK(c, ...) do { g_checks++; if (!(c)) { g_fail++; printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n"); } } while (0)

void pvr_set_pal_entry(uint32_t i, uint32_t v) {
    if (s_render_busy && s_row_in_flight[i / 16] && s_ram[i] != (uint16_t)v)
        s_bad_writes++;
    s_ram[i] = (uint16_t)v;
}
void pvr_set_pal_format(int f) { (void)f; }
int pvr_wait_ready(void) { return 0; }
int pvr_wait_render_done(void) { s_waits++; s_render_busy = 0; return 0; }
int pvr_scene_begin(void) { return 0; }
int pvr_scene_finish(void) { return 0; }
int pvr_list_begin(pvr_list_t l) { (void)l; return 0; }
int pvr_list_finish(void) { return 0; }
void pvr_poly_cxt_txr(pvr_poly_cxt_t *c, pvr_list_t l, int f, int w, int h, pvr_ptr_t p, int filt) { (void)c; (void)l; (void)f; (void)w; (void)h; (void)p; (void)filt; }
void pvr_poly_cxt_col(pvr_poly_cxt_t *c, pvr_list_t l) { (void)c; (void)l; }
void pvr_poly_compile(pvr_poly_hdr_t *h, pvr_poly_cxt_t *c) { (void)h; (void)c; }
int pvr_prim(void *d, int size) { (void)d; (void)size; return 0; }
pvr_ptr_t pvr_mem_malloc(size_t n) { return malloc(n); }
void pvr_mem_free(pvr_ptr_t p) { free(p); }
uint32_t pvr_mem_available(void) { return 1u << 20; }
void pvr_txr_load(const void *src, pvr_ptr_t dst, uint32_t n) { memcpy(dst, src, n); }
file_t fs_open(const char *p, int mode) { (void)p; (void)mode; return FILEHND_INVALID; }
void fs_close(file_t f) { (void)f; }
ssize_t fs_read(file_t f, void *b, size_t n) { (void)f; (void)b; (void)n; return -1; }
size_t fs_total(file_t f) { (void)f; return 0; }
int irq_disable(void) { return 0; }
void irq_restore(int old) { (void)old; }
static asic_evt_handler s_hdl;   /* the render-done handler installed (KOS's, then render.c's wrapper) */
static int s_kos_calls;
static void kos_rdone(uint32_t code, void *data) { (void)code; (void)data; s_kos_calls++; }
asic_evt_handler_entry_t asic_evt_set_handler(uint16_t code, asic_evt_handler handler, void *data) {
    asic_evt_handler_entry_t old;
    (void)code; (void)data;
    old.hdl = s_hdl; old.data = NULL;
    s_hdl = handler;
    return old;
}
/* the PVR finishes the frame in flight: the interrupt */
static void render_done(void) {
    if (!s_render_busy) return;
    s_render_busy = 0;
    if (s_hdl) s_hdl(ASIC_EVT_PVR_RENDERDONE_TSP, NULL);
}
int pak_load(const char *path, void **out, uint32_t *size) { (void)path; (void)out; (void)size; return 0; }
ssize_t loading_read(file_t f, void *buf, size_t n) { (void)f; (void)buf; (void)n; return -1; }

/* the frame being built draws with these rows; at the frame end it is handed to the PVR */
static uint8_t s_row_used[64];
static void frame_begin(void) { render_begin_frame(); memset(s_row_used, 0, sizeof(s_row_used)); }
/* the PVR starts rendering a submitted frame once the one before is done (render_done) */
static void render_done(void);
static void frame_end(void) {
    render_done();   /* the previous frame has rendered by the time this one is submitted */
    render_end_frame();
    memcpy(s_row_in_flight, s_row_used, sizeof(s_row_in_flight));
    s_render_busy = 1;   /* rendered while the next frame is built */
}
static void pal(uint16_t *c, uint16_t base) { int i; for (i = 0; i < 16; i++) c[i] = (uint16_t)(base + i); }
static int row_matches(int r, const uint16_t *c, uint16_t mask) {
    int k;
    for (k = 1; k < 16; k++)
        if ((mask >> k & 1) && s_ram[r * 16 + k] != c[k]) return 0;
    return 1;
}

int main(void) {
    uint16_t a[16], b[16], c[16];
    int ra, rb, rc, i, n, rows[64];
    PalRef ref = PALREF_INIT, ref2 = PALREF_INIT;
    pal(a, 0x100); pal(b, 0x200); pal(c, 0x300);
    s_hdl = kos_rdone;   /* KOS's pvr_int_handler, installed by pvr_init */

    /* 1: a new row is written at once (nobody drew with it) */
    frame_begin();
    ra = pal4_get(a, 0xfffe);
    CHECK(ra >= 0 && row_matches(ra, a, 0xfffe), "static row written at once (row %d)", ra);
    s_row_used[ra] = 1;
    frame_end();

    /* 2: the row is given back while the PVR renders the frame that drew with it, and a
     * different palette is requested: it gets another (cold) row, written at once */
    frame_begin();
    pal4_put(ra);
    rb = pal4_get(b, 0xfffe);
    CHECK(rb >= 0 && rb != ra, "released row %d not retaken while in flight (got %d)", ra, rb);
    CHECK(row_matches(rb, b, 0xfffe), "cold row written at once");
    CHECK(row_matches(ra, a, 0xfffe), "in-flight row keeps its colours");
    s_row_used[rb] = 1;
    frame_end();

    /* 3: fill the palette RAM, then give back the row in flight and ask for one more:
     * the only free row is hot, so its colours wait for the next frame start */
    frame_begin();
    n = 0;
    for (i = 0; i < 64; i++) {
        uint16_t x[16];
        pal(x, (uint16_t)(0x1000 + i * 0x20));
        rows[n] = pal4_get(x, 0xfffe);
        if (rows[n] < 0) break;
        s_row_used[rows[n]] = 1;
        n++;
    }
    CHECK(n > 0 && n < 64, "palette RAM filled after %d rows", n);
    frame_end();
    frame_begin();
    pal4_put(rows[0]);
    {
        uint32_t q0 = pal_queued_writes();
        int waits0 = s_waits;
        rc = pal4_get(c, 0xfffe);
        CHECK(rc == rows[0], "the hot row is the only free one (%d / %d)", rc, rows[0]);
        CHECK(pal_queued_writes() > q0, "hot row writes queued (%u)", (unsigned)(pal_queued_writes() - q0));
        CHECK(!row_matches(rc, c, 0xfffe), "hot row not written during the frame");
        frame_end();
        frame_begin();   /* flush: handed to the render-done interrupt, no wait */
        CHECK(s_waits == waits0, "no render wait at the frame start (%d)", s_waits - waits0);
        CHECK(!row_matches(rc, c, 0xfffe), "queued colours not written while the frame in flight renders");
        {
            int k0 = s_kos_calls;
            render_done();
            CHECK(s_kos_calls == k0 + 1, "KOS's render-done handler still called");
        }
        CHECK(row_matches(rc, c, 0xfffe), "queued colours written at the render-done interrupt");
        frame_end();
        render_done();
    }

    /* 3b: pal_write_rows (zone palette animation) while a frame renders: deferred to its
     * render-done interrupt; with no render in progress: at once */
    {
        uint16_t p8[256];
        int waits0 = s_waits;
        for (i = 0; i < 256; i++) p8[i] = (uint16_t)(0x7000 + i);
        frame_begin();
        s_row_used[rows[1]] = 1;
        frame_end();   /* in flight, drew with rows[1] */
        frame_begin();
        pal_write_rows(rows[1] / 16, (uint16_t)(1u << (rows[1] % 16)), p8);
        CHECK(!row_matches(rows[1], p8 + (rows[1] % 16) * 16, 0xfffe), "zone palette not written during the render");
        render_done();
        CHECK(row_matches(rows[1], p8 + (rows[1] % 16) * 16, 0xfffe), "zone palette written at the render-done interrupt");
        frame_end();
        render_done();   /* nothing in flight now */
        for (i = 0; i < 256; i++) p8[i] = (uint16_t)(0x6000 + i);
        pal_write_rows(rows[1] / 16, (uint16_t)(1u << (rows[1] % 16)), p8);
        CHECK(row_matches(rows[1], p8 + (rows[1] % 16) * 16, 0xfffe), "zone palette written at once with no render in progress");
        CHECK(s_waits == waits0, "no render waits (%d)", s_waits - waits0);
    }

    /* 4: lazy (UI) rows: idle for 3 frames, then taken back and rewritten at once */
    for (i = 1; i < n; i++) pal4_put(rows[i]);
    pal4_put(rb);
    pal4_put(rc);
    for (i = 0; i < 3; i++) { frame_begin(); frame_end(); render_done(); }
    frame_begin();
    ra = pal4_use(&ref, a, 0xfffe);
    CHECK(ra >= 0 && row_matches(ra, a, 0xfffe), "UI row written at once");
    s_row_used[ra] = 1;
    frame_end();
    frame_begin();
    rb = pal4_use(&ref2, b, 0xfffe);
    CHECK(rb >= 0 && rb != ra, "second UI palette gets its own row");
    CHECK(pal4_use(&ref, a, 0xfffe) == ra, "UI row kept while used");
    frame_end();

    CHECK(s_bad_writes == 0, "%d palette writes hit rows the frame in flight drew with", s_bad_writes);
    printf("palette: %d checks, %d failed (queued writes %u, render waits %d)\n", g_checks, g_fail, (unsigned)pal_queued_writes(), s_waits);
    return g_fail != 0;
}
