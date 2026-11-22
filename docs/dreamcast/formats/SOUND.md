# Sound: SDAT formats and the Dreamcast audio converter

This page covers the Nintendo DS sound archives in Sonic Rush (`files/snd/*/sound_data.sdat`),
how the NitroSDK sound driver plays them, and how `tools/dreamcast/audio/` turns them into
Dreamcast AICA ADPCM. Every measured number on this page comes from the EUR ROM
(`extract/eur`, run of 2026-11-22). The ROM supplies all audio: the converter reads
`extract/eur` and writes only to `build/dreamcast/audio/`. No ROM data is in the source tree.

## 1. What the ROM contains

There are 33 SDATs: `sys`, `extra_end`, and `z{1..7}{s,b,es,eb}`, `ze`, `zfes`, `zfeb`.

| SDAT | SEQ entries | SSAR (sequences) | SBNK | SWAR (waves) | STRM |
|---|---|---|---|---|---|
| `sys` | 37 (33 distinct files) | 19 archives | 27 | 26 | 2 (stereo IMA-ADPCM, 37,403 Hz) |
| `z1s` and other zone SDATs | 25 entries; usually 2 to 4 real songs, the rest are 40-byte dummy sequences | 1 (`Game_SE*`, 237 sequences) | 3 or 4 | 2 (`WAVE_BGM`, `WAVE_ZONEn`) | 0 |
| `extra_end` | 1 | 0 | 1 | 1 | 0 |

- **Zone naming.** `zNs` and `zNb` hold act music (`SEQ_Nsonic`, `SEQ_Nblaze`). `zNes` and `zNeb` hold boss music (`SEQ_boss`, `SEQ_bossW`). Each zone SDAT also has `SEQ_muteki` (invincibility), `SEQ_1up` and `SEQ_brethless`. Leaf Storm is zone 1: `z1s/SEQ_1sonic` and `z1b/SEQ_1blaze`.
- **Totals.** 813 SEQ entries, 7,512 SSAR sequences, 3,695 SWAV waves (74 distinct SWARs, all IMA-ADPCM) and 2 STRMs.
- **Players.** SEQ info entries use player 0 (`PLAYER_BGM`). All PLAYER channel masks are 0, which means "all channels".

## 2. File formats (little-endian)

The offsets below were checked against the ROM by `sdatlib.py` and `sseqrender.c`. The independent parsers (ndspy and loveemu's decoders) agree with them; see section 6.

### SDAT
- Header: `"SDAT"`, BOM, version, file size, header size, block count. The block table follows at 0x10: SYMB offset and size, INFO offset and size, FAT offset and size, FILE offset and size (u32 each).
- **SYMB** (optional, present here): 8 u32 record offsets relative to SYMB, for SEQ, SEQARC, BANK, WAVEARC, PLAYER, GROUP, PLAYER2 and STRM. Each record is a u32 count followed by u32 name offsets.
  - The SEQARC record differs: count, then pairs (archive name offset, sub-record offset). Each sub-record holds the names of that archive's sequences.
  - The converter takes every output name from SYMB.
- **INFO**: 8 record offsets with the same order and count+offset layout as SYMB. Entries:
  - SEQ: u16 file ID, u16, u16 bank, u8 volume, u8 channel priority, u8 player priority, u8 player, 2 bytes padding.
  - SEQARC and WAVEARC: u16 file ID.
  - BANK: u16 file ID, u16, u16 wave archive[4] (0xFFFF = none).
  - PLAYER: u8 max sequences, u8, u16 channel mask, u32 heap size.
  - STRM: u16 file ID, u16, u8 volume, u8 priority, u8 player.
- **FAT**: `"FAT "`, size, count, then 16-byte entries (u32 offset, u32 size, 8 bytes padding). Offsets are relative to the SDAT start.

### SSEQ / SSAR
- **SSEQ**: `"SSEQ"` header (0x10 bytes), then `"DATA"`. The u32 at 0x18 is the offset of the command data from the file start. Track 0 starts at the data start.
- **SSAR**: same layout, with the data offset at 0x18 and a sequence count at 0x1C. Records of 12 bytes start at 0x20:
  - u32 offset into the data (0xFFFFFFFF = empty), u16 bank, u8 volume, u8 channel priority, u8 player priority, u8 player, 2 bytes padding.
  - All sequences of an archive share one command stream.

### SSEQ command set (as implemented in `sseqrender.c`)

Variable-length numbers ("VLQ") are 7 bits per byte, high bit = more. Prefix `A0` makes the last argument a random value: s16 min and s16 max follow. Prefix `A1` makes the last argument a variable: a u8 index follows. Prefix `A2` executes the next command only if the compare flag is set.

| cmd | args | meaning |
|---|---|---|
| 00-7F | u8 velocity, VLQ length | Note (key = cmd + transpose). Length 0 = no length (plays until the wave ends or the note is released). With note-wait on, the track waits `length` ticks; for length 0 it waits until its channels end. |
| 80 | VLQ | Rest. |
| 81 | VLQ | Program (instrument) number. |
| 93 | u8 track, u24 offset | Open a track (it must be allocated by `FE`). |
| 94 | u24 | Jump. A backward jump at call depth 0 is a song loop. |
| 95 | u24 | Call. The stack has 3 levels, shared with loops. |
| B0-BD | u8 var, s16 | Variable ops: set, add, sub, mul, div, shift, rand, (B7 unused), compare ==, >=, >, <=, <, != (sets the compare flag). Variables 0-15 belong to the player; 16-31 are global. Initial value -1. |
| C0 | u8 | Pan (64 = centre). |
| C1 | u8 | Track volume. |
| C2 | u8 | Player (master) volume. |
| C3 | s8 | Transpose. |
| C4 | s8 | Pitch bend. ndspy calls this "portamento". |
| C5 | u8 | Bend range (semitones). |
| C6 | u8 | Track priority. |
| C7 | u8 | Note-wait: 1 = "mono" (notes wait), 0 = "poly" (only rests advance). |
| C8 | u8 | Tie (legato). Switching releases the track's channels. |
| C9 | u8 | Portamento start key (+transpose); also turns portamento on. |
| CA-CD | u8 | LFO depth, speed, type (0 pitch, 1 volume, 2 pan), range. |
| CE / CF | u8 | Portamento on/off; portamento time. |
| D0-D3 | u8 | Attack, decay, sustain and release overrides (0xFF = use the instrument's value). |
| D4 | u8 count | Loop start; count 0 = infinite. |
| D5 | u8 | Expression. |
| D6 | u8 | Print variable (ignored). |
| D7 | u8 | Mute. |
| E0 | s16 | LFO delay (frames). |
| E1 | s16 | Tempo (BPM; 48 ticks per quarter note). |
| E3 | s16 | Sweep pitch (1/64 semitone). |
| FC | - | Loop end. |
| FD | - | Return. |
| FE | u16 mask | Allocate tracks; only valid at the sequence start. |
| FF | - | Track end. |

### SBNK
- `"SBNK"` header, `"DATA"` block, 4 × 8 bytes of wave archive links, u32 instrument count at 0x38, then u32 records at 0x3C: type in the low 8 bits, offset from the file start in the high 24 bits.
- Instrument parameters (10 bytes): u16 wave index (PSG: duty 0-7), u16 wave archive slot (0-3, mapped through INFO BANK), u8 root key, u8 attack, u8 decay, u8 sustain, u8 release, u8 pan.
- Types:
  - 1 = PCM (wave), 2 = PSG square, 3 = noise, 4 = direct PCM.
  - 16 = drum set: u8 low key, u8 high key, then per key a u16 type and 10 bytes of parameters.
  - 17 = key split: 8 bytes of upper key limits, then per region a u16 type and 10 bytes of parameters.
  - 0 = none.

### SWAR / SWAV
- **SWAR**: `"SWAR"` header, `"DATA"` block, 32 bytes, u32 wave count at 0x38, u32 offsets (from the file start) at 0x3C.
- **Wave header (12 bytes)**: u8 format (0 PCM8, 1 PCM16, 2 IMA-ADPCM), u8 loop flag, u16 rate, u16 timer, u16 loop start (32-bit words), u32 loop length (words). Data follows.
- **Playback rate.** The hardware plays at 16,756,991 / timer Hz, and the converter uses `timer`. The `rate` field is a rounded copy.
- **Loop points in samples.**
  - PCM8: start = loop start × 4, total = (loop start + loop length) × 4.
  - PCM16: × 2.
  - IMA-ADPCM: the first word is the ADPCM header (s16 sample, u8 step index), so start = (loop start − 1) × 8 and total = (loop start + loop length − 1) × 8.
- **IMA-ADPCM decoding (DS variant).** Nibbles are read low first. diff = step/8 + bits. The sample is clamped to ±0x7FFF (−0x7FFF, not −0x8000). Hardware restores the decoder state saved at the loop start, so a fully decoded wave loops exactly.

### STRM
- `"STRM"` header, then `"HEAD"` at 0x10 with these fields at the given offsets from HEAD:
  - +8 u8 type, +9 u8 loop flag, +10 u8 channels, +12 u16 rate
  - +16 u32 loop start, +20 u32 sample count, +24 u32 data offset, +28 u32 block count
  - +32 u32 block length, +36 u32 samples per block, +40 u32 last block length, +44 u32 last block samples
- `"DATA"` holds the blocks, interleaved per channel. Each IMA-ADPCM block carries its own 4-byte header.
- In this ROM both streams have the loop flag off.

## 3. Driver model reproduced by `sseqrender.c`

The model follows the NitroSDK ARM7 driver (SND_seq / SND_exChannel / SND_util behaviour as documented by the community) and GBATEK. The driver runs on the ARM7, which is not decompiled in this repo. The ARM9 side (`src/libs/NitroSDK/snd`, `src/libs/NitroSystem/snd`) only posts commands, apart from `SND_CalcChannelVolume`, whose table the converter reads from `arm9.bin`.

**Timing**
- One driver frame = 2728 × 64 cycles at 33.513982 MHz = 191.96 Hz. The mixer rate is 33,513,982 / 1024 = 32,728.5 Hz, so one frame is exactly 170.5 output samples. Output files are written at **32,728 Hz** (0.0015 % slow).
- Per frame, in order: (1) apply the channel registers computed in the previous frame (start, stop, volume, pan, timer); (2) sequencer; (3) envelope, LFO and sweep, then compute new registers; (4) mix.
- Tempo: counter += tempo each frame; one tick per 240. The counter starts at 240.

**Tracks**
- Defaults: volume 127, expression 127, pan centre, bend range 2, priority 64, note-wait on, portamento key 60, LFO depth 0 / speed 16 / range 1 / pitch.
- Every tick each channel's length counts down. At 0 the channel is released and its priority drops to 1.
- Each frame the track pushes these into its channels (frozen once a channel is releasing):
  - volume (dB) = DecibelSquare[vol] + DecibelSquare[expr] + DecibelSquare[player vol]
  - pitch = bend × range × 64 / 128
  - pan = pan scaled by pan range
  - LFO parameters
- The SEQ or SSAR record's INFO volume becomes the player fader (DecibelSquare[vol]), like NNS's initial volume.

**Channels**
- 16 channels: PCM on any channel, PSG on 8-13, noise on 14-15.
- Allocation scans the driver's channel order table. It picks the lowest priority; on a tie, the quietest. It steals the channel if the new priority ≥ the old one. Channel priority = player priority + track priority.
- Envelope (units of dB/10 × 128; floor −92544):
  - attack: a = −((−a × k) >> 8), with k = 255 − attack (attack < 109) or the attack table;
  - decay and release subtract the rate: 127 → 0xFFFF, 126 → 0x3C00, < 50 → 2x+1, else 0x1E00 / (126 − x);
  - sustain level = DecibelSquare[sustain] << 7.
- Total dB = DecibelSquare[velocity] + envelope + track dB + fader + LFO (volume type). A releasing channel dies when the total dB ≤ −723.
- The register volume and shift come from the channel volume table (0..127, shift 0/1/2/4).
- Pitch in 1/64 semitone = (key − root) × 64 + sweep + bend + LFO. timer = base × 2^(−pitch/768), with the driver's octave and shift arithmetic, clamped to 0x10..0xFFFF. PSG timers are masked & ~3.
- PSG and noise use base timer 8006 (C4 when key = root). The pitch table could not be located in `arm7.bin` and is computed (see the gaps in section 7).
- LFO (per frame): counter += speed << 6 with a 128-step sine. value = sin × depth × range; pitch and pan use × 64 >> 14, volume uses × 60 >> 14. The LFO delay counts frames.
- Portamento and sweep:
  - start offset = sweep pitch + (portamento key − key) × 64;
  - with portamento time 0 the length is the note length, counted in ticks;
  - otherwise the length is time² × |offset| >> 11, counted in frames.

**Mixer**
- Zero-order hold: no interpolation, exactly like the DS. A channel advances 512/timer samples per output sample.
- Each sample is scaled by vol/128 >> shift, then split L = (128 − pan)/128 and R = pan/128.
- The 16 channels are summed and scaled by master volume 127/128, then clipped like the DS 10-bit output: one full-scale channel panned hard to one side reaches full scale.
- PSG: low for 7 − duty steps, then high for duty + 1 of 8 steps (duty 7 = always low), at ±0x7FFF.
- Noise: 15-bit LFSR, X >>= 1, XOR 0x6000 on carry.

**Driver tables come from the ROM, not the source.** `sdatlib.extract_driver_tables()` finds them structurally and passes them to the renderer (`build/dreamcast/audio/tables.bin`):
- DecibelSquare table: `arm7.bin` +0xF4EC.
- LFO sine table: `arm7.bin` +0xF4C8.
- Channel order: `arm7.bin` +0xF5F0.
- Attack table: `arm7.bin` +0xF600.
- Channel volume table: arm9 0x020726E0, which is `data_020726e0` used by `src/libs/NitroSDK/snd/SND_CalcChannelVolume.c`.

## 4. Third-party tool evaluation

All five tools were cloned or installed into `/tmp/audio/ext` (not vendored) and their licence files were read. No third-party code is used in the converter.

| Tool | Licence | Formats | Renders SSEQ to PCM faithfully? | Automation | Use here |
|---|---|---|---|---|---|
| [Oreo639/sdatxtract](https://github.com/Oreo639/sdatxtract) | MIT (© 2018 Oreo639); `src/decoder/` MIT (© 2017 loveemu) | SDAT extract; SSEQ→MIDI (loveemu sseq2mid); SWAV/STRM→WAV; SBNK raw. **Ignores SSAR**: on `z1s` it wrote 25 .mid, 3 .sbnk and 83 .wav, and no SSAR output. | No (MIDI only) | Linux CLI; builds with CMake (verified) | Independent SWAV and STRM decode reference. Its `cioutil.c` `mput2l` stores negative samples wrongly (integer division); the validation harness links a corrected store. |
| [Gota7/SequenceConvert](https://github.com/Gota7/SequenceConvert) | GPL-3.0 | SSEQ/BRSEQ/BCSEQ/BFSEQ ↔ text assembly (MIDI "planned") | No | Windows .NET exe; depends on GotaSoundIO/GotaSequenceLib | Not used |
| [Gota7/NitroTools](https://github.com/Gota7/NitroTools) | GPL-3.0 | SYMB/INFO tools, Nitro Studio GUI SDAT editor with real-time playback (prebuilt LibNitro.dll by Gericom + NAudio) | Real-time GUI playback only; no offline export | Windows WinForms GUI, binary DLLs | Not used |
| ndspy 4.2.0 (pip; [RoadrunnerWMC/ndspy](https://github.com/RoadrunnerWMC/ndspy)) | GPL-3.0-or-later | Parse/save SDAT with SYMB names, SSEQ/SSAR event disassembly, SBNK, SWAR/SWAV headers, STRM container. **No** IMA-ADPCM decode, no sequence playback. | No | Python library | Independent header and loop-tick checks (validation only; nothing copied) |
| [vgmtrans/vgmtrans](https://github.com/vgmtrans/vgmtrans) | zlib (playback uses the bundled proprietary BASS/BASSMIDI binaries) | NDS scanner: SSEQ→MIDI, SBNK+SWAR→DLS/SF2 (PSG as generated samples), sample export to WAV | No: playback is a SoundFont synth (SF2 envelopes, synth interpolation, no 192 Hz stepping, no 16-voice stealing) | Qt GUI plus an interactive "shell" app for conversions; not built here (Qt) | Not used |

**Conclusion.** None of the tools renders SSEQ the way the DS does, so `sseqrender.c` is our own implementation.

## 5. Converter

Files in `tools/dreamcast/audio/`:
- `sseqrender.c`: renderer and decoders (C, built automatically with gcc into `build/dreamcast/audio/bin/`). Sub-commands:
  - `render`: SSEQ or SSAR sequence to stereo PCM16 WAV, plus JSON stats and loop points;
  - `swar`: every wave to mono PCM16 at its native rate;
  - `strm`: STRM to PCM16;
  - `aicadec`: AICA ADPCM back to PCM, for checks.
- `sdatlib.py`: SDAT INFO/SYMB/FAT parser and driver-table locator.
- `sdat2dc.py`: batch converter.
- `validate_audio.py`: checks and PNGs.

```sh
python3 tools/dreamcast/audio/sdat2dc.py            # all 33 SDATs, about 65 s on 2 cores
    [--only sys,z1s] [--skip music,sfx,waves,strm] [--keep-pcm DIR] [--interp] [--wav2adpcm PATH]
python3 tools/dreamcast/audio/validate_audio.py --pcm DIR [--swav-ref BIN] [--strm-ref BIN]
```

### Output (`build/dreamcast/audio/`)

All files are KOS `wav2adpcm` WAVs: Yamaha/AICA 4-bit ADPCM, encoded with `wav2adpcm -t` (`-i` for interleaved).

| Path | What | Format |
|---|---|---|
| `music/<sdat>/<SEQ name>.wav` | every SEQ entry, rendered | stereo, fmt 0x20 (byte = L nibble << 4 \| R nibble, the layout `snd_stream`/`snd_adpcm_split` expect), 32,728 Hz |
| `sfx/<sdat>/<SSAR>/<nnn>_<name>.wav` | every SSAR sequence, rendered | mono fmt 0x14 if L == R, else stereo fmt 0x14 (L block then R block; `snd_sfx_load` accepts both), 32,728 Hz |
| `waves/<sdat>/<SWAR>/<nnnn>.wav` | every raw SWAV wave | mono fmt 0x14, native rate (16,756,991 / timer) |
| `strm/<sdat>/<name>.wav` | STRMs | stereo fmt 0x20, 37,403 Hz |
| `manifest.json` | one entry per source item | see below |
| `tables.bin` | driver tables copied from the owner's ROM | renderer input |

**Manifest entries** have these fields:
- `kind` (music, sfx, wave or strm), `sdat`, `name` (SYMB), `source` (SDAT, index, file, bank, volume, player);
- `status`: `ok`, `duplicate` with `same_as`, `silent` with `reason`, or `failed`;
- for `ok` entries: `output`, `rate`, `channels`, `samples`, `duration`, `loop`, `loop_start`, `loop_end` (sample frames in the output file), `adpcm_bytes`, `pcm16_bytes`, `format`;
- SFX and waves also carry `fits_snd_sfx` (≤ 65,534 samples, the `snd_sfx` limit);
- rendered items carry `render` stats: loop ticks, voices, steals, clipping, `first_pass_differs`, `converged`, `held_stopped`, missing instruments.

Identical PCM is written once. Typical case: zone SFX that resolve to the same instruments in many SDATs.

### Loop handling (music and looping SFX)
1. **Find the loop.** Each track's first backward jump (`94` at call depth 0, or an infinite `D4…FC`) to an already executed address gives that track's loop: target first-tick → jump tick. Song loop start S = the latest track loop start; loop length L = LCM of the track periods (capped).
2. **Shift when the first pass differs.** If any track reaches its loop jump with different parameters than it had at the target, the first pass sounds different from later passes, and the DS plays it that way too. Then the loop starts at the second pass, S + L. Portamento key is ignored while portamento is off. This happens in 32 of 77 looping songs, for example a pitch bend left at 64 at the end of the boss loop.
3. **Choose the seam point.** The renderer plays two full passes and picks the first frame A ≥ frame(S) where the notes still sounding in pass 1 are the same notes (track, key, tick offset) as those sounding at A + Lf in pass 2.
   - This absorbs intro notes that are still ringing.
   - It also absorbs voice-stealing differences. Example: in `ze/SEQ_bossE`, a long note from the end of the loop rings 7 s into pass 2 and has no counterpart in pass 1.
   - Output = audio[0 : A + Lf); `loop_start` = A, `loop_end` = A + Lf, in samples.
4. **Crossfade.** Tick→frame quantisation drifts between passes when 240·L/tempo is not an integer. The last 1,024 samples before `loop_end` are therefore crossfaded into the audio before `loop_start`, so the wrap is continuous.

**Held SFX.** A one-shot whose remaining tracks all wait on a length-0 note over a looping wave never ends by itself; the game stops it. At 4 s the converter fades it out over 32 frames and stops it (20 SFX; `held_stopped`). The fade length is an assumption. SFX are capped at 30 s, music at 900 s; none hit a cap.

## 6. Validation (run of 2026-11-22)

**Coverage**
- 33 of 33 SDATs processed, 0 failures, 65 s total.
- Music: 813 SEQ entries → 93 unique renders. The other entries are 28 duplicates and 692 silent entries, which are the 40-byte dummy sequences with no notes. 77 renders loop and 16 end (jingles, `SEQ_1up`, `SEQ_EXTRA_END`).
- SFX: 7,512 SSAR sequences → 357 unique renders, 5,735 duplicates and 1,420 silent:
  - 323 contain no notes;
  - 1,097 use program numbers that the SDAT's bank does not contain. Example: voice clips such as `SE_Sonic` on `BANK_ZONEb`, which is a 2-instrument stub in act SDATs. 415 SFX names are silent in every SDAT.
  - Of the 357 renders: 17 loop, 20 are held SFX that were faded and stopped, 72 are stereo, and 157 are longer than 65,534 samples (they need streaming or resampling for `snd_sfx`).
- Waves: 3,695 SWAV → 1,315 unique (all IMA-ADPCM; 58 loop; 24 exceed 65,534 samples).
- STRM: 2 of 2.

**Sizes** (AICA ADPCM / PCM16 equivalent)

| Output | ADPCM | PCM16 equivalent | Duration |
|---|---|---|---|
| music | 245.8 MB | 983 MB | 7,512 s |
| SFX | 16.9 MB | 68 MB | |
| waves | 7.5 MB | 30 MB | |
| STRM | 14.5 MB | 58 MB | |
| **total** | **288 MB** on disk | | |

**Checks** (`validate_audio.py`; report in `build/dreamcast/audio/validation/report.json`)
- SWAV decode vs loveemu nsswav (MIT, built as a harness from the sdatxtract clone): 3,110 / 3,110 waves in 74 distinct SWARs are **bit-exact**. Headers (format, loop, rate, timer, loop start, length) match ndspy for all 3,110.
- STRM decode vs loveemu nsstrm: 2 / 2 **bit-exact**. The older decoder in `tools/dreamcast/convert_assets.py` is off by ±1 on about 2 % of samples because it clamps at −32768, not −32767, so `sdat2dc` uses `sseqrender strm`.
- Loop points vs an independent tick walk over ndspy's event parse: 77 / 77 agree. The 32 "first pass differs" songs are expected at S + L.
- Loop seam: each looping song was re-rendered across two passes, comparing 2 s short-time spectra after `loop_start` and after `loop_end`.
  - Relative spectral error: median 0.19, max 0.31.
  - Baseline: about 0.15-0.19 between passes 2 and 3 of the same song (frame jitter only), about 1.0 between unrelated sections.
  - Before the seam search was added, `ze/SEQ_bossE` measured 0.55.
- ADPCM round trip (decode with `sseqrender aicadec`, the AICA step model, which matches KOS `wav2adpcm` and MAME) against the kept PCM16:

  | Output | SNR median | SNR worst |
  |---|---|---|
  | music | 22.4 dB | 14.2 dB (p5 18.8) |
  | waves | 23.3 dB | |
  | STRM | 17.2 dB | |
  | SFX | 14.5 dB | 6.5 dB (noise-like) |

  Codec sanity: a 440 Hz sine gives 43 dB and white noise 14 dB, so the low SFX figures come from the content, not from a decoder mismatch. Sample counts match in all files.
- Levels (DS mixer scale, no normalisation):
  - music peak median −8.1 dBFS, RMS median −23 dBFS;
  - 3 songs clip: `SEQ_METAMORPHO` 14 samples, `SEQ_BOSS_AFTER1` 2, `SEQ_BLAZE_PINCH` 1;
  - 7 SFX clip (at most 18 samples).
  - The DS clips the same sums, so this is faithful.
- PNGs (waveform with loop markers, spectrogram, 50 ms RMS) in `build/dreamcast/audio/validation/` for `z1s_SEQ_1sonic`, `z1b_SEQ_1blaze` (Leaf Storm), `z1es_SEQ_boss` and `sys_SEQ_TITLE`. What they show:
  - Leaf Storm: a 14.9 s intro, then a loop of 97.8 s at tempo 162, steady −25 to −35 dBFS RMS, the expected rhythmic structure, and the same arrangement in the Blaze version with different instrumentation.
  - Boss: the loop starts at the second pass (96.0 s); the 82 s period is visible as repeated sections.
  - Aliasing from the DS's lack of interpolation is visible as broadband energy up to 16 kHz.
  - Nobody has listened to the output; these images are the only check of the music itself.

**Rebuild the reference harnesses** (sources in `/tmp/audio/ext/sdatxtract/src/decoder`, MIT):
- Build `swavref.c` / `strmref.c` with `nsswav.c`, `nsstrm.c`, `nssamp.c` and a copy of `cioutil.c` whose `mput2l` is renamed.
- Link `fixput.c`, which provides a correct `mput2l`.
- The harness sources are in `/tmp/audio/val/`; this is validation tooling only.

## 7. Known fidelity gaps

1. **No ground truth.** No hardware capture or emulator render was available. The driver model follows documented NitroSDK behaviour and has not been compared sample-for-sample with a real DS.
   - Pitch table: computed as round(65536·(2^(i/768) − 1)); the ROM table was not found in `arm7.bin`. Timer error is at most ±1 LSB.
   - PSG/noise base timer 8006 and the LFO multipliers (64, 64, 60) are from documentation.
   - The DS's few-sample channel start delay and its 10-bit output quantisation are not modelled.
   - The volume, decibel, attack and sine tables are the ROM's own.
2. **One frame of latency** (5.2 ms) before the first note, as on the DS. One-shots keep it.
3. **Interactive behaviour is lost.** The game can change tempo and volume, mute tracks or fade out at run time (player volume and fades are in the NNS layer). The renders use INFO volume and tempo ratio 1.0, with no game-side calls. Any game logic that drives the music must be handled by the DC sound code (for example by switching streams).
4. **Voice stealing.** Each render plays one sequence alone. On the DS, music and SFX share 16 channels, so in-game SFX can steal music voices (and the reverse). Renders are "music with no SFX playing".
5. **Held SFX.** The 4 s hold and 32-frame fade are a stand-in for the game's `NNS_SndPlayerStopSeq` call. A faithful port should loop the sustained part (the raw looping waves are in `waves/`) and stop it from game code.
6. **ADPCM quality.** KOS `wav2adpcm` is a simple greedy Yamaha encoder.
   - Waveform SNR: about 22 dB on music; lower on bright or noisy content and on STRM (17 dB). DS-style aliasing adds high-frequency content that ADPCM codes poorly.
   - `--interp` (linear interpolation, not DS-like) raises `SEQ_1sonic` from 23.1 to 25.8 dB.
   - For better quality use a smarter encoder or PCM16 streams (4× size; music would be 983 MB).
7. **Levels.** Levels follow the DS mixer scale, with no normalisation; music sits around −23 dBFS RMS. Apply one common playback gain on the DC so music and SFX keep their relative levels.
8. **Size.** Music is 246 MB, because each loop is stored once in full plus the intro, plus a second pass for the 32 songs whose first pass differs. `snd_sfx` cannot load 157 SFX and 24 raw waves (> 65,534 samples) at 32,728 Hz without streaming or a lower rate.
9. **Silent SFX.** 1,097 SSAR sequences reference instruments that do not exist in their SDAT's bank. They render silent here, as they would on the DS with that bank loaded. If the game loads other banks at run time for those sounds, that is not modelled.
