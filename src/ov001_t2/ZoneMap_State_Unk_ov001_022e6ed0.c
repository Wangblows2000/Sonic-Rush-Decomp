#include <nitro/types.h>

typedef struct {
    u8 pad0[0x74];
    u32 timer;
    u8 pad78[0x9c - 0x78];
    void *unk9c;
    void *unka0;
} UnkWork_022e6ed0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e6ed0 *work;
} UnkTask_022e6ed0;

extern UnkTask_022e6ed0 *gTaskCurrent;

void ZoneMap_Unk_ov001_022e6dec(UnkWork_022e6ed0 *w);
void NaviM_Unk_02064788(void *p);
void NaviM_Unk_02064674(void *p);
BOOL NaviM_Unk_02064758(void *p);
void ZoneMap_Unk_ov001_022e6870(void);

#pragma thumb on
void ZoneMap_State_Unk_ov001_022e6ed0(void) {
    UnkWork_022e6ed0 *w = gTaskCurrent->work;

    ZoneMap_Unk_ov001_022e6dec(w);
    if (w->timer == 0) {
        if (w->unka0 != NULL) {
            NaviM_Unk_02064788(w->unka0);
        }
        if (w->unk9c != NULL) {
            NaviM_Unk_02064674(w->unk9c);
        }
    }
    w->timer++;
    if (w->timer >= 30) {
        if (w->unka0 == NULL || !NaviM_Unk_02064758(w->unka0)) {
            w->unk9c = NULL;
            w->unka0 = NULL;
            w->timer = 0;
            ZoneMap_Unk_ov001_022e6870();
        }
    }
}
#pragma thumb reset
