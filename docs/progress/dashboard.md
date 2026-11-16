# Progress dashboard

Last updated: **2026-11-16 04:42:45Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `████████████████░░░░` 82.04% | 368,832 / 449,566 | 2,589 / 2,856 | 267 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `█████████████████░░░` 83.32% | 535,524 / 642,724 | 2,341 / 2,799 | 458 |
| ov001 | `██████████████████░░` 87.67% | 287,656 / 328,118 | 1,564 / 1,723 | 159 |
| **ARM9 total** | `█████████████████░░░` 83.94% | 1,194,076 / 1,422,472 | 6,508 / 7,392 | 884 |

Next milestone: **90%** matched code — 86,149 more bytes (currently 83.94%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 04:40:09Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 04:42:39Z
- objdiff report: `build/eur/report.json` — 2026-11-16 04:42:43Z

## Task board

- 4 claimed · 9 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-hi | main_b7 | main 0x02038000-0x0206e470 | 163,008/222,234 B (73.3%), 1200/1406 fn |
| ov000-lo | ov000_e | ov000 0x022c5c80-0x02310000 | 276,900/304,276 B (91.0%), 1279/1433 fn |
| ov000-hi | ov000_g | ov000 0x02310000-0x02362b58 | 258,624/338,448 B (76.4%), 1062/1366 fn |
| ov001-thumb | ov001_t6 | ov001 0x022c5c80-0x022ed000 | 132,668/160,664 B (82.6%), 905/1032 fn |

## History

Matched bytes trend: ▁▁▆▇▇█  (6 changes, 6 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
