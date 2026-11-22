/*
 * frameprof.c - frame pacing and memory probe for test builds (frameprof.h).
 * Everything is recorded in RAM; printing happens only at a scene change, or every N seconds in a
 * non-gameplay scene, so the serial log does not disturb the frames it measures.
 */
#include "frameprof.h"

#include <stdio.h>
#include <string.h>

#ifdef _arch_dreamcast
#include <kos.h>
#include <malloc.h>
#include <dc/sound/sound.h>

#include "render.h"

#define BIN_US 500                 /* histogram bin: 0.5 ms */
#define BINS 100                   /* 0 .. 50 ms, last bin = longer */
#define WORST 8

typedef struct {
    uint32_t frame, vbl;   /* vblanks the frame took (1 = on time) */
    uint32_t interval, work;
    uint32_t sec[FP_SECTIONS];
} fp_rec_t;

static int s_on;
static uint32_t s_period_us = 10000000;
static char s_name[32];
static int s_gameplay;
static uint64_t s_t0, s_first, s_last, s_work_end;
static uint32_t s_frames, s_nframe;
static uint32_t s_hist[BINS], s_whist[BINS];
static uint32_t s_over1, s_over2, s_over3;   /* intervals > 17.5, > 34.2, > 50.9 ms */
static uint32_t s_vbl[5];                     /* frames that took 0, 1, 2, 3, 4+ vblanks */
static int s_vbl_last = -1, s_vbl_first;
static uint64_t s_sum_work;
static fp_rec_t s_worst[WORST];
static fp_rec_t s_cur;
static struct {
    uint32_t heap_used, heap_free, brk, vram, snd;
    int pal8, pal4;
} s_mem0;

static void mem_now(uint32_t *heap_used, uint32_t *heap_free, uint32_t *brk, uint32_t *vram, uint32_t *snd, int *p8,
                    int *p4) {
    struct mallinfo mi = mallinfo();
    *heap_used = (uint32_t)mi.uordblks;
    *heap_free = (uint32_t)mi.fordblks;
    *brk = (uint32_t)(uintptr_t)sbrk(0) - 0x8c000000u;
    *vram = (uint32_t)pvr_mem_available();
    *snd = (uint32_t)snd_mem_available();
    pal_rows_used(p8, p4);
}

static void reset(void) {
    s_frames = 0;
    memset(s_hist, 0, sizeof(s_hist));
    memset(s_whist, 0, sizeof(s_whist));
    s_over1 = s_over2 = s_over3 = 0;
    memset(s_vbl, 0, sizeof(s_vbl));
    s_vbl_last = -1;
    s_sum_work = 0;
    memset(s_worst, 0, sizeof(s_worst));
    s_t0 = timer_us_gettime64();
    s_first = 0;
    s_last = 0;
    s_work_end = 0;
    memset(&s_cur, 0, sizeof(s_cur));
    mem_now(&s_mem0.heap_used, &s_mem0.heap_free, &s_mem0.brk, &s_mem0.vram, &s_mem0.snd, &s_mem0.pal8,
            &s_mem0.pal4);
}

static uint32_t pct(const uint32_t *h, uint32_t n, uint32_t p) {
    uint32_t want = (n * p + 99) / 100, acc = 0, i;
    for (i = 0; i < BINS; i++) {
        acc += h[i];
        if (acc >= want)
            return (i + 1) * BIN_US;
    }
    return BINS * BIN_US;
}

static void summary(const char *why) {
    uint64_t now = timer_us_gettime64();
    uint32_t hu, hf, brk, vram, snd, i, el = s_first && s_last > s_first ? (uint32_t)(s_last - s_first) : 0;
    int p8, p4;
    if (!s_frames) {
        reset();
        return;
    }
    (void)now;
    printf("frameprof %s [%s]: %lu frames in %lu vblanks = %.2f fps (60 Hz: %.2f); vblanks per frame 1:%lu 2:%lu 3:%lu "
           "4+:%lu; timer %.2f s; interval p50 %.1f p95 %.1f p99 %.1f ms, "
           ">17.5 ms %lu, >34.2 ms %lu, >50.9 ms %lu; work p50 %.1f p95 %.1f p99 %.1f ms, mean %.2f ms\n",
           s_name, why, (unsigned long)s_frames, (unsigned long)(s_vbl_last - s_vbl_first),
           s_frames * 59.94 / (s_vbl_last > s_vbl_first ? s_vbl_last - s_vbl_first : 1),
           s_frames * 60.0 / (s_vbl_last > s_vbl_first ? s_vbl_last - s_vbl_first : 1), (unsigned long)s_vbl[1],
           (unsigned long)s_vbl[2], (unsigned long)s_vbl[3], (unsigned long)s_vbl[4], el / 1e6,
           pct(s_hist, s_frames, 50) / 1000.0, pct(s_hist, s_frames, 95) / 1000.0, pct(s_hist, s_frames, 99) / 1000.0,
           (unsigned long)s_over1, (unsigned long)s_over2, (unsigned long)s_over3, pct(s_whist, s_frames, 50) / 1000.0,
           pct(s_whist, s_frames, 95) / 1000.0, pct(s_whist, s_frames, 99) / 1000.0,
           (double)s_sum_work / s_frames / 1000.0);
    printf("frameprof %s hist (ms:frames):", s_name);
    for (i = 0; i < BINS; i++)
        if (s_hist[i])
            printf(" %.1f:%lu", (i + 1) * BIN_US / 1000.0, (unsigned long)s_hist[i]);
    printf("\n");
    for (i = 0; i < WORST && s_worst[i].interval; i++) {
        const fp_rec_t *w = &s_worst[i];
        printf("frameprof %s worst %lu: frame %lu, %lu vblanks, interval %.2f work %.2f ms (objects %.2f player %.2f "
               "draw %.2f [bg %.2f layers %.2f model %.2f obj %.2f ui %.2f boss %.2f omodel %.2f] wait %.2f)\n",
               s_name, (unsigned long)i, (unsigned long)w->frame, (unsigned long)w->vbl, w->interval / 1000.0,
               w->work / 1000.0, w->sec[FP_OBJECTS] / 1000.0, w->sec[FP_PLAYER] / 1000.0, w->sec[FP_DRAW] / 1000.0,
               w->sec[FP_D_BG] / 1000.0, w->sec[FP_D_LAYERS] / 1000.0, w->sec[FP_D_PMODEL] / 1000.0,
               w->sec[FP_D_OBJ] / 1000.0, w->sec[FP_D_UI] / 1000.0, w->sec[FP_D_BOSS] / 1000.0, w->sec[FP_D_OMODEL] / 1000.0,
               w->sec[FP_WAIT] / 1000.0);
    }
    mem_now(&hu, &hf, &brk, &vram, &snd, &p8, &p4);
    printf("frameprof %s mem: heap used %lu KB (%+ld), heap free %lu KB, brk %lu KB (%+ld), vram free %lu KB (%+ld), "
           "pal rows %d+%d (%+d), sound ram free %lu KB (%+ld)\n",
           s_name, (unsigned long)(hu >> 10), (long)((int32_t)(hu - s_mem0.heap_used) / 1024), (unsigned long)(hf >> 10),
           (unsigned long)(brk >> 10), (long)((int32_t)(brk - s_mem0.brk) / 1024), (unsigned long)(vram >> 10),
           (long)((int32_t)(vram - s_mem0.vram) / 1024), p8, p4, p8 + p4 - s_mem0.pal8 - s_mem0.pal4,
           (unsigned long)(snd >> 10), (long)((int32_t)(snd - s_mem0.snd) / 1024));
    reset();
}

void frameprof_init(void) {
    file_t f = fs_open("/cd/debug/frameprof.txt", O_RDONLY);
    char b[16];
    ssize_t n;
    if (f == FILEHND_INVALID)
        return;
    n = fs_read(f, b, sizeof(b) - 1);
    fs_close(f);
    if (n > 0) {
        int s;
        b[n] = 0;
        if (sscanf(b, "%d", &s) == 1 && s > 0)
            s_period_us = (uint32_t)s * 1000000u;
    }
    s_on = 1;
    strcpy(s_name, "boot");
    reset();
    printf("frameprof: on, summaries per scene, every %lu s outside gameplay\n", (unsigned long)(s_period_us / 1000000));
}

int frameprof_enabled(void) { return s_on; }

const char *frameprof_scene_name(int *gameplay) {
    if (gameplay)
        *gameplay = s_gameplay;
    return s_name;
}

void frameprof_scene(const char *name, int gameplay) {
    if (!s_on)
        return;
    summary("scene end");
    snprintf(s_name, sizeof(s_name), "%s", name);
    s_gameplay = gameplay;
    reset();
}

void frameprof_flush(void) {
    if (s_on)
        summary("flush");
}

void frameprof_work_end(void) {
    if (s_on)
        s_work_end = timer_us_gettime64();
}

void frameprof_add(int section, uint32_t us) {
    if (s_on && section >= 0 && section < FP_SECTIONS)
        s_cur.sec[section] += us;
}

void frameprof_frame(void) {
    uint64_t now;
    if (!s_on)
        return;
    now = timer_us_gettime64();
    s_nframe++;
    {
        int vb = pvr_get_vbl_count();
        if (s_vbl_last >= 0) {
            uint32_t d = (uint32_t)(vb - s_vbl_last);
            s_vbl[d < 4 ? d : 4]++;
            s_cur.vbl = d;
        } else {
            s_vbl_first = vb;
        }
        s_vbl_last = vb;
    }
    if (s_last) {
        uint32_t iv = (uint32_t)(now - s_last), wk = s_work_end > s_last ? (uint32_t)(s_work_end - s_last) : iv, i;
        wk = wk > s_cur.sec[FP_WAIT] ? wk - s_cur.sec[FP_WAIT] : 0;   /* blocking waits inside the frame are not work */
        uint32_t b = iv / BIN_US, wb = wk / BIN_US;
        s_hist[b < BINS ? b : BINS - 1]++;
        s_whist[wb < BINS ? wb : BINS - 1]++;
        s_frames++;
        s_sum_work += wk;
        if (iv > 17500)
            s_over1++;
        if (iv > 34200)
            s_over2++;
        if (iv > 50900)
            s_over3++;
        s_cur.interval = iv;
        s_cur.work = wk;
        for (i = 0; i < WORST; i++)
            if (s_cur.vbl > s_worst[i].vbl || (s_cur.vbl == s_worst[i].vbl && iv > s_worst[i].interval)) {
                memmove(&s_worst[i + 1], &s_worst[i], sizeof(s_worst[0]) * (WORST - 1 - i));
                s_worst[i] = s_cur;
                break;
            }
    }
    memset(&s_cur, 0, sizeof(s_cur));
    s_cur.frame = s_nframe;
    if (!s_first)
        s_first = now;
    s_last = now;
    if (!s_gameplay && now - s_t0 >= s_period_us)
        summary("period");
}

#else /* host builds: nothing */
void frameprof_init(void) {}
int frameprof_enabled(void) { return 0; }
const char *frameprof_scene_name(int *gameplay) {
    if (gameplay)
        *gameplay = 0;
    return "";
}
void frameprof_scene(const char *name, int gameplay) {
    (void)name;
    (void)gameplay;
}
void frameprof_flush(void) {}
void frameprof_work_end(void) {}
void frameprof_add(int section, uint32_t us) {
    (void)section;
    (void)us;
}
void frameprof_frame(void) {}
#endif
