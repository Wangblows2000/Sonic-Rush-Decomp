#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1cc4];
    s32 unk1cc4;
} UnkSub_022e5f80;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padc[0x14 - 0xc];
    s32 state;
    u8 pad18[0x74 - 0x18];
    s32 unk74;
    u8 pad78[0x90 - 0x78];
    s32 unk90;
    s32 unk94;
    s32 unk98;
    u8 pad9c[0xa4 - 0x9c];
    UnkSub_022e5f80 *sub;
} UnkWork_022e5f80;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e5f80 *work;
} UnkTask_022e5f80;

extern UnkTask_022e5f80 *gTaskCurrent;

void ZoneMap_State_Unk_ov001_022e7114(void);
void ZoneMap_SetNaviMessage(s32 a, s32 b);
void ZoneMap_Action_PlayTalkDemoBlaze(void);

#pragma thumb on
void ZoneMap_Unk_ov001_022e5f80(void) {
    UnkWork_022e5f80 *w;

    w = gTaskCurrent->work;
    switch (w->state) {
    case 1:
    case 2:
    case 3:
    case 6:
    case 8:
    case 12:
    case 13:
    case 14:
        w->unk74 = 0;
        gTaskCurrent->func = ZoneMap_State_Unk_ov001_022e7114;
        return;
    case 16:
        ZoneMap_SetNaviMessage(w->sub->unk1cc4, 0xf);
        return;
    case 4:
        if (w->unk8 == 0xf) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x10;
            ZoneMap_Action_PlayTalkDemoBlaze();
            return;
        }
        break;
    case 5:
        if (w->unk8 == 0x13) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x14;
            ZoneMap_Action_PlayTalkDemoBlaze();
            return;
        }
        break;
    case 10:
        if (w->unk8 == 0x17) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x18;
            ZoneMap_Action_PlayTalkDemoBlaze();
            return;
        }
        break;
    case 11:
        if (w->unk8 == 0x1b) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk98 = 0x1c;
            ZoneMap_Action_PlayTalkDemoBlaze();
        }
        break;
    }
}
#pragma thumb reset
