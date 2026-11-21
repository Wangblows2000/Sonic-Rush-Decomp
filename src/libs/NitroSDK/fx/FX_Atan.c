#include <nitro/types.h>

extern const s16 data_02072470[];

fx32 FX_Inv(fx32 x);

u16 FX_Atan(fx32 x)
{
    if (x >= 0) {
        if (x > 0x1000) {
            return (u16)(16384 - data_02072470[FX_Inv(x) >> 5]);
        } else if (x < 0x1000) {
            return (u16)data_02072470[x >> 5];
        } else {
            return (u16)8192;
        }
    } else {
        if (x < -0x1000) {
            return (u16)(-16384 + data_02072470[FX_Inv(-x) >> 5]);
        } else if (x > -0x1000) {
            return (u16)(-data_02072470[-x >> 5]);
        } else {
            return (u16)(-8192);
        }
    }
}
