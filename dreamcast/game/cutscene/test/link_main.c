/* KOS link / emulator test for the cutscene module (not part of the game ELF).
 * Runs a list of cutscenes one after another, as main.c would: /cd/cutscene/test.txt
 * holds lines "kind id character" (CsKind numbers, see cutscene.h); without it a
 * built-in list plays one of each kind. Full-screen cutscenes draw with
 * cutscene_draw(); overlays (TalkDemo) draw over a flat backdrop through
 * cutscene_draw_list(), the way the zone map / a stage would host them.
 * Serial log: "cs: ..." lines of the module and one "cstest:" line per result. */
#include <kos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "../../engine/render.h"
#include "../../engine/input.h"
#include "../../engine/audio.h"
#include "../../sound/sound.h"
#include "../cutscene.h"

typedef struct { int kind, id, chara; } Entry;

static const Entry k_default[] = {
    {CS_TALK_MAP, 0x15, 0}, {CS_TRANSFORM, 0, 0}, {CS_PLAYER, 1, 0}, {CS_SCRIPT, 0, 0},
    {CS_ENDING, 0, 0}, {CS_STAFFROLL, 1, 1}, {CS_EXTRA_ENDING, 0, 0},
};

static int load_list(Entry *e, int max) {
    file_t f = fs_open("/cd/cutscene/test.txt", O_RDONLY);
    char buf[2048], *p;
    int n = 0, len;
    if (f == FILEHND_INVALID)
        return 0;
    len = (int)fs_read(f, buf, sizeof(buf) - 1);
    fs_close(f);
    if (len <= 0)
        return 0;
    buf[len] = 0;
    for (p = strtok(buf, "\r\n"); p && n < max; p = strtok(NULL, "\r\n")) {
        Entry x;
        if (p[0] == '#')
            continue;
        if (sscanf(p, "%i %i %i", &x.kind, &x.id, &x.chara) == 3)
            e[n++] = x;
    }
    return n;
}

int main(int argc, char **argv) {
    static Entry list[64];
    InputState in;
    CsStory story;
    int n, i;
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
    memset(&in, 0, sizeof(in));
    memset(&story, 0, sizeof(story));
    story.stage_new = 1;
    story.language = 1;
    story.sonic_emeralds = 0x7f;
    story.zone = 0;
    n = load_list(list, 64);
    if (n == 0) {
        n = (int)(sizeof(k_default) / sizeof(k_default[0]));
        memcpy(list, k_default, sizeof(k_default));
    }
    printf("cstest: %d cutscenes, vram free %lu\n", n, (unsigned long)render_vram_free());
    for (i = 0; i < n; i++) {
        int frames = 0, done = 0;
        uint64_t t0 = timer_ms_gettime64();
        story.character = list[i].chara;
        cutscene_set_story(&story);
        printf("cstest: start kind %d id %d chara %d\n", list[i].kind, list[i].id, list[i].chara);
        if (cutscene_start((CsKind)list[i].kind, list[i].id, list[i].chara) != 0) {
            printf("cstest: kind %d id %d: data missing\n", list[i].kind, list[i].id);
            continue;
        }
        while (!done) {
            input_poll(&in);
            done = cutscene_run_frame(&in);
            audio_update();
            sound_update();
            render_begin_frame();
            if (cutscene_is_fullscreen()) {
                cutscene_draw();
            } else {
                /* host scene stand-in: a flat backdrop in OP, then the overlay */
                render_begin_list(PVR_LIST_OP_POLY);
                draw_rect_screen(NULL, PVR_LIST_OP_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 1.0f, 0xff304060);
                cutscene_draw_list(PVR_LIST_OP_POLY);
                render_end_list();
                render_begin_list(PVR_LIST_PT_POLY);
                cutscene_draw_list(PVR_LIST_PT_POLY);
                render_end_list();
                render_begin_list(PVR_LIST_TR_POLY);
                cutscene_draw_list(PVR_LIST_TR_POLY);
                render_end_list();
            }
            render_end_frame();
            frames++;
            if (frames % 600 == 0) {
                struct mallinfo mi = mallinfo();
                printf("cstest: frame %d ms %u ram used %d vram free %lu\n", frames,
                       (unsigned)(timer_ms_gettime64() - t0), mi.uordblks, (unsigned long)render_vram_free());
            }
        }
        {
            const CsResult *r = cutscene_result();
            printf("cstest: kind %d id %d done frames %d ms %u next %d progress %d flags %08lx skipped %d\n",
                   list[i].kind, list[i].id, frames, (unsigned)(timer_ms_gettime64() - t0), r ? (int)r->next : -1,
                   r ? r->progress : -1, r ? (unsigned long)r->set_save_flags : 0ul, r ? r->skipped : 0);
        }
        cutscene_shutdown();
        printf("cstest: vram free %lu\n", (unsigned long)render_vram_free());
    }
    printf("cstest: all done\n");
    for (;;) {
        render_begin_frame();
        render_begin_list(PVR_LIST_OP_POLY);
        draw_rect_screen(NULL, PVR_LIST_OP_POLY, 0, 0, 640, 480, 0, 0, 0, 0, 1.0f, 0xff000000);
        render_end_list();
        render_end_frame();
    }
    return 0;
}
