#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u8 pad2;
    u8 unk3;
} UnkCfg_02043f0c;

typedef struct {
    u8 pad0[2];
    u16 unk2;
    u32 planeMain;
    u32 planeSub;
    UnkCfg_02043f0c *cfg;
} UnkWork_02043f0c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02043f0c *work;
} UnkTask_02043f0c;

extern UnkTask_02043f0c *gScreenFlashTask;
extern UnkCfg_02043f0c gScreenFlashDefaultTable;
extern u16 *gBgPaletteRamTable[2];

UnkTask_02043f0c *Task_Create(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void ScreenFlash_Main(void);
void ScreenFlash_Destructor(void);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void ScreenFlash_Create(UnkCfg_02043f0c *cfg) {
    UnkTask_02043f0c *t;
    UnkWork_02043f0c *w;

    if (gScreenFlashTask == NULL) {
        vu16 z;
        t = Task_Create(ScreenFlash_Main, ScreenFlash_Destructor, 0, 0x3e00, 3, 0x10);
        if (t == (UnkTask_02043f0c *)func_02006abc(0))
            return;
        gScreenFlashTask = t;
        w = t->work;
        z = 0;
        MIi_CpuClear16(z, w, 0x10);
        w->planeMain = (REG_DISPCNT & 0x1f00) >> 8;
        w->planeSub = (REG_DB_DISPCNT & 0x1f00) >> 8;
    }
    if (cfg != NULL)
        w->cfg = cfg;
    else
        w->cfg = &gScreenFlashDefaultTable;
    w->unk2 = w->cfg->unk3;
    *gBgPaletteRamTable[0] = w->cfg->unk0;
    *gBgPaletteRamTable[1] = w->cfg->unk0;
    REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1000;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1000;
}
