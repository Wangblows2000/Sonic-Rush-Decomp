/*
 * Game sound layer, native. The Sound_* functions are reused from
 * src/game/Sound/ (same names, same logic); their NNS_Snd* calls go to the
 * native NitroSystem layer (snd_nns.c). sound_* is the port's public API
 * (sound.h). Handle storage: data_0225045c (in-use bits), data_02250468[0x20]
 * (pool), data_02250460 (fallback handle) as in the original.
 */
#include "sound.h"
#include "snd_nns.h"
#include "bgm.h"
#include <stdio.h>
#include <string.h>
#ifdef _arch_dreamcast
#include <arch/timer.h>
#endif

SoundHandle gSESoundHandle, gVoiceSoundHandle, gBGMSoundHandle;
uint8_t gSEVolume, gVoiceVolume, gBGMVolume;
static uint8_t data_0225045c[4];
static SoundHandle data_02250468[0x20];
static SoundHandle data_02250460;

static SndBank s_bank;
static char s_dir[64] = "/cd/sound";
static char s_bank_name[16];
const char *sound_bank_name(void) { return s_bank_name; }

/* trace builds: effect requests that produce no sound in the loaded bank, logged
 * once per (bank, archive, id): no record / player full ("start"), the sequence
 * ended without one note finding an instrument + wave ("silent") */
extern uint32_t g_sound_frame;
static struct { uint16_t arc, id; uint8_t why; } s_miss[256];
static int s_nmiss;
uint32_t g_sound_miss_count;
static void miss(int32_t arc, int32_t id, int why) {
    int i;
    g_sound_miss_count++;
    for (i = 0; i < s_nmiss; i++) if (s_miss[i].arc == arc && s_miss[i].id == id && s_miss[i].why == why) return;
    if (s_nmiss < 256) { s_miss[s_nmiss].arc = (uint16_t)arc; s_miss[s_nmiss].id = (uint16_t)id; s_miss[s_nmiss].why = (uint8_t)why; s_nmiss++; }
    printf("snd MISS f%lu bank %s arc %ld id %ld: %s\n", (unsigned long)g_sound_frame, s_bank_name, (long)arc, (long)id,
           why == 0 ? "start failed (no record / no bank / player full)" : "silent (no note found an instrument / wave)");
}
static void seq_end_cb(int32_t arc, int32_t id, int tries, int ok) { if (tries == 0 || ok == 0) miss(arc, id, 1); }
static void start_check(const SoundHandle *h, int32_t arc, int32_t id) { if (g_sound_trace && h->player == NULL) miss(arc, id, 0); }
static uint32_t s_frac;
static int s_init;
/* debug trace (serial): enabled when <dir>/../debug/sndtrace.txt exists on
 * the disc (/cd/debug/sndtrace.txt); logs starts/stops and frames whose
 * summed channel amplitude reaches -6 dBFS. */
int g_sound_trace;
uint32_t g_sound_frame;

/* ------------------------------------------------ src/game/Sound (reused) */
void Sound_InitHandles(void) {
    uint32_t i;
    for (i = 0; i < 0x20; i++) NNS_SndHandleInit(&data_02250468[i]);
    NNS_SndHandleInit(&data_02250460);
    memset(data_0225045c, 0, 4); /* MI_CpuFill8 */
}

SoundHandle *Sound_AllocHandle(void) {
    uint32_t i;
    for (i = 0; i < 0x20; i++) {
        uint32_t bit = 1 << (i & 7);
        if (!(data_0225045c[i >> 3] & bit)) {
            data_0225045c[i >> 3] |= bit;
            return &data_02250468[i];
        }
    }
    return &data_02250460;
}

void Sound_FreeHandle(SoundHandle *p) {
    uint32_t i;
    if (&data_02250460 == p) { NNS_SndHandleReleaseSeq(p); return; }
    for (i = 0; i < 0x20; i++) {
        if (&data_02250468[i] == p) {
            NNS_SndHandleReleaseSeq(&data_02250468[i]);
            data_0225045c[i >> 3] &= ~(1 << (i & 7));
            return;
        }
    }
}

void Sound_Init(void) {
    NNSi_SndPlayerInit();                 /* NNS_SndInit (player part) */
    /* gSoundHeap = NNS_SndHeapCreate(gSoundHeapBuffer, 0x64000): no sound heap natively */
    NNS_SndHandleInit(&gBGMSoundHandle);
    NNS_SndHandleInit(&gSESoundHandle);
    NNS_SndHandleInit(&gVoiceSoundHandle);
    Sound_InitHandles();
    gBGMVolume = 0x7F;
    gSEVolume = 0x7F;
    gVoiceVolume = 0x7F;
}

void Sound_PlaySeqArcEx(SoundHandle *a, int32_t b, int32_t c) {
    if (g_sound_trace) printf("snd f%lu play %ld h%p\n", (unsigned long)g_sound_frame, (long)c, (void *)a);
    if (a == 0) a = &gSESoundHandle;
    if (gSESoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(a);
    NNS_SndArcPlayerStartSeqArcEx(a, -1, -1, -1, b, c);
    NNS_SndPlayerSetVolume(a, gSEVolume);
    start_check(a, b, c);
}

void Sound_PlaySeqArc(SoundHandle *p, uint32_t a, uint32_t b) {
    if (g_sound_trace) printf("snd f%lu playarc %lu:%lu h%p\n", (unsigned long)g_sound_frame, (unsigned long)a, (unsigned long)b, (void *)p);
    if (p == NULL) p = &gSESoundHandle;
    if (gSESoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, (int32_t)a, (int32_t)b);
    NNS_SndPlayerSetVolume(p, gSEVolume);
    start_check(p, (int32_t)a, (int32_t)b);
}

void Sound_PlaySeqArcBankPrio(SoundHandle *a, int32_t b, int32_t c, int32_t d, int32_t e) {
    if (a == 0) a = &gSESoundHandle;
    if (gSESoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(a);
    NNS_SndArcPlayerStartSeqArcEx(a, -1, e, d, b, c);
    NNS_SndPlayerSetVolume(a, gSEVolume);
}

void Sound_PlayVoice(SoundHandle *p, uint32_t a, uint32_t b) {
    if (g_sound_trace) printf("snd f%lu voice %lu:%lu h%p\n", (unsigned long)g_sound_frame, (unsigned long)a, (unsigned long)b, (void *)p);
    if (p == NULL) p = &gVoiceSoundHandle;
    if (gVoiceSoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gVoiceSoundHandle);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, (int32_t)a, (int32_t)b);
    NNS_SndPlayerSetVolume(p, gVoiceVolume);
    start_check(p, (int32_t)a, (int32_t)b);
}

int32_t Sound_CalcPan(int32_t a, int32_t b) {
    a = (a >> 8) - (b >> 8);
    if (a < -128) return -128;
    if (a > 127) a = 127;
    return a;
}

int32_t Sound_CalcDistanceVolume(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    int32_t dy = (y1 >> 8) - (y0 >> 8);
    int32_t dx, v;
    if (dy < 0) dy = -dy;
    dx = (x1 >> 8) - (x0 >> 8);
    if (dx < 0) dx = -dx;
    dx += dy;
    if (dx > 0x200) v = 0;
    else v = (0x200 - dx) >> 2;
    return (v * gSEVolume) >> 7;
}

/* original: Sound_SetPositional(snd, a, b) with a/b objects (x @0x10, y @0x14) */
void Sound_SetPositional(SoundHandle *snd, int32_t ax, int32_t ay, int32_t bx, int32_t by) {
    NNS_SndPlayerSetVolume(snd, Sound_CalcDistanceVolume(ax, ay, bx, by));
    NNS_SndPlayerSetTrackPan(snd, 0xffff, Sound_CalcPan(ax, bx));
}

/* ------------------------------------------------ src/game/Sound SysSound_* (sys bank) */
/* gSysSound->snd[2] (SE), snd[3] / snd[4] (voice, normal / Alt) */
static SoundHandle s_sys_se, s_sys_voice[2];
void SysSound_StartSEArc(int32_t arc, int32_t seq);

/* SysSound_StartSE(n): n 0..22; chara = gGameState.unk0 (0 Sonic -> SSAR 2
 * ARC_ZONE_MAP_S, else 3 ARC_ZONE_MAP_B). Needs the "sys" bank. */
void SysSound_StartSE(int32_t n, int chara) {
    int32_t a = chara == 0 ? 2 : 3, b;
    switch (n) {
    case 0: a = 1; b = 0; break;
    case 1: a = 1; b = 1; break;
    case 2: a = 1; b = 2; break;
    case 3: a = 1; b = 3; break;
    case 4: a = 1; b = 5; break;
    case 5: a = 1; b = 0xb; break;
    case 6: a = 1; b = 4; break;
    case 7: a = 0x11; b = 0; break;
    case 8: a = 0x11; b = 1; break;
    case 9: a = 0x11; b = 2; break;
    case 10: b = 0; break;
    case 11: b = 1; break;
    case 12: b = 2; break;
    case 13: b = 3; break;
    case 14: b = 4; break;
    case 15: b = 5; break;
    case 16: b = 6; break;
    case 17: b = 7; break;
    case 18: b = 8; break;
    case 19: b = 0x10; break;
    case 20: a = 0xa; b = 0; break;
    case 21: a = 0xa; b = 1; break;
    case 22: a = 0xa; b = 2; break;
    default: return;
    }
    SysSound_StartSEArc(a, b);
}

/* tail of SysSound_StartSE for callers that already resolved (SSAR, sequence):
   stop + release the previous SE on gSysSound->snd[2], then Sound_PlaySeqArc */
void SysSound_StartSEArc(int32_t arc, int32_t seq) {
    if (s_sys_se.player != NULL) {
        NNS_SndPlayerStopSeq(&s_sys_se, 0);
        NNS_SndHandleReleaseSeq(&s_sys_se);
    }
    Sound_PlaySeqArc(&s_sys_se, (uint32_t)arc, (uint32_t)seq);
}

/* SysSound_StartVoice(n): n 0..7; alt = gSysSound->unk2C (1 inside
 * SysSound_PlayMenuVoiceAlt: second voice handle, the first keeps playing). */
void SysSound_StartVoice(int32_t n, int alt) {
    static const uint8_t tab[8][2] = {{1, 6}, {1, 5}, {1, 7}, {1, 8}, {1, 9}, {1, 0xa}, {0x11, 3}, {0x11, 4}};
    SoundHandle *h = &s_sys_voice[alt ? 1 : 0];
    if (n < 0 || n > 7) return;
    if (h->player != NULL) {
        NNS_SndPlayerStopSeq(h, 0);
        NNS_SndHandleReleaseSeq(h);
    }
    Sound_PlayVoice(h, tab[n][0], tab[n][1]);
}

/* ------------------------------------------------ native API */
void sound_set_dir(const char *dir) {
    strncpy(s_dir, dir, sizeof s_dir - 1);
    s_dir[sizeof s_dir - 1] = 0;
}

int sound_init(void) {
    if (s_init) return 0;
    snd_out_init();
    Sound_Init();
    s_init = 1;
    return 0;
}

void sound_shutdown(void) {
    if (!s_init) return;
    sound_stop_all();
    snd_bank_free(&s_bank);
    snd_out_shutdown();
    s_init = 0;
}

int sound_load_bank(const char *sdat) {
    char path[96];
    int r;
    if (!s_init) sound_init();
    sound_stop_all();
    snd_bank_free(&s_bank);
    snprintf(path, sizeof path, "%s/%s.srb", s_dir, sdat);
    r = snd_bank_load(&s_bank, path);
    s_bank_name[0] = 0;
    if (r) return r;
    snprintf(s_bank_name, sizeof s_bank_name, "%s", sdat);
    s_nmiss = 0;
    g_sound_trace = snd_out_file_size("/cd/debug/sndtrace.txt") >= 0;
    g_snd_seq_end_cb = g_sound_trace ? seq_end_cb : NULL;
    snd_drv_init(&s_bank);
    NNSi_SndPlayerInit();
    NNS_SndArcPlayerSetup(&s_bank);
    Sound_InitHandles();
    NNS_SndHandleInit(&gSESoundHandle);
    NNS_SndHandleInit(&gVoiceSoundHandle);
    NNS_SndHandleInit(&s_sys_se); NNS_SndHandleInit(&s_sys_voice[0]); NNS_SndHandleInit(&s_sys_voice[1]);
    s_frac = 0;
    return 0;
}

/* gStageZoneFileListTable (0x020745a8) [zone][act].entries[3].name: the
 * zone sound archive; the character letter is written by
 * StageLoader_SetCharacterSoundPath (path[7], acts) / ...Path8 (path[8], boss). */
const char *sound_stage_sdat(int zone, int act, int chara) {
    static const char *const tab[9][3] = {
        {"z1s", "z1s", "z1es"}, {"z2s", "z2s", "z2es"}, {"z3s", "z3s", "z3es"},
        {"z4s", "z4s", "z4es"}, {"z5s", "z5s", "z5es"}, {"z6s", "z6s", "z6es"},
        {"z7s", "z7s", "z7es"}, {"z1s", "z3s", "zfes"}, {"z4s", "z5s", NULL},
    };
    static char buf[8];
    const char *n;
    size_t len;
    if (zone < 0 || zone > 8 || act < 0 || act > 2 || !(n = tab[zone][act])) return NULL;
    len = strlen(n);
    memcpy(buf, n, len + 1);
    buf[len - 1] = chara == 0 ? 's' : 'b';
    return buf;
}

SoundHandle *sound_play(int id, int vol, int pan) {
    SoundHandle *h = Sound_AllocHandle();
    Sound_PlaySeqArcEx(h, 0, id);
    if (!h->player) { Sound_FreeHandle(h); return NULL; }
    NNS_SndPlayerSetVolume(h, vol);
    if (pan) NNS_SndPlayerSetTrackPan(h, 0xffff, pan);
    return h;
}
void sound_play_se(int id) { Sound_PlaySeqArcEx(NULL, 0, id); }
void sound_set_volume(SoundHandle *h, int vol) { if (h) NNS_SndPlayerSetVolume(h, vol); }
void sound_set_pan(SoundHandle *h, int pan) { if (h) NNS_SndPlayerSetTrackPan(h, 0xffff, pan); }
void sound_stop(SoundHandle *h, int fade_frames) { if (h) NNS_SndPlayerStopSeq(h, fade_frames); }
int sound_is_playing(const SoundHandle *h) { return h && h->player != NULL; }
void sound_release(SoundHandle *h) { if (h) Sound_FreeHandle(h); }

void sound_set_fade(uint32_t q16) { snd_drv_set_fade(q16); }

void sound_stop_all(void) {
    if (!s_init || !s_bank.meta) return;
    NNS_SndPlayerStopSeqAll(0);
    snd_drv_stop_all();
}

void sound_update(void) {
#ifdef _arch_dreamcast
    if (g_sound_trace) {   /* debug: game-loop frames longer than 17.5 ms (sound_update runs once per frame);
                              one summary line per 600 frames, serial output is slow */
        static uint64_t last;
        static uint32_t n, slow, worst, at[6];
        uint64_t now = timer_us_gettime64();
        if (last) {
            uint32_t dt = (uint32_t)(now - last);
            if (dt > worst) worst = dt;
            if (dt > 17500) { if (slow < 6) at[slow] = (uint32_t)(now / 1000); slow++; }
            if (++n == 600) {
                printf("snd frames t%lu: %lu of 600 over 17.5 ms, worst %lu us; at t%lu %lu %lu %lu %lu %lu\n", (unsigned long)(now / 1000),
                       (unsigned long)slow, (unsigned long)worst, (unsigned long)at[0], (unsigned long)at[1], (unsigned long)at[2],
                       (unsigned long)at[3], (unsigned long)at[4], (unsigned long)at[5]);
                n = slow = worst = 0; memset(at, 0, sizeof at);
            }
        }
        last = now;
    }
#endif
    bgm_update();                         /* StageJingle_Main, gravity-flip layer (bgm.c) */
    if (!s_init || !s_bank.meta) return;
    NNSi_SndPlayerMain();                 /* NNS_SndMain */
    s_frac += SND_FRAME_HZ_X10000;        /* 191.9555 driver frames per second at 60 Hz */
    while (s_frac >= 600000) {
        s_frac -= 600000;
        snd_drv_frame();
    }
    snd_drv_flush();
    g_sound_frame++;
    if (g_sound_trace) snd_drv_trace(g_sound_frame);
}
