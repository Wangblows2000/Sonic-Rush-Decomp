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
  lost).
- STRM: streamed from disc with `snd_stream` double buffers.
- Sound effects and voices: **done as a real-time sequenced runtime**, see
  below. The sys SDAT (menus, title, cutscenes) has no bank yet.

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
vol/128 / 2^shift × 127/128 × max(left, right) of the DS linear pan; the
weaker side is attenuated in 3 dB DIPAN steps. The music stream plays the
DS-model render at full level hard left/right, so effects and music keep the
DS level ratio (but see deviation 2).

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
2. **Volume model of the offline renders**: `NNSi_SndPlayerMain` converts the
   NNS volumes with `data_020725e0` = `SNDi_DecibelTable`
   (200·log10(v/127)), and the runtime does the same. `sseqrender.c`
   (`cmd_render`, `playerFader = tblDecibelSq[vol]`) uses the squared table
   (400·log10), so every offline render — **including the streamed music** —
   is quieter than the DS by `dbsq[vol] − db[vol]`: 0 dB at vol 127, −2.1 dB
   at 100, −4.2 dB at 80, −8.1 dB at 50. Effects from this runtime are
   therefore up to that much louder relative to the music than on the DS.
   Fix belongs in `sseqrender.c` + a re-run of `sdat2dc.py` (not done here,
   it would re-render all music).
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
8. No sys-SDAT bank (menus/title/cutscenes need group loading like
   `SysSound_LoadSEGroup`: the sys SDAT waves are 4.2 MB).

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
| `Sound_PlaySeq`, `Sound_PlayBGMEx`, `Sound_PlayBGMOnPlayer3`, `SysSound_*`, `TensionSound_*`, `StageJingle_*`, `Sound_LoadSoundArchive*`, `Sound_ResetHeap` | — | not translated (music is streamed; no sys bank) |
