#include <nitro/types.h>

extern s32 sSysState;
extern s32 sErrCode;

void WH_SetError(s32 value) {
    if ((u32)(sSysState - 9) > 1) {
        sErrCode = value;
    }
}
