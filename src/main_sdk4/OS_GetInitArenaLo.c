#include <nitro/types.h>

extern u32 data_0207d2d8;

u32 OS_GetConsoleType(void);

void *OS_GetInitArenaLo(u32 id)
{
    switch (id) {
    case 0:
        return (void *)0x0236aca0;
    case 2:
        if (data_0207d2d8 == 0 || (OS_GetConsoleType() & 3) == 1) {
            return NULL;
        }
        return (void *)0x02400000;
    case 3:
        return (void *)0x01fff840;
    case 4:
        return (void *)0x027e2060;
    case 5:
        return (void *)0x027ff000;
    case 6:
        return (void *)0x037f8000;
    default:
        return NULL;
    }
}
