# Research log

Persistent knowledge base. One numbered note per question; this table is the
log. Confidence: H/M/L. Status: done / open. Superseded statements stay in the
notes for history and are marked as such.

| # | Question | Sources | Finding (short) | Conf. | Tests | Open issues | Status |
| - | -------- | ------- | --------------- | ----- | ----- | ----------- | ------ |
| [0001](0001-rom-identification.md) | Which ROM, what layout? | ROM, GBATEK, ds-rom | EUR `ASCP` rev 0, 2 ARM9 overlays at shared base 0x022C5C80, NitroSDK 2.1, decrypted secure area | H | two parsers agree; header/logo CRC valid | SDK relstep meaning; ARM7 identity | done |
| [0002](0002-reference-projects.md) | Which architecture? | pokediamond, ph, mkds-re, mpds, SRA | dsd + ninja + objdiff (ph model) with extra checks | H | full matching build end to end | — | done |
| [0003](0003-toolchain-environment.md) | What runs where? | local env | all OSS tools build from source; mwccarm (via wibo) and Ghidra 12.2 run in the cloud workspace | H | doctor; full build | — | done |
| [0004](0004-compiler.md) | Which compiler/flags? | build info, probes, matching | CW for NDS 1.2/sp2 for all ARM9 code; linker 1.2/sp3 | H | 7,389 functions match; ROM SHA-1 | — | done |
| [0005](0005-rom-rebuild.md) | Byte-identical rebuild? | dsd, GBATEK | yes, after secure-area CRC + zero-length file order fixes | H | SHA-1 equal | report ds-rom ordering bug | done |
| [0006](0006-gnu-relink.md) | Are dsd relocations right? | dsd objects, AAELF | all 11 sections relink identically after 4 normalisations | H | byte compare | upstream dsd ELF fixes | done |
| [0007](0007-ghidra.md) | Ghidra workflow | Ghidra API docs | Ghidra 12.2 + JDK 25 headless import; per-function decompiler drafts | H | run on all 3 programs; drafts used by agents | — | done |
| [0008](0008-dsd-thumb-jumptables.md) | Why does ov001 analysis fail? | dsd source, ARM ARM | unsigned Thumb jump tables unsupported; patched → +1,848 functions; second patch for asm entry points | H | coverage 100 %, relink OK | upstream | done |
| [0009](0009-sonic-rush-prior-work.md) | Prior Rush RE? | web | no public Rush code decomp; SRA decomp exists; asset formats partly known | M | — | asset tools licence | done |
| [0010](0010-sdk-identification.md) | Which functions are NitroSDK? | pokediamond SDK, call graph | 5 manual + 102 automatic SDK names (sdk_ident.py) | H | instruction / masked byte compare | — | done |
| [0011](0011-mwccarm-levers.md) | Which source changes move mwccarm codegen? | SM64DS notes, probes | lever catalogue checked on CW 1.2/sp2 (pragmas, ordering, types) | M | probes | — | done (living note) |
| [0012](0012-secure-area.md) | How to match the secure-area BIOS stubs? | ROM bytes, LCF | one GNU-as TU with `.incbin` filler; `$t` mapping symbols | M | SHA-1 + relink | — | done |
| [0013](0013-data-ownership.md) | How can data, rodata and DTCM move into TUs? | delinks, dsd gap objects, LCF | all data is still in dsd gap objects; `OS_IRQTable` (DTCM) proven as the first data TU | M | SHA-1 + relink with the pilot | strategy for the remaining data (task `r-data`) | open |
| [0014](0014-zones.md) | How are zones numbered and which code is zone-specific? | stage tables, title cards | `gGameState+0x1C/+0x1D` = 0-based zone/act, 9×3 stage tables; zones 1–7 titled, 8/act 3 = Final Zone boss, 9/act 3 = Extra Zone | H | table cross-checks | zone 8/9 acts 1–2 meaning | done |
