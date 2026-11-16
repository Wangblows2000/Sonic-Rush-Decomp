/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSysState[];
void WH_StateOutStartParent();
s32 func_02011904();
s32 WH_SetError();
extern u8 sMyAid[];
extern u8 sConnectBitmap[];

s32 WH_StateInStartParent(void) {
    s32 t0 = (*(u32 *)sSysState - 0x4);
    if ((u32)t0 <= (u32)2) {
        return 1;
    }
    s32 t1 = func_02011904((u32)WH_StateOutStartParent);
    if (t1 != 2) {
        WH_SetError();
        return 0;
    }
    *(u16 *)sMyAid = 0;
    *(u16 *)sConnectBitmap = 1;
    return 1;
}
