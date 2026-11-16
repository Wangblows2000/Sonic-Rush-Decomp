#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 timer;
    s32 timer2;
    s32 pos;
    s32 unk10;
} UnkWork_022e8f6c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e8f6c *work;
} UnkTask_022e8f6c;

typedef struct {
    u8 pad0[8];
    u16 unk8;
    s16 unka;
} UnkStruct_0208922c;

extern UnkTask_022e8f6c *gTaskCurrent;
extern UnkStruct_0208922c gDisplayMain;

void ZoneMapPause_Unk_ov001_022e8b5c(UnkWork_022e8f6c *w);
void ZoneMapPause_Unk_ov001_022e8be0(UnkWork_022e8f6c *w, s32 n);
void ZoneMapPause_Action_OpenB(s32 a);
void ZoneMapPause_Finish(s32 a);

#pragma thumb on
void ZoneMapPause_State_Close(void) {
    UnkWork_022e8f6c *w;
    s32 v;

    w = gTaskCurrent->work;
    v = 0;
    v += w->timer * 6;
    if (v > 0x48) v = 0x48;
    w->pos = v;
    gDisplayMain.unka = v;
    ZoneMapPause_Unk_ov001_022e8b5c(w);
    switch (w->mode) {
    case 4:
        ZoneMapPause_Unk_ov001_022e8be0(w, 0);
        ZoneMapPause_Unk_ov001_022e8be0(w, 1);
        break;
    case 8:
        ZoneMapPause_Unk_ov001_022e8be0(w, 2);
        ZoneMapPause_Unk_ov001_022e8be0(w, 3);
        break;
    }
    w->timer++;
    w->timer2++;
    if (v >= 0x48) {
        switch (w->mode) {
        case 4:
            if (w->unk10 == 0) {
                ZoneMapPause_Action_OpenB(0);
            } else {
                ZoneMapPause_Finish(0);
            }
            break;
        case 8:
            ZoneMapPause_Finish(0);
            break;
        }
    }
}
#pragma thumb reset
