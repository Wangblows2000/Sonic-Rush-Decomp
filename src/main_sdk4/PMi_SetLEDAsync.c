#include <nitro/types.h>

u32 PM_SendUtilityCommandAsync(u32 v);

u32 PMi_SetLEDAsync(u32 type)
{
    u32 v;
    switch (type) {
    case 1:
        v = 1;
        break;
    case 3:
        v = 2;
        break;
    case 2:
        v = 3;
        break;
    default:
        v = 0;
        break;
    }
    if (v == 0) {
        return 0xffff;
    }
    return PM_SendUtilityCommandAsync(v);
}
