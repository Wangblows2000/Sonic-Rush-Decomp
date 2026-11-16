#include <nitro/types.h>

extern vu32 *data_02072c90[];
extern vu16 *data_02072c98[][4];
extern vu16 *data_02072c9c[][4];
extern u32 data_02072c88[];

void func_02039200(s32 eng, s32 mode, s32 *outType, u32 *outOfs) {
    if (*data_02072c90[eng] & 0x40000000) {
        *outType = eng == 0 ? 1 : 3;
        *outOfs = 0;
        if (mode == 0)
            *outOfs += (*data_02072c98[eng][0] & 0x2000) ? 0x4000 : 0;
        else if (mode == 1)
            *outOfs += (*data_02072c9c[eng][0] & 0x2000) ? 0x6000 : 0x2000;
        else
            *outOfs += mode * 0x2000;
    } else {
        *outType = 0;
        *outOfs = data_02072c88[eng];
    }
}
