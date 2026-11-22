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
/* a controller frame as engine/input.c builds it: with the raw-input engine patch (IN_HAS_RAW)
 * the triggers / D-pad also appear in lt, rt, dpad */
static void raw_from_held(InputState *i) {
#ifdef IN_HAS_RAW
    i->lt = (i->held & IN_L) ? 255 : 0;
    i->rt = (i->held & IN_R) ? 255 : 0;
    i->dpad = i->held & (IN_LEFT | IN_RIGHT | IN_UP | IN_DOWN);
#else
    (void)i;
#endif
}
static InputState in_press(u32 b) {
    InputState i = in_none();
    i.held = i.pressed = b;
    raw_from_held(&i);
    return i;
}
static InputState in_hold(u32 b) {
    InputState i = in_none();
    i.held = b;
    raw_from_held(&i);
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

/* run the intro: press START at the "PRESS START TO BEGIN" prompt */
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
            InputState a = in_press(IN_START);
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

/* stick x that targets pipe angle a (inverse of sp_stick_to_angle: pipe x = R sin a) */
static int stick_for(s16 a) {
    s32 sn = SIN((u16)a), m = sn < 0 ? -sn : sn;
    int v;
    if (m < 8)
        return 0;
    v = g_input_deadzone + 1 + (int)((120 - g_input_deadzone) * (m / 4096.0f));
    if (v > 127)
        v = 127;
    return sn < 0 ? -v : v;
}
static InputState steer_to(u16 a) { return in_stick(stick_for((s16)a), 0); }

/* the InputState of Trick Spring input k (0..9) pressed this frame (D-pad / triggers through the
 * engine's digital bits: the fallback without the raw-input engine patch) */
static InputState qte_press(int k) {
    static const u32 bits[10] = {IN_JUMP, IN_TRICK, IN_BOOST, IN_ALT, IN_L, IN_R, IN_UP, IN_RIGHT, IN_DOWN, IN_LEFT};
    InputState i = in_none();
    i.held = i.pressed = bits[k];
    raw_from_held(&i);
    return i;
}
static int in_trick(void) {
    SpObj *p = SpecialStage_GetPlayer();
    return p && p->unk228 == 0xa && p->unk128;
}

typedef struct {
    int result, frames, rings_at_cp, cp_outcome, max_rings, hurt;
} RunInfo;

/* autopilot: collect rings with the stick, do the Trick Spring inputs; optional: force the quota */
static RunInfo run_stage(int stage, int autopilot, int force_quota, int max_frames) {
    RunInfo ri;
    int f, r = 0, gap = 0;
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
        if (autopilot && in_trick()) {
            /* the button of the current target, every 6th frame (a press, then a release) */
            if (++gap % 6 == 0 && p->unk14C >= 0 && p->unk14C < 10)
                i = qte_press(p->unk14C);
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

/* to the first Trick Spring sequence of a stage (steering at the ramps); 1 = in the sequence */
static int reach_trick(int stage, int max_frames) {
    int f;
    special_set_emerald_flags(0);
    if (special_init(stage, 0) != 0)
        return 0;
    run_intro();
    for (f = 0; f < max_frames; f++) {
        SpObj *p = SpecialStage_GetPlayer();
        InputState i = in_none();
        SpObj *ramp = find_type(3, 3, -0x50000, -0x2000);
        if (in_trick())
            return 1;
        if (ramp && p->unk228 == 2)
            i = steer_to(ramp->unk16);
        if (p->unk148 < 0x40)   /* keep going past the checkpoint */
            p->unk148 = 0x40 > SpecialStage_GetRingQuota() ? 0x40 : SpecialStage_GetRingQuota();
        if (special_run_frame(&i))
            return 0;
    }
    return 0;
}

/* do the open sequence: press the target inputs, `delay` frames between presses; returns the
 * rating (0 Nice .. 2 Fantastic, 3 miss) and the rings the trick gave */
static int do_sequence(int delay, int *rings_gained, int *reward, int *count) {
    SpObj *p = SpecialStage_GetPlayer();
    SpTrickSeq *e = SpecialStageTrick_GetSequence(p->unk150);
    int before = p->unk148, f, ok0 = g_sp_trace.tricks_ok, bad0 = g_sp_trace.tricks_fail, rating = -1;
    InputState n = in_none();
    *count = e->count;
    *reward = 0;
    for (f = 0; f < 2000 && in_trick(); f++) {
        InputState i = n;
        if (f % (delay + 1) == delay && p->unk14C >= 0 && p->unk14C < e->count)
            i = qte_press(p->unk14C);
        special_run_frame(&i);
    }
    if (g_sp_trace.tricks_ok > ok0) {
        rating = p->unk158;
        *reward = p->unk154;
    } else if (g_sp_trace.tricks_fail > bad0)
        rating = 3;
    /* until the reward rings are in (TrickEnd / back on the pipe) */
    for (f = 0; f < 400 && SpecialStage_GetPlayer()->unk228 != 2; f++)
        special_run_frame(&n);
    for (f = 0; f < 60; f++)
        special_run_frame(&n);
    *rings_gained = p->unk148 - before;
    return rating;
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

    printf("[intro: legend, PRESS START TO BEGIN]\n");
    {
        SpRes *rs = data_ov001_0231f5c4;
        InputState n = in_none();
        CHECK(rs->spr[4].loaded && rs->spr[4].n_anims >= 80, "controller art loaded (ac_spst_pad, %d anims)",
              rs->spr[4].n_anims);
        CHECK(gSpecialStageWork->spr[2].res == &rs->spr[4] && gSpecialStageWork->spr[2].anim == 0,
              "the intro prompt is the PRESS START TO BEGIN art (ac_spst_pad anim 0)");
        for (i = 0; i < 0x5c + 40; i++)
            special_run_frame(&n);
        CHECK(gSpecialStageWork->timer == 0x5c && gSpecialStageWork->taskC == 0,
              "intro waits at frame 0x5c for the begin input (timer %d)", (int)gSpecialStageWork->timer);
        CHECK(sp_legend_pending(), "first special stage of the session: the controller legend is up");
        dump_ppm("intro_legend");
        {
            static const u32 other[4] = {IN_TRICK, IN_BOOST, IN_ALT, IN_R};
            int k, moved = 0;
            for (k = 0; k < 4; k++) {
                InputState b = in_press(other[k]);
                special_run_frame(&b);
                special_run_frame(&n);
                moved |= gSpecialStageWork->timer != 0x5c;
            }
            CHECK(!moved, "B / X / Y / R do not begin");
        }
        {
            InputState st = in_press(IN_START);
            special_run_frame(&st);
        }
        CHECK(gSpecialStageWork->timer == 0x5d && g_sp_trace.sfx[0x10][0x14] == 1 && !sp_legend_pending(),
              "START begins: SE 0x14, intro continues, legend dismissed");
        for (i = 0; i < 400 && gSpecialStageWork->taskC == 0; i++)
            special_run_frame(&n);
        CHECK(gSpecialStageWork->taskC == 1 && g_sp_trace.music_last == 0x1e, "play starts at intro frame 0x12c, SEQ_SS_GAME");
        CHECK(SpecialStage_GetPlayer()->unk228 == 0, "player state 0 (Start0)");
        for (i = 0; i < 0x17c + 20; i++)
            special_run_frame(&n);
        dump_ppm("play_hint");   /* the steering hint where the DS shows the stylus */
        for (; i < 0x168 + 10 + 0x60; i++)
            special_run_frame(&n);
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

    printf("[steering: analogue stick]\n");
    {
        SpObj *p = SpecialStage_GetPlayer();
        InputState n = in_none();
        s32 a_half = sp_stick_to_angle(32 + 44), a_full = sp_stick_to_angle(127), a_lo = sp_stick_to_angle(33);
        int mono = 1, prev = -1, x;
        for (x = 0; x <= 127; x++) {
            int a = sp_stick_to_angle(x);
            if (a < prev)
                mono = 0;
            prev = a;
            if (sp_stick_to_angle(-x) != -a)
                mono = 0;
        }
        CHECK(sp_stick_to_angle(20) == 0 && sp_stick_to_angle(32) == 0 && a_lo > 0 && a_lo < 0x100,
              "dead zone: |x| <= 32 no target; just outside: a small angle (0x%x)", (int)a_lo);
        CHECK(mono, "the stick curve is monotonic and symmetric (x -> -x gives -angle)");
        CHECK(a_full == 0x4000 && sp_stick_to_angle(120) == 0x4000 && sp_stick_to_angle(-127) == -0x4000 &&
                  sp_stick_to_angle(119) < 0x4000,
              "full tilt (|x| >= 120) = the wall target 0x4000 (the DS wall)");
        CHECK(a_half > 0x1000 && a_half < 0x1800, "half tilt = half the pipe width across: angle 0x%x (30 deg = 0x1555)",
              (int)a_half);
        {
            InputState h = in_stick(32 + 44, 0);
            int k, settled = 0;
            special_run_frame(&h);
            CHECK(p->unk134 == a_half, "no input delay: the target follows the stick in the same frame (0x%x)",
                  (int)p->unk134);
            for (k = 0; k < 60; k++) {
                special_run_frame(&h);
                if ((s16)p->unk16 == (s16)a_half)
                    settled++;
            }
            CHECK(settled > 20 && p->unk228 == 2,
                  "half tilt right: Sonic turns at the original rate and stays at 0x%x on the pipe (angle 0x%x, state %d)",
                  (int)a_half, (s16)p->unk16, p->unk228);
        }
        {
            InputState q = in_stick(-(32 + 22), 0);
            for (i = 0; i < 60; i++)
                special_run_frame(&q);
            CHECK((s16)p->unk16 < 0 && (s16)p->unk16 > -0x1000, "quarter tilt left: angle %d (proportional)",
                  (s16)p->unk16);
        }
        {
            InputState r = in_stick(127, 0);
            int maxa = 0, jumped = 0;
            for (i = 0; i < 60; i++) {
                special_run_frame(&r);
                if ((s16)p->unk16 > maxa)
                    maxa = (s16)p->unk16;
                if (p->unk228 == 6)
                    jumped = 1;
            }
            CHECK(maxa >= 0x3800 && jumped, "full tilt right: to the wall (0x%x), the edge jump (state 6) as on the DS",
                  maxa);
        }
        for (i = 0; i < 120; i++)
            special_run_frame(&n);
        CHECK(p->unk228 == 2 && (s16)p->unk16 > -0x200 && (s16)p->unk16 < 0x200,
              "stick released: back to the bottom (angle %d)", (s16)p->unk16);
    }

    printf("[steering: D-pad]\n");
    {
        SpObj *p = SpecialStage_GetPlayer();
        InputState dl = in_hold(IN_LEFT), dd = in_hold(IN_DOWN), n = in_none();
        int stay;
        for (i = 0; i < 8; i++)
            special_run_frame(&dl);
        CHECK((s16)p->unk134 < -0x2000 && (s16)p->unk134 > -0x4000, "D-pad left held 8 frames: target moves along the pipe (%d)",
              (s16)p->unk134);
        for (i = 0; i < 40; i++)
            special_run_frame(&n);
        stay = (s16)p->unk16;
        CHECK(stay < -0x1000, "released: the target stays, Sonic stays on the left (angle %d)", stay);
        for (i = 0; i < 60; i++)
            special_run_frame(&dd);
        CHECK((s16)p->unk16 > -0x200 && (s16)p->unk16 < 0x200, "D-pad down: back to the bottom (angle %d)",
              (s16)p->unk16);
        {
            InputState dr = in_hold(IN_RIGHT);
            int jumped = 0;
            for (i = 0; i < 60; i++) {
                special_run_frame(&dr);
                jumped |= p->unk228 == 6;
            }
            CHECK(jumped, "D-pad right held: the right wall, edge jump");
        }
        for (i = 0; i < 90; i++)
            special_run_frame(&dd);
        dump_ppm("play_move");
    }

    printf("[pause]\n");
    {
        InputState st = in_press(IN_START), a = in_press(IN_JUMP), b = in_press(IN_TRICK), n = in_none();
        s32 frame = gSpecialStageWork->unk28;
        special_run_frame(&st);
        CHECK(special_paused(), "START pauses");
        for (i = 0; i < 10; i++)
            special_run_frame(&n);
        CHECK(gSpecialStageWork->unk28 == frame, "the stage is frozen while paused");
        dump_ppm("pause");
        special_run_frame(&b);
        CHECK(special_paused(), "B: no GO BACK (this emerald is not owned yet)");
        special_run_frame(&a);
        CHECK(!special_paused() && gSpecialStageWork->unk18->unk228 == 2, "A continues");
        special_run_frame(&n);
    }
    special_shutdown();

    printf("[trigger thresholds]\n");
    {
        SpTrig t = {0, 0};
        int presses = 0, k;
        static const int noisy[] = {120, 140, 126, 131, 150, 100, 159, 128, 90, 155};
        presses += sp_trigger_step(&t, 0);
        presses += sp_trigger_step(&t, 170);
        CHECK(presses == 1, "L/R: a press at >= %d", SP_TRIG_ON);
        for (k = 0; k < 30; k++)
            presses += sp_trigger_step(&t, 200);
        CHECK(presses == 1, "held: no repeat");
        presses += sp_trigger_step(&t, 100);
        presses += sp_trigger_step(&t, 200);
        presses += sp_trigger_step(&t, 50);
        presses += sp_trigger_step(&t, 200);
        CHECK(presses == 1, "hysteresis / debounce: a dip to 100, or one frame <= %d, is not a release", SP_TRIG_OFF);
        presses += sp_trigger_step(&t, 40);
        presses += sp_trigger_step(&t, 40);
        presses += sp_trigger_step(&t, 170);
        CHECK(presses == 2, "released (2 frames <= %d), pressed again: a second press", SP_TRIG_OFF);
        t.down = 0;
        t.low = 0;
        presses = 0;
        for (k = 0; k < 10; k++)
            presses += sp_trigger_step(&t, noisy[k]);
        CHECK(presses == 0, "a half-pulled trigger wobbling around 128 never presses");
    }

    printf("[Trick Spring rule]\n");
    {
        s32 c;
        int ok = 1, k;
        for (k = 0; k < 10; k++) {   /* each numbered input advances its own target */
            c = sp_qte_step(k, 10, 1u << k);
            if (c != (k < 9 ? k + 1 : 0x1234))
                ok = 0;
        }
        CHECK(ok, "inputs 1..10 (A B X Y L R Up Right Down Left) in order complete a 10-target sequence");
        CHECK(sp_qte_step(0, 6, 1u << 3) == -1 && sp_qte_step(2, 6, 1u << 5) == -1,
              "wrong order: the input of a later target is a MISS");
        CHECK(sp_qte_step(1, 6, 1u << 0) == 1, "a repeated press of a done target does nothing");
        CHECK(sp_qte_step(1, 3, 1u << 7) == 1 && sp_qte_step(0, 3, 1u << 9) == 0,
              "inputs of numbers the sequence does not have do nothing (as a touch off the targets)");
        CHECK(sp_qte_step(0, 6, (1u << 0) | (1u << 1)) == -1, "two target inputs in one frame (mashing): MISS");
        CHECK(sp_qte_step(2, 3, 0) == 2, "no input: the sequence waits (the timer decides)");
    }

    printf("[input context]\n");
    {
        int k, ok = 1, steer = 0, att = 0;
        SpObj *p;
        u16 st;
        u8 f128;
        special_init(0, 0);
        run_intro();
        p = SpecialStage_GetPlayer();
        st = p->unk228;
        f128 = (u8)p->unk128;
        p->unk228 = 0xa;   /* a sequence open */
        p->unk128 = 1;
        for (k = 0; k < 10; k++) {
            InputState i = qte_press(k), n = in_none();
            sp_input_update(&n);
            sp_input_update(&i);
            if (g_sp_pad.qte != (1u << k))
                ok = 0;
            steer |= g_sp_pad.steer;
            att |= g_sp_pad.attack;
        }
        CHECK(ok, "sequence open: each of A B X Y L R Up Right Down Left gives its own target input");
        CHECK(!steer && !att, "sequence open: no steering, A does not attack");
        {
            InputState hold = qte_press(0);
            hold.pressed = 0;
            sp_input_update(&hold);
            CHECK(g_sp_pad.qte == 0, "a held button is not a press");
        }
        {
            InputState st8 = in_press(IN_START);
            sp_input_update(&st8);
            CHECK(g_sp_pad.qte == 0 && (gPad[2] & 8), "START is never a target input (it pauses)");
        }
#ifdef IN_HAS_RAW
        {   /* raw inputs (engine patch): the stick is not the D-pad; analogue trigger hysteresis */
            InputState sx = in_none(), t1 = in_none(), t2 = in_none(), n = in_none();
            int presses = 0;
            sx.ax = 127;
            sx.held = sx.pressed = IN_RIGHT;   /* engine/input.c: the stick also sets the direction bits */
            sp_input_update(&sx);
            CHECK(g_sp_pad.qte == 0, "raw: a stick push is not D-pad Right");
            sp_input_update(&n);
            sp_input_update(&n);
            t1.rt = 170;
            t2.rt = 100;
            sp_input_update(&t1);
            presses += g_sp_pad.qte == (1u << 5);
            sp_input_update(&t2);
            sp_input_update(&t1);
            presses += g_sp_pad.qte == (1u << 5);
            CHECK(presses == 1, "raw: R pulled to 170, eased to 100, pulled again: one press");
        }
#endif
        p->unk228 = st;
        p->unk128 = f128;
        {
            InputState a = in_press(IN_JUMP), dl = in_none();
            sp_input_update(&a);
            CHECK(g_sp_pad.attack && g_sp_pad.qte == 0, "no sequence: A is the attack");
            dl = in_press(IN_LEFT);
            sp_input_update(&dl);
            CHECK(g_sp_pad.steer && g_sp_pad.qte == 0, "no sequence: the D-pad steers");
        }
        special_shutdown();
        special_show_legend(0);
    }

    printf("[Trick Spring in the stages]\n");
    {
        int reward, gained, count, r, k, done = 0;
        /* the three ratings: fast / medium / slow presses */
        for (k = 0; k < 3; k++) {
            int s2 = 0, got = 0;
            for (s2 = 0; s2 < 7 && !got; s2++) {
                if (!reach_trick(s2, 15000)) {
                    special_shutdown();
                    continue;
                }
                {
                    SpTrickSeq *e = SpecialStageTrick_GetSequence(SpecialStage_GetPlayer()->unk150);
                    /* Fantastic: done within the first third; Great: second; Nice: last third */
                    int third = e->unk20 / 3, delay;
                    delay = k == 0 ? 2 : (k == 1 ? (third + third / 2) / e->count : (2 * third + third / 2) / e->count);
                    if (k == 0)
                        dump_ppm("trick_targets");
                    r = do_sequence(delay, &gained, &reward, &count);
                    CHECK(r == 2 - k && reward == (k == 0 ? 15 : k == 1 ? 10 : 5) && gained >= reward,
                          "stage %d: %d targets pressed %d frames apart: rating %s, %d reward rings (rings +%d)",
                          s2 + 1, count, delay, r == 2 ? "Fantastic" : r == 1 ? "Great" : r == 0 ? "Nice" : "MISS",
                          reward, gained);
                    got = 1;
                    done++;
                }
                special_shutdown();
            }
        }
        CHECK(done == 3, "all three ratings reached");
        /* the first sequence of every stage, pressed promptly (sequences up to 8 targets: L, R,
         * Up, Right through the trigger / D-pad paths) */
        {
            int maxc = 0, all = 1;
            for (k = 0; k < 7; k++) {
                if (reach_trick(k, 20000)) {
                    r = do_sequence(2, &gained, &reward, &count);
                    if (count > maxc)
                        maxc = count;
                    printf("  stage %d: first sequence %d targets -> %s, +%d rings\n", k + 1, count,
                           r == 2 ? "Fantastic" : r == 1 ? "Great" : r == 0 ? "Nice" : "MISS", gained);
                    if (r != 2 || gained < 15)
                        all = 0;
                } else {
                    printf("  stage %d: no Trick Spring reached by the test driver\n", k + 1);
                }
                special_shutdown();
            }
            CHECK(all, "every stage's first sequence done promptly: Fantastic, 15 reward rings (longest %d targets)", maxc);
        }
        /* timeout, wrong order, held button, repeated press */
        if (reach_trick(0, 15000)) {
            SpObj *p = SpecialStage_GetPlayer();
            SpTrickSeq *e = SpecialStageTrick_GetSequence(p->unk150);
            InputState n = in_none();
            int bad0 = g_sp_trace.tricks_fail, f;
            for (f = 0; f < e->unk20 + 5 && in_trick(); f++)
                special_run_frame(&n);
            CHECK(g_sp_trace.tricks_fail == bad0 + 1 && p->unk228 == 0xe && gSpecialStageHud->spr1.anim == 0xb,
                  "no input: the time limit (%d frames) runs out -> MISS (after %d frames)", e->unk20, f);
        }
        special_shutdown();
        if (reach_trick(0, 15000)) {
            SpObj *p = SpecialStage_GetPlayer();
            SpTrickSeq *e = SpecialStageTrick_GetSequence(p->unk150);
            InputState w = qte_press(e->count - 1), n = in_none();
            int bad0 = g_sp_trace.tricks_fail;
            special_run_frame(&n);
            special_run_frame(&w);
            CHECK(g_sp_trace.tricks_fail == bad0 + 1 && p->unk228 == 0xe, "the last target's input first: MISS");
        }
        special_shutdown();
        if (reach_trick(0, 15000)) {
            SpObj *p = SpecialStage_GetPlayer();
            InputState a = qte_press(0), hold = qte_press(0), n = in_none();
            int f;
            hold.pressed = 0;
            special_run_frame(&a);
            CHECK(p->unk14C == 1, "A: target 1 done");
            for (f = 0; f < 10; f++)
                special_run_frame(&hold);
            CHECK(p->unk14C == 1 && in_trick(), "A held: no repeat");
            special_run_frame(&n);
            special_run_frame(&a);
            CHECK(p->unk14C == 1 && in_trick(), "A again (a done target): ignored, no miss");
            {
                InputState b = qte_press(1);
                special_run_frame(&b);
                CHECK(p->unk14C == 2 || p->unk14C == 0x1234 || !in_trick(), "B: target 2 done (%d)", p->unk14C);
            }
        }
        special_shutdown();
        if (reach_trick(0, 15000)) {
            InputState st = in_press(IN_START), a = in_press(IN_JUMP), n = in_none();
            SpObj *p = SpecialStage_GetPlayer();
            special_run_frame(&st);
            CHECK(special_paused(), "START pauses during a sequence");
            special_run_frame(&n);
            special_run_frame(&a);   /* A continues; it is not target 1 */
            CHECK(!special_paused() && p->unk14C == 0 && in_trick(), "the A that closes the pause is not a target input");
        }
        special_shutdown();
    }

    printf("[enemies]\n");
    {
        RunInfo a = run_stage(1, 0, 1, 20000);
        CHECK(g_sp_trace.attacks == 0, "no input through stage 2: no enemy is defeated (attacks %d, result %d)",
              g_sp_trace.attacks, a.result);
        special_shutdown();
    }

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

    printf("[Chaos Emerald progression]\n");
    {
        u32 flags = 0;
        int ok = 1;
        for (s = 0; s < 7; s++) {
            int f, r = 0;
            special_set_emerald_flags(flags);
            special_init(s, 0);
            run_intro();
            for (f = 0; f < 30000 && !r; f++) {
                InputState n = in_none();
                SpObj *p = SpecialStage_GetPlayer();
                if (p->unk148 < SpecialStageHud_Unk_ov001_02314368())
                    p->unk148 = SpecialStageHud_Unk_ov001_02314368();
                r = special_run_frame(&n);
            }
            special_result(&res);
            if (r != 1 || res.emerald_flags != (flags | (1u << s)))
                ok = 0;
            flags = res.emerald_flags;
            special_shutdown();
        }
        CHECK(ok && flags == 0x7f, "stages 1..7 cleared in turn (checkpoint and goal quotas met): emeralds 0x%x", flags);
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

    printf("[attack: A on an enemy]\n");
    {
        int got = 0, f, idle_ok = 0;
        for (s = 0; s < 7 && !got; s++) {
            special_set_emerald_flags(0);
            special_init(s, 0);
            run_intro();
            for (f = 0; f < 12000 && !got; f++) {
                SpObj *p = SpecialStage_GetPlayer();
                SpObj *e = find_type(7, 0xc, -0x7e000, -0xa000);
                InputState i = in_none();
                if (!idle_ok && p->unk228 == 2 && p->unk114 && !e &&
                    !SpecialStageGimmick_FindAttackTarget(p, *SpecialStage_GetGimmickManager())) {
                    InputState a = in_press(IN_JUMP), n = in_none();
                    special_run_frame(&a);
                    CHECK(p->unk228 == 2, "A with nothing attackable: no attack (state %d)", p->unk228);
                    special_run_frame(&n);
                    idle_ok = 1;
                    continue;
                }
                if (e && SpecialStageObj_IsInReach(p, e) && p->unk228 == 2 && p->unk114 &&
                    SpecialStageGimmick_FindAttackTarget(p, *SpecialStage_GetGimmickManager()) == e) {
                    int rings = p->unk148, att = g_sp_trace.attacks, reward = e->unk5E, hp = e->unk5C;
                    i = in_press(IN_JUMP);
                    special_run_frame(&i);
                    CHECK(p->unk228 == 0x12 && p->unk118 == e, "stage %d: A attacks the marked enemy -> TargetAttack", s + 1);
                    for (int k = 0; k < 120 && p->unk228 == 0x12; k++) {
                        InputState n = in_none();
                        special_run_frame(&n);
                    }
                    CHECK((hp > 1 && g_sp_trace.sfx[0x10][0x18] >= 1) ||
                              (g_sp_trace.attacks == att + 1 && p->unk148 >= rings + reward),
                          "homing attack hits: hp %d, defeated %d, rings %d -> %d (reward %d)", hp,
                          g_sp_trace.attacks - att, rings, p->unk148, reward);
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
