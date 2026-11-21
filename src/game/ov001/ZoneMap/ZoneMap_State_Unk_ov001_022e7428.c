#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 unk4;
    s32 unk8;
    u8 padc[4];
    u32 flags;
    s32 state;
    u8 pad18[0x74 - 0x18];
    u32 timer;
} UnkWork_022e7428;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e7428 *work;
} UnkTask_022e7428;

extern UnkTask_022e7428 *gTaskCurrent;

void ZoneMap_UpdateView(UnkWork_022e7428 *w);
s32 ZoneMap_GetSonicMaxSelectableZone(s32 a);
s32 ZoneMap_GetBlazeMaxSelectableZone(s32 a);
BOOL ZoneMap_IsSonicNaviAvailable(s32 a);
BOOL ZoneMap_IsBlazeNaviAvailable(s32 a);
void ZoneActSelect_Create(s32 a, s32 b, u32 c);
void ZoneMap_State_Unk_ov001_022e736c(void);

#pragma thumb on
void ZoneMap_State_Unk_ov001_022e7428(void) {
    UnkWork_022e7428 *w;
    s32 idx;
    s32 n;

    w = gTaskCurrent->work;
    ZoneMap_UpdateView(w);
    w->timer++;
    if (w->timer >= 0x10) {
        idx = 0;
        w->timer = 0;
        switch (w->state) {
        case 1:
            idx = 0;
            break;
        case 2:
            idx = 1;
            break;
        case 3:
            idx = 2;
            break;
        case 6:
            idx = 3;
            break;
        case 8:
            idx = 4;
            break;
        case 12:
            idx = 5;
            break;
        case 13:
            idx = 6;
            break;
        case 14:
            idx = 7;
            break;
        }
        switch (w->mode) {
        case 0:
            n = ZoneMap_GetSonicMaxSelectableZone(w->unk4);
            break;
        case 1:
            n = ZoneMap_GetBlazeMaxSelectableZone(w->unk8);
            break;
        }
        if (w->mode == 0) {
            if (!ZoneMap_IsSonicNaviAvailable(w->unk4)) {
                w->flags |= 2;
            }
        } else {
            if (!ZoneMap_IsBlazeNaviAvailable(w->unk8)) {
                w->flags |= 2;
            }
        }
        if (idx > n) idx = n;
        ZoneActSelect_Create(idx, n, w->flags);
        w->flags = 0;
        gTaskCurrent->func = ZoneMap_State_Unk_ov001_022e736c;
    }
}
#pragma thumb reset
