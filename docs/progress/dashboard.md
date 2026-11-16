# Progress dashboard

Last updated: **2026-11-16 10:27:13Z** · version `eur` · regenerate with `python tools/project.py dashboard`

Numbers come only from the objdiff report (100 % matched functions in non-generated units) and `symbols.txt` sizes. Nothing estimated, nothing unverified.

## Verified matched code (ARM9)

| Module | Matched code | Bytes | Functions | Remaining |
| ------ | ------------ | ----- | --------- | --------- |
| main | `████████████████████` 99.91% | 449,150 / 449,566 | 2,853 / 2,872 | 19 |
| itcm | `████████████████████` 100.00% | 2,064 / 2,064 | 14 / 14 | 0 |
| ov000 | `████████████████████` 100.00% | 642,724 / 642,724 | 2,799 / 2,799 | 0 |
| ov001 | `████████████████████` 100.00% | 328,118 / 328,118 | 1,723 / 1,723 | 0 |
| **ARM9 total** | `████████████████████` 99.97% | 1,422,056 / 1,422,472 | 7,389 / 7,408 | 19 |

Next milestone: **100%** matched code — 416 more bytes (currently 99.97%).

- ROM SHA-1 (`build/eur/rush_eur.sha1.ok`): **OK** — last OK 2026-11-16 10:26:23Z
- GNU relink (`build/eur/reports/gnu_relink.json`): **OK (11/11 sections)** — 2026-11-16 10:27:11Z
- objdiff report: `build/eur/report.json` — 2026-11-16 10:27:12Z

## Task board

- 1 claimed · 12 open · 1 done (`python tools/tasks.py list`)

| Task | Agent | Scope | Verified in scope |
| ---- | ----- | ----- | ----------------- |
| ov001-arm | ov001_j | ov001 0x022ed000-0x02315eb4 | 167,454/167,454 B (100.0%), 691/691 fn |

## History

Matched bytes trend: ▁▁▅▅▆▆▇▇▇▇▇▇▇▇▇▇█  (17 changes, 18 runs)

| UTC | Matched bytes | % code | Functions |
| --- | ------------- | ------ | --------- |
| 2026-11-16 10:27:13Z | 1,422,056 | 99.97% | 7,389 |
| 2026-11-16 10:17:39Z | 1,420,980 | 99.90% | 7,386 |
| 2026-11-16 09:51:06Z | 1,408,712 | 99.03% | 7,357 |
| 2026-11-16 09:39:54Z | 1,372,122 | 96.46% | 7,292 |
| 2026-11-16 08:54:28Z | 1,369,110 | 96.25% | 7,266 |
| 2026-11-16 08:08:26Z | 1,358,238 | 95.48% | 7,251 |
| 2026-11-16 07:50:47Z | 1,356,818 | 95.38% | 7,249 |
| 2026-11-16 07:36:16Z | 1,346,618 | 94.67% | 7,232 |
| 2026-11-16 06:46:17Z | 1,317,362 | 92.61% | 7,111 |
| 2026-11-16 06:14:24Z | 1,303,530 | 91.64% | 7,059 |
