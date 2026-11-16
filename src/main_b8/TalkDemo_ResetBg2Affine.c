#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    s32 x;
    s32 y;
    s16 sx;
    s16 sy;
    u16 angle;
    u8 pad32[0xe];
} UnkObj_02061a78;

void TalkDemo_ApplyBg2Affine(UnkObj_02061a78 *o);

#pragma thumb on
BOOL TalkDemo_ResetBg2Affine(UnkObj_02061a78 *o) {
    UnkObj_02061a78 tmp = {0};

    if (o == NULL) {
        o = &tmp;
        tmp.sx = 0x100;
        tmp.sy = 0x100;
        tmp.angle = 0;
    }
    o->x = -0x5000;
    o->y = -0x5000;
    TalkDemo_ApplyBg2Affine(o);
    return TRUE;
}
#pragma thumb reset
