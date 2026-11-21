#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 unk8;
    s32 type;
} UnkNode_022e7728;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    s32 unkc;
} UnkSub_022e7728;

typedef struct {
    u8 pad0[4];
    UnkSub_022e7728 sub;
    u8 pad14[0x674 - 0x14];
    UnkNode_022e7728 nodes[(0x1b08 - 0x674) / 16];
    u8 pad1b04[4];
    u8 obj1b08[0x1cc4 - 0x1b08];
    s32 unk1cc4;
} UnkMap_022e7728;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x54];
    s32 unk6c;
    s32 unk70;
    s32 unk74;
    u8 pad78[0x2c];
    UnkMap_022e7728 *map;
} UnkWork_022e7728;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e7728 *work;
} UnkTask_022e7728;

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e7728;

typedef struct {
    u16 cont;
} UnkPad_022e7728;

extern UnkTask_022e7728 *gTaskCurrent;
extern UnkTouch_022e7728 gTouchState;
extern UnkPad_022e7728 gPad;

void ZoneMap_SetScrollArrowMode(void *p, s32 a);
void ZoneMap_SetScrollArrowBlink(void *p, s32 a);
void ZoneMap_RestartBackButtonAnim(UnkWork_022e7728 *w);
void ZoneMap_DrawBackButton(UnkWork_022e7728 *w);
void ZoneMap_UpdateScrollArrows(UnkWork_022e7728 *w);
void ZoneMap_UpdateView(UnkWork_022e7728 *w);
u32 ZoneMap_GetTouchedArrow(u16 a, u16 b);
BOOL ZoneMap_IsCornerButtonOrBPressed(void);
BOOL ZoneMap_IsPointOnNode(UnkWork_022e7728 *w, s32 i, s32 x, s32 y);
void ZoneMap_SetNodeNaviMessage(UnkWork_022e7728 *w, s32 i);
void ZoneMap_SetNaviMessage(s32 a, s32 b);
void ZoneMap_CalcClampedScroll(void *p, s32 x, s32 y, s32 *a, s32 *b);
void ZoneMap_Action_ScrollToPos(s32 x, s32 y, s32 a);

#pragma thumb on
void ZoneMap_State_ScrollView(void) {
    UnkWork_022e7728 *w = gTaskCurrent->work;
    UnkSub_022e7728 *sub = &w->map->sub;
    BOOL done;
    s32 x;
    s32 y;
    u32 dir;
    s32 i;

    ZoneMap_SetScrollArrowMode(w->map->obj1b08, 1);
    ZoneMap_SetScrollArrowBlink(w->map->obj1b08, 0);
    if (w->unk74 == 0) {
        ZoneMap_RestartBackButtonAnim(w);
    }
    ZoneMap_DrawBackButton(w);
    ZoneMap_UpdateScrollArrows(w);
    ZoneMap_UpdateView(w);
    dir = 0;
    done = FALSE;
    if (gTouchState.flags & 0x10) {
        dir = ZoneMap_GetTouchedArrow(gTouchState.unk0, gTouchState.unk2);
        if (dir < 4) {
            dir = 1 << dir;
        } else {
            dir = 0;
        }
    } else {
        u16 k = gPad.cont;
        if (k & 0x20) {
            dir |= 1;
        } else if (k & 0x10) {
            dir |= 4;
        }
        if (k & 0x40) {
            dir |= 2;
        } else if (k & 0x80) {
            dir |= 8;
        }
    }
    if (dir != 0) {
        s32 maxx = sub->unk8 - 0x108;
        s32 maxy = sub->unkc - 0xc0;
        if (dir & 1) {
            w->unk6c -= 2;
            if (w->unk6c < 8) {
                w->unk6c = 8;
            }
        } else if (dir & 4) {
            w->unk6c += 2;
            if (w->unk6c >= maxx) {
                w->unk6c = maxx;
            }
        }
        if (dir & 2) {
            w->unk70 -= 2;
            if (w->unk70 < 0) {
                w->unk70 = 0;
            }
        } else if (dir & 8) {
            w->unk70 += 2;
            if (w->unk70 >= maxy) {
                w->unk70 = maxy;
            }
        }
    } else if (ZoneMap_IsCornerButtonOrBPressed()) {
        done = TRUE;
    } else if (gTouchState.flags & 0x40) {
        s32 tx = gTouchState.x;
        s32 ty = gTouchState.y;
        tx += w->unk6c;
        ty += w->unk70;
        for (i = 0; i < 17; i++) {
            if (ZoneMap_IsPointOnNode(w, i, tx, ty)) {
                s32 t = w->map->nodes[i].type;
                if (t == 0) {
                    ZoneMap_SetNodeNaviMessage(w, i);
                } else if (t == 1 || t == 3) {
                    ZoneMap_SetNaviMessage(w->map->unk1cc4, 0x17);
                }
                break;
            }
        }
        if (i >= 17) {
            done = TRUE;
        }
    }
    w->unk74++;
    if (done) {
        ZoneMap_SetScrollArrowMode(w->map->obj1b08, 0);
        ZoneMap_SetScrollArrowBlink(w->map->obj1b08, 0);
        ZoneMap_CalcClampedScroll(sub, w->map->nodes[w->unk14].x, w->map->nodes[w->unk14].y, &x, &y);
        ZoneMap_Action_ScrollToPos(x, y, 0x5000);
    }
}
#pragma thumb reset
