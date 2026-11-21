#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_0234e894;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x2e - 0x18];
    s16 vx;
    s16 vy;
    u8 pad32[0x64 - 0x32];
    s8 left;
    s8 top;
    s8 right;
    s8 bottom;
    u8 pad68[0x3e8 - 0x68];
    UnkObj_0234e894 *unk3E8;
} UnkStruct_0234e894;

void Player_ClampToGliderArea(UnkStruct_0234e894 *w) {
    UnkObj_0234e894 *o = w->unk3E8;

    if (o == NULL)
        return;
    if (w->x + (w->left << 8) < o->x - 0x7000) {
        w->x = o->x - 0x7000 - (w->left << 8);
        if (w->vx < 0)
            w->vx = 0;
    }
    if (w->x + (w->right << 8) > o->x + 0x7000) {
        w->x = o->x + 0x7000 - (w->right << 8);
        if (w->vx > 0)
            w->vx = 0;
    }
    if (w->y + (w->bottom << 8) > o->y + 0x5000) {
        w->y = o->y + 0x5000 - (w->bottom << 8);
        if (w->vy > 0)
            w->vy = 0;
    }
    if (w->y + (w->top << 8) < o->y - 0x5000) {
        w->y = o->y - 0x5000 - (w->top << 8);
        if (w->vy < 0)
            w->vy = 0;
    }
}
