#include <nitro/types.h>

BOOL G3X_GetMtxStackLevelPV(s32 *level);
BOOL G3X_GetMtxStackLevelPJ(s32 *level);

#define REG_GXSTAT (*(vu32 *)0x04000600)
#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_POP (*(vu32 *)0x04000448)
#define REG_MTX_IDENTITY (*(vu32 *)0x04000454)

void G3X_InitMtxStack(void)
{
    s32 PV_level, PJ_level;

    REG_GXSTAT |= 0x8000;
    while (G3X_GetMtxStackLevelPV(&PV_level)) {
    }
    while (G3X_GetMtxStackLevelPJ(&PJ_level)) {
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
