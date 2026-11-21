#include <nitro/types.h>

extern u32 data_0207efbc;
extern void CARD_LockRom(u16 id);
extern void CARD_UnlockRom(u16 id);

u32 FSi_RomArchiveProc(void *arc, u32 cmd)
{
    switch (cmd) {
    case 9:
        CARD_LockRom((u16)data_0207efbc);
        return 0;
    case 10:
        CARD_UnlockRom((u16)data_0207efbc);
        return 0;
    case 1:
        return 4;
    default:
        return 8;
    }
}
