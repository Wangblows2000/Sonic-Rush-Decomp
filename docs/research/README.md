# Research log

Persistent knowledge base. One numbered note per question; this table is the
log. Confidence: H/M/L. Status: done / open.

| # | Question | Sources | Finding (short) | Conf. | Tests | Open issues | Status |
| - | -------- | ------- | --------------- | ----- | ----- | ----------- | ------ |
| [0001](0001-rom-identification.md) | Which ROM, what layout? | ROM, GBATEK, ds-rom | EUR `ASCP` rev 0, 2 ARM9 overlays at shared base 0x022C5C80, NitroSDK 2.1, decrypted secure area | H | two parsers agree; header/logo CRC valid | SDK relstep meaning; ARM7 identity | done |
| [0002](0002-reference-projects.md) | Which architecture? | pokediamond, ph, mkds-re, mpds, SRA | dsd + ninja + objdiff (ph model) with extra checks | H | pipeline runs end-to-end without compiler | — | done |
| [0003](0003-toolchain-environment.md) | What runs where? | local env | all OSS tools build from source; mwccarm execution blocked here | H | doctor | compiler on user machine | done |
| [0004](0004-compiler.md) | Which compiler/flags? | build info, SRA, idioms | unknown; CW 1.2 spN most likely | L | none possible yet | needs mwccarm | **open** |
| [0005](0005-rom-rebuild.md) | Byte-identical rebuild? | dsd, GBATEK | yes, after secure-area CRC + zero-length file order fixes | H | SHA-1 equal | report ds-rom ordering bug | done |
| [0006](0006-gnu-relink.md) | Are dsd relocations right? | dsd objects, AAELF | all 11 sections relink identically after 4 normalisations | H | byte compare | upstream dsd ELF fixes | done |
| [0007](0007-ghidra.md) | Ghidra workflow | Ghidra API docs | scripts written | L | **not run** | run with Ghidra | open |
| [0008](0008-dsd-thumb-jumptables.md) | Why does ov001 analysis fail? | dsd source, ARM ARM | unsigned Thumb jump tables unsupported; patched → +1,848 functions | H | coverage 100 %, relink OK | upstream; ITCM gap | done |
| [0009](0009-sonic-rush-prior-work.md) | Prior Rush RE? | web | no public Rush code decomp; SRA decomp exists; asset formats partly known | M | — | asset tools licence | done |
