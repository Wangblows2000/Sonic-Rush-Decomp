#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkDisp_0204edc0;

extern u32 gSystemResetParameter;
extern UnkDisp_0204edc0 gDisplayMain;

void Vram_SetBankConfig(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void GX_DispOn(void);
BOOL Game_StartupSaveDataOnBoot(void);
BOOL Game_StartupSaveDataAfterReset(void);
s32 SaveData_LoadOrShowError(void (*fn)(void));
void SceneChange_ToSegaLogo(void);
void SceneChange_ToTitleScreen(void);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)
#define REG_POWCNT (*(vu16 *)0x04000304)

void Game_Init(void) {
    Vram_SetBankConfig(0);
    GX_SetGraphicsMode(1, 0, 0);
    REG_DISPCNT &= ~0x1f00;
    REG_POWCNT = (REG_POWCNT & ~0x20e) | 0x202;
    REG_POWCNT |= 0x8000;
    GX_DispOn();
    REG_DB_DISPCNT |= 0x10000;
    gDisplayMain.unk4 = 0;
    gDisplayMain.unk6 = 0;
    if ((gSystemResetParameter & 0x80000000) == 0) {
        if (!Game_StartupSaveDataOnBoot())
            return;
    } else {
        if (!Game_StartupSaveDataAfterReset())
            return;
    }
    if (!SaveData_LoadOrShowError(SceneChange_ToSegaLogo))
        return;
    if ((gSystemResetParameter & 0x80000000) == 0)
        SceneChange_ToSegaLogo();
    else
        SceneChange_ToTitleScreen();
}
