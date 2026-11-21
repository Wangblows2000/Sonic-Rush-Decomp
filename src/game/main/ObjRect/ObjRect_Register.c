#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u16 flags;
    u8 padc[0x1c];
    u8 layers;
} UnkSpr_0206e080;

extern u8 gObjRectNextCount[];
extern UnkSpr_0206e080 *gObjRectNextList[][0x30];
extern u8 gObjRectNextGroupFlags[];

void ObjRect_Register(UnkSpr_0206e080 *s) {
    u8 i;

    if (!(s->flags & 4))
        return;
    for (i = 0; i < 4; i++) {
        if (s->layers & (1 << i)) {
            if (gObjRectNextCount[i] >= 0x30)
                return;
            gObjRectNextList[i][gObjRectNextCount[i]] = s;
            gObjRectNextGroupFlags[i] |= s->layers;
            gObjRectNextCount[i]++;
            return;
        }
    }
}
