/*
 * Cutscene sound: the original call sites keep their ids.
 *  - BGM (Sound_PlaySeq / NNS_SndArcStrmStart): the sys-SDAT sequences are
 *    streamed as <music>/sNN.wav with the loop points of <music>/sys.txt (the
 *    front end's files, build_disc.sh); duplicate SSEQ files (SEQ_BOSS_TALK =
 *    SEQ_CONVER_TEN) are aliased with <data>/seqalias.txt. Staff roll STRMs and
 *    the extra-ending SDAT are streamed from <music>/<name>.wav.
 *  - SE / voices (Sound_PlaySeqArc / Sound_PlayVoice): the sound module runs
 *    the sys SSAR sequences from a bank holding only the SSAR the cutscene
 *    loads (NNS_SndArcLoadGroup -> sound_load_bank("sys_aNN"), csbank.py).
 * NNS_SndPlayerStopSeq(bgm, fade) on the stream: the stream has no volume
 * ramp, so it stops when the fade would end (CUTSCENES.md section 8).
 * Host builds (CS_HOST) only log the calls.
 */
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"

static char s_return_bank[16];
static int s_bank_ssar = -1;
static int s_fade;
static int s_bgm_on;
static int s_bgm_seq = -1;

void cutscene_set_return_bank(const char *sdat) {
    snprintf(s_return_bank, sizeof(s_return_bank), "%s", sdat ? sdat : "");
}

static int seq_alias(int seq) {
    char path[160];
    u32 size;
    char *t, *p;
    int r = seq;
    snprintf(path, sizeof(path), "%s/seqalias.txt", g_cs_dir);
    t = (char *)cs_load_file(path, &size);
    if (!t)
        return seq;
    t = (char *)realloc(t, size + 1);
    t[size] = 0;
    for (p = t; p && *p; p = strchr(p, '\n') ? strchr(p, '\n') + 1 : NULL) {
        int a, b;
        if (sscanf(p, "%d %d", &a, &b) == 2 && a == seq) {
            r = b;
            break;
        }
    }
    free(t);
    return r;
}

#ifdef CS_HOST
static int s_active[CSH_COUNT];
void cs_snd_bank(int ssar) {
    if (ssar < 0) {
        if (s_bank_ssar >= 0)
            cs_log("snd bank restore %s", s_return_bank[0] ? s_return_bank : "-");
        s_bank_ssar = -1;
        return;
    }
    if (ssar == s_bank_ssar)
        return;
    s_bank_ssar = ssar;
    cs_log("snd bank sys_a%02d", ssar);
}
void cs_bgm_play(int seq) {
    s_fade = 0;
    s_bgm_on = 1;
    s_bgm_seq = seq;
    cs_log("bgm s%02d", seq_alias(seq));
}
void cs_bgm_play_file(const char *name, int loop) {
    s_fade = 0;
    s_bgm_on = 1;
    cs_log("bgm %s loop %d", name, loop);
}
static void bgm_stop_now(void) {
    s_bgm_on = 0;
    cs_log("bgm stop");
}
void cs_se(int h, int arc, int id) {
    if (h >= 0 && h < CSH_COUNT)
        s_active[h] = 1;
    cs_log("se %d/%d h%d", arc, id, h);
}
void cs_voice(int arc, int id) { cs_log("voice %d/%d", arc, id); }
void cs_snd_stop(int h, int fade) {
    if (h >= 0 && h < CSH_COUNT && s_active[h]) {
        s_active[h] = 0;
        cs_log("se stop h%d fade %d", h, fade);
    }
}
int cs_snd_active(int h) { return h >= 0 && h < CSH_COUNT && s_active[h]; }
void cs_snd_stop_all(void) {
    memset(s_active, 0, sizeof(s_active));
    cs_log("snd stop all");
}
#else
#include "../engine/audio.h"
#include "../sound/sound.h"
static SoundHandle *s_h[CSH_COUNT];

static SoundHandle *handle(int h) {
    if (h <= CSH_SE || h >= CSH_COUNT)
        return NULL;   /* NULL = gSESoundHandle, as the original call sites */
    if (!s_h[h])
        s_h[h] = Sound_AllocHandle();
    return s_h[h];
}
void cs_snd_bank(int ssar) {
    char name[16];
    int i;
    for (i = 0; i < CSH_COUNT; i++)
        if (s_h[i]) {
            Sound_FreeHandle(s_h[i]);
            s_h[i] = NULL;
        }
    if (ssar < 0) {
        if (s_bank_ssar >= 0 && s_return_bank[0])
            sound_load_bank(s_return_bank);
        s_bank_ssar = -1;
        return;
    }
    if (ssar == s_bank_ssar)
        return;
    snprintf(name, sizeof(name), "sys_a%02d", ssar);
    if (sound_load_bank(name) != 0) {
        printf("cutscene: sound bank %s missing\n", name);
        s_bank_ssar = -1;
        return;
    }
    s_bank_ssar = ssar;
    cs_log("snd bank %s", name);
}
static void play_wav(const char *path, const char *key, int keyn) {
    char p2[160];
    uint32_t ls = 0, le = 0, sz;
    char *t, *p;
    snprintf(p2, sizeof(p2), "%s/sys.txt", g_cs_music_dir);
    t = (char *)cs_load_file(p2, &sz);
    if (t && keyn >= 0) {
        t = (char *)realloc(t, sz + 1);
        t[sz] = 0;
        for (p = t; p && *p; p = strchr(p, '\n') ? strchr(p, '\n') + 1 : NULL) {
            unsigned id, a, b;
            if (sscanf(p, "%u %u %u", &id, &a, &b) == 3 && (int)id == keyn) {
                ls = a;
                le = b;
                break;
            }
        }
    }
    free(t);
    (void)key;
    if (audio_music_play(path, ls, le) != 0)
        printf("cutscene: music %s FAILED\n", path);
}
void cs_bgm_play(int seq) {
    char path[160];
    int s = seq_alias(seq);
    s_fade = 0;
    s_bgm_on = 1;
    s_bgm_seq = seq;
    snprintf(path, sizeof(path), "%s/s%02d.wav", g_cs_music_dir, s);
    cs_log("bgm %s", path);
    play_wav(path, NULL, s);
}
void cs_bgm_play_file(const char *name, int loop) {
    char path[160];
    s_fade = 0;
    s_bgm_on = 1;
    snprintf(path, sizeof(path), "%s/%s.wav", g_cs_music_dir, name);
    cs_log("bgm %s", path);
    if (audio_music_play(path, 0, 0) != 0)
        printf("cutscene: music %s FAILED\n", path);
    (void)loop;
}
static void bgm_stop_now(void) {
    s_bgm_on = 0;
    audio_music_stop();
}
void cs_se(int h, int arc, int id) {
    if (s_bank_ssar < 0)
        return;   /* no sys SSAR loaded: the ids would address the caller's bank */
    Sound_PlaySeqArc(handle(h), (uint32_t)arc, (uint32_t)id);
}
void cs_voice(int arc, int id) {
    if (s_bank_ssar < 0)
        return;
    Sound_PlayVoice(NULL, (uint32_t)arc, (uint32_t)id);
}
void cs_snd_stop(int h, int fade) {
    SoundHandle *p = handle(h);
    if (p && sound_is_playing(p))
        sound_stop(p, fade);
}
int cs_snd_active(int h) {
    SoundHandle *p = (h > CSH_SE && h < CSH_COUNT) ? s_h[h] : NULL;
    return p && sound_is_playing(p);
}
void cs_snd_stop_all(void) { sound_stop_all(); }
#endif

void cs_bgm_stop(int fade) {
    if (!s_bgm_on)
        return;
    if (fade <= 0) {
        bgm_stop_now();
        s_fade = 0;
    } else if (!s_fade || fade < s_fade) {
        s_fade = fade;
    }
}
void cs_bgm_update(void) {
    if (s_fade > 0 && --s_fade == 0)
        bgm_stop_now();
}
int cs_bgm_playing(void) { return s_bgm_on; }
void cs_snd_shutdown(void) {
    if (s_fade) {
        bgm_stop_now();
        s_fade = 0;
    }
    cs_snd_bank(-1);
}
