#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkDisp_0204edc0;

extern u32 data_02087cc0;
extern UnkDisp_0204edc0 gDisplayMain;

void Vram_SetBankConfig(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void GX_DispOn(void);
BOOL func_0204ed94(void);
BOOL func_0204ed68(void);
s32 func_0205a37c(void (*fn)(void));
void SceneChange_Unk_02054e4c(void);
void SceneChange_Unk_02054e68(void);

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
    if ((data_02087cc0 & 0x80000000) == 0) {
        if (!func_0204ed94())
            return;
    } else {
        if (!func_0204ed68())
            return;
    }
    if (!func_0205a37c(SceneChange_Unk_02054e4c))
        return;
    if ((data_02087cc0 & 0x80000000) == 0)
        SceneChange_Unk_02054e4c();
    else
        SceneChange_Unk_02054e68();
}
