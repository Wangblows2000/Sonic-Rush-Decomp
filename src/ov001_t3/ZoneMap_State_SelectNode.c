#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1994];
    u8 c[4];
} UnkSub_022e7f64;

typedef struct {
    s32 mode;
    u8 pad4[0x14 - 4];
    s32 state;
    s32 next;
    s32 unk1c;
    u8 pad20[0x74 - 0x20];
    s32 timer;
    u8 pad78[4];
    s32 unk7c;
    u8 pad80[0xa4 - 0x80];
    UnkSub_022e7f64 *sub;
} UnkWork_022e7f64;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e7f64 *work;
} UnkTask_022e7f64;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e7f64;

extern UnkTask_022e7f64 *gTaskCurrent;
extern UnkPad_022e7f64 gPad;

void ZoneMap_UpdateView(UnkWork_022e7f64 *w);
BOOL ZoneMap_IsCornerButtonOrXPressed(void);
void ZoneMap_State_ScrollView(void);
s32 ZoneMap_GetNextNodeFromTouch(UnkWork_022e7f64 *w, s32 a, s32 b);
s32 ZoneMap_GetNextNodeFromPad(UnkWork_022e7f64 *w);
s32 ZoneMap_GetPathIndex(s32 a, s32 b, s32 c);
void ZoneMap_State_MoveToNode(void);
void ZoneMap_RestartScrollButtonAnim(UnkWork_022e7f64 *w);
void ZoneMap_DrawScrollButton(UnkWork_022e7f64 *w);
void ZoneMap_SetCharModelRotY(void *c, s32 a);
void ZoneMap_SetCharModelAnim(void *c, s32 a);
void ZoneMap_ConfirmNode(void);
void ZoneMap_Action_OpenPause(void);

#pragma thumb on
void ZoneMap_State_SelectNode(void) {
    UnkWork_022e7f64 *w;
    s32 n;

    w = gTaskCurrent->work;
    ZoneMap_UpdateView(w);
    if (ZoneMap_IsCornerButtonOrXPressed() && w->timer != 0) {
        w->timer = 0;
        gTaskCurrent->func = ZoneMap_State_ScrollView;
        return;
    }
    n = ZoneMap_GetNextNodeFromTouch(w, w->state, 0x12);
    if (n == 0x12) {
        n = ZoneMap_GetNextNodeFromPad(w);
    }
    if (n != 0x12 && n != w->state) {
        w->timer = 0;
        w->next = n;
        w->unk1c = ZoneMap_GetPathIndex(w->mode, w->state, w->next);
        gTaskCurrent->func = ZoneMap_State_MoveToNode;
        return;
    }
    if (w->timer == 0) {
        ZoneMap_RestartScrollButtonAnim(w);
    }
    ZoneMap_DrawScrollButton(w);
    ZoneMap_SetCharModelRotY(w->sub->c, 0x1000);
    ZoneMap_SetCharModelAnim(w->sub->c, 0);
    if ((gPad.trg & 1) || n == w->state) {
        ZoneMap_ConfirmNode();
        return;
    }
    if (gPad.trg & 8) {
        ZoneMap_Action_OpenPause();
        return;
    }
    if (w->timer == 0) {
        ZoneMap_SetCharModelRotY(w->sub->c, 0x1000);
        ZoneMap_SetCharModelAnim(w->sub->c, 0);
    }
    w->timer++;
    w->unk7c = 0;
}
#pragma thumb reset
