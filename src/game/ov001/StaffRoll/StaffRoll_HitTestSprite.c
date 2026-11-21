#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
} UnkRect_022cfb44;

typedef struct {
    u8 pad0[0xc];
    s32 ofs;
} UnkRes_022cfb44;

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x10 - 8];
    UnkRes_022cfb44 *res;
    u8 pad14[0x24 - 0x14];
    s32 frame;
} UnkSprite_022cfb44;

#pragma thumb on
BOOL StaffRoll_HitTestSprite(UnkSprite_022cfb44 *s, s32 x, s32 y) {
    UnkRect_022cfb44 *r;
    s32 l;
    s32 rt;
    s32 t;
    s32 b;

    r = (UnkRect_022cfb44 *)(s->frame + ((s32)s->res + s->res->ofs));
    l = s->x + r->left;
    b = s->y + r->bottom;
    t = s->y + r->top;
    rt = s->x + r->right;
    if (rt < x || l > x || t > y || b < y) {
        return FALSE;
    }
    return TRUE;
}
#pragma thumb reset
