#include <nitro/types.h>

typedef struct { u32 rom1st; u32 rom2nd; } CTRDGRomCycle;
void CTRDGi_ChangeLatestAccessCycle(CTRDGRomCycle *c) {
    c->rom1st = (*(vu16 *)0x4000204 & 0xc) >> 2;
    c->rom2nd = (*(vu16 *)0x4000204 & 0x10) >> 4;
    *(vu16 *)0x4000204 = (u16)((*(vu16 *)0x4000204 & ~0xc) | 0xc);
    *(vu16 *)0x4000204 = (u16)(*(vu16 *)0x4000204 & ~0x10);
}
