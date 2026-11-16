#include <nitro/types.h>

typedef struct {
    s32 mode;
    u32 timer;
    s32 frame;
} UnkWork_022e8d88;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e8d88 *work;
} UnkTask_022e8d88;

extern UnkTask_022e8d88 *gTaskCurrent;

void ZoneMapPause_Unk_ov001_022e8b5c(UnkWork_022e8d88 *w);
void ZoneMapPause_Unk_ov001_022e8be0(UnkWork_022e8d88 *w, s32 i);
void ZoneMapPause_Action_CloseA(void *t);
void ZoneMapPause_Action_CloseB(void *t);

#pragma thumb on
void ZoneMapPause_State_ConfirmFlash(void) {
    UnkWork_022e8d88 *w = gTaskCurrent->work;

    ZoneMapPause_Unk_ov001_022e8b5c(w);
    switch (w->mode) {
    case 3:
        ZoneMapPause_Unk_ov001_022e8be0(w, 0);
        ZoneMapPause_Unk_ov001_022e8be0(w, 1);
        break;
    case 7:
        ZoneMapPause_Unk_ov001_022e8be0(w, 2);
        ZoneMapPause_Unk_ov001_022e8be0(w, 3);
        break;
    }
    w->timer++;
    w->frame++;
    if (w->timer >= 0x18) {
        switch (w->mode) {
        case 3:
            ZoneMapPause_Action_CloseA(NULL);
            break;
        case 7:
            ZoneMapPause_Action_CloseB(NULL);
            break;
        }
    }
}
#pragma thumb reset
