# Dreamcast port — asset pipeline

Bring-your-own-ROM: every Dreamcast asset is generated from the owner's
`extract/eur/` by `tools/dreamcast/convert_assets.py` into
`build/dreamcast/assets/` with `manifest.json` (source path + SHA-1, output
format, rate/channels/loop/duration, settings, size). Converted assets are not
committed; anyone with the ROM regenerates identical outputs.

| Asset | Source | Output | Status |
| --- | --- | --- | --- |
| test sprite | banner icon (`banner/bitmap.png`, 32x32) | `sprite.argb4444` | done |
| test sample | `snd/sys` SDAT STRM #105, first 6 s, L+R mix | `sample.wav` AICA ADPCM (fmt 0x14) | done, round-trip decoded RMS 10012 |
| all STRM | 2 STRM in `snd/sys` (stereo IMA-ADPCM 37,403 Hz) | `strm_*.wav` (`--all-strm`) | implemented |
| SWAR waves | 26+ SWAR (IMA-ADPCM / PCM) | AICA ADPCM banks | not started |
| SSEQ music/SFX | 33 SSEQ in `sys`, ~4 per zone SDAT | sequencer data or pre-rendered ADPCM | not started |
| BAC sprites | `act/`, `act2/`, zone `act` NARCs | PVR paletted textures + frame tables | not started (format partly known from `AnimatorSprite_*`) |
| BBG backgrounds | `bg/`, `bg2/` | PVR textures / tile atlases | not started |
| NSBMD/NSBCA | `mod/` | PVR meshes (same vertex/poly counts) + animation | not started |
| NARC + LZ77 | `narc/` | unpacked files | not started |
