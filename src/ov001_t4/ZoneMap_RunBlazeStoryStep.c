#include <nitro/types.h>

typedef struct {
    u8 pad0[0x6c0];
    s32 unk6c0;
    u8 pad6c4[0xc];
    s32 unk6d0;
    u8 pad6d4[0x720 - 0x6d4];
    s32 unk720;
    u8 pad724[0xc];
    s32 unk730;
} UnkChild_022e61f4;

typedef struct {
    u8 pad0[8];
    s32 state;
    u8 padc[8];
    s32 unk14;
    u8 pad18[0x90 - 0x18];
    s32 unk90;
    u8 pad94[4];
    s32 unk98;
    u8 pad9c[8];
    UnkChild_022e61f4 *child;
} UnkWork_022e61f4;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e61f4 *work;
} UnkTask_022e61f4;

extern UnkTask_022e61f4 *gTaskCurrent;

void ZoneMap_Action_WaitTimer(s32 v);
void ZoneMap_Action_PlayTalkDemoBlaze(void);
void ZoneMap_Action_ScrollToPathStart(s32 a, s32 b, s32 c);
void ZoneMap_Action_MoveToNode(s32 a);
void ZoneMap_Unk_ov001_022e5de0(s32 a, s32 b);
void NaviMsg_StartIntro(void);
void ZoneMap_State_SelectNode(void);

#pragma thumb on
void ZoneMap_RunBlazeStoryStep(void) {
    UnkWork_022e61f4 *w = gTaskCurrent->work;

    switch (w->state) {
    case 0:
        w->unk98 = 1;
        ZoneMap_Action_WaitTimer(0x3c);
        break;
    case 1:
        w->unk98 = 2;
        ZoneMap_Action_PlayTalkDemoBlaze();
        break;
    case 2:
        w->unk98 = 3;
        ZoneMap_Action_ScrollToPathStart(0xb, 0, 1);
        break;
    case 3:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 4:
        w->unk98 = 5;
        ZoneMap_Action_PlayTalkDemoBlaze();
        break;
    case 5:
        w->unk98 = 6;
        ZoneMap_Action_ScrollToPathStart(0xc, 1, 0x10);
        break;
    case 6:
        w->unk98 = 7;
        ZoneMap_Action_MoveToNode(0x10);
        break;
    case 7:
        w->unk98 = 8;
        ZoneMap_Action_PlayTalkDemoBlaze();
        break;
    case 8:
        w->unk98 = 10;
        NaviMsg_StartIntro();
        ZoneMap_Action_WaitTimer(0xf0);
        break;
    case 10:
        w->unk98 = 0xb;
        ZoneMap_Action_ScrollToPathStart(0xd, 0x10, 2);
        break;
    case 11:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 12:
        w->unk98 = 0xd;
        ZoneMap_Action_ScrollToPathStart(0xe, 2, 3);
        break;
    case 13:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 14:
        w->unk98 = 0xf;
        ZoneMap_Action_ScrollToPathStart(0xf, 3, 4);
        break;
    case 15:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 16:
        w->unk98 = 0x11;
        w->child->unk6c0 = 5;
        w->child->unk6d0 = 1;
        w->unk14 = 5;
        ZoneMap_Action_ScrollToPathStart(0x10, 4, 6);
        break;
    case 17:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 18:
        w->unk98 = 0x13;
        ZoneMap_Unk_ov001_022e5de0(5, 4);
        break;
    case 19:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 20:
        w->unk98 = 0x15;
        w->child->unk6d0 = 1;
        ZoneMap_Action_ScrollToPathStart(0x11, 5, 8);
        break;
    case 21:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 22:
        w->unk98 = 0x17;
        ZoneMap_Action_ScrollToPathStart(0x12, 8, 10);
        break;
    case 23:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 24:
        w->unk98 = 0x19;
        w->child->unk720 = 5;
        w->child->unk730 = 1;
        w->unk14 = 0xb;
        ZoneMap_Action_ScrollToPathStart(0x13, 10, 0xc);
        break;
    case 25:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 26:
        w->unk98 = 0x1b;
        ZoneMap_Unk_ov001_022e5de0(0xb, 10);
        break;
    case 27:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 28:
        w->unk98 = 0x1d;
        w->child->unk730 = 1;
        ZoneMap_Action_ScrollToPathStart(0x14, 0xb, 0xd);
        break;
    case 29:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 30:
        w->unk98 = 0x1f;
        ZoneMap_Action_PlayTalkDemoBlaze();
        break;
    case 31:
        w->unk98 = 0x20;
        ZoneMap_Action_ScrollToPathStart(0x15, 0xd, 0xe);
        break;
    case 32:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 33:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    }
}
#pragma thumb reset
