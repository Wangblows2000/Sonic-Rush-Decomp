#include <nitro/types.h>

extern u32 data_02072a18[];

BOOL MBi_IsAbleToRecv(s32 i, u32 addr, u32 size);

BOOL MB_IsAbleToLoad(s32 i, u32 addr, u32 size)
{
    switch (data_02072a18[i]) {
    case 0:
    case 2:
        return MBi_IsAbleToRecv(i, addr, size);
    case 1:
        if (addr >= 0x02000000 && addr < 0x023fe800) {
            u32 end = addr + size;
            if (addr < 0x02300000 && end > 0x02300000) {
                return FALSE;
            }
            if (end <= 0x02300000) {
                return TRUE;
            }
            if (end < 0x023fe800 && size <= 0x40000) {
                return TRUE;
            }
            return FALSE;
        }
        if (addr >= 0x037f8000 && addr < 0x0380f000) {
            return addr + size <= 0x0380f000 ? TRUE : FALSE;
        }
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
