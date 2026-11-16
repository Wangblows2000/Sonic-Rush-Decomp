#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 unk4;
    s32 unk8;
    u8 padc[0x74 - 0xc];
    s32 timer;
    u8 pad78[0x90 - 0x78];
    s32 unk90;
    s32 unk94;
    s32 unk98;
    void *unk9c;
    void *unka0;
} UnkWork_022e6870;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022e6870 *work;
} UnkTask_022e6870;

extern UnkTask_022e6870 *gTaskCurrent;

void ZoneMap_State_Unk_ov001_022e6ed0(void);
void ZoneMap_State_Unk_ov001_022e7f64(void);
void ZoneMap_Unk_ov001_022e6538(void);
void ZoneMap_Unk_ov001_022e61f4(void);

#pragma thumb on
void ZoneMap_Unk_ov001_022e6870(void) {
    UnkWork_022e6870 *w = gTaskCurrent->work;

    w->timer = 0;
    if (w->unk9c != NULL || w->unka0 != NULL) {
        gTaskCurrent->fn = ZoneMap_State_Unk_ov001_022e6ed0;
        return;
    }
    if (w->unk90 != 0) {
        gTaskCurrent->fn = ZoneMap_State_Unk_ov001_022e7f64;
        return;
    }
    switch (w->mode) {
    case 0:
        if (w->unk94 != 0x24) {
            w->unk4 = w->unk94;
            w->unk94 = 0x24;
        }
        ZoneMap_Unk_ov001_022e6538();
        break;
    case 1:
        if (w->unk98 != 0x23) {
            w->unk8 = w->unk98;
            w->unk98 = 0x23;
        }
        ZoneMap_Unk_ov001_022e61f4();
        break;
    }
}
#pragma thumb reset
