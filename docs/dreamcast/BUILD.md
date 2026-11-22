# Dreamcast port — build and test

## Toolchain (built 2026-11-22 in this environment)

```sh
git clone --depth 1 https://github.com/KallistiOS/KallistiOS /opt/toolchains/dc/kos
cd /opt/toolchains/dc/kos/utils/kos-chain
cp Makefile.dreamcast.cfg Makefile.cfg     # set makejobs=2 (a number, not -j2)
make                                        # sh-elf binutils/gcc/newlib (~1-2 h on 2 cores)
cp /opt/toolchains/dc/kos/doc/environ.sh.sample /opt/toolchains/dc/kos/environ.sh
. /opt/toolchains/dc/kos/environ.sh && make -C /opt/toolchains/dc/kos
make -C $KOS_BASE/utils/scramble && make -C $KOS_BASE/utils/makeip
```

Host tools: `genisoimage`, `cdi4dc` (github.com/Kazade/img4dc), `wav2adpcm`
(`gcc -O2 -o wav2adpcm $KOS_BASE/utils/wav2adpcm/wav2adpcm.c`), Python 3 + Pillow.

## Foundation disc

```sh
sh dreamcast/foundation/build_cdi.sh                     # placeholder content only
python3 tools/dreamcast/convert_assets.py               # needs extract/eur from your ROM
sh dreamcast/foundation/build_cdi.sh --with-rom-assets  # adds your converted assets
```

Output: `build/dreamcast/foundation/*.cdi` (selfboot, data/data). Controls:
analog/D-pad move the sprite, A plays the sample, START exits.

## Testing

Flycast (built from source, run under Xvfb) boots the CDI with its HLE BIOS.
Emulator results are not retail-hardware results; see KNOWN-ISSUES.md.
