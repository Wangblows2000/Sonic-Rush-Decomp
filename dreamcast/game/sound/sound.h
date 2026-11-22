/*
 * Native sound effects / voices for the Dreamcast port of Sonic Rush.
 *
 * Plays the original SSAR sound effects of the loaded SDAT bank (zone SDAT
 * "z1s" ... "zfes": SSAR 0 = Game_SE, 237 ids) the way the DS does: the
 * sequence of each id runs on the native SND driver and drives up to 16 AICA
 * channels, with the original players (per-player sequence caps from the
 * SDAT), priorities, volumes and handle semantics. Music stays on
 * engine/audio.c (streamed).
 *
 * Integration (main.c):
 *     audio_init();                      // existing: snd_stream_init (snd_init)
 *     sound_init();                      // after audio_init
 *     sound_hooks_install();             // once, BEFORE the first objects_init
 *     per stage load: sound_load_bank(sound_stage_sdat(zone, act, chara)); // "z1s"..
 *                     sound_hooks_reset_players();  // after dcp_player_create
 *     every frame, after objects_update():  sound_update();
 * Disc: /cd/sound/<sdat>.srb (build/dreamcast/audio/sfxbank/, sfxbank.py).
 */
#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

struct NNSSndHandle;
typedef struct NNSSndHandle SoundHandle;

void sound_set_dir(const char *dir);           /* default "/cd/sound" */
int sound_init(void);                          /* Sound_Init; 0 = ok */
void sound_shutdown(void);
/* Load <dir>/<sdat>.srb (stops every sound, replaces the previous bank).
 * Returns 0 on success. */
int sound_load_bank(const char *sdat);
/* SDAT name of a stage: gStageZoneFileListTable[zone][act].entries[3].name
 * with the character letter (StageLoader_SetCharacterSoundPath(8)):
 * zone 0..8 (row), act 0 / 1 / 2 = boss, chara 0 = Sonic, else Blaze.
 * Returns NULL if the table has no entry. */
const char *sound_stage_sdat(int zone, int act, int chara);

/* Play SSAR 0 sequence `id` on a pooled handle (Sound_AllocHandle);
 * vol 0..127 = NNS player volume (127 = as authored), pan -128..127 = track
 * pan offset (0 = as authored). Returns the handle (keep it to stop / move
 * the sound, give it back with sound_release), or NULL if it did not start. */
SoundHandle *sound_play(int id, int vol, int pan);
void sound_play_se(int id);                    /* Sound_PlaySeqArcEx(NULL, 0, id) */
void sound_set_volume(SoundHandle *h, int vol);/* NNS_SndPlayerSetVolume */
void sound_set_pan(SoundHandle *h, int pan);   /* NNS_SndPlayerSetTrackPan(h, 0xffff, pan) */
void sound_stop(SoundHandle *h, int fade_frames); /* NNS_SndPlayerStopSeq (fade in 60 Hz frames) */
int sound_is_playing(const SoundHandle *h);
void sound_release(SoundHandle *h);            /* Sound_FreeHandle (detaches; sound continues) */
void sound_stop_all(void);
void sound_update(void);                       /* once per 60 Hz game frame */

/* hooks adapter (sound_hooks.c): g_dcp_hooks.sfx / sfx_stop, g_obj_hooks.sfx.
 * Install before the first objects_init() (objects_shutdown restores the
 * hooks saved by objects_init). */
void sound_hooks_install(void);
void sound_hooks_reset_players(void);

/* ---- original game-layer functions (src/game/Sound/), native signatures */
extern SoundHandle gSESoundHandle, gVoiceSoundHandle, gBGMSoundHandle;
extern uint8_t gSEVolume, gVoiceVolume, gBGMVolume;
void Sound_Init(void);
void Sound_InitHandles(void);
SoundHandle *Sound_AllocHandle(void);
void Sound_FreeHandle(SoundHandle *p);
void Sound_PlaySeqArcEx(SoundHandle *a, int32_t b, int32_t c);
void Sound_PlaySeqArc(SoundHandle *p, uint32_t a, uint32_t b);
void Sound_PlaySeqArcBankPrio(SoundHandle *a, int32_t b, int32_t c, int32_t d, int32_t e);
void Sound_PlayVoice(SoundHandle *p, uint32_t a, uint32_t b);
int32_t Sound_CalcPan(int32_t a, int32_t b);
int32_t Sound_CalcDistanceVolume(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void Sound_SetPositional(SoundHandle *snd, int32_t ax, int32_t ay, int32_t bx, int32_t by);
/* SysSound_StartSE / SysSound_StartVoice (src/game/Sound), on the "sys" bank
 * (sound_load_bank("sys")): menu / zone-map / staff-roll effects and menu
 * voices. chara = gGameState.unk0 (0 Sonic, 1 Blaze: zone-map SSAR 2 / 3);
 * alt = 1 for SysSound_PlayMenuVoiceAlt (second voice handle). Each call
 * stops the previous sound on its handle (StopSeq 0 + release), as the original. */
void SysSound_StartSE(int32_t n, int chara);
void SysSound_StartSEArc(int32_t arc, int32_t seq);   /* same, (SSAR, sequence) already resolved */
void SysSound_StartVoice(int32_t n, int alt);

#endif
