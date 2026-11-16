# Progress dashboard

Last updated: **2026-11-16 07:50:47Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `██████████████████░░` 91.81% | 412,734 / 449,566 | 2,779 / 2,855 | 76 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 97.73% | 628,140 / 642,724 | 2,764 / 2,799 | 35 |
| ov001 | `███████████████████░` 95.66% | 313,880 / 328,118 | 1,692 / 1,723 | 31 |
| **ARM9 total** | `███████████████████░` 95.38% | 1,356,818 / 1,422,472 | 7,249 / 7,391 | 142 |

Next milestone: **100%** matched code — 65,654 more bytes (currently 95.38%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 07:49:48Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 07:50:45Z
- objdiff report: `build/eur/report.json` — 2026-11-16 07:50:47Z

## Task board

- 2 claimed · 11 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-mid | main_c2 | main 0x0201c000-0x02038000 | 100,000/114,664 B (87.2%), 592/613 fn |
| ov000-hi | ov000_r | ov000 0x02310000-0x02362b58 | 329,092/338,448 B (97.2%), 1346/1366 fn |

## History

Matched bytes trend: ▁▁▅▆▆▆▇▇▇▇█  (11 changes, 11 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 07:50:47Z | 1,356,818 | 95.38% | 7,249 |
| 2026-11-16 07:36:16Z | 1,346,618 | 94.67% | 7,232 |
| 2026-11-16 06:46:17Z | 1,317,362 | 92.61% | 7,111 |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
