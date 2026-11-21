#include <nitro/types.h>

extern void SDK_IRQ_STACKSIZE(void);
#define OSi_IRQ_STACKSIZE ((s32)SDK_IRQ_STACKSIZE)
extern void SDK_SYS_STACKSIZE(void);
#define OSi_SYS_STACKSIZE ((s32)SDK_SYS_STACKSIZE)
extern void SDK_SECTION_ARENA_DTCM_START(void);
#define OSi_DTCM_ARENA_LO_DEFAULT ((u32)SDK_SECTION_ARENA_DTCM_START)

extern char data_027e0000[];
#define HW_DTCM ((u32)data_027e0000)
#define HW_DTCM_IRQ_STACK_END (HW_DTCM + 0x3f80)

extern BOOL data_0207d2d8;
u32 OS_GetConsoleType(void);

void *OS_GetInitArenaHi(int id)
{
    switch (id) {
    case 0:
        return (void *)0x023e0000;
    case 2:
        if (!data_0207d2d8 || ((OS_GetConsoleType() & 3) == 1))
            return (void *)0;
        else
            return (void *)0x02700000;
    case 3:
        return (void *)0x02000000;
    case 4: {
        u32 irqStackLo;
        u32 sysStackLo;

        irqStackLo = (u32)HW_DTCM_IRQ_STACK_END - OSi_IRQ_STACKSIZE;

        if (OSi_SYS_STACKSIZE == 0) {
            sysStackLo = HW_DTCM;
            if (sysStackLo < OSi_DTCM_ARENA_LO_DEFAULT)
                sysStackLo = OSi_DTCM_ARENA_LO_DEFAULT;
        } else if (OSi_SYS_STACKSIZE < 0) {
            sysStackLo = HW_DTCM - OSi_SYS_STACKSIZE;
        } else {
            sysStackLo = irqStackLo - OSi_SYS_STACKSIZE;
        }
        return (void *)sysStackLo;
    }
    case 5:
        return (void *)0x027ff800;
    case 6:
        return (void *)0x037f8000;
    }
    return NULL;
}
