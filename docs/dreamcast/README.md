# Dreamcast port — documentation index

Native Sega Dreamcast port of Sonic Rush (EUR), built on the matching
decompilation. All results are from the Flycast emulator; nothing has been
verified on real hardware.

## Branches (permanent policy)

- **`dreamcast-port` is the only development branch** of the native Dreamcast port, Battle Play (two
  players and Vs CPU), the racing-line tools, the cameras, fixes, documentation and releases. All
  commits and pushes for the port go here.
- **`battle-play`** is kept as a historical reference: it was merged into `dreamcast-port` (both at
  `aff303ad`, then development continued on `dreamcast-port` only). No new work goes there; it is not
  deleted or force-updated.
- **`main`** (the matching decompilation) and the original DS branch are not touched by the port.
- No other long-lived feature branches. A short local branch for an experiment is merged into
  `dreamcast-port` as soon as it is verified.
- No resets, rebases or force pushes of shared history.

| Document | Contents |
| --- | --- |
| [KNOWN-ISSUES.md](KNOWN-ISSUES.md) | What works, differences from the DS, known issues |
| [BUILD.md](BUILD.md) | Toolchain, asset conversion, disc build, the no-data kit, test switches |
| [REGRESSION.md](REGRESSION.md) | How the port is tested and the latest results |
| [ARCHITECTURE.md](ARCHITECTURE.md) | Module layout, frame order, rules the code follows |
| [GRAPHICS.md](GRAPHICS.md) | PVR texture memory, paletted atlases, palette effects, loading I/O, zone backgrounds |
| [AUDIO.md](AUDIO.md) | Music streams, sound-effect sequencer, banks, mixing, fades |
| [CAMERA.md](CAMERA.md) | Single-screen rule, the translated cameras, the wider single-player view |
| [BATTLE-PLAY.md](BATTLE-PLAY.md) | Battle Play: two players on one Dreamcast, Vs CPU, racing lines |
| [formats/](formats/) | DS file formats and the converters: [STAGES](formats/STAGES.md), [SPRITES](formats/SPRITES.md), [MODELS](formats/MODELS.md), [SOUND](formats/SOUND.md) |
| [translation/](translation/) | Per-module notes and function maps (original → port): [PLAYER](translation/PLAYER.md), [OBJECTS](translation/OBJECTS.md), [BOSSES](translation/BOSSES.md), [SPECIAL](translation/SPECIAL.md), [CUTSCENES](translation/CUTSCENES.md), [MENUS](translation/MENUS.md), [UI](translation/UI.md) |
