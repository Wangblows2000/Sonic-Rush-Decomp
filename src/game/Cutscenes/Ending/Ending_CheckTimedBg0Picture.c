#include <nitro/types.h>

extern s32 data_ov001_02315f44[][5][3];
void Ending_CreateBg0PictureTask(void *p);

void Ending_CheckTimedBg0Picture(s32 *p) {
    s32 i;
    s32 *row = data_ov001_02315f44[p[0]][p[0x6a0 / 4]];
    for (i = 0; i < 3; i++) {
        if (row[i] == 0) {
            return;
        }
        if (p[3] == row[i]) {
            Ending_CreateBg0PictureTask(p);
            return;
        }
    }
}
