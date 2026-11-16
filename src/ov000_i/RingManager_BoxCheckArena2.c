#include <nitro/types.h>

BOOL ObjRect_BoxCheck(s32 *p, s32 a, s32 b, s32 c, s16 d, u16 e, u16 f);

void RingManager_BoxCheckArena2(s32 *p, s32 a, s32 b, s32 c, s16 d, u16 e, u16 f) {
    s16 old = *p;

    if (ObjRect_BoxCheck(p, a, b, c, d, e, f))
        return;
    if (*p < 0x2ff)
        *p += 0x5fe;
    else
        *p -= 0x5fe;
    ObjRect_BoxCheck(p, a, b, c, d, e, f);
    *p = old;
}
