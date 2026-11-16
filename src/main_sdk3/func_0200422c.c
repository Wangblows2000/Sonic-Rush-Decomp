#include <nitro/types.h>

BOOL func_02003f5c(s32 *level);
BOOL func_02003f30(s32 *level);

#define REG_GXSTAT (*(vu32 *)0x04000600)
#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_POP (*(vu32 *)0x04000448)
#define REG_MTX_IDENTITY (*(vu32 *)0x04000454)

void func_0200422c(void)
{
    s32 PV_level, PJ_level;

    REG_GXSTAT |= 0x8000;
    while (func_02003f5c(&PV_level)) {
    }
    while (func_02003f30(&PJ_level)) {
    }
    REG_MTX_MODE = 3;
    REG_MTX_IDENTITY = 0;
    REG_MTX_MODE = 0;
    if (PJ_level) {
        REG_MTX_POP = PJ_level;
    }
    REG_MTX_IDENTITY = 0;
    REG_MTX_MODE = 2;
    REG_MTX_POP = PV_level;
    REG_MTX_IDENTITY = 0;
}
