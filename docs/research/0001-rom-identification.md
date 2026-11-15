# 0001 — ROM identification and layout (EUR)

**Question.** Which Sonic Rush ROM do we target, and what is its structure?
**Sources.** User-supplied dump; GBATEK "DS Cartridge Header", "NitroROM file
system"; `tools/ndslib/rom.py`; `dsd rom extract`.
**Confidence.** High — all values read directly from the ROM and cross-checked
by two independent parsers (ours and ds-rom).
**Reproduce.** `python tools/project.py analyze eur` → `build/eur/reports/rom.json`.

## Identity

| Field | Value |
| ----- | ----- |
| File | `Sonic Rush (Europe) (En,Ja,Fr,De,Es,It).nds` |
| Size | 67,108,864 (64 MiB; capacity byte 0x09) |
| SHA-1 | `52e5024efcf7617a820fa23789b37e5168c3c4fc` |
| SHA-256 | `2606a10ead1656e201d8127ffae5614a970201fc37627688e5bccb48abf705c2` |
| Title / code / maker | `SONIC RUSH` / `ASCP` (Europe) / `8P` (Sega) |
| ROM version | 0 |
| Unit code | 0x00 (NDS) |
| Used ROM size | 0x03CA8DE0 |
| Header CRC / logo CRC | valid / valid |
| Secure area | **decrypted** (starts `E7FFDEFF E7FFDEFF`); stored secure-area CRC 0x190B |
| Banner | version 1, English title "SONIC RUSH / SEGA" |

## ROM regions

| Region | ROM start | ROM end |
| ------ | --------- | ------- |
| header (0x4000) | 0x00000000 | 0x00004000 |
| ARM9 static | 0x00004000 | 0x0008A898 |
| ARM9 overlay table (2 entries) | 0x0008AA00 | 0x0008AA40 |
| overlay 0 (file 0) | 0x0008AC00 | 0x0012EA40 |
| overlay 1 (file 1) | 0x0012EC00 | 0x00187FC0 |
| ARM7 static | 0x00188000 | 0x001AEF24 |
| FNT | 0x001AF000 | 0x001B9136 |
| FAT (2,247 entries) | 0x001B9200 | 0x001BD838 |
| banner | 0x001BDA00 | 0x001BE240 |
| file image | 0x001BE400 … | 0x03CA8DE0 |

No ARM7 overlays. Neither overlay is compressed or signed.

## Executable modules (RAM)

| Module | Load | Size | Entry / notes |
| ------ | ---- | ---- | ------------- |
| ARM9 static | 0x02000000 | 0x86898 in ROM | entry 0x02000800; autoload callback 0x02000A10 |
| ARM9 main (after autoload split) | 0x02000000–0x0207CFE0 | 0x7CFE0 | .text→0x0206E470, .rodata→0x02077958, .data 0x02077960 |
| main BSS | 0x0207CFE0–0x022C5C80 | 0x248CA0 | from build info |
| ITCM autoload | 0x01FF8000 | 0x7840 | code 0x782C |
| DTCM autoload | 0x027E0000 | 0x2060 | data |
| overlay 0 | 0x022C5C80 | 0xA3E40 + BSS 0x11E0 | static-init ctor 0x02364EF8 |
| overlay 1 | 0x022C5C80 | 0x593C0 + BSS 0x6C0 | static-init ctor 0x02317E8C |
| ARM7 static | 0x02380000 | 0x26F24 | entry 0x02380000; autoload callback 0x02380118 |

Overlays 0 and 1 occupy the same address range: only one is resident at a time.
Main code calls directly into both (relocations resolve to `overlay(0)` or
`overlay(1)`), so which overlay a main function needs is part of its contract.

### ARM9 build info (NitroSDK module params, ARM9+0xB00)

```
autoload list   0x02086880 – 0x02086898   (2 entries: ITCM, DTCM)
autoload start  0x0207CFE0
bss             0x0207CFE0 – 0x022C5C80
sdk version     0x02017532   -> NitroSDK 2.1, relstep 30002 (decoding: tentative)
magic           0xDEC00621
```

No `[SDK+NINTENDO:...]` library marker strings exist in ARM9 or ARM7. The ARM7
contains the string `Failed sound alarm OS_SendMessage`, typical of the
NitroSDK sound driver.

## Filesystem

2,247 FAT entries: 2 overlay files + 2,245 named files.

| Directory | Files | Notes |
| --------- | ----- | ----- |
| `bg/`, `bg2/` | 670, 557 | `.bbg` backgrounds (1,218 total) |
| `act/`, `act2/` | 343, 61 | `.bac` sprite animations (404) |
| `talk/` | 240 | dialogue |
| `mod/` | 149 | `.nsbmd` 3D models (147) |
| `narc/` | 141 | NARC archives (stages) |
| `snd/` | 33 | `.sdat` sound archives (33) |
| `pldm/`, `dat/`, `ama/`, `ss/`, `banner/`, `st/` | | misc data |
| `mb/` | 3 | `contest.srl` (1,004,232 B), `preview.srl` (826,568 B): Download Play child programs |

Other extensions: `.btk` (240), `.bpd`, `.ama`, `.bb`, `.df`, `.sst`, `.nbfc`,
`.nbfp`, `.nsbta`, `.nsbtp`, `.ssd`, `.sld`, `.dmy` (4 zero-length dummies).
The `.srl` children are separate DS programs and future decompilation targets.

## Remaining uncertainties

- SDK version decoding (relstep semantics) — verify against SDK code once
  functions match.
- ARM7: whether it is the stock NitroSDK ARM7 component (not analysed yet).
