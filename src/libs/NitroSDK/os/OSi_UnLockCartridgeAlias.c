#include <nitro/types.h>

void OS_UnlockCartridge(void);

asm void OSi_UnLockCartridgeAlias(void)
{
    ldr r1, =OS_UnlockCartridge
    bx r1
}

