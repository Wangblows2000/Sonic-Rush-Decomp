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

## Plan (status)

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

### 6. Verification (what was run)

- `make -C dreamcast/game/sound dc`: all six sources compile with
  `kos-cc -std=gnu99 -O2 -Wall -Wextra -Werror`.
- `make -C dreamcast/game/sound host-test` (`test/test_sound.c`, OK, 0
  failures):
  - all 31 banks load; tables consistent; every wave ≤ 65,534 samples,
    32-byte aligned, inside the blob;
  - **coverage: all 7,347 SSAR ids (31 × 237) map to a record and a bank;
    5,931 start at least one channel**; the other 1,416 are exactly the ids
    the offline render (`sdat2dc.py`) found silent (no instrument for the
    program in that SDAT, e.g. Blaze voices in Sonic SDATs, trick voices in
    boss SDATs). All 56 ids the native player/objects code passes play in
    z1s;
  - NNS layer: a 5th sound on player 3 (`max_seq` 4) stops the oldest one;
    a lower-priority start on a full player fails; `StopSeq(h, 8)` sets
    status 2 / priority 0 and frees after 8 frames; fire-and-forget calls
    release `gSESoundHandle`; finished sequences free their seq players;
    `Sound_CalcPan` / `Sound_CalcDistanceVolume`; invalid ids; stage table;
  - hooks: `g_dcp_hooks.sfx` (TRICK, GLOBAL), `g_obj_hooks.sfx`,
    `sfx_stop` (fade 0x20) produce AICA starts/updates/stops in range.
- Driver vs `sseqrender.c` (the renderer of all converted audio):
  `SND_DUMP_DIR=/tmp/sound/dump build/dreamcast/audio/sfxbank/test_sound …` then
  `python3 tools/dreamcast/audio/sfxbank_check.py --dump /tmp/sound/dump --sdat z1s`:
  203 ids, **57 bit-exact** (PSG/noise-only effects), SNR median 42.2 dB,
  min 16.0 dB (sample-based effects: the runtime uses the AICA-ADPCM waves,
  sseqrender the DS-decoded PCM). So sequencing, envelopes, sweeps, LFO and
  channel allocation are the same.
- Flycast (`test/build_cdi.sh` → `build/dreamcast/audio/sfxbank/sndtest.cdi`,
  recorded with `fcrec.py`, section 7): budget numbers above; 14 triggers
  over 15 s; onsets in the recording are at the logged times (±10 ms);
  50 ms envelopes of SE_Ring, SE_Spin, SE_TMAX_bonus match the reference
  render within ~1 dB down to −38 dB; the held SE_Grind fades from −22 to
  −48 dBFS in ~250 ms after `sound_stop(h, 16)`; the 5-ring burst (player 7,
  cap 4 — the cap itself is checked by the host test) plays and ends. The only
  clipping is the 4 simultaneous full-level rings (DS mix would also exceed
  full scale there).
- Not verified: real hardware; the whole game with sound (main.c wiring is
  left to the integrator); boss / Blaze banks beyond the host test.

### 7. Listening test in this environment

Flycast has no output-recording option (its "audio recording" is microphone
input for the Maple mic). It works through ALSA: the predefined alsa-lib PCM
`file` (`/usr/share/alsa/alsa.conf`, slave `null`) writes the stream to a file:

```sh
export HOME=/root DISPLAY=:99 LIBGL_ALWAYS_SOFTWARE=1
pgrep -x Xvfb >/dev/null || (Xvfb :99 -screen 0 1024x768x24 &)
/home/claude/dcwork/flycast/build/flycast \
    -config config:Debug.SerialConsoleEnabled=yes \
    -config audio:backend=alsa \
    -config "alsa:device=file:/tmp/rec.raw,raw" \
    build/dreamcast/game/srdc.cdi > /tmp/serial.log 2>&1 &
sleep 30; kill %1
```

`/tmp/rec.raw`: S16LE stereo 44,100 Hz in emulated time (no sound card is
needed; the null slave never blocks, so emulation runs ~5× real time: 30 s
of wall time = ~150 s of audio). Helper:
`tools/dreamcast/audio/fcrec.py record DISC SECONDS OUT.raw`,
`fcrec.py wav OUT.raw OUT.wav`, `fcrec.py onsets OUT.raw`,
`fcrec.py compare OUT.raw ONSET_MS reference.wav` (envelope + spectrogram
match against a render).

### 8. Deviations and known issues

1. **Driver update rate**: the DS driver runs at 191.96 Hz; here 3–4 driver
   frames run back to back once per 60 Hz frame and the AICA registers are
   written once per frame. Notes inside a sequence start on 16.7 ms
   boundaries (DS: 5.2 ms) and pitch/volume envelopes move in 60 Hz steps.
   A driver thread at ~192 Hz would remove this.
2. ~~Volume model of the offline renders~~ **fixed** (section 10):
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

## 10. Volume model, mix level and clipping (fix of 2026-11-22)

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

### 10.4 Integrator notes

- `make` of the game has no header dependencies: after this change delete
  `sound/*.o` and `engine/audio.o` (SrbHeader grew; an old `snd_bank.o`
  rejects the v2 banks and the game runs with no effects).
- Rebuild the disc with the new `build/dreamcast/audio/music/*` (stage BGM
  and sys `sNN.wav`, both re-rendered) and `sfxbank/*.srb` (v2).
- `audio_update()` must be called once per game-loop frame (it is): it now
  also marks "load over" for the deferred music start. Starting the music after
  the stage load (end of `load_stage`) and reading big files in chunks would
  shorten the start delay; not required.
- `sys.srb` now exists (section 11).

### 10.5 Remaining known issues

- ~~Music loop seam DC~~ fixed (section 12).
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

### 13.1 Measurements (Flycast, z11, hold Right; trace build)

Instrumentation (trace builds, `/cd/debug/sndtrace.txt`): `sound_update()`
measures every game-loop frame interval and prints one line per 600 frames
("snd frames: N of 600 over 17.5 ms, worst, timestamps"); `engine/audio.c`
keeps `AudioStats` (`audio_stats()`: reads, read bytes, max read ms, refills,
max refill CPU us, poll gaps, underruns) and prints one line per 10 s. Per-event
prints were removed: every serial line costs milliseconds of blocking output in
Flycast and distorted the measurement.

| build | frames > 17.5 ms | worst | cause |
|---|---|---|---|
| reader as before (16 KB `fs_read` at file offset 44 + n) | 6-8 of 120 | 41.5 ms | each read went through fs_iso9660's one-sector cache (one GD-ROM command per 2 KB); ~25-30 ms game-loop stall per read (with 64 KB reads: one 46.8 ms frame per read, at the end of the read) |
| reader at lower priority only | 3-4 of 120 | 41.3 ms | same stall |
| sector-aligned 64 KB DMA reads (this fix), main.c profiling prints on | 3 of 120 | 31.9-41.7 ms | the 4 profiling lines main.c / ui_draw.c print every 120 frames (removing them in a test copy removes these frames) |
| same, prints off | 10-11 of 600 | 19.2-19.4 ms | one ~19.2 ms frame per second |
| same, prints off, **no music at all** | 10 of 600 | 19.2 ms | baseline |
| bare KOS `vid_waitvbl` loop, no audio code (`dc_loadtest.c -DLOADTEST_NO_AUDIO`) | 6 of 361 | 19.19 ms | Flycast / KOS timing, once per second |

So the audio no longer adds any frame over 17.5 ms: what remains (one 19.2 ms
frame per second) is also there without audio, in a bare vblank loop. Zero
frames over 17.5 ms in Flycast is not reachable while that baseline exists,
and the 41 ms frames that remain in normal builds come from the periodic
profiling printf lines (4 lines / 120 frames), not from audio.

Fix in `engine/audio.c`:
- reads are whole sectors from a sector-aligned file offset into a 32-byte
  aligned block (`s_blk`, `AUDIO_CHUNK` = 64 KB), copied into the ring: one
  multi-sector DMA command per read (64 KB ~0.5 s of disc time in Flycast,
  one read per 2 s of music);
- reader thread priority `PRIO_DEFAULT + 2` (below the game loop);
- AICA ring 16 KB per channel (0.5 s halves; refill CPU max ~6 ms in the
  poll thread). 4 KB and 32 KB were also measured: no effect on frame times.

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

