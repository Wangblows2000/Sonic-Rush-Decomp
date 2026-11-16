# Progress dashboard

Last updated: **2026-11-16 06:46:17Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `██████████████████░░` 89.18% | 400,942 / 449,566 | 2,762 / 2,856 | 94 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `███████████████████░` 94.79% | 609,208 / 642,724 | 2,672 / 2,799 | 127 |
| ov001 | `███████████████████░` 93.00% | 305,148 / 328,118 | 1,663 / 1,723 | 60 |
| **ARM9 total** | `███████████████████░` 92.61% | 1,317,362 / 1,422,472 | 7,111 / 7,392 | 281 |

Next milestone: **100%** matched code — 105,110 more bytes (currently 92.61%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 06:43:36Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 06:46:12Z
- objdiff report: `build/eur/report.json` — 2026-11-16 06:46:16Z

## Task board

- 5 claimed · 8 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-mid | main_c1 | main 0x0201c000-0x02038000 | 99,500/114,664 B (86.8%), 590/614 fn |
| main-hi | main_b7 | main 0x02038000-0x0206e470 | 194,302/222,234 B (87.4%), 1369/1406 fn |
| ov000-lo | ov000_n | ov000 0x022c5c80-0x02310000 | 297,380/304,276 B (97.7%), 1415/1433 fn |
| ov000-hi | ov000_l | ov000 0x02310000-0x02362b58 | 311,828/338,448 B (92.1%), 1257/1366 fn |
| ov001-arm | ov001_h | ov001 0x022ed000-0x02315eb4 | 159,382/167,454 B (95.2%), 677/691 fn |

## History

Matched bytes trend: ▁▁▅▆▆▇▇▇█  (9 changes, 9 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 06:46:17Z | 1,317,362 | 92.61% | 7,111 |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
