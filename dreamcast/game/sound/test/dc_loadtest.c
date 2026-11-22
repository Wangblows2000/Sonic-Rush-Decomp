/*
 * Dreamcast / Flycast test for the loading bracket (engine/audio.c
 * audio_load_begin / audio_load_end) and the music reader.
 * A 60 Hz "game loop" (vid_waitvbl; sound_update + audio_update) plays the zone music and
 * an SE_Ring every 0.5 s; every 6 s it runs a simulated stage load inside
 * audio_load_begin() / audio_load_end(): one big blocking fs_read of
 * /cd/load/blob.bin (disc contention) plus 400 ms of CPU work without
 * yielding, and on every second cycle a music restart (audio_music_play, as
 * load_stage does). Serial log lines "LOADTEST t=<ms> ..." give the times for
 * the analysis of the recording; frame times over 17.5 ms are counted.
 * Disc: /cd/1ST_READ.BIN, /cd/sound/z1s.srb, /cd/music/z1.wav, /cd/load/blob.bin.
 */
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/stream.h>
#include "../sound.h"
#include "../../engine/audio.h"

KOS_INIT_FLAGS(INIT_DEFAULT);

#define LOOP_S 486777u
#define LOOP_E 3687574u

static volatile uint32_t s_sink;
static void busy_ms(uint32_t ms) {
    uint64_t end = timer_ms_gettime64() + ms;
    uint32_t x = 1;
    while (timer_ms_gettime64() < end) for (int i = 0; i < 1000; i++) x = x * 1664525u + 1013904223u;
    s_sink = x;
}

int main(int argc, char **argv) {
    static uint8_t blob[2 << 20] __attribute__((aligned(32)));
    uint64_t last = 0, t0;
    uint32_t slow = 0, worst = 0, frames = 0;
    int cycle = 0;
    (void)argc; (void)argv;
#ifndef LOADTEST_NO_AUDIO
    audio_init();
    sound_init();
    printf("LOADTEST bank %d\n", sound_load_bank("z1s"));
    printf("LOADTEST music %d\n", audio_music_play("/cd/music/z1.wav", LOOP_S, LOOP_E));
#else   /* control: same loop without any audio (frame-time baseline) */
#define sound_play_se(x) ((void)0)
#define sound_update() ((void)0)
#define audio_update() ((void)0)
#define audio_load_begin() ((void)0)
#define audio_load_end() ((void)0)
#define audio_music_play(a, b, c) 0
#define audio_stream_underruns() 0u
#endif
    t0 = timer_ms_gettime64();
    for (;;) {
        uint64_t now = timer_ms_gettime64();
        uint32_t ms = (uint32_t)(now - t0);
        if (frames % 30 == 0) sound_play_se(0x04);      /* SE_Ring every 0.5 s */
        sound_update();
        audio_update();
        vid_waitvbl();   /* 60 Hz pacing like the game loop */
        frames++;
        {
            uint64_t u = timer_us_gettime64();
            if (last) {
                uint32_t dt = (uint32_t)(u - last);
                if (dt > worst) worst = dt;
                if (dt > 17500) slow++;
            }
            last = u;
        }
        if (ms >= 6000u * (uint32_t)(cycle + 1)) {
            file_t f;
            int r = -1;
            uint64_t a = timer_ms_gettime64(), b, c, d;
            printf("LOADTEST t=%lu frames %lu, %lu over 17.5 ms, worst %lu us\n", (unsigned long)(a - t0),
                   (unsigned long)frames, (unsigned long)slow, (unsigned long)worst);
            audio_load_begin();
            b = timer_ms_gettime64();
            f = fs_open("/cd/load/blob.bin", O_RDONLY);
            if (f != FILEHND_INVALID) { r = fs_read(f, blob, sizeof blob); fs_close(f); }
            busy_ms(400);
            if (cycle & 1) audio_music_play("/cd/music/z1.wav", LOOP_S, LOOP_E);   /* as load_stage */
            c = timer_ms_gettime64();
            audio_load_end();
            d = timer_ms_gettime64();
            printf("LOADTEST cycle %d: begin t=%lu faded t=%lu loaded t=%lu (read %d) end t=%lu restart %d underruns %lu\n",
                   cycle, (unsigned long)(a - t0), (unsigned long)(b - t0), (unsigned long)(c - t0), r,
                   (unsigned long)(d - t0), cycle & 1, (unsigned long)audio_stream_underruns());
            cycle++;
            last = 0;
        }
    }
    return 0;
}
