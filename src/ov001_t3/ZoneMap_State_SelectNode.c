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
s32 ZoneMap_Unk_ov001_022e6c90(UnkWork_022e7f64 *w, s32 a, s32 b);
s32 ZoneMap_GetNextNodeFromPad(UnkWork_022e7f64 *w);
s32 ZoneMap_Unk_ov001_022e6998(s32 a, s32 b, s32 c);
void ZoneMap_Unk_ov001_022e794c(void);
void ZoneMap_Unk_ov001_022e3510(UnkWork_022e7f64 *w);
void ZoneMap_Unk_ov001_022e349c(UnkWork_022e7f64 *w);
void ZoneMap_Unk_ov001_022e3840(void *c, s32 a);
void ZoneMap_Unk_ov001_022e3844(void *c, s32 a);
void ZoneMap_Unk_ov001_022e612c(void);
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
    n = ZoneMap_Unk_ov001_022e6c90(w, w->state, 0x12);
    if (n == 0x12) {
        n = ZoneMap_GetNextNodeFromPad(w);
    }
    if (n != 0x12 && n != w->state) {
        w->timer = 0;
        w->next = n;
        w->unk1c = ZoneMap_Unk_ov001_022e6998(w->mode, w->state, w->next);
        gTaskCurrent->func = ZoneMap_Unk_ov001_022e794c;
        return;
    }
    if (w->timer == 0) {
        ZoneMap_Unk_ov001_022e3510(w);
    }
    ZoneMap_Unk_ov001_022e349c(w);
    ZoneMap_Unk_ov001_022e3840(w->sub->c, 0x1000);
    ZoneMap_Unk_ov001_022e3844(w->sub->c, 0);
    if ((gPad.trg & 1) || n == w->state) {
        ZoneMap_Unk_ov001_022e612c();
        return;
    }
    if (gPad.trg & 8) {
        ZoneMap_Action_OpenPause();
        return;
    }
    if (w->timer == 0) {
        ZoneMap_Unk_ov001_022e3840(w->sub->c, 0x1000);
        ZoneMap_Unk_ov001_022e3844(w->sub->c, 0);
    }
    w->timer++;
    w->unk7c = 0;
}
#pragma thumb reset
