/*
 * test_special.c - host tests of the native special stage on the converted ROM
 * data (build/dreamcast/special): the original logic runs headless, driven
 * through the controller mapping (sp_input.c) exactly as on the Dreamcast.
 *
 *   test_special <build/dreamcast/special> [ppm-dir]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../sp_internal.h"

/* engine/input.c (KOS) defaults */
int g_input_deadzone = 32;
int g_input_dir_threshold = 64;

static int g_fail, g_pass;
#define CHECK(c, ...)                                   \
    do {                                                \
        if (c) {                                        \
            g_pass++;                                   \
            printf("  ok   ");                          \
        } else {                                        \
            g_fail++;                                   \
            printf("  FAIL ");                          \
        }                                               \
        printf(__VA_ARGS__);                            \
        printf("\n");                                   \
    } while (0)

static InputState in_none(void) {
    InputState i;
    memset(&i, 0, sizeof(i));
    return i;
}
static InputState in_press(u32 b) {
    InputState i = in_none();
    i.held = i.pressed = b;
    return i;
}
static InputState in_stick(int ax, int ay) {
    InputState i = in_none();
    i.ax = ax;
    i.ay = ay;
    return i;
}

static const char *s_ppm_dir;
static void dump_ppm(const char *name) {
    char path[256];
    FILE *f;
    if (!s_ppm_dir || !g_sp_host_fb)
        return;
    special_draw();
    snprintf(path, sizeof(path), "%s/%s.ppm", s_ppm_dir, name);
    f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6 640 480 255\n");
    fwrite(g_sp_host_fb, 1, 640 * 480 * 3, f);
    fclose(f);
}

/* run the intro: press A at the "touch the screen" prompt */
static int run_intro(void) {
    int f, waited = 0;
    InputState n = in_none();
    for (f = 0; f < 600 && gSpecialStageWork->taskC == 0; f++) {
        if (gSpecialStageWork->timer == 0x5c && waited < 30) {
            waited++;
            special_run_frame(&n);
            continue;
        }
        if (gSpecialStageWork->timer == 0x5c) {
            InputState a = in_press(IN_JUMP);
            special_run_frame(&a);
        } else
            special_run_frame(&n);
    }
    return waited;
}

/* the gimmick the autopilot heads for: the nearest ring ahead, else nothing */
static SpObj *next_ring(void) {
    SpObjList *l = *gSpecialStageWork->unk1C;
    SpObj *o, *best = NULL;
    for (o = l->head.next; o != &l->tail; o = o->next) {
        if (o->unk60 & 9)
            continue;
        if ((o->unk68 == 0 || o->unk68 == 1) && o->pos.z < -0x6000 && o->pos.z > -0x60000 && o->unk78)
            if (!best || o->pos.z > best->pos.z)
                best = o;
    }
    return best;
}
static SpObj *find_type(int lo, int hi, s32 zmin, s32 zmax) {
    SpObjList *l = *gSpecialStageWork->unk1C;
    SpObj *o;
    for (o = l->head.next; o != &l->tail; o = o->next)
        if (!(o->unk60 & 1) && o->unk68 >= lo && o->unk68 <= hi && o->pos.z > zmin && o->pos.z < zmax)
            return o;
    return NULL;
}

/* stick toward pipe angle a (0 = bottom) */
static InputState steer_to(u16 a) {
    s16 s = SIN(a), c = COS(a);
    return in_stick((int)(s * 120 / 4096), (int)(c * 120 / 4096));
}

typedef struct {
    int result, frames, rings_at_cp, cp_outcome, max_rings, hurt;
} RunInfo;

/* autopilot: collect rings; optional: force the quota at the checkpoint (force_quota) */
static RunInfo run_stage(int stage, int autopilot, int force_quota, int max_frames) {
    RunInfo ri;
    int f, r = 0;
    memset(&ri, 0, sizeof(ri));
    ri.cp_outcome = -1;
    special_set_emerald_flags(0);
    if (special_init(stage, 0) != 0) {
        ri.result = -1;
        return ri;
    }
    run_intro();
    for (f = 0; f < max_frames; f++) {
        InputState i = in_none();
        SpObj *p = SpecialStage_GetPlayer();
        int cp_before = g_sp_trace.sfx[0x10][1];
        if (force_quota && p->unk148 < SpecialStageHud_Unk_ov001_02314368())
            p->unk148 = SpecialStageHud_Unk_ov001_02314368();
        if (autopilot && p->unk228 == 2) {
            SpObj *ring = next_ring();
            if (ring)
                i = steer_to(ring->unk16);
        }
        if (autopilot && p->unk228 == 0xa && p->unk128) {
            /* trick: flick toward the next circle (alternate with neutral) */
            SpTrickSeq *e = SpecialStageTrick_GetSequence(p->unk150);
            int cur = p->unk14C;
            if (cur >= 0 && cur < e->count && (f % 4) < 2) {
                int px = cur ? e->pts[cur - 1].x : 128, py = cur ? e->pts[cur - 1].y : 96;
                float dx = e->pts[cur].x - px, dy = e->pts[cur].y - py, m = sqrtf(dx * dx + dy * dy);
                if (m > 0)
                    i = in_stick((int)(dx * 120 / m), (int)(dy * 120 / m));
            }
        }
        if (p->unk148 > ri.max_rings)
            ri.max_rings = p->unk148;
        r = special_run_frame(&i);
        if (g_sp_trace.sfx[0x10][1] != cp_before && ri.cp_outcome < 0) {
            /* the checkpoint trigger (SE_Check_point) ran this frame: SpecialStageHud_UpdateCheckpoint case 1 */
            ri.rings_at_cp = p->unk148;
            ri.cp_outcome = gSpecialStageHud->spr1.anim == 3;
        }
        if (r)
            break;
    }
    ri.result = r;
    ri.frames = f;
    ri.hurt = g_sp_trace.hurt;
    return ri;
}

int main(int argc, char **argv) {
    int s, i;
    SpecialResult res;
    if (argc < 2) {
        fprintf(stderr, "usage: %s <build/dreamcast/special> [ppm-dir]\n", argv[0]);
        return 2;
    }
    special_set_path(argv[1]);
    if (argc > 2) {
        s_ppm_dir = argv[2];
        g_sp_host_fb = (u8 *)malloc(640 * 480 * 3);
    }

    printf("[init]\n");
    CHECK(special_init(0, 1) == -2, "Blaze has no special stage (character 1 refused)");
    CHECK(special_init(0, 0) == 0, "stage 1 loads (spdata.bin, SST, SSD, models, sprites, BGs)");
    CHECK(gSpecialStagePlayerResources->mdl[0].loaded && gSpecialStagePlayerResources->bca.loaded, "player model + animations");
    CHECK(gSpecialStageMap->res_all[0].loaded && gSpecialStageMap->res[3].loaded, "map + sky models");
    CHECK(SpecialStage_GetRingQuota() == 100 && SpecialStageHud_Unk_ov001_02314368() == 200, "stage 1 quotas 100 / 200 (SST header)");
    CHECK(g_sp_trace.music_last == 0x1f, "intro music SEQ_SS_END (Sound_PlaySeq(0, 0x1f))");
    CHECK(SpecialStage_PipeXToAngle(0x1e000) == 0x4000 && SpecialStage_PipeXToAngle(0) == 0, "PipeXToAngle walls/bottom");
    CHECK(SpecialStage_TouchToPipeAngle(128, 145) == 0 && SpecialStage_TouchToPipeAngle(28, 45) == -0x4000,
          "touch below the pipe centre = bottom, level left = left wall");

    printf("[intro]\n");
    {
        InputState n = in_none();
        for (i = 0; i < 0x5c + 40; i++)
            special_run_frame(&n);
        CHECK(gSpecialStageWork->timer == 0x5c && gSpecialStageWork->taskC == 0,
              "intro waits at frame 0x5c for a touch (timer %d)", (int)gSpecialStageWork->timer);
        dump_ppm("intro_wait");
        {
            InputState a = in_press(IN_JUMP);
            special_run_frame(&a);
        }
        CHECK(gSpecialStageWork->timer == 0x5d && g_sp_trace.sfx[0x10][0x14] == 1, "A taps: SE 0x14, intro continues");
        for (i = 0; i < 400 && gSpecialStageWork->taskC == 0; i++)
            special_run_frame(&n);
        CHECK(gSpecialStageWork->taskC == 1 && g_sp_trace.music_last == 0x1e, "play starts at intro frame 0x12c, SEQ_SS_GAME");
        CHECK(SpecialStage_GetPlayer()->unk228 == 0, "player state 0 (Start0)");
        for (i = 0; i < 0x168 + 10; i++)
            special_run_frame(&n);
        dump_ppm("play_start");
        CHECK(SpecialStage_GetPlayer()->unk228 == 2, "player runs (state 2) after the start sequence");
        CHECK(SpecialStageMap_GetScrollSpeed() == 0x3000, "scroll speed 0x3000");
        {
            extern u32 g_sp_matanim_draws;
            u32 before = g_sp_matanim_draws;
            special_draw();
            CHECK(gSpecialStageMap->nsbta.set.hdr && gSpecialStageMap->nsbta.set.hdr->kind == DCA_KIND_TEXSRT &&
                      gSpecialStageGimmickModels->unkEC4.set.hdr &&
                      gSpecialStageGimmickModels->unkEC4.set.hdr->kind == DCA_KIND_TEXPAT,
                  "ss.nsbta / ss.nsbtp converted sets loaded (mod__ss.dct / .dcp)");
            CHECK(g_sp_matanim_draws > before, "the sky is drawn with its texture SRT animation (%u draws)",
                  (unsigned)(g_sp_matanim_draws - before));
        }
        CHECK(SpecialStage_GetPlayer()->unk114 == 1, "steering enabled after START!");
    }

    printf("[movement]\n");
    {
        SpObj *p = SpecialStage_GetPlayer();
        InputState r = in_stick(127, 0), l = in_stick(-127, 0), d = in_stick(0, 127), n = in_none();
        int maxa = 0;
        for (i = 0; i < 40; i++) {
            special_run_frame(&r);
            if ((s16)p->unk16 > maxa)
                maxa = (s16)p->unk16;
        }
        CHECK(p->unk134 == 0x4000 && maxa > 0x3000, "stick right: target = right wall, angle reached 0x%x", maxa);
        for (i = 0; i < 80; i++)
            special_run_frame(&l);
        CHECK((s16)p->unk16 < -0x3000 || p->unk228 != 2, "stick left: Sonic on the left side (angle %d, state %d)",
              (s16)p->unk16, p->unk228);
        for (i = 0; i < 120; i++)
            special_run_frame(&d);
        for (i = 0; i < 30; i++)
            special_run_frame(&n);
        CHECK(p->unk228 == 2 && (s16)p->unk16 > -0x400 && (s16)p->unk16 < 0x400, "stick down: back at the bottom (angle %d)",
              (s16)p->unk16);
        {
            InputState dl = in_none();
            dl.held = IN_LEFT;
            for (i = 0; i < 30; i++)
                special_run_frame(&dl);
            CHECK((s16)p->unk16 < -0x2000, "D-pad left slides the stylus: angle %d", (s16)p->unk16);
            for (i = 0; i < 60; i++)
                special_run_frame(&n);
        }
        dump_ppm("play_move");
    }
    special_shutdown();

    printf("[rings, quota, completion per stage]\n");
    for (s = 0; s < 7; s++) {
        RunInfo a = run_stage(s, 1, 0, 20000);
        special_result(&res);
        printf("  stage %d autopilot: result %d after %d frames, rings at checkpoint %d (quota %d) -> %s, max rings %d, hurt %d, tricks ok %d fail %d, rings picked %d\n",
               s + 1, a.result, a.frames, a.rings_at_cp, res.quota_1, a.cp_outcome == 1 ? "pass" : a.cp_outcome == 0 ? "fail" : "-",
               a.max_rings, a.hurt, g_sp_trace.tricks_ok, g_sp_trace.tricks_fail, g_sp_trace.rings_collected);
        CHECK(a.result == 1 || a.result == 2, "stage %d ends (result %d)", s + 1, a.result);
        CHECK(g_sp_trace.rings_collected > 0 && a.max_rings > 0, "stage %d: rings picked up (%d)", s + 1, a.max_rings);
        CHECK(a.cp_outcome != -1, "stage %d: checkpoint reached", s + 1);
        CHECK((a.result == 1) == (a.cp_outcome == 1 && res.rings >= res.quota_2) || a.result == 2,
              "stage %d: outcome consistent with the quotas", s + 1);
        special_shutdown();
        a = run_stage(s, 1, 1, 20000);
        special_result(&res);
        CHECK(a.result == 1 && res.cleared && (res.emerald_flags & (1u << s)),
              "stage %d cleared with the quota met: emerald %d earned (flags 0x%x, %d frames)", s + 1, s + 1,
              res.emerald_flags, a.frames);
        CHECK(g_sp_trace.sfx[14][3] == 1, "stage %d: SE_Emerald_Light on the results", s + 1);
        special_shutdown();
    }

    printf("[failure]\n");
    {
        RunInfo a = run_stage(0, 0, 0, 20000);
        special_result(&res);
        CHECK(a.result == 2 && !res.cleared && !(res.emerald_flags & 1),
              "no input: quota missed at the checkpoint (%d/%d rings) -> failed, no emerald", a.rings_at_cp, res.quota_1);
        special_shutdown();
    }

    printf("[obstacles: bomb damage]\n");
    for (s = 0; s < 7; s++) {
        int got = 0, f, hurt_rings = 0;
        special_set_emerald_flags(0);
        special_init(s, 0);
        run_intro();
        for (f = 0; f < 12000 && !got; f++) {
            SpObj *p = SpecialStage_GetPlayer();
            SpObj *b = find_type(0xd, 0xe, -0x40000, -0x2000);
            InputState i = in_none();
            if (p->unk228 == 2 && p->unk148 < 20)
                p->unk148 = 20;
            if (b && p->unk228 == 2 && p->unk74 == 0)
                i = steer_to(b->unk16);
            hurt_rings = p->unk148;
            if (special_run_frame(&i))
                break;
            if (p->unk228 == 0x10) {
                int before = hurt_rings;
                for (int k = 0; k < 3; k++) {
                    InputState n = in_none();
                    special_run_frame(&n);
                }
                CHECK(p->unk148 == (before > 10 ? before - 10 : 0), "stage %d: bomb hit -> hurt state 0x10, 10 rings lost (%d left)", s + 1,
                      p->unk148);
                CHECK(g_sp_trace.sfx[0x10][4] >= 1 && g_sp_trace.sfx[0x10][0x11] >= 1, "SE_Ringlost + explosion SE");
                got = 1;
            }
        }
        if (got) {
            special_shutdown();
            break;
        }
        special_shutdown();
    }

    printf("[attack: tap an enemy]\n");
    {
        int got = 0, f;
        for (s = 0; s < 7 && !got; s++) {
            special_set_emerald_flags(0);
            special_init(s, 0);
            run_intro();
            for (f = 0; f < 12000 && !got; f++) {
                SpObj *p = SpecialStage_GetPlayer();
                SpObj *e = find_type(7, 0xc, -0x7e000, -0xa000);
                InputState i = in_none();
                if (e && SpecialStageObj_IsInReach(p, e) && p->unk228 == 2 && p->unk114) {
                    int rings = p->unk148;
                    i = in_press(IN_JUMP);
                    special_run_frame(&i);
                    CHECK(p->unk228 == 0x12 && p->unk118 != NULL, "stage %d: A taps the enemy -> TargetAttack", s + 1);
                    for (int k = 0; k < 120 && p->unk228 == 0x12; k++) {
                        InputState n = in_none();
                        special_run_frame(&n);
                    }
                    CHECK(g_sp_trace.attacks >= 1 || p->unk148 > rings || g_sp_trace.sfx[0x10][0x18] >= 1,
                          "homing attack hits (enemy destroyed / hit), rings %d -> %d", rings, p->unk148);
                    got = 1;
                    break;
                }
                if (special_run_frame(&i))
                    break;
            }
            special_shutdown();
        }
        CHECK(got, "an enemy was attacked");
    }

    printf("[tricks]\n");
    {
        int okk = 0, bad = 0, f;
        for (s = 0; s < 7 && !(okk && bad); s++) {
            special_set_emerald_flags(0);
            special_init(s, 0);
            run_intro();
            for (f = 0; f < 15000; f++) {
                SpObj *p = SpecialStage_GetPlayer();
                InputState i = in_none();
                SpObj *ramp = find_type(3, 3, -0x50000, -0x2000);
                if (ramp && p->unk228 == 2)
                    i = steer_to(ramp->unk16);
                if (p->unk228 == 0xa && p->unk128) {
                    SpTrickSeq *e = SpecialStageTrick_GetSequence(p->unk150);
                    int cur = p->unk14C;
                    if (!okk) {
                        if (cur >= 0 && cur < e->count && (f % 4) < 2) {
                            int px = cur ? e->pts[cur - 1].x : 128, py = cur ? e->pts[cur - 1].y : 96;
                            float dx = e->pts[cur].x - px, dy = e->pts[cur].y - py, m = sqrtf(dx * dx + dy * dy);
                            i = in_stick((int)(dx * 120 / m), (int)(dy * 120 / m));
                        }
                    } else if (!bad && cur >= 0 && cur < e->count && e->count > 2 && (f % 4) < 2) {
                        /* flick at a later circle: a wrong tap */
                        int px = cur ? e->pts[cur - 1].x : 128, py = cur ? e->pts[cur - 1].y : 96;
                        float dx = e->pts[e->count - 1].x - px, dy = e->pts[e->count - 1].y - py, m = sqrtf(dx * dx + dy * dy);
                        if (m > 0)
                            i = in_stick((int)(dx * 120 / m), (int)(dy * 120 / m));
                    }
                }
                if (special_run_frame(&i))
                    break;
                if (g_sp_trace.tricks_ok && !okk) {
                    okk = 1;
                    CHECK(p->unk228 >= 0xb && p->unk228 <= 0xd, "stage %d: trick circles done by flicks -> trick state %d, rating %d, %d reward rings",
                          s + 1, p->unk228, p->unk158, p->unk154);
                    dump_ppm("trick");
                }
                if (g_sp_trace.tricks_fail && okk && !bad) {
                    bad = 1;
                    CHECK(p->unk228 == 0xe && gSpecialStageHud->spr1.anim == 0xb, "wrong circle -> MISS, TrickEnd");
                }
            }
            special_shutdown();
        }
        CHECK(okk, "a trick sequence was completed");
        CHECK(bad, "a trick sequence was failed by a wrong circle");
    }

    printf("[results card]\n");
    {
        RunInfo a;
        special_set_emerald_flags(0x5);
        special_init(3, 0);
        run_intro();
        a.result = 0;
        for (i = 0; i < 30000; i++) {
            InputState n = in_none();
            SpObj *p = SpecialStage_GetPlayer();
            if (p->unk148 < SpecialStageHud_Unk_ov001_02314368())
                p->unk148 = SpecialStageHud_Unk_ov001_02314368();
            if (gSpecialStageHud->unkBD4 == 0x180)
                dump_ppm("results");
            if ((a.result = special_run_frame(&n)) != 0)
                break;
        }
        special_result(&res);
        CHECK(a.result == 1 && res.emerald_flags == 0xd, "stage 4 with emeralds 1+3 owned: flags 0x5 -> 0x%x", res.emerald_flags);
        special_shutdown();
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
