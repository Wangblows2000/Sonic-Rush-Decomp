/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSysState[];
s32 WH_ChangeSysState();
void WH_StateOutStartChildKeyShare();
extern u8 sWMKeySetBuf[];
s32 WM_StartKeySharing();
s32 WH_SetError();

s32 WH_StateInStartChildKeyShare(void) {
    s32 t0 = *(u32 *)sSysState;
    if (t0 == 6) {
        return 1;
    }
    if (t0 != 4) {
        return 0;
    }
    WH_ChangeSysState(6);
    s32 t1 = WM_StartKeySharing((u32)WH_StateOutStartChildKeyShare, sWMKeySetBuf);
    if (t1 == 2) {
        return 1;
    }
    WH_SetError(t1);
    return 0;
}
