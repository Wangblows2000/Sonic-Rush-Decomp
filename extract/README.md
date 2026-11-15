# extract/

Put your own legally dumped ROM here. Nothing in this directory except this
README is tracked by Git.

| Version | File name                 | SHA-1                                      |
| ------- | ------------------------- | ------------------------------------------ |
| `eur`   | `baserom_rush_eur.nds`    | `52e5024efcf7617a820fa23789b37e5168c3c4fc` |

`eur` is *Sonic Rush (Europe) (En,Ja,Fr,De,Es,It)*, game code `ASCP`,
ROM version byte `0x00`. Other regions/revisions (`ASCE`, `ASCJ`) are not
configured yet. Do not rename a different revision to the name above: the
tooling checks the SHA-1 and refuses to continue.

`python tools/project.py extract eur` populates `extract/eur/` from the ROM.
