# Progress dashboard

Last updated: **2026-11-16 03:37:49Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `███████████████░░░░░` 77.22% | 347,144 / 449,566 | 2,526 / 2,856 | 330 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `███████████████░░░░░` 72.80% | 467,884 / 642,724 | 2,141 / 2,799 | 658 |
| ov001 | `███████████████░░░░░` 75.02% | 246,160 / 328,118 | 1,485 / 1,723 | 238 |
| **ARM9 total** | `███████████████░░░░░` 74.75% | 1,063,252 / 1,422,472 | 6,166 / 7,392 | 1,226 |

Next milestone: **80%** matched code — 74,726 more bytes (currently 74.75%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 03:35:36Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 03:37:44Z
- objdiff report: `build/eur/report.json` — 2026-11-16 03:37:47Z

## Task board

- 7 claimed · 7 open · 0 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-sdk | main_sdk6 | main 0x02000800-0x0201c000 | 107,140/112,592 B (95.2%), 803/818 fn |
| main-mid | main_a5 | main 0x0201c000-0x02038000 | 96,440/114,664 B (84.1%), 583/614 fn |
| main-hi | main_b6 | main 0x02038000-0x0206e470 | 143,564/222,234 B (64.6%), 1140/1406 fn |
| ov000-lo | ov000_e | ov000 0x022c5c80-0x02310000 | 249,468/304,276 B (82.0%), 1162/1433 fn |
| ov000-hi | ov000_f | ov000 0x02310000-0x02362b58 | 218,416/338,448 B (64.5%), 979/1366 fn |
| ov001-thumb | ov001_t4 | ov001 0x022c5c80-0x022ed000 | 95,680/160,664 B (59.6%), 830/1032 fn |
| ov001-arm | ov001_f | ov001 0x022ed000-0x02315eb4 | 150,480/167,454 B (89.9%), 655/691 fn |

## History

Matched bytes trend: ▁▂▆█  (4 changes, 4 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
