/*
 * Host test of the attract-demo module (DemoInput / DemoPlay) on the owner's
 * converted data (build/dreamcast/demo, tools/demodata.py).
 *
 * The expected pad of every frame is computed here straight from the key files
 * (records of {keys, frames}), independently of the module's state machine, and
 * compared with what demo_input_frame hands to the player (held / pressed /
 * released = gPad cur / trg / rel). Usage: test_demo <build/dreamcast/demo>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../demo.h"
#include "../../player/dcp_player.h"

DcpWorld g_dcp_world;
DcpHooks g_dcp_hooks;
static int g_fail, g_checks;
#define CHECK(c, ...)                                     \
    do {                                                  \
        g_checks++;                                       \
        if (!(c)) {                                       \
            g_fail++;                                     \
            printf("  FAIL %s:%d: ", __FILE__, __LINE__); \
            printf(__VA_ARGS__);                          \
            printf("\n");                                 \
        }                                                 \
    } while (0)

/* --------------------------------------------------- UI stubs (ui_state.c) */
static int s_fade_kind = -1, s_fade_speed, s_fade_left, s_fade_calls, s_label;
void ui_fade(s32 kind, s32 speed) {
    s_fade_kind = kind;
    s_fade_speed = speed;
    s_fade_calls++;
    /* BrightnessFade: 0x1000 / step frames, then done */
    s_fade_left = 0x1000 / speed + 1;
}
int ui_fade_done(void) {
    if (s_fade_left > 0)
        s_fade_left--;
    return s_fade_left == 0;
}
void ui_demo_label(int visible) { s_label = visible; }

/* ------------------------------------------------------------ reference */
static unsigned char *load(const char *path, long *n) {
    FILE *f = fopen(path, "rb");
    unsigned char *b;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *n = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*n);
    if (fread(b, 1, *n, f) != (size_t)*n)
        exit(2);
    fclose(f);
    return b;
}
/* keys of recorded frame k (0-based); -1 past the end record */
static int ref_keys(const unsigned char *kf, long n, int k) {
    long o;
    int t = 0;
    for (o = 4; o + 4 <= n; o += 4) {
        int keys = kf[o] | kf[o + 1] << 8, dur = kf[o + 2];
        if (dur == 0)
            return k == t ? keys : -1;   /* the end record is applied once */
        if (k < t + dur)
            return keys;
        t += dur;
    }
    return -1;
}

static void reset_world(void) {
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    s_fade_kind = -1;
    s_fade_left = 0;
    s_fade_calls = 0;
}

static void test_playback(const char *dir, int id, int lead) {
    DemoInfo di;
    char path[256];
    long n;
    unsigned char *kf;
    int k, prev = 0, bad = 0, frames, end_frame = -1, first_bad = -1;
    DemoPad p;
    InputState real;
    reset_world();
    demo_set_lead_frames(lead);
    CHECK(demo_start(id, &di) == 0, "demo_start(%d)", id);
    snprintf(path, sizeof path, "%s/key/key0%d.bin", dir, id == 0 ? 4 : id);
    kf = load(path, &n);
    CHECK(kf != NULL, "key file %s", path);
    if (!kf)
        return;
    CHECK(di.seed == (unsigned)(kf[0] | kf[1] << 8 | kf[2] << 16 | (unsigned)kf[3] << 24), "seed");
    CHECK(g_dcp_world.rand_seed == di.seed, "gRandSeed = key file word 0");
    CHECK(g_dcp_world.mode == 3, "gGameState.unk10 = 3");
    CHECK((g_dcp_world.flags & 0xc) == 0xc, "flags 4 | 8 (start point)");
    CHECK(g_dcp_world.check_x == di.start_x && g_dcp_world.check_y == di.start_y, "start point -> check_x/y");
    g_dcp_world.rand_seed = 0x1234;   /* the stage set-up may not change it */
    demo_after_stage_load();
    CHECK(g_dcp_world.rand_seed == di.seed, "seed restored after the stage load");
    CHECK(demo_timer() == (unsigned)lead, "DemoPlay timer counts the lead frames");
    /* frame 0: main() read the hardware pad before DemoInput_Main set gSystemFlags 0x40
     * (as on the DS), so gPad.prev is the real pad; with lead frames it is the record */
    prev = lead ? ref_keys(kf, n, lead - 1) : (0x001 | 0x010);
    (void)k;
    /* a real pad is held all along: it must never reach the player while playing */
    memset(&real, 0, sizeof real);
    real.held = IN_JUMP | IN_RIGHT;
    for (frames = 0; frames < 4000; frames++) {
        int want, rk = frames + lead;
        demo_input_frame(&real, &p);
        want = ref_keys(kf, n, rk);
        if (want < 0) {
            end_frame = frames;
            break;
        }
        if (p.held != want || p.pressed != (want & ~prev) || p.released != (prev & ~want) || !p.recorded) {
            if (first_bad < 0)
                first_bad = frames;
            bad++;
        }
        prev = want;
    }
    printf("[demo %d lead %d] zone %d act %d chara %d start (%d,%d) limit %d: %d records, %d frames played, %d "
           "mismatches\n",
           id, lead, di.zone, di.act, di.character, di.start_x, di.start_y, di.limit, di.records, end_frame, bad);
    CHECK(bad == 0, "demo %d: frame-exact pad (first bad frame %d)", id, first_bad);
    /* the frame after the end record: gSystemFlags & 0x40 was still set when main()
     * would have read the hardware, so gPad stays as the end record left it */
    CHECK(p.held == prev && !p.recorded && demo_input_mode() == 1, "end record: pad unchanged that frame");
    demo_input_frame(&real, &p);
    CHECK(p.held == (0x001 | 0x010) && p.pressed == ((0x011) & ~prev) && !p.recorded,
          "after the end: the real pad (DS Pad_Update)");
    demo_stop();
    CHECK((g_dcp_world.flags & 0xc) == 0, "demo_stop clears flags 4 / 8");
    free(kf);
}

/* DemoPlay_Main: fade-in once the timer runs, limit -> SEGA logo, key -> title */
static void test_play(void) {
    DemoInfo di;
    DemoPad p;
    InputState none, a;
    int f, r = 0, fade_in_frame = -1, label_on = -1, label_off = -1;
    memset(&none, 0, sizeof none);
    memset(&a, 0, sizeof a);
    a.held = IN_JUMP;
    /* timeout */
    reset_world();
    demo_set_lead_frames(0);
    demo_start(1, &di);
    demo_after_stage_load();
    for (f = 0; f < 5000; f++) {
        demo_input_frame(&none, &p);
        if (f >= 1)
            g_dcp_world.frame++;   /* Stage_Main: the timer runs from the first stage frame */
        r = demo_play_frame(&none);
        if (fade_in_frame < 0 && s_fade_kind == 3)
            fade_in_frame = f;
        if (label_on < 0 && s_label)
            label_on = f;
        if (label_on >= 0 && label_off < 0 && !s_label)
            label_off = f;
        if (r != DEMO_RUNNING)
            break;
    }
    printf("[play] fade-in at frame %d (speed %#x), label on %d..%d, timeout exit %d at frame %d (limit %d)\n",
           fade_in_frame, s_fade_speed, label_on, label_off, r, f, di.limit);
    CHECK(fade_in_frame == 1, "BrightnessFade_Create(3, 0x40) once gGameState.unk20 != 0");
    CHECK(label_on == 0x40 && label_off == 0x80, "label blinks with timer & 0x40");
    CHECK(r == DEMO_EXIT_SEGA_LOGO && s_fade_kind == 5 && s_fade_speed == 0x100, "limit -> fade 5/0x100 -> SEGA");
    CHECK(f == di.limit + 1 + 0x1000 / 0x100 + 1, "exit frame = limit + fade");
    CHECK(demo_input_mode() == 5, "gDemoInputMode = 5 at the exit");
    demo_stop();
    /* skip: a key before the timer runs is ignored, then A skips to the title */
    reset_world();
    demo_start(3, &di);
    demo_after_stage_load();
    demo_input_frame(&a, &p);
    r = demo_play_frame(&a);
    CHECK(r == DEMO_RUNNING && s_fade_calls == 0, "skip ignored while gGameState.unk20 == 0");
    for (f = 1; f < 200; f++) {
        demo_input_frame(&a, &p);
        g_dcp_world.frame++;
        r = demo_play_frame(f >= 10 ? &a : &none);
        if (r != DEMO_RUNNING)
            break;
    }
    printf("[skip] exit %d at frame %d, fade %d/%#x\n", r, f, s_fade_kind, s_fade_speed);
    CHECK(r == DEMO_EXIT_TITLE && s_fade_kind == 5 && s_fade_speed == 0x400, "A -> fade 5/0x400 -> title");
    demo_stop();
    CHECK(demo_start(7, &di) != 0, "bad demo index rejected");
}

int main(int argc, char **argv) {
    int i;
    if (argc < 2) {
        printf("usage: %s <build/dreamcast/demo>\n", argv[0]);
        return 2;
    }
    demo_set_path(argv[1]);
    for (i = 0; i < 4; i++)
        test_playback(argv[1], i, 0);
    test_playback(argv[1], 1, 37);   /* lead frames: the first stage frame gets record frame 37 */
    test_play();
    printf("%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
