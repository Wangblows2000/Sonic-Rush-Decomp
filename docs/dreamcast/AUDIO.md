# Dreamcast port — audio

## What the ROM contains (measured)

- `snd/sys/sound_data.sdat` (21 MB): 33 SSEQ, 19 SSAR, 27 SBNK, 26 SWAR (6.0 MB),
  2 STRM (stereo DS IMA-ADPCM, 37,403 Hz, 174.9 s and 212.9 s).
- One SDAT per zone/boss/event (`z1s`, `z1b`, `z1es`, `z1eb`, …, `ze`, `zfes`,
  ~390 KB each: SSEQ + SBNK + SWAR).
- Music and most effects are **sequenced** (SSEQ playing SBNK instruments made
  of SWAR waves), not recorded tracks.

## Format on AICA

AICA ADPCM is Yamaha 4-bit ADPCM. KOS `snd_sfx_load` accepts WAV fmt 0x14 /
0x20 and plays it as `AICA_SM_ADPCM`. The pipeline encodes with KOS's own
`utils/wav2adpcm` (`-t`), so the encoder matches what the KOS driver expects;
decoding the output back with `wav2adpcm -f` gives the expected length and
level (6 s test: 224,418 frames, RMS 10,012).

## Overview

- Music: rendered offline to stereo ADPCM by `sseqrender.c` and streamed by
  `dreamcast/game/engine/audio.c` (option (b); tension-gauge track muting is
  lost). Volume model = the decompiled `NNSi_SndPlayerMain`; one shared port
  mix gain for music and effects (section 10).
- STRM: streamed from disc with `snd_stream` double buffers.
- Sound effects and voices: **done as a real-time sequenced runtime**, see
  below (sys SDAT banks: section 11, cutscene banks `sys_aNN`, ClearDemo banks
  `sys_a05`..`sys_a07`: section 14).
- Music selection and in-stage music events (act / character / boss / pinch /
  jingles / underwater and gravity layers / results / special stage): **done**,
  section 14 (`sound/bgm.c`, stream table from `musictab.py`).

---

## Sound effects and voices (`dreamcast/game/sound/`)

### 1. How the game plays effects (from the decompilation)

- Stage code calls `Sound_PlaySeqArcEx(handle, 0, id)` (also `Sound_PlaySeqArc`,
  `Sound_PlayVoice`, `Sound_PlaySeqArcBankPrio`). Argument 2 is the sequence
  archive (SSAR) number, always 0 in stages; `id` is the sequence index inside
  SSAR 0 of the zone SDAT. So the stage sound-ID space is **SSAR 0 of the
  loaded zone SDAT: 237 ids** (`Game_SE` / `Game_SE_13` / `Game_SE_EX` /
  `Game_SE_FS`, same names and order in every zone SDAT).
- `Sound_PlaySeqArcEx` (src/game/Sound) releases `gSESoundHandle`, re-inits
  the handle and calls `NNS_SndArcPlayerStartSeqArcEx(h, -1, -1, -1, 0, id)`,
  then `NNS_SndPlayerSetVolume(h, gSEVolume = 127)`. With -1 the player
  number, bank and player priority come from the SSAR record of the id
  (`NNS_SndArcPlayerStartSeqArcEx`): zone SDATs use bank 1 (203 ids) or 2
  (34 ids), player priority 64, channel priority 96 (106 for one id), and
  **players 3..7 round-robin by id** (`max_seq` 4 each), 8 = Sonic voices and
  9 = Blaze voices (`max_seq` 2). Boss SDATs (`z*e*`) use players 1..7 with
  `max_seq` 2/1. Player channel masks are 0 (all 16 channels).
- `StartSeqArc` sets the NNS initial volume to the record volume (50..127) and
  the driver channel priority to the record's channel priority.
- The SDAT is chosen per stage by `StageLoader_LoadZoneSoundData`:
  `gStageZoneFileListTable[zone][act].entries[3].name`
  (`/snd/z1s/sound_data.sdat`, … boss `/snd/z1es/…`, row 7 `z1s/z3s/zfes`,
  row 8 `z4s/z5s/-`), with the character letter written by
  `StageLoader_SetCharacterSoundPath` (path[7]) / `…Path8` (path[8]):
  `s` Sonic, `b` Blaze.
- Handles: player sounds use the player work handles @384 (VOICE), @38C
  (BOOST), @390 (TRICK); `NULL` means `gSESoundHandle`, which each new
  fire-and-forget call *releases* (the old sound keeps playing). Held loops
  (grind, homing charge, generators…) are stopped by
  `NNS_SndPlayerStopSeq(handle, fade)`; fades seen in src/game/Player:
  @390 0x20 (9 of 11 calls) / 0, @388 0x10 / 0, @384 0.
- Positional sounds: `Sound_SetPositional` = `Sound_CalcDistanceVolume`
  (manhattan distance, silent past 0x200 px) as player volume and
  `Sound_CalcPan` (dx px, clamped) as track pan.

### 2. Why a sequenced runtime and not one sample per id

Measured on the converted assets (`build/dreamcast/audio/sfx/`, manifest):
one zone's 237 SSAR effects rendered offline are **5.7–10.2 MB of AICA ADPCM**
(z1s: 203 distinct files, 9.9 MB; 106 of them over the 65,534-sample AICA
limit; trimming tails below −48 dB still leaves 7.9 MB), against
**1,835,136 B of free sound RAM** with the music stream. Even resampling every
effect to ~7 kHz would not fit. The DS keeps them small because they are
sequences: the waves of one zone bank are **101–146 KB** in AICA ADPCM
(22 waves + generated PSG/noise waves). So the bank holds the SSAR sequences,
the SBNK instruments and the SWAR waves, and the SH-4 runs the DS sequence
driver in real time, driving 16 AICA channels.

### 3. Offline bank tool: `tools/dreamcast/audio/sfxbank.py`

```
python3 tools/dreamcast/audio/sfxbank.py            # all 31 zone SDATs, < 1 s
  [--only z1s,z1b] [--extract extract/eur] [--audio build/dreamcast/audio]
  [--out build/dreamcast/audio/sfxbank] [--budget BYTES]
```

Inputs: `extract/eur/files/snd/<sdat>/sound_data.sdat` (SSAR, SBNK, player
info), the waves already converted by `sdat2dc.py`
(`build/dreamcast/audio/waves/` + `manifest.json`, duplicates resolved through
`same_as`), and the driver tables (`sdatlib.extract_driver_tables`) plus the
ARM9 NNS decibel table (`data_020725e0`, located structurally). PSG duty
waves (8 × 8 samples PCM16) and the noise wave (15-bit LFSR, 32,767 samples
PCM8) are generated; the pitch table is computed (same formula as
`sseqrender.c`). Nothing from the ROM goes into source.

Outputs per SDAT: `<sdat>.srb` (layout in the script header and
`snd_internal.h`), `<sdat>.json` (id → name, bank, volume, channel/player
priority, player, player max_seq; sizes), `<sdat>.rendered.txt` (index of the
offline renders, for the host test), plus `budget.json`.
`sdat2dc.py` is not modified.

### 4. Sound-RAM budget

| item | bytes | how measured |
|---|---|---|
| KOS `snd_mem` pool after `snd_init` | 1,900,544 | Flycast, `snd_mem_available()` (`test/dc_sndtest.c`) |
| music stream (`audio_music_play`, 2 × `SND_STREAM_BUFFER_MAX_ADPCM`) | 65,408 | Flycast, difference |
| free with music stream | 1,835,136 | Flycast |
| z1s bank waves | 144,800 | Flycast, difference (= `.srb` wave blob) |
| free with music + z1s bank | **1,755,744** | Flycast |
| largest bank (z1b/z2b/z2s/z5*/z6b/z6s/z7b/z7s) | 146,272 | sfxbank.py |
| smallest bank (z7eb) | 101,056 | sfxbank.py |

Main RAM per bank: 11.6–13.6 KB (sequences, instruments, tables) + driver
state ~14 KB (bss). Disc: 31 banks, 4,569,760 B total (z1s.srb 158,048 B).
Effects use 16 AICA channels (reserved with `snd_sfx_chn_alloc`), the music
stream 2; 46 remain free.

### 5. Runtime: `dreamcast/game/sound/`

| file | role |
|---|---|
| `sound.h` / `sound.c` | public API + the original `Sound_*` game layer |
| `snd_nns.h` / `snd_nns.c` | NitroSystem player layer (player caps, priorities, handles, faders) |
| `snd_drv.c` | SND driver: 16 sequence players, 32 tracks, 16 channels (envelope, LFO, sweep, portamento, allocation/stealing) |
| `snd_bank.c` | `.srb` loader (sequences/tables in main RAM, waves to sound RAM) |
| `snd_out_kos.c` | AICA output through the KOS command queue (+ direct TL writes) |
| `sound_hooks.c` | adapter for `g_dcp_hooks.sfx/sfx_stop` and `g_obj_hooks.sfx` |
| `test/` | host test, host backend, Flycast test disc |

API (`sound.h`): `sound_init()`, `sound_set_dir()`, `sound_load_bank(sdat)`,
`sound_stage_sdat(zone, act, chara)`, `sound_play(id, vol, pan)` → handle,
`sound_play_se(id)`, `sound_set_volume/pan()`, `sound_stop(h, fade_frames)`,
`sound_is_playing()`, `sound_release()`, `sound_stop_all()`,
`sound_update()`, `sound_hooks_install()`, `sound_hooks_reset_players()`,
plus the original `Sound_*` functions and `gSESoundHandle` etc.

Per game frame `sound_update()` runs `NNSi_SndPlayerMain` (NNS_SndMain), then
191.9555/60 driver frames (3 or 4), then pushes changed channel registers to
the AICA: key-on (`AICA_CH_CMD_START`), pitch/pan (`AICA_CH_CMD_UPDATE`),
volume as a direct TL register write, key-off on stop. Mapping: AICA rate =
16,756,991 / DS timer (PCM, ADPCM, PSG and noise alike); amplitude =
vol/128 / 2^shift × 127/128 × max(left, right) of the DS linear pan × the port
mix gain from the bank header (section 10); the weaker side is attenuated in
3 dB DIPAN steps. The music stream plays the render (DS model × the same mix
gain) at full level hard left/right, so effects and music keep the DS level
ratio.

#### Wiring into `src/main.c`

```c
#include "../sound/sound.h"
/* main(), after audio_init(): */
sound_init();
sound_hooks_install();              /* before the first objects_init() */
/* load_stage(), after level_load() and dcp_player_create():
   k_stages[idx] = "z<zone><act>" */
sound_load_bank(sound_stage_sdat(k_stages[idx][1] - '1', k_stages[idx][2] - '1', g_chara));
sound_hooks_reset_players();
/* game loop, after objects_update() (next to audio_update()): */
sound_update();
```

Makefile `OBJS += sound/snd_bank.o sound/snd_drv.o sound/snd_nns.o
sound/sound.o sound/sound_hooks.o sound/snd_out_kos.o` (needs `-lm`, already
linked). `objects_init` saves `g_dcp_hooks` and `objects_shutdown` restores the
saved copy, so the hooks must be installed before the first `objects_init`
(re-calling `sound_hooks_install()` after a stage change is also safe).

Disc: `/cd/sound/<sdat>.srb` — copy `build/dreamcast/audio/sfxbank/*.srb`
(at least the SDATs of the stages on the disc; z1s…z7s, z*b for Blaze,
z*es/z*eb for bosses). For `build_disc.sh`:
`mkdir -p "$OUT/disc/sound"; cp "$ROOT"/build/dreamcast/audio/sfxbank/*.srb "$OUT/disc/sound/"`.

### 8. Deviations and known issues

1. **Driver update rate**: the DS driver runs at 191.96 Hz; here 3–4 driver
   frames run back to back once per 60 Hz frame and the AICA registers are
   written once per frame. Notes inside a sequence start on 16.7 ms
   boundaries (DS: 5.2 ms) and pitch/volume envelopes move in 60 Hz steps.
   A driver thread at ~192 Hz would remove this.
2. Volume model of the offline renders (section 10):
   `sseqrender.c` now runs the decompiled `NNSi_SndPlayerMain` with the
   linear table `data_020725e0`.
3. The 16 effect channels are exclusive (on the DS, BGM sequences compete
   for the same 16 channels and can steal from effects).
4. AICA interpolates between samples (DS: no interpolation), so PSG square
   edges are slightly softened; DIPAN gives 3 dB pan steps instead of the
   DS linear pan; TL has 0.375 dB steps.
5. Key-on volume goes through the KOS 8-bit linear volume (coarse below
   −30 dB) for one frame; later volume changes are exact TL writes.
6. `sfx_stop` hook carries no fade: VOICE/BOOST use 0, TRICK 0x20 (most
   frequent original values). The player's volume shift `0x7f >> unk3D0`
   and object positional handles are dropped by the player/objects modules,
   so object sounds play centred at full volume.
7. Faithful quirk kept: `NNS_SndHandleInit` only clears `handle->player`;
   if a handle is reused while its sound plays, the old sound's shutdown
   later clears the handle even though it then points at the new sound.
8. The sys-SDAT bank holds only the title / menu / zone-map / staff-roll
   sounds (section 11); cutscene / clear-demo / special-stage archives of the
   sys SDAT are not in it.

### 9. Mapping table (original → native → status)

Status: **as-is** = reused unchanged apart from types; **modified** = same
logic, DS services replaced; **rewritten** = substantially rewritten;
**new** = no original counterpart.

| original | native | status |
|---|---|---|
| `Sound_Init` | `sound.c Sound_Init` | modified (NNS_SndInit/heap → `NNSi_SndPlayerInit`) |
| `Sound_InitHandles` | `Sound_InitHandles` | as-is (MI_CpuFill8 → memset) |
| `Sound_AllocHandle` / `Sound_FreeHandle` | same | as-is |
| `Sound_PlaySeqArcEx` / `Sound_PlaySeqArc` / `Sound_PlaySeqArcBankPrio` / `Sound_PlayVoice` | same | as-is |
| `Sound_CalcPan` / `Sound_CalcDistanceVolume` | same | as-is |
| `Sound_SetPositional` | same | modified (positions passed as x/y) |
| `StageLoader_LoadZoneSoundData` + `gStageZoneFileListTable` + `StageLoader_SetCharacterSoundPath(8)` | `sound_stage_sdat` + `sound_load_bank` | rewritten |
| `NNS_SndMain` | `sound_update` | rewritten (driver frames + AICA flush) |
| `NNS_FndInitList/AppendListObject/InsertListObject/RemoveListObject/GetNextListObject` | `snd_nns.c` | new (native list) |
| `NNSi_SndFaderInit/Get/Set/Update/IsFinished` | same | as-is |
| `NNS_SndHandleInit` / `NNS_SndHandleReleaseSeq` | same | as-is |
| `InsertPlayerList` / `InsertPrioList` / `AllocSeqPlayer` / `SetPlayerPriority` | same | as-is |
| `NNSi_SndPlayerAllocSeqPlayer` / `NNSi_SndPlayerFreeSeqPlayer` / `InitPlayer` / `NNSi_SndPlayerStopSeq` / `NNS_SndPlayerStopSeqAll` | same | as-is |
| `ShutdownPlayer_snd_player` | same | modified (no player heaps) |
| `ForceStopSeq` | same | modified (SND_* → `snd_drv_*`) |
| `NNSi_SndPlayerStartSeq` | same | modified (`SND_PrepareSeq` → `snd_drv_prepare_seq`, command tag emulated) |
| `NNSi_SndPlayerMain` | same | modified (status/tags from the native driver) |
| `NNSi_SndPlayerInit` / `NNS_SndArcPlayerSetup` | same | modified (no heaps; player info from the bank) |
| `NNS_SndArcPlayerStartSeqArcEx` / `NNS_SndArcPlayerStartSeqArc` / `StartSeqArc` | same | modified (resident bank instead of archive/heap loads) |
| `NNSi_SndSeqArcGetSeqInfo` | `snd_bank_seq_info` | modified |
| `NNS_SndPlayerStopSeq` | same | modified (fade argument restored: the decompiled prototype lost r1) |
| `NNS_SndPlayerSetVolume` / `SetInitialVolume` | same | modified (clamped to 0..127) |
| `NNS_SndPlayerSetChannelPriority` / `NNS_SndPlayerSetTrackPan` | same | modified (direct driver call) |
| `NNS_SndArcInit` / `NNS_SndArcLoadGroup` (zone archive load) | `snd_bank_load` | rewritten |
| ARM7 SND driver (not in the decompilation): `SND_PrepareSeq`, `SND_StartPreparedSeq`, `SND_StopSeq`, `SND_SetPlayerVolume`, `SND_SetPlayerChannelPriority`, `SND_SetTrackPan`, `SND_SetTrackAllocatableChannel`, sequence/track/ExChannel main | `snd_drv.c` (from `sseqrender.c`: `track_step`, `play_note`, `alloc_chan`, `note_on`, `exchannel_main`, `seq_main` reused) | rewritten (multi-player, track pool, extPan, no mixer) |
| `SND_UpdateExChannel` (hardware) | `update_hw` + `hw_advance` + `snd_drv_flush` | rewritten (AICA output) |
| — | `snd_out_kos.c`, `sound_hooks.c`, `sound_play*` API | new |
| `Stage_PlayBGM`, `Stage_SelectBGMLayer`, `Stage_FadeBGMLayer`, `StageJingle_Create` / `_Main` / `_Destructor`, `Sound_PlayBGMOnPlayer3`, `Sound_PlaySeq` / `Sound_PlayBGMEx` (BGM handles), `Player_AddLives` (sound), `Player_GiveInvincibility` (sound + timers), `Player_UpdateGravityFlip` (layer + SE) | `bgm.c` | modified (NNS calls on the BGM handles -> stream engine, section 14) |
| `NNS_SndPlayerPause` / `NNS_SndPlayerPauseByPlayerNo` / `NNSi_SndPlayerPause`, ARM7 `SND_PauseSeq` | `snd_nns.c`, `snd_drv.c` | as-is / rewritten |
| `SysSound_PlayBGM*`, `SysSound_FadeBGMVolume`, `TensionSound_*`, `Sound_LoadSoundArchive*`, `Sound_ResetHeap` | — | not translated (menus call `bgm_play`; TensionSound has no native caller) |

---

## 10. Volume model, mix level and clipping

### 10.1 Volume stages (decompiled code -> renderer -> runtime)

| stage | DS code | table | `sseqrender.c` | runtime |
|---|---|---|---|---|
| NNS player volume: `db[fader>>8] + db[volume1 = SEQ/SSAR info vol] + db[volume2 = NNS_SndPlayerSetVolume (gBGMVolume / gSEVolume = 127)] + db[NNSSndPlayer volume = 127]`, clamp −723..0, `SND_SetPlayerVolume` | `NNSi_SndPlayerMain` (src/libs/NitroSystem/snd) | **linear** `data_020725e0` = SNDi_DecibelTable, 200·log10 | `nns_player_main()` (same code, once per 60 Hz frame, faders `NNSi_SndFader*`) — was `dbsq[vol]` (bug) | `snd_nns.c` `NNSi_SndPlayerMain` (already right) |
| track: `dbsq[track vol] + dbsq[expression] + dbsq[seq master vol 0xC2]` | ARM7 driver (not in the decompilation) | squared, 400·log10 | unchanged | unchanged |
| channel: `dbsq[velocity]` + envelope (sustain `dbsq`) + track + player extFader + LFO, `SND_CalcChannelVolume` | ARM7 | squared + volume table | unchanged | unchanged |
| mixer: vol/128 >> shift, pan (128−p)/128, p/128, master 127/128, clamp at 16-bit full scale | hardware (melonDS SPU::Mix gives the same scale: a centred full channel = 0.49 FS) | — | unchanged | AICA TL/DIPAN (`out_vol`) |
| port mix gain 2^(−24/16) = −9.03 dB | — | — | `--gain-tl` (new) | `.srb` header -> `out_vol` (new) |

The table is located structurally in the owner's arm9.bin
(`sdatlib.nns_decibel_table`, found at 0x020725e0 = `data_020725e0`) and is
appended to `build/dreamcast/audio/tables.bin` (offset 1048) by `sdat2dc.py`.
Held-SFX stop in the renderer now uses `NNSi_SndPlayerStopSeq` (fader -> 0 over
10 NNS frames, `ForceStopSeq` at the end) instead of a dB ramp.

Measured old -> new (render RMS, `/tmp/audiofix/tools/cmpman.py` on the two
manifests; volume fix alone, before the mix gain): change = `db[v] − dbsq[v]`
exactly: music vol 40 (z1–z7 stage BGM) **+10.06 dB**, 45 +9.0, 50 +8.1,
60 +6.5, 80 +3.9, 100 +2.1, 127 0; effects vol 100 +2.0 (4,100 entries),
110 +1.2, 120 +0.5, 127 0. With the fix the DS-level BGM renders peak up to
**+4.4 dBFS** (z5b SEQ_5blaze; z1 SEQ_1sonic +1.4 dBFS): the DS mix itself
exceeds full scale, so a 16-bit render clipped (94 music files).

### 10.2 One shared mix gain

`MIX_GAIN_TL = 24` in `sdat2dc.py` is the only definition: every render
(music, sys music, SFX renders) is scaled by 2^(−24/16) after the DS master
volume; `manifest.json` records `mix_gain_tl`; `sfxbank.py` copies it into the
`.srb` header (format v2, u32 at 0x44, header 0x48 bytes); `snd_drv.c`
`out_vol()` multiplies every effect channel by the same gain. Music and effects
keep exactly the DS ratio, with 9 dB of headroom over the DS full scale. Why
24: worst cases measured — BGM +4.4 dBFS (DS scale), the doubled SE_Spring
(see 10.3) 1.58 FS, three overlapping SE_Ring 1.8 FS; music + spring could
reach ~2.7 FS = +8.6 dB.

Final renders (`validate_audio.py`, report.json): music peak max −4.6 dBFS
(was 0.0, 3 files at full scale; now 0), RMS median −23.8 dBFS (was −23.0),
z1 SEQ_1sonic RMS −26.6 dBFS (old −27.7); SFX renders peak max −4.5 dBFS, 0 at
full scale (was 66 clipped). ADPCM SNR unchanged (music median 22.4 dB; the
Yamaha ADPCM SNR does not depend on level, checked 0/−4.5/−9/−15 dB: 23.1 dB
each). swav 3110/3110 exact, strm 2/2, loops 77/77, seams median 0.196.
`sfxbank_check.py` (runtime driver vs renderer, both with the gain): 203 ids,
SNR median 42.2 dB, min 16.0 dB (as before; bit-exact ids 57 -> 15 because the
gain is applied in float vs double: off-by-one LSB rounding).

### 10.3 Clipping in the game: causes found in Flycast

Captures of z11 (hold Right, DC X = boost every 2 s; `fcrec.py record ...
--hold Right --press s`), analysed per window / onsets / spectrogram
(`build/dreamcast/audio/validation/captures/*.png`). Trace build: an empty
`/cd/debug/sndtrace.txt` on the disc makes `sound.c`/`snd_drv.c` log every
start/stop and every frame whose summed channel amplitude is >= −6 dBFS, and
`engine/audio.c` log poll gaps, slow disc reads, underruns.

1. **Music stream fault (main cause of the full-scale bursts and of the 2 s at
   full scale)**: the stream refill read the disc inside `snd_stream_poll`.
   While the main thread loads (fs_iso9660 serialises reads) one poll took
   5.7 s and 6.1 s (z11 load; one 16 KB read waited 16.7 s in a later run).
   The AICA then replays stale ring data; its ADPCM decoder state (previous
   sample, step, no leak) no longer matches the encoder, so the error stays:
   full-scale noise for seconds and a permanent DC offset (up to +0.75 FS on R
   in one run; the original capture already had DC ~1000 LSB). Fix in
   `engine/audio.c`: reader thread -> 384 KB RAM ring (12 s) -> poll thread
   (callback copies from RAM, never touches the disc); the stream starts when
   the ring holds 128 KB and either `audio_update()` ran since
   `audio_music_play()` (the game loop runs again, load over) or the disc is
   idle (last 16 KB read < 300 ms: menus, whose loop does not call
   `audio_update`). If it still runs dry, the stream stops (silence) and
   restarts from the top of the file (the only point where the decoder reset
   state matches the encoder). `audio_stream_underruns()` counts them.
2. **Effect level**: the DS mix has no headroom; with the volume fix effects
   and music sum above full scale. Faithful DS behaviour adds to it:
   `Player_Action_SpringLaunch` calls `Player_Action_Launch` (which plays
   SE_Spring on @384) and then plays SE_Spring again on @384, and
   `NNS_SndHandleInit` only detaches the first one, so two in-phase copies
   play (+6 dB); ring bursts start 3 SE_Ring within 10 frames. Handled by the
   mix gain (10.2), not by changing the original logic.

| capture | build | peak | clipped samples | DC L/R | notes |
|---|---|---|---|---|---|
| `rec0s` (before) | shared tree 09:0x, old levels | 32762 | 4437 (50–55 s, 60–65 s) | 1054/−1003 | 5 s windows RMS/peak: 40 s −16.5/−4.5, 50 s −12.4/0.0, 60 s −12.5/0.0 |
| `rec0t7` | volume fix, no gain, old stream | 32762 | 26660 | 1241/855 | stream burst from 33 s (L full scale ~20 s) |
| `rec1t2` | gain, old stream | 32762 | 15484 | R +0.75 FS | stream bursts 23/34–38 s, permanent DC |
| `rec3` (after) | gain + ring stream | **18939 (−4.8 dBFS)** | **0** | 30/24 | 99 s, player hit/died/respawned as in rec0s; windows −30.9…−20.4 dBFS RMS, peak ≤ −4.8 |
| `rec3b` (after, X+A) | same | 20155 (−4.2 dBFS) | 0 | 32/25 | 102 s |

Onsets (10 ms RMS +9 dB over the previous 300 ms): rec0s 20.09 (music),
42.16, 44.17, 50.18, 52.14, 54.14, 56.89, 58.53, 59.87, 62.76, 65.48 s;
rec3 20.71, 47.44, 50.25, 60.27, 66.02, 68.77, 83.07 s. Music level in rec3
(first 15 s) −29.35 dBFS vs the render −29.19 dBFS (L/R within 0.2 dB).
Trace (rec3t): summed effect amplitude max 41,557/65,535 q16 (−3.9 dBFS before
music), 0 underruns, ring low-water 65,664 B.

### 10.5 Remaining known issues

- Real hardware not tested; Flycast's GD-ROM is slow (~120 KB/s measured
  for 16 KB reads), real drives are faster, so starvation there is less
  likely, but the ring is still the safe design.

---

## 11. Sys bank (`sys.srb`, menus / title / zone map)

`sfxbank.py` builds `sys.srb` with a selection (`SELECT["sys"]`) instead of all
19 SSARs (4.2 MB of waves): SSAR 0 ARC_TITLE (6 seqs: TitleScreen_PlayStartSound
0, TitleDemo_PlaySeq1-4, TitleScreen_PlayTitleVoice 5), 1 ARC_MENU (12),
0x11 ARC_DL_PLAY (5), 2 / 3 ARC_ZONE_MAP_S / _B (seqs 0-8, 0x10), 0xa
ARC_STAFF_ROLL (0-2) = the arcs of `SysSound_StartSE` / `SysSound_StartVoice`;
other records are written empty (offset 0xffffffff, so their banks are left
out). 46 ids, banks 0/1/3/4/18/25, 196 waves: **1,254,816 B sound RAM**
(free with the music stream 1,831,040 -> 576,224 B), 16.5 KB main RAM,
file 1.27 MB. The runtime needed no change: `snd_bank_seq_info` /
`snd_bank_seq_data` and `NNS_SndArcPlayerStartSeqArc(Ex)` take any SSAR index
(stage banks only ever used SSAR 0).

Translated game layer (`sound.c`, src/game/Sound):

| original | native | status |
|---|---|---|
| `SysSound_StartSE(n)` | `SysSound_StartSE(n, chara)` (chara = `gGameState.unk0`) | modified (gSysSound->snd[2] -> static handle) |
| — (its tail) | `SysSound_StartSEArc(arc, seq)`: StopSeq(0) + release previous SE, `Sound_PlaySeqArc` | new |
| `SysSound_StartVoice(n)` | `SysSound_StartVoice(n, alt)` (alt = `gSysSound->unk2C`, 1 only inside `SysSound_PlayMenuVoiceAlt`) | modified |
| `SysSound_LoadSEGroup` / `LoadVoiceGroup` / `NNS_SndArcLoadGroup` | `sound_load_bank("sys")` (everything resident) | rewritten |

Hooks for the front end:
- se: `SysSound_StartSEArc(arc, seq)` (the original stops the previous menu SE
  on its handle; `Sound_PlaySeqArc(NULL, ...)` lets them overlap).
- voice: `SysSound_PlayMenuVoice(i)` -> `SysSound_StartVoice(i, 0)`;
  `SysSound_PlayMenuVoiceAlt(i)` -> `SysSound_StartVoice(i, 1)` (PlayerSelect
  confirm plays voice 4 then Alt 5: both must sound). The title voice is not a
  menu voice: `TitleScreen_PlayTitleVoice` = `Sound_PlayVoice(NULL, 0, 5)`.

Verified: host test `test_sys` (all 23 SE ids x 2 characters, 8 voices,
6 title sounds start a channel; voice + Alt voice both on), render vs offline
46 ids SNR median 14.2 dB; Flycast front-end capture with trace
(`build/dreamcast/audio/validation/captures/frontend_sys.png`): no "sys sound
bank missing", calls `0:0` (title start), `1:5` cursor and `1:0` decide every
3 s; SE_M_Cursol found in the recording by correlation with its render at
every press (normalised correlation 0.58-0.70 vs 0.07 elsewhere), no clipping.

## 12. Music loop seam (encoder)

The snd_stream "long stream" ADPCM decoder keeps its state (previous sample,
step) across the loop jump. The plain encoder (wav2adpcm) encodes the loop body
for the state reached after the intro, but on the jump the decoder brings the
state reached at loop_end: the difference stays (no leak) and, because the
step differs too, grows during the pass. Measured on the old encoding of all
77 looping tracks (`sseqrender aicadec --loop LS,LE --passes 3`): mean offset
of pass 3 vs pass 1 median 359 LSB, max 21,927 (sys SEQ_SS_GAME); z1
SEQ_1sonic L channel +15,836 LSB (~half full scale) from the first wrap on,
i.e. ~112 s into the stage.

Fix: `sseqrender aicaenc IN OUT --loop LS,LE` (used by `sdat2dc.py` for all
music; same quantiser and bytes as `wav2adpcm -i -t`, checked byte-identical
without --loop):
1. the renderer's loop crossfade (2048 samples) now ends with
   `LOOP_TAIL_EXACT` = 1024 samples that equal the 1024 samples before
   loop_start;
2. the file is encoded normally (intro and first pass untouched);
3. in that tail the loop-end trajectory is joined onto the trajectory that
   leads into loop_start: first sample where both states are equal, else an
   exact meet-in-the-middle search (2+2 .. 4+4 replaced codes, inverse step
   update, hash match, least sample error), else a step chase then the same
   search; from the join on the tail uses the loop-start codes, so the state at
   loop_end equals the state at loop_start exactly.

Result: 77 / 77 looping tracks exact (`music_loop_seam_exact` in the manifest
counts, per-channel report in `adpcm_loop`); decoding 3 passes gives pass 2 and
3 bit-identical to pass 1 for all 77 (max difference 0). Cost: up to ~6 replaced
codes in the 23 ms before the wrap (z1 tail SNR 26.1 -> 26.1 dB, z4s 16.7 ->
16.6); one channel (z2s SEQ_2sonic R) needed the step chase, tail SNR 21.6 ->
18.3 dB. ADPCM SNR of the music unchanged (median 22.3 dB), validate_audio all
checks as before. `aicadec` gained `--loop LS,LE --passes N` for this check.

## 13. Frame hitches from the music reader; fades around loading

### 13.2 Loading bracket: `audio_load_begin()` / `audio_load_end()`

- `audio_load_begin()` (game thread): the poll thread ramps the stream volume
  to 0 (amplitude-linear KOS stream volume, 10 per 10 ms = ~260 ms) while the
  call fades the effects (`sound_set_fade`, 16 steps, `sound_update()` keeps
  running at ~60 Hz), then stops the effects and returns. Blocks ~0.33 s.
  The music stream keeps playing muted (AICA ring serviced by the poll /
  reader threads, track position advances); `audio_music_play()` inside the
  bracket starts the new track muted.
- `audio_load_end()`: ramps the music back to full (~260 ms, non-blocking).
- Nested pairs count (only the outermost fades). Safety: if the game loop
  calls `audio_update()` 120 times with the bracket still open, it closes
  itself (never a permanent mute). Outside the bracket nothing changes.

Verified:
- `test/dc_loadtest.c` (`sh dreamcast/game/sound/test/build_loadtest.sh OUT`):
  music + SE_Ring every 0.5 s; every 6 s a bracket around a blocking 2 MB read,
  400 ms of CPU work, and on odd cycles a music restart. Over 9 cycles: fade-out
  ~300 ms, silence through the load, fade-in ~300 ms; no click (no 10 ms block
  with a sample step over 4x the median); RMS 2.8 s before / after each load
  -24.2…-25.2 / -24.1…-25.2 dBFS (music-content variation, no level change);
  0 underruns (`captures/loadtest_fades.png`).
- In game (patched test copy of main.c): z11 -> special stage: zone music
  fades -27 -> -43 dB in ~0.35 s, silence through the ~22 s special-stage load,
  special-stage music fades in within ~0.3 s, no clicks
  (`captures/z11_to_special_fade.png`).

Where to call (main.c): `load_stage` (begin at the top, end before return);
`run_special_stage` (begin before `objects_save_event_state`, end after
`special_init`; begin before `special_result`, end after the act reload);
the menu -> stage path (begin before `load_player_model`, end after
`spawn_player`); `run_cutscene` (begin before `cutscene_start`, end right after
it, not around the playback loop); `enter_front_end` / `run_extra_zone` bank
loads.

## 14. Music selection and in-stage music events

### 14.1 How the original selects music (decompilation)

Every track is a sequence of the loaded sound archive, played on one of three handles:

| original | handle | what |
|---|---|---|
| `Stage_SetupZoneResources` -> `Stage_PlayBGM(0)` (stage create, each restart after a death, special-stage return) | acts: `gStageBGMHandleB` + `gStageBGMHandleA`; boss acts / zone >= 9: `gBGMSoundHandle` | `data_02074850[zone][act][0 / 1]` in the zone SDAT. Acts start **both** sequences; B = `[0]` for Sonic / `[1]` for Blaze, A = the other one; `Stage_SelectBGMLayer(0 / 1)` only moves their volumes (instant). Boss acts: `Sound_PlaySeq(&gBGMSoundHandle, [zone][2][0])` |
| `Player_UpdateUnderwater` (Water Palace) / `Player_UpdateGravityFlip` (Dead Line) | stage handles | `Stage_SelectBGMLayer(1)` entering water / flipping, `(0)` leaving, unless invincible |
| `Boss*_OnHit*` | `gBGMSoundHandle` | `Sound_PlaySeq(0, 0xf)` SEQ_bossW (pinch), Boss7 `0x11` SEQ_boss7W, BossF talk `0x13` SEQ_bossFW, Extra `0x14` / `0x15` SEQ_bossE / SEQ_bossEW (`ExtraBoss_ResetRound` / `_AdvancePhase`, archive `/snd/ze`) |
| `Player_GiveInvincibility` -> `StageJingle_Create(p, 0x16)` | StageJingle handle | SEQ_muteki; `Stage_FadeBGMLayer(0, 0)`: both stage layers to 0 **while they keep playing**; `StageJingle_Main`: at invincibility 0x5A the jingle fades out over 0x5A frames and the active layer fades back to `127 * 127 >> 7 = 126` over 0x5A frames (at 0: both at once) |
| `Player_UpdateUnderwater`, air left 0x2a8 frames -> `StageJingle_Create(p, 0x17)` | StageJingle handle | SEQ_brethless (drowning countdown, 11.6 s); paused while the game is frozen; surfacing (`unk3CA == 0`): stop, the layer of the water flag back over 0x3C frames |
| `Player_AddLives` -> `Sound_PlayBGMOnPlayer3(0, 0x18)` (not in VS / time attack) | `gBGMSoundHandle`, player 3 | SEQ_1up over the stage music (not ducked); in a boss act it replaces the boss music (the boss SDATs have no SEQ_1up: silence) |
| `ClearDemo_Create` / `ClearDemoTA_*` / `GameOverSeq_*` (sys archive + group loaded first) | `gBGMSoundHandle` | sys 0xf SEQ_ACT_CLEAR, 0x10 SEQ_BOSS_CLEAR, 0x11 SEQ_FINAL_CLEAR, 0xb / 0xd time-attack clears, 0xc / 0xe new record, 0x14 SEQ_GAME_OVER; SE / voices of SSAR 5 (time attack), 6 (act / boss), 7 (final) |
| `SpecialStage_StartPlay` / `_Init` / goal | `gBGMSoundHandle` | sys 0x1e SEQ_SS_GAME, 0x1f SEQ_SS_END |
| `PauseMenu_Create` / `_Destructor` | players 3..9 | `NNS_SndPlayerPauseByPlayerNo(3..9)`: effects, voices, SEQ_1up (player 3), SEQ_brethless (player 9) pause; the stage music (player 0 / 2) and SEQ_muteki (player 1) keep playing |

There is no speed-up item in Sonic Rush (item boxes: rings, 1-up, tension, invincibility,
barrier, magnet, VS items) and no speed-up sequence in any SDAT.

### 14.2 Stream table (`tools/dreamcast/audio/musictab.py`)

```
python3 tools/dreamcast/audio/sdat2dc.py      # renders (once)
python3 tools/dreamcast/audio/musictab.py     # -> build/dreamcast/audio/disc_music/ (~40 s)
```

Output = the disc's `/cd/music/`: `streams.txt` (813 entries
`sdat seq file loop_start loop_end layer player ram name`; `-` = silent in that SDAT;
plus `bgmtab zone act s0 s1` = `data_02074850` read from the owner's arm9.bin),
`st/<sdat>_<seq>.wav` (89 files: canonical renders, duplicates resolved),
`st/<sdat>_<b>_<a>.lyr` (layer pairs), `sNN.wav` + `sys.txt` (legacy sys names for the
cutscene / special-stage code, now with the duplicates SEQ_ACT_CLEAR 15, SEQ_BOSS_CLEAR 16,
SEQ_BOSS_TALK 22 that the old build_disc snippet skipped). 246 MB.

Layer pairs: z2s, z2b (Water Palace) and z7s, z7b (Dead Line) render both layers (same
SSEQ file, other bank). The renderer had found a different loop start for each layer
(z2s 633407 / 635794, z7s 12276 / 16368; same loop length), so musictab re-renders them,
extends the shorter one by its own period (`x[t] = x[t - P]`), encodes both with one loop
(`aicaenc --loop`, seam exact for all 8 layers) and interleaves 32 KB blocks of each layer
(`.lyr`, 2048-byte header). All other zones have one audible layer (the other sequence is
silent in that SDAT, as on the DS).

Stage music (host test `test_bgm`, from the table):

| zone | act 1 / 2 Sonic | Blaze | boss Sonic / Blaze |
|---|---|---|---|
| 1 Leaf Storm | z1s_00 (SEQ_1sonic) | z1b_01 (SEQ_1blaze) | z1es_14 / z1eb_14 SEQ_boss |
| 2 Water Palace | z2s_02_03.lyr (B = layer 0 SEQ_2sonic, underwater = SEQ_2blaze) | z2b_02_03.lyr (B = layer 1 SEQ_2blaze, underwater = SEQ_2sonic) | SEQ_boss (= z1eb_14 render) / z2eb_14 |
| 3 Mirage Road | z3s_04 | z3b_05 | z3es_14 / z3eb_14 |
| 4 Night Carnival | z4s_06 | z4b_07 | z4es_14 / z4eb_14 |
| 5 Huge Crisis | z5s_08 | z5b_09 | SEQ_boss (z3es / z3eb renders) |
| 6 Altitude Limit | z6s_10 | z6b_11 | z6es_14 / SEQ_boss (z1eb render) |
| 7 Dead Line | z7s_12_13.lyr (gravity = SEQ_7blaze) | z7b_12_13.lyr | SEQ_boss7 z7eb_16 (both) |
| 8 (z81 / z82, zf3) | z1s_00 / z3s_04 | z1b_01 / z3b_05 | final boss zfes_18 / zfeb_18 SEQ_bossF |
| 9 (z91 / z92, Extra boss) | z4s_06 / z5s_08 | z4b_07 / z5b_09 | Extra boss ze_20 SEQ_bossE (pinch ze_21) |

Events: pinch z?es_15 / z?eb_15 (SEQ_bossW), Boss7 0x11 z7eb_17, BossF 0x13 zf?_19,
muteki / brethless / 1up = seq 22 / 23 / 24 of the zone SDAT (RAM jingles), results:
sys_11 (act / TA act clear, one render), sys_13 (boss / TA boss clear), sys_17 (final),
sys_12 / sys_14 (new record), sys_20 (game over), sys_30 / sys_31 (special stage).

### 14.3 Runtime

- `sound/bgm.c` (new): `Stage_PlayBGM`, `Stage_SelectBGMLayer`, `Stage_FadeBGMLayer`,
  `StageJingle_Create` / `_Main` (from `sound_update`, every frame incl. pause),
  `Sound_PlayBGMOnPlayer3`, `Player_AddLives` (sound), `Player_GiveInvincibility` (SE 0x3d,
  jingle, `invinc = 0x3e7`, `unk3CA = 0`; EffInvincibility not translated), the music / SE
  half of `Player_UpdateGravityFlip` (the player module flips gravity; bgm polls
  `grav_rot`: layer + SE 0x66 on the voice handle), `bgm_play(sdat, seq)` = Sound_PlaySeq,
  pause by player number, StopSoundAll, hooks for ui / boss / special / objects.
  Debug: `/cd/debug/bgmtest.txt` triggers (`frame command arg`: layer, jingle, oneup,
  invinc, seq, ui, pause, stopall, air, inv) for captures.
- `sound_hooks.c`: also chains `g_dcp_hooks.add_life` (1-up), `g_dcp_hooks.restart`
  (Stage_PlayBGM again after a death, not in VS / pf2 & 0x400000) and `g_obj_hooks.item`
  (OBJ_ITEM_INVINCIBLE).
- `snd_nns.c` / `snd_drv.c`: `NNSi_SndPlayerPause`, `NNS_SndPlayerPause`,
  `NNS_SndPlayerPauseByPlayerNo`, `SND_PauseSeq` (channels released at rate 127, the
  sequence stops ticking).
- `engine/audio.c`:
  - disc music: WAV or `.lyr` (2 layers, lockstep reader, per-layer ring 192 KB, per-layer
    NNS-style fader `audio_music_layer_volume(layer, vol, frames)`, `audio_music_fade_stop`).
    The two layer streams are keyed on back to back (KOS 2.3 asserts when two stereo
    stream starts are queued: `snd_stream_start_type` takes the queue mutex that queueing
    keeps), measured 6.3-12.8 ms apart in Flycast; `layer_resync()` then keys the four
    channels (predicted with `snd_sfx_chn_alloc`, checked: keyed on, ADPCM long stream)
    off and on with one sync key-on: both restart from file position 0 together.
  - RAM jingles: 2 slots; the cache (4 files) is read at stage start inside the load
    bracket (whole-file sector-aligned read: z2 set 500 KB in 267 ms; the first version
    read from byte 44 through the one-sector cache and added ~4 s to the z21 load);
    the reader thread (low priority) decodes ADPCM into a 0.5 s PCM16 ring, the poll
    thread only copies it to a PCM16 stream (callback max 101 us; decoding in the callback
    took 4.7 ms per refill). Pause keeps the position (decoder checkpoints every 4096
    frames); fades, end detection, loop with the exact seam state.
- Hooks for main.c (patch, 14.5): `g_boss_hooks.music / music_stop`, `g_ui_hooks.music /
  music_pause / sfx`, `special_hooks.music / music_stop`, `obj_effect_hook` default ->
  `bgm_obj_effect` (OBJ_FX_BGM_LAYER, OBJ_FX_JINGLE), `bgm_stage_start` in `load_stage`
  (replaces `start_zone_music`), `bgm_bind_player` in `spawn_player`, `bgm_stage_end` +
  `bgm_set_context` in `enter_front_end`, `run_special_stage`, `run_extra_zone`, menus'
  `menu_bgm` -> `bgm_play("sys", seq)`.
- ClearDemo banks: `sfxbank.py` builds `sys_a05`, `sys_a06`, `sys_a07` (sys SSAR 5 / 6 / 7
  alone: 161 / 158 / 362 KB sound RAM); `bgm_ui_music_hook` loads the one of the clear
  music before playing it (as ClearDemo_Create loads its group). Every ClearDemo SE / voice
  id the ui requests exists (SSAR 6 voice 7 SE_Boss_Watashi is silent in the sys SDAT).

### 14.5 Integration (main.c)

- `main.c` calls `bgm_set_context` / `bgm_bind_player` when a stage is set up,
  `bgm_stage_start` after the load (it also prefetches the act-clear bank and track),
  `bgm_stage_end` when the stage ends, and routes object music events through
  `bgm_obj_effect`; `bgm_play` / `bgm_stop_sound_all` serve the menus, cutscenes and
  bosses.
- `build_disc.sh` copies `build/dreamcast/audio/disc_music/` (stream table `streams.txt`
  and the renders) to `/cd/music/`; run `tools/dreamcast/audio/musictab.py` and
  `sfxbank.py` first.
- Test discs: `SRDC_SNDTRACE=1` and `SRDC_BGMTEST=<file>` (BUILD.md §4).

### 14.6 Known gaps

- Channel competition is not modelled: on the DS a jingle (SEQ_1up channel priority 70)
  steals voices from the stage BGM (priority 64) and effects compete with music for the
  16 channels; here music streams and effects never interfere.
- The ARM7 driver pause behaviour (release rate 127) is from the SDK sources' behaviour as
  understood; not compared with a DS capture.
- `bgm_boss_music_stop_hook` = `NNS_SndPlayerStopSeqAll(0)` (music + effects) as boss.h
  documents; boss7 maps `NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0x3c)` onto the same hook
  (the 60-frame fade and the effects' survival are lost there: boss module).
- Invincibility timers / jingle come from the item hook here; EffInvincibility (graphics)
  is not translated. TensionSound_* has no caller in the native code.
- Real hardware not tested; the layer re-sync relies on KOS allocating stream channels
  lowest-first in the poll thread (checked at run time; if the check fails the layers
  stay a few ms apart).

### 14.7 Boss music fades and channel competition

Boss fades (`boss/boss.h` `music_fade` / `music_volume`, appended to BossHooks):

| original | native | before |
|---|---|---|
| `Boss7_State_ClashFadeOutBGM`: `NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0x3c)` | `music_fade(0x3c)` | hard stop of music and effects |
| `BossFPartD8_State_SkyChargeStartFadeOut`: same, 0x3c | `music_fade(0x3c)` | hard stop |
| `Boss7PartD5_MoveState_WaitDefeatScene`: `NNS_SndPlayerMoveVolume(&gBGMSoundHandle, 0x20, 0xb4)` | `music_volume(0x20, 0xb4)` | logged only |

All other boss calls are `NNS_SndPlayerStopSeqAll(0)` (the `*_Action_Defeat`, Boss7 ClashEnd)
or effect handles, so they stay hard stops. `bgm_boss_music_fade_hook` = `bgm_stop(frames)`,
`bgm_boss_music_volume_hook` = `bgm_move_volume` (the layer / jingle fader of
gBGMSoundHandle). Host tests: test_boss7, test_bossf and test_bgm check the fades.

