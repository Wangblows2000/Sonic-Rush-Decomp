# Dreamcast port — known issues and blockers

- The port is at Milestone 2 (hardware foundation). The game itself does not
  run on Dreamcast yet; see PORTING-REPORT.md for the remaining work.
- No retail-hardware test has been done. Emulator tests only.
- ARM7 code is not decompiled; its services (sound sequencing, touch, wireless)
  must be re-implemented, not ported.
- Music is sequenced (SSEQ); the "convert all audio to ADPCM" goal needs either
  a sequencer port or an offline renderer (AUDIO.md).
- Wireless VS / Download Play have no Dreamcast equivalent.
- Converted assets are generated locally from the owner's ROM and are not
  committed (bring-your-own-ROM).
- KOS `snd_sfx_load` one-shots hold at most 65,534 samples (warning seen with a
  6 s sample). The test sample is 1.7 s; music/streams need `snd_stream`.
