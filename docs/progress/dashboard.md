# Progress dashboard

Last updated: **2026-11-15 20:39:17Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `█████░░░░░░░░░░░░░░░` 24.59% | 110,564 / 449,566 | 1,628 / 2,856 | 1,228 |
| itcm | `██████████████████░░` 90.12% | 1,860 / 2,064 | 12 / 14 | 2 |
| ov000 | `███░░░░░░░░░░░░░░░░░` 13.58% | 87,308 / 642,724 | 826 / 2,799 | 1,973 |
| ov001 | `███░░░░░░░░░░░░░░░░░` 13.15% | 43,144 / 328,118 | 596 / 1,723 | 1,127 |
| **ARM9 total** | `███░░░░░░░░░░░░░░░░░` 17.07% | 242,876 / 1,422,472 | 3,062 / 7,392 | 4,330 |

Next milestone: **20%** matched code — 41,619 more bytes (currently 17.07%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-15 20:34:57Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-15 20:35:27Z
- objdiff report: `build/eur/report.json` — 2026-11-15 20:35:28Z

## Task board

- 6 claimed · 8 open · 0 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-sdk | main_sdk2 | main 0x02000800-0x0201c000 | 35,816/112,592 B (31.8%), 497/818 fn |
| main-mid | main_a2 | main 0x0201c000-0x02038000 | 41,436/114,664 B (36.1%), 431/614 fn |
| main-hi | main_b3 | main 0x02038000-0x0206e470 | 33,312/222,234 B (15.0%), 700/1406 fn |
| ov000-lo | ov000_a | ov000 0x022c5c80-0x02310000 | 43,448/304,276 B (14.3%), 479/1433 fn |
| ov000-hi | ov000_b | ov000 0x02310000-0x02362b58 | 43,860/338,448 B (13.0%), 347/1366 fn |
| ov001-arm | ov001_b | ov001 0x022ed000-0x02315eb4 | 27,688/167,454 B (16.5%), 262/691 fn |

## History

Matched bytes trend: ▁  (1 changes, 1 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
