#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 prevMode;
    s32 folded;
    s32 saved;
} UnkFold_0202fd94;

extern UnkFold_0202fd94 gSaveDataBusy;

s32 System_GetBacklightScreens(void);
void PM_GoSleepMode(u32 a, u32 b, u32 c);
BOOL PM_SetLCDPower(u32 a);
s32 PM_SetBackLight(u32 a, u32 b);
void GX_DispOn(void);

static inline BOOL PAD_DetectFold_0202fd94(void)
{
    return (BOOL)((*(vu16 *)0x027fffa8 & 0x8000) >> 15);
}

void System_HandleLidClose(void)
{
    if (PAD_DetectFold_0202fd94()) {
        if (gSaveDataBusy.folded != 1) {
            gSaveDataBusy.folded = 1;
            gSaveDataBusy.saved = System_GetBacklightScreens();
        } else if (gSaveDataBusy.prevMode == gSaveDataBusy.mode) {
            return;
        }
        switch (gSaveDataBusy.mode) {
        case 0:
            PM_GoSleepMode(12, 0, 0);
            break;
        case 1:
            PM_SetLCDPower(0);
            break;
        case 2:
            while (!PM_SetLCDPower(1)) {
            }
            PM_SetBackLight(2, 0);
            break;
        }
        gSaveDataBusy.prevMode = gSaveDataBusy.mode;
    } else {
        if (gSaveDataBusy.folded == 0) {
            return;
        }
        gSaveDataBusy.folded = 0;
        switch (gSaveDataBusy.prevMode) {
        case 0:
            while (!PM_SetLCDPower(1)) {
            }
            if (gSaveDataBusy.saved != -1) {
                PM_SetBackLight(gSaveDataBusy.saved, 1);
            }
            GX_DispOn();
            *(vu32 *)0x04001000 |= 0x10000;
            break;
        case 1:
            while (!PM_SetLCDPower(1)) {
            }
            if (gSaveDataBusy.saved != -1) {
                PM_SetBackLight(gSaveDataBusy.saved, 1);
            }
            break;
        case 2:
            if (gSaveDataBusy.saved != -1) {
                PM_SetBackLight(gSaveDataBusy.saved, 1);
            }
            break;
        }
    }
}
