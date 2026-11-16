# Progress dashboard

Last updated: **2026-11-16 07:36:16Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `██████████████████░░` 90.43% | 406,530 / 449,566 | 2,773 / 2,855 | 82 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `███████████████████░` 97.11% | 624,144 / 642,724 | 2,753 / 2,799 | 46 |
| ov001 | `███████████████████░` 95.66% | 313,880 / 328,118 | 1,692 / 1,723 | 31 |
| **ARM9 total** | `███████████████████░` 94.67% | 1,346,618 / 1,422,472 | 7,232 / 7,391 | 159 |

Next milestone: **100%** matched code — 75,854 more bytes (currently 94.67%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 07:34:22Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 07:36:14Z
- objdiff report: `build/eur/report.json` — 2026-11-16 07:36:15Z

## Task board

- 4 claimed · 9 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| main-mid | main_c2 | main 0x0201c000-0x02038000 | 99,748/114,664 B (87.0%), 591/613 fn |
| main-hi | main_b10 | main 0x02038000-0x0206e470 | 199,498/222,234 B (89.8%), 1378/1406 fn |
| ov000-hi | ov000_r | ov000 0x02310000-0x02362b58 | 325,096/338,448 B (96.1%), 1335/1366 fn |
| ov001-arm | ov001_i | ov001 0x022ed000-0x02315eb4 | 159,382/167,454 B (95.2%), 677/691 fn |

## History

Matched bytes trend: ▁▁▅▆▆▇▇▇▇█  (10 changes, 10 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 07:36:16Z | 1,346,618 | 94.67% | 7,232 |
| 2026-11-16 06:46:17Z | 1,317,362 | 92.61% | 7,111 |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
| 2026-11-16 03:37:49Z | 1,063,252 | 74.75% | 6,166 |
| 2026-11-16 03:05:06Z | 946,034 | 66.51% | 5,793 |
| 2026-11-15 21:31:28Z | 373,344 | 26.25% | 3,520 |
| 2026-11-15 20:39:17Z | 242,876 | 17.07% | 3,062 |
