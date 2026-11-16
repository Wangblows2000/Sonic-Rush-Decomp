#include <nitro/types.h>

typedef struct {
    s32 active : 1;
    s32 rest : 31;
    u16 vol;
    u8 pan;
    u8 pad7;
    u8 pad8[4];
} UnkInfo_022c8d7c;

void NNS_SndUpdateDriverInfo(void);
BOOL NNS_SndReadDriverChannelInfo(s32 ch, UnkInfo_022c8d7c *info);

#pragma thumb on
void SoundTest_GetChannelLevels(u16 *l, u16 *r) {
    u16 sl;
    u16 sr;
    s32 n;
    s32 i;
    u32 pan;
    u32 vol;
    UnkInfo_022c8d7c info;

    NNS_SndUpdateDriverInfo();
    sr = 0;
    sl = 0;
    n = 0;
    for (i = 0; i < 16; i++) {
        if (NNS_SndReadDriverChannelInfo(i, &info) && info.active) {
            pan = info.pan;
            vol = info.vol;
            sl += (s32)(vol * (0x7f - pan)) / 0x7f;
            sr += (s32)(vol * pan) / 0x7f;
            n++;
        }
    }
    if (n == 0) {
        *l = 0;
        *r = 0;
        return;
    }
    *l = sl / n;
    *r = sr / n;
}
#pragma thumb reset
