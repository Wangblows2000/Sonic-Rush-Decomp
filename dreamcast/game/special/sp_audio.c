/*
 * sp_audio.c - the special stage's sound calls.  Every call keeps the original
 * sequence-archive / sequence numbers of the sys SDAT:
 *   Sound_PlaySeqArc(0, 0x10, id)  special-stage SE (SSAR 16: SE_Ring 3, SE_Dash_panel 0x17 ...)
 *   data_ov001_0231f5c8 archive     SSAR 9 seq 0 (SE_Shake_hands), SSAR 14 seq 3 (SE_Emerald_Light)
 *   Sound_PlaySeq(0, 0x1e / 0x1f)   SEQ_SS_GAME / SEQ_SS_END
 * special_hooks override everything; on the Dreamcast, without hooks, the
 * effects play through dreamcast/game/sound with the "ss" bank (tools/spdata.py:
 * sys SSAR 9/14/16 -> sound/ss.srb; special_init loads it with sound_load_bank)
 * and the music streams /cd/music/sNN.wav (loop points /cd/music/sys.txt),
 * the same files the menus use.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"
#ifdef _arch_dreamcast
#include "../sound/sound.h"
#include "../engine/audio.h"
#endif

void Sound_PlaySeqArc_SS(int arc, int id) {
    if (arc >= 0 && arc < 32 && id >= 0 && id < 64)
        g_sp_trace.sfx[arc][id]++;
    if (special_hooks.sfx) {
        special_hooks.sfx(arc, id);
        return;
    }
#ifdef _arch_dreamcast
    Sound_PlaySeqArc(NULL, (uint32_t)arc, (uint32_t)id);
#endif
}

void Sound_PlaySeq_SS(int seq) {
    g_sp_trace.music_last = seq;
    if (special_hooks.music) {
        special_hooks.music(seq);
        return;
    }
#ifdef _arch_dreamcast
    {
        char path[40];
        uint32_t ls = 0, le = 0;
        FILE *f = fopen("/cd/music/sys.txt", "r");
        unsigned id, a, b;
        if (f) {
            while (fscanf(f, "%u %u %u", &id, &a, &b) == 3)
                if ((int)id == seq) {
                    ls = a;
                    le = b;
                    break;
                }
            fclose(f);
        }
        snprintf(path, sizeof(path), "/cd/music/s%02d.wav", seq);
        audio_music_play(path, ls, le);
    }
#endif
}

void Sound_StopBGM_SS(int fade) {
    if (special_hooks.music_stop) {
        special_hooks.music_stop(fade);
        return;
    }
#ifdef _arch_dreamcast
    (void)fade;
    audio_music_stop();
#else
    (void)fade;
#endif
}

/* load the special-stage effect bank (Dreamcast; sound_set_dir must point at the .srb dir) */
int sp_audio_load_bank(void) {
#ifdef _arch_dreamcast
    if (special_hooks.sfx)
        return 0;
    return sound_load_bank("ss");
#else
    return 0;
#endif
}

/* NNS_SndStopSoundAll (SpecialStage_ReleaseHudAssets) */
void sp_audio_stop_all(void) {
    if (special_hooks.music_stop)
        special_hooks.music_stop(0);
#ifdef _arch_dreamcast
    else
        audio_music_stop();
    if (!special_hooks.sfx)
        sound_stop_all();
#endif
}
