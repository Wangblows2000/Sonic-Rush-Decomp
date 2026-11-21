#include <nitro/types.h>

extern u8 gObjRectCurCount[];
extern void *gObjRectCurList[][48];

void *ObjRect_RegistGet(u32 mask, s16 idx) {
    s16 i;
    u8 *cnt = gObjRectCurCount;

    for (i = 0; i < 4; i++, cnt++) {
        if (mask & (1 << i)) {
            if (idx < *cnt)
                return gObjRectCurList[i][idx];
            idx -= *cnt;
        }
    }
    return NULL;
}
