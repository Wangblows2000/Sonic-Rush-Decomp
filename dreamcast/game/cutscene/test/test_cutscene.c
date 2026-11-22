/* Host test: runs every cutscene headless to its end and checks the executed
 * script commands, the timings and the page counts against the original data
 * (the #BTK / #BPD files and the ROM tables of csdata.bin, parsed here
 * independently of the module). Optional frame dumps (PPM) of the single
 * 640x480 composition.
 *   test_cutscene <cutscene data> <sprites/bac> <models> [dump dir]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cs_internal.h"
#include "cs_raster.h"

static int g_fail, g_checks;
static const char *g_dump;
#define CHECK(c, ...) do { g_checks++; if (!(c)) { g_fail++; printf("FAIL: " __VA_ARGS__); printf("\n"); } } while (0)

static u8 *load(const char *path, u32 *size) { return (u8 *)cs_load_file(path, size); }
static s32 rd32(const u8 *p) { return (s32)(p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24); }

/* frame number of every log line */
static int g_line_frame[8192];
typedef int (*InputFn)(int frame);
static int press_a(int f) { return (f % 6) == 0 ? IN_JUMP : 0; }
static int no_input(int f) { (void)f; return 0; }
static int press_start_200(int f) { return f == 200 ? IN_START : 0; }

static int run(CsKind k, int id, int chara, InputFn in, int max, const char *dumpname, const int *dumpf, int ndump) {
    InputState st;
    int f, prev = 0, d = 0, done = 0, n;
    cutscene_log_clear();
    if (cutscene_start(k, id, chara) != 0)
        return -1;
    for (f = 0; f < max; f++) {
        u32 b = in(f);
        memset(&st, 0, sizeof(st));
        st.held = b;
        st.pressed = b;
        done = cutscene_run_frame(&st);
        cutscene_draw();
        n = cutscene_log_count();
        while (prev < n && prev < 8192)
            g_line_frame[prev++] = f;
        if (g_dump && dumpname && d < ndump && f == dumpf[d]) {
            char path[256];
            fb_render();
            snprintf(path, sizeof(path), "%s/%s_%05d.ppm", g_dump, dumpname, f);
            fb_save(path);
            d++;
        }
        if (done)
            break;
    }
    return done ? f + 1 : -max;
}

static int find_line(const char *prefix, int from) {
    int i;
    for (i = from; i < cutscene_log_count(); i++)
        if (!strncmp(cutscene_log_line(i), prefix, strlen(prefix)))
            return i;
    return -1;
}
static int count_lines(const char *prefix) {
    int i, n = 0;
    for (i = 0; i < cutscene_log_count(); i++)
        if (!strncmp(cutscene_log_line(i), prefix, strlen(prefix)))
            n++;
    return n;
}

static int has_missing_msg(void) {
    int i;
    for (i = 0; i < cutscene_log_count(); i++)
        if (strstr(cutscene_log_line(i), "missing"))
            return 1;
    return 0;
}
/* --------------------------------------------------------------- TalkDemo */
static void test_talk(const char *dir) {
    const char *names = (const char *)cs_sec("td_name", NULL);
    const s16 *bgm = (const s16 *)cs_sec("td_bgm", NULL);
    const s32 *snd = (const s32 *)cs_sec("td_snd", NULL);
    int id, ok = 0;
    for (id = 1; id < 0x3a; id++) {
        char path[256], pfx[48];
        u8 *b;
        u32 size;
        s32 pages, bgmi, mode0;
        int frames, map = id >= 0x15 && id <= 0x2f;
        static const int dumpf[2] = {150, 400};
        if (!names[32 * id])
            continue;
        snprintf(path, sizeof(path), "%s/talk/%s_eng.btk", dir, names + 32 * id);
        b = load(path, &size);
        CHECK(b && !memcmp(b, "#BTK", 4), "talk %d script %s", id, path);
        if (!b)
            continue;
        pages = rd32(b + 0x10);
        bgmi = (s16)rd32(b + 0x18);
        mode0 = rd32(b + 0x1c + 4);
        CHECK(0x1c + 0x34 * pages == (s32)size, "talk %d size %u pages %d", id, size, pages);
        snprintf(pfx, sizeof(pfx), "talk_%02d", id);
        frames = run(map ? CS_TALK_MAP : CS_TALK, id, id >= 0x28 && id <= 0x2f ? 1 : 0, press_a, 30000,
                     (id == 0x15 || id == 1 || id == 0x37 || id == 0x16 || id == 0x28 || id == 0x14 || id == 5 || id == 0x1a) ? pfx : NULL, dumpf, 2);
        CHECK(frames > 0, "talk %d did not end", id);
        CHECK(!has_missing_msg(), "talk %d: a message page or sprite is missing", id);
        snprintf(pfx, sizeof(pfx), "talk %d page", id);
        if (mode0 == 3)
            CHECK(count_lines("talk ") >= 1 && find_line("talk", 0) >= 0 &&
                  count_lines(pfx) == 0, "talk %d full image", id);
        else
            CHECK(count_lines(pfx) == pages, "talk %d pages %d run %d", id, pages, count_lines(pfx));
        /* the scene ends: window band 20 + 15 frames open, 20 close, dim 30 */
        CHECK(frames >= 35, "talk %d frames %d", id, frames);
        if (bgmi >= 3 && bgmi <= 9 && snd && ((u32)snd[4 * id + 3] & 0x80000000u)) {
            char want[32];
            snprintf(want, sizeof(want), "bgm s%02d", bgm[bgmi] == 22 ? 8 : bgm[bgmi]);
            CHECK(find_line(want, 0) >= 0, "talk %d %s", id, want);
        }
        /* page start: the first page begins after the band opened (20 + 15 frames) */
        {
            int l = find_line(mode0 == 3 ? "talk" : pfx, 1);
            if (l >= 0 && mode0 != 3)
                CHECK(g_line_frame[l] >= 34 && g_line_frame[l] <= 40, "talk %d first page at frame %d", id,
                      g_line_frame[l]);
        }
        free(b);
        ok++;
    }
    printf("talk: %d scripts run\n", ok);
    /* the other five languages: each script runs with its own page count and message pack */
    {
        static const char *const sfx[6] = {"jpn", "eng", "fra", "deu", "ita", "spa"};
        int lang, n = 0;
        for (lang = 0; lang < 6; lang++) {
            if (lang == 1)
                continue;
            g_cs_story.language = lang;
            for (id = 1; id < 0x3a; id++) {
                char path[256], pfx[48];
                u8 *b;
                u32 size;
                s32 pages, mode0;
                int frames, map = id >= 0x15 && id <= 0x2f;
                if (!names[32 * id])
                    continue;
                snprintf(path, sizeof(path), "%s/talk/%s_%s.btk", dir, names + 32 * id, sfx[lang]);
                b = load(path, &size);
                CHECK(b != NULL, "talk %d %s", id, path);
                if (!b)
                    continue;
                pages = rd32(b + 0x10);
                mode0 = rd32(b + 0x1c + 4);
                free(b);
                frames = run(map ? CS_TALK_MAP : CS_TALK, id, id >= 0x28 && id <= 0x2f ? 1 : 0, press_a, 30000, NULL,
                             NULL, 0);
                snprintf(pfx, sizeof(pfx), "talk %d page", id);
                CHECK(frames > 0 && (mode0 == 3 || count_lines(pfx) == pages) && (mode0 == 3 || count_lines("talk msg ") > 0) && !has_missing_msg(),
                      "talk %d lang %d pages %d run %d", id, lang, pages, count_lines(pfx));
                n++;
            }
        }
        g_cs_story.language = 1;
        printf("talk: %d scripts run in the other languages\n", n);
    }
    /* zone-map story steps (ZoneMap_Unk_ov001_022e1aac / ZoneMap_SelectTalkDemo) */
    {
        static const int ss[9] = {0, 6, 9, 12, 15, 19, 24, 28, 31}, bs[8] = {1, 4, 7, 15, 19, 23, 27, 30};
        int i;
        for (i = 0; i < 9; i++)
            CHECK(cutscene_zonemap_talk_id(0, ss[i]) == 0x15 + i, "sonic step %d", ss[i]);
        for (i = 0; i < 8; i++)
            CHECK(cutscene_zonemap_talk_id(1, bs[i]) == 0x28 + i, "blaze step %d", bs[i]);
    }
}

/* --------------------------------------------------------------- PlayerDemo */
static void test_player(const char *dir) {
    const s32 *idx = (const s32 *)cs_sec("pd_bpd", NULL);
    const char *names = (const char *)cs_sec("pd_bpdn", NULL);
    const s32 *demo = (const s32 *)cs_sec("pd_demo", NULL);
    int id, n = 0;
    for (id = 1; id < 0x2c; id++) {
        char path[256], pfx[48], dn[32];
        u8 *b;
        u32 size;
        int frames, i, ncmd, from = 0, scenes = 0;
        static const int dumpf[3] = {60, 200, 420};
        if (!idx[id] || !names[24 * idx[id]])
            continue;
        snprintf(path, sizeof(path), "%s/pldm/%s.bpd", dir, names + 24 * idx[id]);
        b = load(path, &size);
        CHECK(b && !memcmp(b, "#BPD", 4), "pdemo %d %s", id, path);
        if (!b)
            continue;
        ncmd = rd32(b + 0xc);
        snprintf(dn, sizeof(dn), "pdemo_%02d", id);
        g_cs_story.save_flags = 0;
        frames = run(CS_PLAYER, id, 0, press_a, 40000, (id == 1 || id == 21 || id == 30 || id == 40) ? dn : NULL,
                     dumpf, 3);
        CHECK(frames > 0, "pdemo %d did not end", id);
        /* every command, in order, at its time (relative to its scene) */
        for (i = 0; i < ncmd; i++) {
            const u8 *c = b + 0x14 + 16 * i;
            int l;
            snprintf(pfx, sizeof(pfx), "pdemo %d cmd %d t %d ", id, i, rd32(c + 4));
            l = find_line(pfx, from);
            CHECK(l >= 0, "pdemo %d command %d (t %d op %d) not run", id, i, rd32(c + 4), rd32(c + 8));
            if (l >= 0) {
                char want[64];
                snprintf(want, sizeof(want), "%d", rd32(c + 12));
                CHECK(strstr(cutscene_log_line(l), want) != NULL, "pdemo %d cmd %d arg", id, i);
                from = l;
            }
            if (rd32(c + 8) == 15)
                scenes++;
        }
        snprintf(pfx, sizeof(pfx), "pdemo %d scene ", id);
        CHECK(count_lines(pfx) == (demo ? demo[3 * id + 1] : 0), "pdemo %d scenes loaded %d of %d", id,
              count_lines(pfx), demo ? demo[3 * id + 1] : 0);
        /* the scene timeline (PlayerDemo_Main: time++ then PlayerDemo_State_RunScript):
         * a command runs on frame F + max(t, 1) of its scene, F = the NextScene frame
         * (0 for the first scene), and never before the previous command */
        {
            int F = 0, prevf = 0;
            for (i = 0; i < ncmd; i++) {
                const u8 *c = b + 0x14 + 16 * i;
                int l, want, t = rd32(c + 4);
                snprintf(pfx, sizeof(pfx), "pdemo %d cmd %d t ", id, i);
                l = find_line(pfx, 0);
                if (l < 0)
                    continue;
                want = F + (t > 1 ? t : 1);
                if (want < prevf)
                    want = prevf;
                CHECK(g_line_frame[l] == want, "pdemo %d cmd %d frame %d want %d (t %d)", id, i, g_line_frame[l], want, t);
                prevf = g_line_frame[l];
                if (rd32(c + 8) == 15)
                    F = g_line_frame[l];
            }
        }
        free(b);
        n++;
        (void)scenes;
    }
    printf("player demo: %d scripts run\n", n);
}

/* --------------------------------------------------------------- TransformDemo */
static void test_transform(void) {
    int id;
    static const int dumpf[3] = {20, 100, 200};
    for (id = 0; id < 5; id++) {
        char dn[32];
        int frames, l;
        snprintf(dn, sizeof(dn), "transform_%d", id);
        g_cs_story.stage_new = 1;
        frames = run(CS_TRANSFORM, id, 0, no_input, 5000, dn, dumpf, 3);
        CHECK(frames > 0, "transform %d did not end", id);
        l = find_line("se 11/", 0);
        CHECK(l >= 0 && g_line_frame[l] == 0x1e - 1, "transform %d SE_Go_to at frame %d", id, l >= 0 ? g_line_frame[l] : -1);
        if (id < 4) {
            l = find_line("voice 11/", 0);
            CHECK(l >= 0 && g_line_frame[l] == (id < 2 ? 0x78 : 0xa0) - 1, "transform %d voice at %d", id,
                  l >= 0 ? g_line_frame[l] : -1);
        } else {
            l = find_line("voice 11/2", 0);
            CHECK(l >= 0 && g_line_frame[l] == 0xb4 - 1, "transform 4 voice 2 at %d", l >= 0 ? g_line_frame[l] : -1);
            l = find_line("voice 11/3", 0);
            CHECK(l >= 0 && g_line_frame[l] == 0x12c - 1, "transform 4 voice 3 at %d", l >= 0 ? g_line_frame[l] : -1);
        }
        printf("transform %d: %d frames\n", id, frames);
    }
}

/* --------------------------------------------------------------- DemoScript */
static void test_script(void) {
    const u8 *t = (const u8 *)cs_sec("ds_table", NULL);
    int chara, zone, nw, mode, runs = 0;
    for (mode = 0; mode < 2; mode++)
        for (chara = 0; chara < 2; chara++)
            for (zone = 0; zone < 8; zone++)
                for (nw = 0; nw < 2; nw++) {
                    int id = (mode ? 0x20 : 0) + (chara ? 0x10 : 0) + (nw ? 0 : 8) + zone, i, n, frames, l = 0;
                    const u8 *e = t + 32 * id;
                    char pfx[64];
                    g_cs_story.zone = zone;
                    g_cs_story.stage_new = nw;
                    g_cs_story.save_flags = 0;
                    frames = run(CS_SCRIPT, mode, chara, press_a, 60000, NULL, NULL, 0);
                    CHECK(frames > 0, "script %#x did not end", id);
                    n = rd32(e);
                    for (i = 0; i < n; i++) {
                        static const char *const op[4] = {"", "PlayerDemo", "TransformDemo", "TalkDemo"};
                        snprintf(pfx, sizeof(pfx), "script %#x op %d %s %d", id, i, op[e[16 + 4 * i] & 3], e[17 + 4 * i]);
                        l = find_line(pfx, l);
                        CHECK(l >= 0, "%s", pfx);
                        if (l < 0)
                            l = 0;
                    }
                    CHECK(cutscene_result()->next == (CsNext)rd32(e + 4), "script %#x next %d want %d", id,
                          cutscene_result()->next, rd32(e + 4));
                    runs++;
                }
    {
        static const int modes[3] = {3, 4, 2};
        for (mode = 0; mode < 3; mode++)
            for (chara = 0; chara < 2; chara++) {
                int frames;
                g_cs_story.zone = 8;
                frames = run(CS_SCRIPT, modes[mode], chara, press_a, 60000, NULL, NULL, 0);
                CHECK(frames > 0, "script mode %d chara %d did not end", modes[mode], chara);
                CHECK(cutscene_result()->next != CS_NEXT_NONE, "script mode %d next", modes[mode]);
                if (modes[mode] == 4)
                    CHECK(cutscene_result()->progress == (chara ? 26 : 24), "script 4 progress %d",
                          cutscene_result()->progress);
                runs++;
            }
    }
    printf("demo script: %d runs\n", runs);
}

/* --------------------------------------------------------------- Ending / StaffRoll */
static void test_ending(void) {
    int c, frames;
    static const int dumpf[6] = {100, 600, 1100, 1700, 2300, 2900};
    static const int dumps[4] = {300, 3000, 9800, 10200};
    for (c = 0; c < 2; c++) {
        char dn[32];
        snprintf(dn, sizeof(dn), "ending_%d", c);
        g_cs_story.sonic_progress = g_cs_story.blaze_progress = 0;
        g_cs_story.sonic_emeralds = 0x7f;
        frames = run(CS_ENDING, 0, c, no_input, 40000, dn, dumpf, 6);
        CHECK(frames > 0, "ending %d did not end", c);
        CHECK(count_lines("ending picture") == 4, "ending %d pictures %d", c, count_lines("ending picture"));
        /* data_ov001_02315f44: 4 picture changes + Sonic 5 / Blaze 4 timed captions */
        CHECK(count_lines("ending caption") == (c == 0 ? 9 : 8), "ending %d captions %d", c, count_lines("ending caption"));
        CHECK(find_line("scene StaffRoll", 0) >= 0, "ending %d -> staff roll", c);
        CHECK(cutscene_result()->next == CS_NEXT_UNLOCK_NOTICES, "ending %d next %d", c, cutscene_result()->next);
        CHECK(c == 0 ? cutscene_result()->set_sonic_progress == 0x1a : cutscene_result()->set_blaze_progress == 0x1c,
              "staff roll progress");
        printf("ending %d + staff roll: %d frames\n", c, frames);
    }
    for (c = 0; c < 3; c++) {
        char dn[32];
        snprintf(dn, sizeof(dn), "staffroll_%d", c);
        frames = run(CS_STAFFROLL, c, c == 1, no_input, 40000, dn, dumps, 4);
        CHECK(frames > 0, "staff roll %d did not end", c);
        CHECK(count_lines("staffroll credit") >= 3, "staff roll %d credits %d", c, count_lines("staffroll credit"));
        CHECK(!has_missing_msg(), "staff roll %d: a credit map is missing", c);
        if (c == 2)
            CHECK(count_lines("staffroll extra picture") == 4 && cutscene_result()->game_cleared, "extra pictures");
        printf("staff roll %d: %d frames\n", c, frames);
    }
    /* skip (START once skippable, after frame 0x1e): fade 0x3c frames, State_Skip ->
     * SkipSonic/SkipBlaze/SkipExtra -> SkipFadeIn -> the closing banner / final picture */
    for (c = 0; c < 3; c++) {
        int full = c == 2 ? 12736 : 10000;
        g_cs_story.sonic_progress = 0x1a;
        g_cs_story.blaze_progress = 0x1c;
        g_cs_story.save_flags = 0x1000;
        frames = run(CS_STAFFROLL, c, c == 1, press_start_200, 40000, NULL, NULL, 0);
        CHECK(frames > 0 && frames < full - 3000 && find_line("staffroll skip", 0) >= 0, "staff roll %d skip (%d frames)",
              c, frames);
        CHECK(c == 2 ? cutscene_result()->game_cleared : (c == 0 ? cutscene_result()->set_sonic_progress == 0x1a
                                                                  : cutscene_result()->set_blaze_progress == 0x1c),
              "staff roll %d skip result", c);
        printf("staff roll %d skipped: %d frames\n", c, frames);
    }
    /* every language reads the same credit maps (the EUR table names ed_staff03_eng) */
    for (c = 0; c < 6; c++) {
        g_cs_story.language = c;
        frames = run(CS_STAFFROLL, 0, 0, press_start_200, 40000, NULL, NULL, 0);
        CHECK(frames > 0 && !has_missing_msg(), "staff roll language %d", c);
    }
    g_cs_story.language = 1;
    for (c = 0; c < 2; c++) {
        frames = run(CS_ENDING, 0, c, press_start_200, 40000, NULL, NULL, 0);
        CHECK(frames > 0 && find_line("ending skip", 0) >= 0 && find_line("scene StaffRoll", 0) >= 0 &&
                  count_lines("ending picture") < 4,
              "ending %d skip", c);
        printf("ending %d skipped at 200: %d frames\n", c, frames);
    }
    g_cs_story.sonic_progress = g_cs_story.blaze_progress = 0;
    g_cs_story.save_flags = 0;
    g_cs_story.sonic_emeralds = 0x3f;
    frames = run(CS_POST_STAFFROLL, 0, 0, press_a, 20000, NULL, NULL, 0);
    CHECK(frames > 0 && find_line("talk 55 start", 0) >= 0, "post staff roll talk 0x37");
    frames = run(CS_EXTRA_ENDING, 0, 0, press_a, 60000, NULL, NULL, 0);
    CHECK(frames > 0 && find_line("pdemo 42 start", 0) >= 0 && find_line("scene ExtraStaffRoll", 0) >= 0,
          "extra ending");
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <cutscene data> <sprites> <models> [dump]\n", argv[0]);
        return 2;
    }
    g_dump = argc > 4 ? argv[4] : NULL;
    cutscene_set_paths(argv[1], argv[2], argv[3], "/nonexistent");
    if (cs_data_load() != 0)
        return 1;
    test_talk(argv[1]);
    test_player(argv[1]);
    test_transform();
    test_script();
    test_ending();
    cutscene_shutdown();
    printf("%d/%d checks passed\n", g_checks - g_fail, g_checks);
    return g_fail ? 1 : 0;
}
