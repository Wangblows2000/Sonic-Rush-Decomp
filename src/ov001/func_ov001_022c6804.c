#include <nitro/types.h>

extern s32 data_ov001_02315f44[][5][3];
void func_ov001_022c6944(void *p);

void func_ov001_022c6804(s32 *p) {
    s32 i;
    s32 *row = data_ov001_02315f44[p[0]][p[0x6a0 / 4]];
    for (i = 0; i < 3; i++) {
        if (row[i] == 0) {
            return;
        }
        if (p[3] == row[i]) {
            func_ov001_022c6944(p);
            return;
        }
    }
}
