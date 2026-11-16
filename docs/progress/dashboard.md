# Progress dashboard

Last updated: **2026-11-16 08:08:26Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `██████████████████░░` 91.81% | 412,734 / 449,566 | 2,779 / 2,855 | 76 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 97.95% | 629,560 / 642,724 | 2,766 / 2,799 | 33 |
| ov001 | `███████████████████░` 95.66% | 313,880 / 328,118 | 1,692 / 1,723 | 31 |
| **ARM9 total** | `███████████████████░` 95.48% | 1,358,238 / 1,422,472 | 7,251 / 7,391 | 140 |

Next milestone: **100%** matched code — 64,234 more bytes (currently 95.48%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 08:07:36Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 08:08:25Z
- objdiff report: `build/eur/report.json` — 2026-11-16 08:08:26Z

## Task board

- 2 claimed · 11 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-mid | main_c2 | main 0x0201c000-0x02038000 | 100,000/114,664 B (87.2%), 592/613 fn |
| ov000-hi | ov000_r | ov000 0x02310000-0x02362b58 | 330,512/338,448 B (97.7%), 1348/1366 fn |

## History

Matched bytes trend: ▁▁▅▆▆▆▇▇▇▇▇█  (12 changes, 12 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 08:08:26Z | 1,358,238 | 95.48% | 7,251 |
| 2026-11-16 07:50:47Z | 1,356,818 | 95.38% | 7,249 |
| 2026-11-16 07:36:16Z | 1,346,618 | 94.67% | 7,232 |
| 2026-11-16 06:46:17Z | 1,317,362 | 92.61% | 7,111 |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
