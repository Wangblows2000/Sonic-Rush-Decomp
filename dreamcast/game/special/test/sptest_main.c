/*
 * sptest_main.c - standalone Dreamcast test program for the special stages
 * (not the game: main.c / the game Makefile / build_disc.sh stay untouched).
 * Plays stage 1..7 in a row (/cd/special/start.txt can hold the first stage
 * 1..7 and the emerald flags), prints the state to the serial console, and
 * optionally an autopilot (/cd/special/auto.txt) for unattended captures.
 */
#include <kos.h>
#include <stdio.h>
#include <stdlib.h>
#include "../special.h"
#include "../sp_internal.h"
#include "../../engine/render.h"
#include "../../engine/input.h"
#include "../../engine/audio.h"
#include "../../sound/sound.h"

/* sound.c calls the streamed-music player of the game (sound/bgm.c); the special stage plays
 * its music itself (sp_audio.c), so the test program has no bgm */
void bgm_update(void);
void bgm_update(void) {}

int main(int argc, char **argv) {
    InputState in;
    int stage = 0, r, frame = 0;
    unsigned flags = 0;
    FILE *f;
    (void)argc;
    (void)argv;
    {
        pvr_init_params_t params = {
            .opb_sizes = {PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16},
            .vertex_buf_size = 512 * 1024,
            .opb_overflow_count = 3};
        pvr_init(&params);
    }
    audio_init();
    sound_init();
    sound_set_dir("/cd/special/sound");
    special_set_path("/cd/special");
    {
        extern int g_sp_verbose;
        g_sp_verbose = 1;
    }
    f = fopen("/cd/special/start.txt", "r");
    if (f) {
        int force = 0, legend = 1;
        if (fscanf(f, "%d %x %d %d", &stage, &flags, &force, &legend) < 1)
            stage = 1;
        special_debug_force_quota(force);
        special_show_legend(legend);
        stage--;
        fclose(f);
    }
    if (stage < 0 || stage > 6)
        stage = 0;
    for (;;) {
        SpecialResult res;
        special_set_emerald_flags(flags);
        r = special_init(stage, 0);
        printf("sptest: stage %d init %d (pvr free %lu)\n", stage + 1, r, (unsigned long)pvr_mem_available());
        if (r != 0)
            break;
        for (frame = 0;; frame++) {
            input_poll(&in);
            r = special_run_frame(&in);
            {   /* controller trace for the Flycast check (SPECIAL.md section 8) */
                static int last_ax, last_ok, last_bad, last_att;
                SpObj *p = gSpecialStageWork ? gSpecialStageWork->unk18 : NULL;
                if (in.ax != last_ax && p)
                    printf("sptest: stick x %d -> steer %d target 0x%x\n", in.ax, g_sp_pad.steer, (int)(s16)p->unk134);
                last_ax = in.ax;
                if (g_sp_pad.qte && p)
                    printf("sptest: trick input 0x%x (next target %d)\n", (unsigned)g_sp_pad.qte, (int)p->unk14C);
                {   /* hints for a test driver: sequence open, ramp ahead, something to attack */
                    static int was_trick, was_target, was_wait;
                    int wait = gSpecialStageWork && gSpecialStageWork->taskC == 0 && gSpecialStageWork->timer == 0x5c;
                    if (wait && !was_wait)
                        printf("sptest: intro wait (legend %d)\n", sp_legend_pending());
                    was_wait = wait;
                    int trick = p && p->unk228 == 0xa && p->unk128, tgt;
                    if (trick && !was_trick) {
                        SpTrickSeq *e = SpecialStageTrick_GetSequence(p->unk150);
                        int k;
                        printf("sptest: trick open seq %d count %d time %d:", (int)p->unk150, e->count, e->unk20);
                        for (k = 0; k < e->count; k++)
                            printf(" %s", sp_qte_names[k]);
                        printf("\n");
                    }
                    was_trick = trick;
                    if (p && p->unk228 == 2 && (frame % 6) == 0 && gSpecialStageWork->unk1C) {
                        SpObjList *l = *gSpecialStageWork->unk1C;
                        SpObj *o;
                        for (o = l->head.next; o != &l->tail; o = o->next)
                            if (!(o->unk60 & 1) && o->unk68 == 3 && o->pos.z > -0x60000 && o->pos.z < -0x8000) {
                                printf("sptest: ramp %d sonic %d\n", (int)(s16)o->unk16, (int)(s16)p->unk16);
                                break;
                            }
                    }
                    tgt = p && p->unk228 == 2 && gSpecialStageWork->unk1C &&
                          SpecialStageGimmick_FindAttackTarget(p, *gSpecialStageWork->unk1C) != NULL;
                    if (tgt && !was_target)
                        printf("sptest: attackable\n");
                    was_target = tgt;
                }
                if (g_sp_pad.attack && p)
                    printf("sptest: A attack -> state 0x%x\n", (unsigned)p->unk228);
                if (g_sp_trace.tricks_ok != last_ok || g_sp_trace.tricks_fail != last_bad)
                    printf("sptest: trick %s rating %d reward %d\n", g_sp_trace.tricks_ok != last_ok ? "OK" : "MISS",
                           p ? (int)p->unk158 : -1, p ? (int)p->unk154 : -1);
                if (g_sp_trace.attacks != last_att)
                    printf("sptest: enemy defeated (attacks %d)\n", g_sp_trace.attacks);
                last_ok = g_sp_trace.tricks_ok;
                last_bad = g_sp_trace.tricks_fail;
                last_att = g_sp_trace.attacks;
            }
            render_begin_frame();
            special_draw();
            render_end_frame();
            sound_update();
            audio_update();
            if ((frame % 300) == 0)
                printf("sptest: stage %d frame %d\n", stage + 1, frame);
            if (r)
                break;
        }
        special_result(&res);
        printf("sptest: stage %d result %d cleared %d rings %d quota %d/%d emeralds 0x%x frames %d\n", stage + 1, r,
               res.cleared, res.rings, res.quota_1, res.quota_2, (unsigned)res.emerald_flags, res.frames);
        flags = res.emerald_flags;
        special_shutdown();
        printf("sptest: shutdown done (pvr free %lu)\n", (unsigned long)pvr_mem_available());
        stage = (stage + 1) % 7;
    }
    return 0;
}
