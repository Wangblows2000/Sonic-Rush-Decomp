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
#include "../../engine/render.h"
#include "../../engine/input.h"
#include "../../engine/audio.h"
#include "../../sound/sound.h"

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
        int force = 0;
        if (fscanf(f, "%d %x %d", &stage, &flags, &force) < 1)
            stage = 1;
        special_debug_force_quota(force);
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
