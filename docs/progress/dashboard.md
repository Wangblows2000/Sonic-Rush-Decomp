# Progress dashboard

Last updated: **2026-11-16 03:05:06Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `██████████████░░░░░░` 70.56% | 317,200 / 449,566 | 2,440 / 2,856 | 416 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `█████████████░░░░░░░` 65.39% | 420,296 / 642,724 | 2,056 / 2,799 | 743 |
| ov001 | `█████████████░░░░░░░` 62.93% | 206,474 / 328,118 | 1,283 / 1,723 | 440 |
| **ARM9 total** | `█████████████░░░░░░░` 66.51% | 946,034 / 1,422,472 | 5,793 / 7,392 | 1,599 |

Next milestone: **70%** matched code — 49,697 more bytes (currently 66.51%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 02:59:13Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 03:05:04Z
- objdiff report: `build/eur/report.json` — 2026-11-16 03:05:06Z

## Task board

- 6 claimed · 8 open · 0 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-sdk | main_sdk6 | main 0x02000800-0x0201c000 | 105,264/112,592 B (93.5%), 799/818 fn |
| main-mid | main_a5 | main 0x0201c000-0x02038000 | 91,456/114,664 B (79.8%), 580/614 fn |
| main-hi | main_b6 | main 0x02038000-0x0206e470 | 120,480/222,234 B (54.2%), 1061/1406 fn |
| ov000-lo | ov000_e | ov000 0x022c5c80-0x02310000 | 229,768/304,276 B (75.5%), 1113/1433 fn |
| ov000-hi | ov000_f | ov000 0x02310000-0x02362b58 | 190,528/338,448 B (56.3%), 943/1366 fn |
| ov001-thumb | ov001_t3 | ov001 0x022c5c80-0x022ed000 | 77,494/160,664 B (48.2%), 751/1032 fn |

## History

Matched bytes trend: ▁▂█  (3 changes, 3 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
