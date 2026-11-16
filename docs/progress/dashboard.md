# Progress dashboard

Last updated: **2026-11-16 09:39:54Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `███████████████████░` 94.62% | 425,378 / 449,566 | 2,815 / 2,872 | 57 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 98.07% | 630,292 / 642,724 | 2,769 / 2,799 | 30 |
| ov001 | `███████████████████░` 95.82% | 314,388 / 328,118 | 1,694 / 1,723 | 29 |
| **ARM9 total** | `███████████████████░` 96.46% | 1,372,122 / 1,422,472 | 7,292 / 7,408 | 116 |

Next milestone: **100%** matched code — 50,350 more bytes (currently 96.46%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 09:35:44Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 09:37:40Z
- objdiff report: `build/eur/report.json` — 2026-11-16 09:37:42Z

## Task board

- 2 claimed · 11 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| ov000-hi | ov000_t | ov000 0x02310000-0x02362b58 | 331,008/338,448 B (97.8%), 1350/1366 fn |
| ov001-arm | ov001_j | ov001 0x022ed000-0x02315eb4 | 159,498/167,454 B (95.2%), 678/691 fn |

## History

Matched bytes trend: ▁▁▅▆▆▆▇▇▇▇▇▇▇█  (14 changes, 15 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 09:39:54Z | 1,372,122 | 96.46% | 7,292 |
| 2026-11-16 08:54:28Z | 1,369,110 | 96.25% | 7,266 |
| 2026-11-16 08:08:26Z | 1,358,238 | 95.48% | 7,251 |
| 2026-11-16 07:50:47Z | 1,356,818 | 95.38% | 7,249 |
| 2026-11-16 07:36:16Z | 1,346,618 | 94.67% | 7,232 |
| 2026-11-16 06:46:17Z | 1,317,362 | 92.61% | 7,111 |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
| 2026-11-16 05:36:01Z | 1,267,512 | 89.11% | 6,879 |
| 2026-11-16 04:42:45Z | 1,194,076 | 83.94% | 6,508 |
| 2026-11-16 04:10:13Z | 1,119,952 | 78.73% | 6,280 |
