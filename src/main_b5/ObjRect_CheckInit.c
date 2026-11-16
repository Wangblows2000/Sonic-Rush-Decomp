#include <nitro/types.h>

extern void *gObjRectNextList[][0x30];
extern void *gObjRectCurList[][0x30];
extern u8 gObjRectNextCount[];
extern u8 gObjRectCurCount[];
extern u8 gObjRectCurGroupFlags[];

void ObjRect_CheckInit(void) {
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < gObjRectNextCount[i]; j++) {
            gObjRectCurList[i][j] = NULL;
            gObjRectNextList[i][j] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        gObjRectCurCount[i] = 0;
        gObjRectNextCount[i] = 0;
        gObjRectCurGroupFlags[i] = 0;
        gObjRectCurGroupFlags[i] = 0;
    }
}
