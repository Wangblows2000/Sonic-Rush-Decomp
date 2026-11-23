# Dreamcast port — documentation index

Native Sega Dreamcast port of Sonic Rush (EUR), built on the matching
decompilation. All results are from the Flycast emulator; nothing has been
verified on real hardware.

| Document | Contents |
| --- | --- |
| [KNOWN-ISSUES.md](KNOWN-ISSUES.md) | What works, differences from the DS, known issues |
| [BUILD.md](BUILD.md) | Toolchain, asset conversion, disc build, the no-data kit, test switches |
| [REGRESSION.md](REGRESSION.md) | How the port is tested and the latest results |
| [ARCHITECTURE.md](ARCHITECTURE.md) | Module layout, frame order, rules the code follows |
| [GRAPHICS.md](GRAPHICS.md) | PVR texture memory, paletted atlases, palette effects, loading I/O, zone backgrounds |
| [AUDIO.md](AUDIO.md) | Music streams, sound-effect sequencer, banks, mixing, fades |
| [CAMERA.md](CAMERA.md) | Single-screen rule and the translated cameras |
| [formats/](formats/) | DS file formats and the converters: [STAGES](formats/STAGES.md), [SPRITES](formats/SPRITES.md), [MODELS](formats/MODELS.md), [SOUND](formats/SOUND.md) |
| [translation/](translation/) | Per-module notes and function maps (original → port): [PLAYER](translation/PLAYER.md), [OBJECTS](translation/OBJECTS.md), [BOSSES](translation/BOSSES.md), [SPECIAL](translation/SPECIAL.md), [CUTSCENES](translation/CUTSCENES.md), [MENUS](translation/MENUS.md), [UI](translation/UI.md) |
