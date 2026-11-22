/*
 * Host test of the PVR palette RAM allocator and its write queue (engine/render.c):
 * a write to a row that the frame being built or the frame being rendered drew with is
 * queued and done at the next render_begin_frame, after the render in progress; other
 * writes go straight to the palette RAM.  Build: make -C dreamcast/game/engine/test
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
int pak_load(const char *path, void **out, uint32_t *size) { (void)path; (void)out; (void)size; return 0; }
ssize_t loading_read(file_t f, void *buf, size_t n) { (void)f; (void)buf; (void)n; return -1; }

/* the frame being built draws with these rows; at the frame end it is handed to the PVR */
static uint8_t s_row_used[64];
static void frame_begin(void) { render_begin_frame(); memset(s_row_used, 0, sizeof(s_row_used)); }
static void frame_end(void) {
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
        frame_begin();   /* flush: waits for the render, then writes */
        CHECK(s_waits == waits0 + 1, "one render wait at the frame start (%d)", s_waits - waits0);
        CHECK(row_matches(rc, c, 0xfffe), "queued colours written at the frame start");
        frame_end();
    }

    /* 4: lazy (UI) rows: idle for 3 frames, then taken back and rewritten at once */
    for (i = 1; i < n; i++) pal4_put(rows[i]);
    pal4_put(rb);
    pal4_put(rc);
    for (i = 0; i < 3; i++) { frame_begin(); frame_end(); }
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
