#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 prevMode;
    s32 folded;
    s32 saved;
} UnkFold_0202fd94;

extern UnkFold_0202fd94 gSaveDataBusy;

s32 func_0202fd14(void);
void func_0200e098(u32 a, u32 b, u32 c);
BOOL func_0200df88(u32 a);
s32 func_0200e3e4(u32 a, u32 b);
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
            gSaveDataBusy.saved = func_0202fd14();
        } else if (gSaveDataBusy.prevMode == gSaveDataBusy.mode) {
            return;
        }
        switch (gSaveDataBusy.mode) {
        case 0:
            func_0200e098(12, 0, 0);
            break;
        case 1:
            func_0200df88(0);
            break;
        case 2:
            while (!func_0200df88(1)) {
            }
            func_0200e3e4(2, 0);
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
            while (!func_0200df88(1)) {
            }
            if (gSaveDataBusy.saved != -1) {
                func_0200e3e4(gSaveDataBusy.saved, 1);
            }
            GX_DispOn();
            *(vu32 *)0x04001000 |= 0x10000;
            break;
        case 1:
            while (!func_0200df88(1)) {
            }
            if (gSaveDataBusy.saved != -1) {
                func_0200e3e4(gSaveDataBusy.saved, 1);
            }
            break;
        case 2:
            if (gSaveDataBusy.saved != -1) {
                func_0200e3e4(gSaveDataBusy.saved, 1);
            }
            break;
        }
    }
}
