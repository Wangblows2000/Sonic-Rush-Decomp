#include <nitro/types.h>

typedef struct { s32 x, y, z; } Vec_022fe550;

typedef struct UnkStruct_022fe550 {
    u8 pad0[0xc];
    Vec_022fe550 c;
    Vec_022fe550 h;
    u8 pad24[4];
    Vec_022fe550 p;
} UnkStruct_022fe550;

BOOL ExtraBoss_TestColliderBoxes(UnkStruct_022fe550 *a, UnkStruct_022fe550 *b) {
    s32 aminx;
    s32 amaxx;
    s32 amaxy;
    s32 aminy;
    s32 bminx;
    s32 bminy;
    s32 bmaxx;
    s32 bmaxy;
    s32 aminz;
    s32 amaxz;
    s32 bminz;
    s32 bmaxz;

    aminz = a->p.z + (a->c.z - a->h.z);
    amaxz = a->p.z + (a->c.z + a->h.z);
    bminz = b->p.z + (b->c.z - b->h.z);
    bmaxz = b->p.z + (b->c.z + b->h.z);
    if (aminz < bminz && amaxz < bminz)
        return FALSE;
    if (bminz < aminz && bmaxz < aminz)
        return FALSE;
    aminx = a->p.x + (a->c.x - a->h.x);
    aminy = a->p.y + (a->c.y - a->h.y);
    amaxx = a->p.x + (a->c.x + a->h.x);
    bminx = b->p.x + (b->c.x - b->h.x);
    amaxy = a->p.y + (a->c.y + a->h.y);
    bminy = b->p.y + (b->c.y - b->h.y);
    bmaxx = b->p.x + (b->c.x + b->h.x);
    bmaxy = b->p.y + (b->c.y + b->h.y);
    if (aminx < bminx && amaxx < bminx)
        return FALSE;
    if (bminx < aminx && bmaxx < aminx)
        return FALSE;
    if (aminy < bminy && amaxy < bminy)
        return FALSE;
    if (bminy < aminy && bmaxy < aminy)
        return FALSE;
    return TRUE;
}
