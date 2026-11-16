#include <nitro/types.h>

u8 *func_02051474(void);

#pragma thumb on
u8 PlayerSelectMenu_GetPartnerChoice(void) {
    u8 *p = func_02051474();
    if (p[1] == 0)
        return 4;
    return p[0];
}
#pragma thumb reset
