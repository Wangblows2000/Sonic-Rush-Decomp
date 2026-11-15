#include <nitro/types.h>

extern u32 data_02072a18[];

BOOL func_02016cac(s32 i, u32 addr, u32 size)
{
    switch (data_02072a18[i]) {
    case 2:
        if (addr >= 0x027ffe00 && addr + size <= 0x027fff60) {
            return TRUE;
        }
        break;
    case 0:
        if (addr >= 0x02000000 && addr + size <= 0x022c0000) {
            return TRUE;
        }
        break;
    case 1:
        if (addr >= 0x022c0000 && addr + size <= 0x02300000) {
            return TRUE;
        }
        if (addr >= 0x02000000 && addr + size <= 0x02300000) {
            return TRUE;
        }
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
