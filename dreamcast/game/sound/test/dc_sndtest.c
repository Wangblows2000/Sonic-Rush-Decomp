/*
 * Dreamcast / Flycast test program for the native sound runtime.
 * Measures the sound-RAM budget (snd_mem_available after snd_init, after the
 * music stream allocation of engine/audio.c, after the zone bank upload),
 * then plays a fixed list of original sound effects (bank z1s) and logs the
 * frame of every trigger on the serial console ("SNDTEST t=<frame> ...") so a
 * recording of the AICA output can be checked against it.
 * Disc: /cd/1ST_READ.BIN, /cd/sound/z1s.srb, /cd/music/z1.wav (optional).
 */
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/stream.h>
#include "../sound.h"
#include "../snd_internal.h"
#include "../../engine/audio.h"

KOS_INIT_FLAGS(INIT_DEFAULT);

static const struct { int frame, id, op; } SEQ[] = {
    /* op 0: Sound_PlaySeqArcEx(NULL, 0, id); 1: start held on handle; 2: stop held (fade 16) */
    {60, 0x04, 0},   /* SE_Ring */
    {120, 0x02, 0},  /* SE_Jump */
    {180, 0x00, 0},  /* SE_Spin */
    {240, 0x3c, 0},  /* SE_TMAX_bonus */
    {330, 0x3e, 1},  /* SE_Grind (held) */
    {450, 0x3e, 2},  /* stop it, fade 16 frames */
    {510, 0x09, 0},  /* SE_Boast_on */
    {600, 0x1d, 0},  /* SE_Yea (voice) */
    {690, 0x7b, 0},  /* SE_Boss_damage */
    {780, 0x04, 0}, {784, 0x04, 0}, {788, 0x04, 0}, {792, 0x04, 0}, {796, 0x04, 0}, /* 5 rings: player 7 cap 4 */
};

int main(int argc, char **argv) {
    uint32_t a0, a1, a2, a3;
    SoundHandle *held = NULL;
    int frame, k = 0, n = sizeof SEQ / sizeof SEQ[0];
    (void)argc; (void)argv;
    audio_init();                               /* snd_stream_init -> snd_init */
    a0 = snd_mem_available();
    int music = audio_music_play("/cd/music/z1.wav", 0, 0);
    a1 = snd_mem_available();
    audio_music_stop();                         /* keep the recording to effects only */
    sound_init();
    sound_set_dir("/cd/sound");
    int r = sound_load_bank("z1s");
    a2 = snd_mem_available();
    printf("SNDTEST budget: snd_mem_available after snd_init %lu, with music stream %lu (music %d), "
           "after z1s bank %lu (load %d)\n", (unsigned long)a0, (unsigned long)a1, music, (unsigned long)a2, r);
    printf("SNDTEST stream bytes %lu, bank uses %lu\n", (unsigned long)(a0 - a1), (unsigned long)(a0 - a2));
    for (frame = 0; frame < 900; frame++) {
        while (k < n && SEQ[k].frame == frame) {
            if (SEQ[k].op == 0) sound_play_se(SEQ[k].id);
            else if (SEQ[k].op == 1) held = sound_play(SEQ[k].id, 127, 0);
            else if (held) { sound_stop(held, 16); sound_release(held); held = NULL; }
            printf("SNDTEST t=%d op=%d id=0x%02x ms=%lu\n", frame, SEQ[k].op, SEQ[k].id, (unsigned long)timer_ms_gettime64());
            k++;
        }
        sound_update();
        vid_waitvbl();
        if (frame % 120 == 0) {
            int on = 0;
            for (int c = 0; c < 16; c++) on += snd_drv_regs(c)->active;
            printf("SNDTEST t=%d channels=%d notes=%d steals=%d\n", frame, on, snd_drv_stats()->notes, snd_drv_stats()->steals);
        }
    }
    a3 = snd_mem_available();
    printf("SNDTEST done: snd_mem_available %lu\n", (unsigned long)a3);
    for (;;) vid_waitvbl();
    return 0;
}
