#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022e3f18;

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[8];
} UnkPt_022e3f18;

typedef struct {
    s32 n;
    s32 st[8];
    u8 data[100 - 0x24];
} UnkPath_022e3f18;

typedef struct {
    UnkPt_022e3f18 pts[0x11];
    UnkPath_022e3f18 paths[4];
} UnkStruct_022e3f18;

void KeyframeAnim_Init(UnkAnim_022e3f18 *a, s32 b, s32 c);
void KeyframeAnim_SetAnim(UnkAnim_022e3f18 *a, s32 id);
void KeyframeAnim_Update(UnkAnim_022e3f18 *a);
void ZoneMap_BuildSplinePath(s32 n, void *out, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3);
void ZoneMap_BuildLinePath(s32 n, void *out, s32 x0, s32 y0, s32 x1, s32 y1, s32 x3, s32 y3);

#pragma thumb on
void func_ov001_022e3f18(UnkStruct_022e3f18 *p, s32 res, s32 i0, s32 a1, s32 a2, s32 i3, s32 idx, s32 n) {
    s32 x0;
    s32 y0;
    s32 x2;
    s32 y2;
    UnkAnim_022e3f18 anim;
    s32 x1;
    s32 y1;
    s32 x3;
    s32 y3;
    s32 i;

    KeyframeAnim_Init(&anim, res, 0);
    x0 = p->pts[i0].x << 12;
    y0 = p->pts[i0].y << 12;
    KeyframeAnim_SetAnim(&anim, a1);
    KeyframeAnim_Update(&anim);
    x1 = anim.x << 12;
    y1 = anim.y << 12;
    if (a2 >= 0) {
        KeyframeAnim_SetAnim(&anim, a2);
        KeyframeAnim_Update(&anim);
        x2 = anim.x << 12;
        y2 = anim.y << 12;
    }
    x3 = p->pts[i3].x << 12;
    y3 = p->pts[i3].y << 12;
    if (a2 >= 0) {
        ZoneMap_BuildSplinePath(n, p->paths[idx].data, x0, y0, x1, y1, x2, y2, x3, y3);
    } else {
        ZoneMap_BuildLinePath(n, p->paths[idx].data, x0, y0, x1, y1, x3, y3);
    }
    p->paths[idx].n = n;
    for (i = 0; i < 8; i++) {
        p->paths[idx].st[i] = 2;
    }
}
#pragma thumb reset
