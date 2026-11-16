# Progress dashboard

Last updated: **2026-11-16 04:10:13Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `████████████████░░░░` 79.64% | 358,012 / 449,566 | 2,537 / 2,856 | 319 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `███████████████░░░░░` 76.29% | 490,308 / 642,724 | 2,206 / 2,799 | 593 |
| ov001 | `████████████████░░░░` 82.16% | 269,568 / 328,118 | 1,523 / 1,723 | 200 |
| **ARM9 total** | `████████████████░░░░` 78.73% | 1,119,952 / 1,422,472 | 6,280 / 7,392 | 1,112 |

Next milestone: **80%** matched code — 18,026 more bytes (currently 78.73%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 04:08:33Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 04:10:10Z
- objdiff report: `build/eur/report.json` — 2026-11-16 04:10:12Z

## Task board

- 6 claimed · 7 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-mid | main_a5 | main 0x0201c000-0x02038000 | 98,684/114,664 B (86.1%), 586/614 fn |
| main-hi | main_b6 | main 0x02038000-0x0206e470 | 152,188/222,234 B (68.5%), 1148/1406 fn |
| ov000-lo | ov000_e | ov000 0x022c5c80-0x02310000 | 262,696/304,276 B (86.3%), 1210/1433 fn |
| ov000-hi | ov000_g | ov000 0x02310000-0x02362b58 | 227,612/338,448 B (67.3%), 996/1366 fn |
| ov001-thumb | ov001_t4 | ov001 0x022c5c80-0x022ed000 | 115,176/160,664 B (71.7%), 865/1032 fn |
| ov001-arm | ov001_f | ov001 0x022ed000-0x02315eb4 | 154,392/167,454 B (92.2%), 658/691 fn |

## History

Matched bytes trend: ▁▂▆▇█  (5 changes, 5 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
