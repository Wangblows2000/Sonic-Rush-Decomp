# Progress dashboard

Last updated: **2026-11-16 05:36:01Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `█████████████████░░░` 85.27% | 383,354 / 449,566 | 2,661 / 2,856 | 195 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `██████████████████░░` 90.62% | 582,452 / 642,724 | 2,563 / 2,799 | 236 |
| ov001 | `██████████████████░░` 91.32% | 299,642 / 328,118 | 1,641 / 1,723 | 82 |
| **ARM9 total** | `██████████████████░░` 89.11% | 1,267,512 / 1,422,472 | 6,879 / 7,392 | 513 |

Next milestone: **90%** matched code — 12,713 more bytes (currently 89.11%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 05:34:18Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 05:35:59Z
- objdiff report: `build/eur/report.json` — 2026-11-16 05:36:00Z

## Task board

- 3 claimed · 10 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-hi | main_b7 | main 0x02038000-0x0206e470 | 177,530/222,234 B (79.9%), 1272/1406 fn |
| ov000-hi | ov000_l | ov000 0x02310000-0x02362b58 | 294,592/338,448 B (87.0%), 1223/1366 fn |
| ov001-thumb | ov001_t7 | ov001 0x022c5c80-0x022ed000 | 144,426/160,664 B (89.9%), 981/1032 fn |

## History

Matched bytes trend: ▁▁▅▆▆▇█  (7 changes, 7 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
