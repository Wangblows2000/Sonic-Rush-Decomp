#include <nitro/types.h>

extern s32 *data_0207ca28[];
extern u8 data_0207cb58[];

BOOL File_LoadArchiveEntry(void *p, u16 id, s32 c);

#pragma thumb on
BOOL TalkDemo_LoadImageByPair(s32 a, s32 b) {
    s32 v;

    if (a == 0 || b == 0)
        return FALSE;
    v = data_0207ca28[a][b];
    if (v < 0)
        return FALSE;
    return File_LoadArchiveEntry(data_0207cb58, v, -1);
}
#pragma thumb reset
