#include <nitro/types.h>

extern const u8 data_020726e0[];

u16 SND_CalcChannelVolume(s32 dB)
{
    u8 result;
    s32 shift;
    if (dB < -723) {
        dB = -723;
    } else if (dB > 0) {
        dB = 0;
    }
    result = data_020726e0[(u32)(dB + 723)];
    if (dB < -240) {
        shift = 3;
    } else if (dB < -120) {
        shift = 2;
    } else if (dB < -60) {
        shift = 1;
    } else {
        shift = 0;
    }
    return (u16)(result | (shift << 8));
}
