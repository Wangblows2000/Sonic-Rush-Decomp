#include <nitro/types.h>

extern s32 data_02074ca8[];
extern s32 data_02074c3c[];
extern void *data_022c4600[];
void MsgBox_CacheFile(s32 idx);

#pragma thumb on
static inline s32 GetStageFile(s32 st) {
    if (st != 0x1c)
        return data_02074ca8[st];
    return -1;
}

static inline s32 GetStageKind(s32 st) {
    if (st != 0x1c)
        return data_02074c3c[st];
    return 0;
}

void MsgBox_PreloadFiles(s32 st, s32 unused, s32 mode) {
    s32 f1 = GetStageFile(st);
    s32 f2;
    s32 f3;
    switch (GetStageKind(st)) {
    case 1:
        f2 = 9;
        break;
    case 0:
        f2 = 8;
        break;
    case 2:
        f2 = 0xa;
        break;
    case 3:
        f2 = 0xb;
        break;
    case 4:
        f2 = 0xc;
        break;
    default:
        f2 = 8;
        break;
    }
    if (mode == 5)
        f3 = -1;
    else
        f3 = 1;
    if (f1 >= 0 && data_022c4600[f1] == NULL)
        MsgBox_CacheFile(f1);
    if (f2 >= 0 && data_022c4600[f2] == NULL)
        MsgBox_CacheFile(f2);
    if (f3 >= 0 && data_022c4600[f3] == NULL)
        MsgBox_CacheFile(f3);
    if (data_022c4600[0] == NULL)
        MsgBox_CacheFile(0);
}
#pragma thumb reset
