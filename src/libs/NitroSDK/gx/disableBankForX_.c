#include <nitro/types.h>

extern u16 data_0207cfe0;

void OSi_UnlockVram(u16 lock, u16 lockID);

u32 disableBankForX_(u16 *pVRam)
{
    u32 old = *pVRam;
    *pVRam = 0;
    if (old & 1) {
        *(vu8 *)0x04000240 = 0;
    }
    if (old & 2) {
        *(vu8 *)0x04000241 = 0;
    }
    if (old & 4) {
        *(vu8 *)0x04000242 = 0;
    }
    if (old & 8) {
        *(vu8 *)0x04000243 = 0;
    }
    if (old & 0x10) {
        *(vu8 *)0x04000244 = 0;
    }
    if (old & 0x20) {
        *(vu8 *)0x04000245 = 0;
    }
    if (old & 0x40) {
        *(vu8 *)0x04000246 = 0;
    }
    if (old & 0x80) {
        *(vu8 *)0x04000248 = 0;
    }
    if (old & 0x100) {
        *(vu8 *)0x04000249 = 0;
    }
    OSi_UnlockVram((u16)old, data_0207cfe0);
    return old;
}
