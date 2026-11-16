#include <nitro/types.h>

typedef struct {
    s16 bg0x;
    s16 bg0y;
    s16 bg1x;
    s16 bg1y;
    s16 bg2x;
    s16 bg2y;
    u8 padc[0x3c];
    s16 bright;
} UnkDisp_02052724;

extern UnkDisp_02052724 data_02087d04;

void GXx_SetMasterBrightness_(vu16 *reg, s32 b);
void GXS_SetGraphicsMode(s32 mode);

#define REG_DB_BG0CNT (*(vu16 *)0x04001008)
#define REG_DB_BG1CNT (*(vu16 *)0x0400100a)
#define REG_DB_BG2CNT (*(vu16 *)0x0400100c)
#define REG_DB_BG3CNT (*(vu16 *)0x0400100e)
#define REG_DB_BG0OFS (*(vu32 *)0x04001010)
#define REG_DB_BG1OFS (*(vu32 *)0x04001014)
#define REG_DB_BG2OFS (*(vu32 *)0x04001018)

void func_02052724(BOOL white) {
    if (white) {
        GXx_SetMasterBrightness_((vu16 *)0x0400106c, 0x10);
        data_02087d04.bright = 0x10;
    } else {
        GXx_SetMasterBrightness_((vu16 *)0x0400106c, -0x10);
        data_02087d04.bright = -0x10;
    }
    GXS_SetGraphicsMode(0);
    REG_DB_BG0CNT = (REG_DB_BG0CNT & 0x43) | 0x2788;
    REG_DB_BG1CNT = (REG_DB_BG1CNT & 0x43) | 0x2698;
    REG_DB_BG2CNT = (REG_DB_BG2CNT & 0x43) | 0x700;
    REG_DB_BG3CNT = (REG_DB_BG3CNT & 0x43) | 0x900;
    REG_DB_BG0CNT = (REG_DB_BG0CNT & ~3) | 1;
    REG_DB_BG1CNT = (REG_DB_BG1CNT & ~3) | 3;
    REG_DB_BG2CNT = (REG_DB_BG2CNT & ~3) | 0;
    REG_DB_BG3CNT = (REG_DB_BG3CNT & ~3) | 2;
    REG_DB_BG0OFS = 0;
    data_02087d04.bg0x = 0;
    data_02087d04.bg0y = 0;
    REG_DB_BG1OFS = 0;
    data_02087d04.bg1x = 0;
    data_02087d04.bg1y = 0;
    REG_DB_BG2OFS = 0;
    data_02087d04.bg2x = 0;
    data_02087d04.bg2y = 0;
}
