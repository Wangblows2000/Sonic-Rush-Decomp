#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u8 enable;
    u8 unk3;
} UnkCfg_02043dec;

typedef struct {
    u8 pad0[2];
    u16 timer;
    u32 planeMain;
    u32 planeSub;
    UnkCfg_02043dec *cfg;
} UnkWork_02043dec;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02043dec *work;
} UnkTask_02043dec;

extern UnkTask_02043dec *gTaskCurrent;
extern u16 *gBgPaletteRamTable[2];

void Task_Destroy(UnkTask_02043dec *t);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void ScreenFlash_Main(void) {
    UnkWork_02043dec *w = gTaskCurrent->work;

    w->timer--;
    if (w->timer != 0)
        return;
    w->cfg++;
    w->timer = w->cfg->unk3;
    *gBgPaletteRamTable[0] = w->cfg->unk0;
    *gBgPaletteRamTable[1] = w->cfg->unk0;
    if (w->cfg->enable != 0 && w->timer != 0) {
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1000;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1000;
    } else {
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | (w->planeMain << 8);
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | (w->planeSub << 8);
    }
    if (w->timer != 0)
        return;
    Task_Destroy(gTaskCurrent);
}
