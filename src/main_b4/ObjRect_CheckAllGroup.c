#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u16 flags;
} UnkCol_0206ded8;

extern u16 gObjRectPrevAtkFlag;
extern u16 gObjRectPrevDefFlag;
extern u8 gObjRectCalledNoHit;
extern UnkCol_0206ded8 *gObjRectCurList[4][48];
extern u8 gObjRectCurCount[4];
extern u8 gObjRectCurGroupFlags[4];

void ObjRect_CheckGroup(UnkCol_0206ded8 **a, UnkCol_0206ded8 **b, u8 na, u8 nb, u8 j);
void ObjRect_CheckOut(void);

void ObjRect_CheckAllGroup(void) {
    u8 i;
    u8 j;
    UnkCol_0206ded8 *c;

    gObjRectPrevAtkFlag = 0;
    gObjRectPrevDefFlag = 0;
    gObjRectCalledNoHit = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < gObjRectCurCount[i]; j++) {
            c = gObjRectCurList[i][j];
            if (c != NULL && (c->flags & 0x800))
                c->flags &= ~0x40;
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (gObjRectCurCount[j] != 0 && (gObjRectCurGroupFlags[i] & (0x10 << j)))
                ObjRect_CheckGroup(gObjRectCurList[i], gObjRectCurList[j], gObjRectCurCount[i], gObjRectCurCount[j], j);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < gObjRectCurCount[i]; j++) {
            c = gObjRectCurList[i][j];
            if (c != NULL && (c->flags & 0x800) && !(c->flags & 0x40))
                c->flags &= ~0x80;
        }
    }
    ObjRect_CheckOut();
}
