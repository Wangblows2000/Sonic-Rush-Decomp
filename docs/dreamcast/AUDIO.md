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

## Plan

- Short SFX: SWAR waves -> AICA ADPCM, loaded per zone into sound RAM
  (2 MB minus driver and stream buffers).
- STRM: streamed from disc with `snd_stream` double buffers.
- Music: two options, not yet decided — (a) port the NitroSDK SSEQ sequencer to
  drive AICA channels with converted SWAR samples (faithful, low disc use, more
  CPU), or (b) render every SSEQ offline to stereo PCM with a DS-accurate
  sequencer and stream it as ADPCM (simple playback, loses interactive tempo /
  track muting used by the tension gauge and boss phases).
