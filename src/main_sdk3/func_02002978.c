#include <nitro/types.h>

extern const s16 data_02072470[];

fx32 func_02002240(fx32 x);

u16 func_02002978(fx32 x)
{
    if (x >= 0) {
        if (x > 0x1000) {
            return (u16)(16384 - data_02072470[func_02002240(x) >> 5]);
        } else if (x < 0x1000) {
            return (u16)data_02072470[x >> 5];
        } else {
            return (u16)8192;
        }
    } else {
        if (x < -0x1000) {
            return (u16)(-16384 + data_02072470[func_02002240(-x) >> 5]);
        } else if (x > -0x1000) {
            return (u16)(-data_02072470[-x >> 5]);
        } else {
            return (u16)(-8192);
        }
    }
}
