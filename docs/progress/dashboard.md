# Progress dashboard

Last updated: **2026-11-16 06:14:24Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `██████████████████░░` 87.92% | 395,252 / 449,566 | 2,741 / 2,856 | 115 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `███████████████████░` 94.03% | 604,372 / 642,724 | 2,657 / 2,799 | 142 |
| ov001 | `██████████████████░░` 91.99% | 301,842 / 328,118 | 1,647 / 1,723 | 76 |
| **ARM9 total** | `██████████████████░░` 91.64% | 1,303,530 / 1,422,472 | 7,059 / 7,392 | 333 |

Next milestone: **100%** matched code — 118,942 more bytes (currently 91.64%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 06:10:45Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 06:14:17Z
- objdiff report: `build/eur/report.json` — 2026-11-16 06:14:22Z

## Task board

- 5 claimed · 8 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-mid | main_c1 | main 0x0201c000-0x02038000 | 99,500/114,664 B (86.8%), 590/614 fn |
| main-hi | main_b7 | main 0x02038000-0x0206e470 | 188,612/222,234 B (84.9%), 1348/1406 fn |
| ov000-lo | ov000_n | ov000 0x022c5c80-0x02310000 | 296,116/304,276 B (97.3%), 1409/1433 fn |
| ov000-hi | ov000_l | ov000 0x02310000-0x02362b58 | 308,256/338,448 B (91.1%), 1248/1366 fn |
| ov001-arm | ov001_g | ov001 0x022ed000-0x02315eb4 | 156,076/167,454 B (93.2%), 661/691 fn |

## History

Matched bytes trend: ▁▁▅▆▆▇▇█  (8 changes, 8 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
