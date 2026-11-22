/*
 * Game sound layer, native. The Sound_* functions are reused from
 * src/game/Sound/ (same names, same logic); their NNS_Snd* calls go to the
 * native NitroSystem layer (snd_nns.c). sound_* is the port's public API
 * (sound.h). Handle storage: data_0225045c (in-use bits), data_02250468[0x20]
 * (pool), data_02250460 (fallback handle) as in the original.
 */
#include "sound.h"
#include "snd_nns.h"
#include <stdio.h>
#include <string.h>

SoundHandle gSESoundHandle, gVoiceSoundHandle, gBGMSoundHandle;
uint8_t gSEVolume, gVoiceVolume, gBGMVolume;
static uint8_t data_0225045c[4];
static SoundHandle data_02250468[0x20];
static SoundHandle data_02250460;

static SndBank s_bank;
static char s_dir[64] = "/cd/sound";
static uint32_t s_frac;
static int s_init;

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
    if (a == 0) a = &gSESoundHandle;
    if (gSESoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(a);
    NNS_SndArcPlayerStartSeqArcEx(a, -1, -1, -1, b, c);
    NNS_SndPlayerSetVolume(a, gSEVolume);
}

void Sound_PlaySeqArc(SoundHandle *p, uint32_t a, uint32_t b) {
    if (p == NULL) p = &gSESoundHandle;
    if (gSESoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, (int32_t)a, (int32_t)b);
    NNS_SndPlayerSetVolume(p, gSEVolume);
}

void Sound_PlaySeqArcBankPrio(SoundHandle *a, int32_t b, int32_t c, int32_t d, int32_t e) {
    if (a == 0) a = &gSESoundHandle;
    if (gSESoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(a);
    NNS_SndArcPlayerStartSeqArcEx(a, -1, e, d, b, c);
    NNS_SndPlayerSetVolume(a, gSEVolume);
}

void Sound_PlayVoice(SoundHandle *p, uint32_t a, uint32_t b) {
    if (p == NULL) p = &gVoiceSoundHandle;
    if (gVoiceSoundHandle.player != 0) NNS_SndHandleReleaseSeq(&gVoiceSoundHandle);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, (int32_t)a, (int32_t)b);
    NNS_SndPlayerSetVolume(p, gVoiceVolume);
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
    if (r) return r;
    snd_drv_init(&s_bank);
    NNSi_SndPlayerInit();
    NNS_SndArcPlayerSetup(&s_bank);
    Sound_InitHandles();
    NNS_SndHandleInit(&gSESoundHandle);
    NNS_SndHandleInit(&gVoiceSoundHandle);
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

void sound_stop_all(void) {
    if (!s_init || !s_bank.meta) return;
    NNS_SndPlayerStopSeqAll(0);
    snd_drv_stop_all();
}

void sound_update(void) {
    if (!s_init || !s_bank.meta) return;
    NNSi_SndPlayerMain();                 /* NNS_SndMain */
    s_frac += SND_FRAME_HZ_X10000;        /* 191.9555 driver frames per second at 60 Hz */
    while (s_frac >= 600000) {
        s_frac -= 600000;
        snd_drv_frame();
    }
    snd_drv_flush();
}
