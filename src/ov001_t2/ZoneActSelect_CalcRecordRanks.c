#include <nitro/types.h>

extern s32 data_ov001_0231c3f4[4];
extern s32 data_ov001_0231c404[4];

#pragma thumb on
void ZoneActSelect_CalcRecordRanks(s32 *p) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if (p[5] >= data_ov001_0231c3f4[0]) {
            p[26] = 0;
        } else if (p[5] >= data_ov001_0231c3f4[1]) {
            p[26] = 1;
        } else if (p[5] >= data_ov001_0231c3f4[2]) {
            p[26] = 2;
        } else {
            p[26] = 3;
        }
        if (p[6] >= data_ov001_0231c3f4[0]) {
            p[27] = 0;
        } else if (p[6] >= data_ov001_0231c3f4[1]) {
            p[27] = 1;
        } else if (p[6] >= data_ov001_0231c3f4[2]) {
            p[27] = 2;
        } else {
            p[27] = 3;
        }
        if (p[7] >= data_ov001_0231c404[0]) {
            p[28] = 0;
        } else if (p[7] >= data_ov001_0231c404[1]) {
            p[28] = 1;
        } else if (p[7] >= data_ov001_0231c404[2]) {
            p[28] = 2;
        } else {
            p[28] = 3;
        }
        p += 3;
    }
}
#pragma thumb reset
