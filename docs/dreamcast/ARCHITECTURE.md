# Dreamcast port — architecture

Branch: `dreamcast-port`. The DS decompilation (`src/`, `config/`, `tools/` apart
from `tools/dreamcast/`) is untouched and still builds the matching ROM.

## Layout

| Path | Component | Status |
| --- | --- | --- |
| `dreamcast/foundation/` | Milestone 2 hardware test: KOS boot, PVR, AICA, maple, asset loading | implemented |
| `tools/dreamcast/convert_assets.py` | asset pipeline (owner's ROM -> DC formats + manifest) | banner icon + STRM audio |
| `dreamcast/platform/` | NitroSDK/NitroSystem shim on KOS (OS, FX, MTX, FS, GX/G2/G3, NNS_*) | not started |
| `dreamcast/render/` | PVR backend: BG layers, OAM sprites, G3D meshes | not started |
| `dreamcast/audio/` | AICA backend: sample banks, streams, sequencer | not started |
| `dreamcast/present/` | single-screen compositor, camera, HUD, cutscene director | design only (CAMERA.md) |
| `build/dreamcast/` | generated assets and disc images (gitignored) | — |

## Separation rules

1. Game logic is the decompiled C in `src/game/`. It is compiled unchanged for
   SH-4; behaviour differences go in the platform layer, not in game files.
2. The platform layer implements the NitroSDK/NNS API the game calls. Library
   code in `src/libs/` that is pure computation (MSL, FX/MTX maths, NNS FND
   heaps/archives) can be compiled as is; hardware drivers (GX, G3, SND, CARD,
   TP, WM, OS IRQ/DMA) are replaced.
3. Presentation (two 256x192 screens -> one 640x480 view) happens in the
   compositor: the game keeps rendering two logical screens of BG/OAM state; the
   compositor decides what part of the combined world is shown (CAMERA.md).
4. Assets are converted offline from the owner's ROM (ASSET-PIPELINE.md); the
   game's file loader maps NitroROM paths to `/cd/` paths of converted files.
