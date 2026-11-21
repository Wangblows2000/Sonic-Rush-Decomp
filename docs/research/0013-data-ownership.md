# 0013 — Data, rodata and DTCM ownership (task `r-data`)

Status: first data TU integrated (DTCM pilot, below). All other data is still in
dsd gap objects. This note records what was measured on 2026-11-21 and what has to be
built before data can move into TUs.

## Current state (measured)

- Every TU in `config/eur/arm9/**/delinks.txt` owns `.text` only. `grep` finds no
  TU entry with a `.rodata`, `.data`, `.bss` or `.ctor` line.
- All data comes from dsd's gap objects: main `.rodata` 0x0206e470–0x02077958,
  `.data` 0x02077960–0x0207cfe0, `.bss` 0x0207cfe0–0x022c5c80, plus the
  overlay and DTCM equivalents.
- TUs reach data only through `extern` declarations of dsd `data_XXXXXXXX`
  symbols; the literal pools hold the relocated addresses.

## DTCM (0x027e0000–0x027e2060)

- `extract/eur/arm9/dtcm.bin` is 0x2060 bytes. Only the first 0x60 bytes are
  non-zero; 0x027e0060–0x027e2060 is all zero.
- The non-zero block is a word table of ARM9 code pointers (0x02005310,
  0x02005224, 0x02005214, 0x02005204, 0x020051f4, …), i.e. NitroSDK's
  `OS_IRQTable` area that `OS_InitIrqTable`, `OS_SetIrqFunction` and
  `OS_IrqHandler` index (all of them reference `data_027e00xx`).
- The zero area is the SDK DTCM work area (IRQ stack / thread queue region
  the init code and `OS_InitThread` address through linker constants).
- dsd's five DTCM symbols (`data_027e0000`, `_0004`, `_0021`, `_0060`,
  `_0fce`) are analysis guesses; `_0021` and `_0fce` are not word aligned and
  should not be taken as object boundaries.
- Owning TU: NitroSDK `os_irqTable.c` (a data-only TU in section `.dtcm`).

## Why a data TU cannot simply be added today

1. **Placement order.** The ROM's data layout follows the original object
   files. Our TUs are split one function per file, and their link order in
   `delinks.txt` is per function. A data item owned by a TU is emitted where
   that TU sits in the link order, so the TU that owns it must sit exactly
   where the original object's data sat, relative to every other data
   contributor in the same section. With gap objects in between this is only
   reproducible if the owning TU's data range is declared in its delinks
   entry (dsd `delink` then cuts the gap object around it).
2. **Static data and string pooling.** CW pools string literals and small
   constants per TU. A C definition must produce the same bytes, alignment
   and padding as the original object, so a string table cannot be moved
   one string at a time; the whole run that one original object contributed
   has to move together.
3. **Shared data.** Many data items are referenced from functions in several
   of our TUs (they belonged to one original `.c`). Ownership needs one TU per
   original object, which is the same evidence the TU-merge task needs
   (PLAN.md section 4).

## Strategy

1. **Pilot: `os_irqTable.c`.** Add a data-only TU in the DTCM delinks with
   `.data start:0x027e0000 end:0x027e0060`, define the table in C with
   `#pragma section` for `.dtcm`, rebuild, and check SHA-1 plus `ninja verify`.
   This proves the delinks/dsd/LCF path for data TUs on the smallest case.
2. **Library rodata first.** NitroSDK/NitroSystem/MSL objects have known
   source files, so their data ranges and contents can be checked against
   reference sources (e.g. MSL's powers-of-5 strings near 0x0202cd1c, NNS
   tables). Assign each run to the TU that will become the merged library
   object.
3. **Game data by reference clusters.** For each `data_XXXXXXXX`, list the
   TUs whose relocations point into it (`relocs.txt`). A run whose referrers
   all lie in one contiguous text range is a candidate for that range's
   original object. Mixed runs stay in gap objects.
4. **Tooling.** A script that, for a proposed `(TU, section, start, end)`,
   writes the delinks line, emits a C skeleton from the bytes (u8/u16/u32
   arrays, strings, float constants recognised by pattern), compiles it with
   `match.py`-style byte comparison, and only then lets the lead integrate.
5. Verify every step with the full build (SHA-1) and `ninja verify`.

## Blocker summary

The data-TU path works (pilot). Moving the remaining data depends on
reconstructing original object boundaries (the TU merge task), because data
order follows the original objects. Next concrete actions: the other DTCM words
(0x027e0000, 0x027e005c) and step 2 (library rodata).
