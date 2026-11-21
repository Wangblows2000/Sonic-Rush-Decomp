#include <nitro/types.h>

typedef struct { u32 rom1st; u32 rom2nd; } CTRDGRomCycle;
void CTRDGi_RestoreAccessCycle(CTRDGRomCycle *c) {
    *(vu16 *)0x4000204 = (u16)(((c->rom1st) << 2) | (*(vu16 *)0x4000204 & ~0xc));
    *(vu16 *)0x4000204 = (u16)(((c->rom2nd) << 4) | (*(vu16 *)0x4000204 & ~0x10));
}
