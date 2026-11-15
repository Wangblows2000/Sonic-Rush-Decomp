# 0005 — Byte-identical ROM rebuild from extraction

**Question.** Does `dsd rom extract` → `dsd rom build` reproduce the base ROM?
**Sources.** dsd v0.11/v0.12.1 (ds-rom 0.8.0); GBATEK cartridge header and KEY1.
**Confidence.** High — verified by SHA-1. **Reproduce:** `python tools/project.py verify eur`.

## Result

Plain round-trip differs in 8 bytes:

1. **Header 0x6C (secure-area CRC) and 0x15E (header CRC).** The ROM is a
   *decrypted* dump (secure area starts `E7FFDEFF E7FFDEFF`). The stored CRC
   (0x190B) is over the KEY1-*encrypted* secure area; re-encryption needs the
   ARM7 BIOS key tables, so dsd writes 0. zeldaret/ph has the same requirement.
   *Fix:* `tools/extraction/fix_header.py` restores the 16-bit value recorded in
   `config/versions.yaml` and recomputes the header CRC. With a user-supplied
   `arm7_bios.bin`, `dsd rom build --arm7-bios` is preferred.
2. **FAT entries 1647 and 1772** (`/dat/dummy.dmy`, `/mod/dummy.nsbmd`):
   zero-length files. The original places each empty file at the offset of the
   *next* non-empty file in image order (i.e. before it). ds-rom's
   `path_order.txt` listed `/mod/dummy.nsbmd` after `/mod/eff_barrier.nsbmd`
   and omitted `/dat/dummy.dmy` (it only lists 1,522 of 2,245 paths).
   *Fix:* `tools/extraction/fix_path_order.py` regenerates the complete order
   from the base ROM's FAT: sort by `(start, size != 0, id)`.

After both fixes: SHA-1 `52e5024efcf7617a820fa23789b37e5168c3c4fc` — identical.

## Remaining

- Report the zero-length ordering issue upstream (ds-rom).
