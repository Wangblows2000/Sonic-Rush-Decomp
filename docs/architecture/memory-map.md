# Memory map (Sonic Rush EUR, ARM9 view)

Derived from the ROM header, overlay table and NitroSDK build info
(docs/research/0001-rom-identification.md). Hardware regions per GBATEK.

| Range | Size | Contents |
| ----- | ---- | -------- |
| 0x01FF8000–0x01FFF82C | 0x782C | ITCM autoload: 14 functions + 0x701C zero-filled buffer at 0x01FF8744 |
| 0x02000000–0x02000800 | 0x800 | secure area: BIOS SWI wrappers + data |
| 0x02000800 | | `Entry` (crt0) |
| 0x02000B00 | 0x20 | `BuildInfo` (NitroSDK module params) |
| 0x02000B24 | | `main` |
| 0x02000000–0x0206E470 | 0x6E470 | main .text (ARM + Thumb) |
| 0x0206E470–0x02077958 | 0x94E8 | main .rodata |
| 0x02077958 | 0 | main .ctor (empty) |
| 0x02077960–0x0207CFE0 | 0x5680 | main .data |
| 0x0207CFE0–0x022C5C80 | 0x248CA0 | main .bss (2.3 MB: heaps/arenas likely inside) |
| 0x022C5C80–0x02369AC0 (+0x11E0 BSS) | 0xA3E40 | overlay 0: .text →0x02362B58, .rodata →0x02364EF8, .data →0x02369AC0 |
| 0x022C5C80–0x0231F040 (+0x6C0 BSS) | 0x593C0 | overlay 1: .text →0x02315EB4 (Thumb first ~160 KB, then ARM), .rodata →0x02317E8C, .data →0x0231F040 |
| 0x02380000–0x023A6F24 | 0x26F24 | ARM7 static (main RAM; ARM7-owned) |
| 0x027E0000–0x027E2060 | 0x2060 | DTCM autoload: data |

Overlay 0 and 1 are mutually exclusive. Which is "stage" vs "menu" is not
established yet; overlay 0 is ARM-heavy and larger (likely gameplay), overlay
1 is Thumb-heavy (hypothesis: front-end/menus). Tentative.
