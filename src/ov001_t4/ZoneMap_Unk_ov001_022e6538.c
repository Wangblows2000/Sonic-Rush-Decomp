#include <nitro/types.h>

typedef struct {
    u8 pad0[0x6f0];
    s32 unk6f0;
    u8 pad6f4[0x1c];
    s32 unk710;
    u8 pad714[0x1cc4 - 0x714];
    s32 unk1cc4;
} UnkChild_022e6538;

typedef struct {
    u8 pad0[4];
    s32 state;
    u8 pad8[0x90 - 8];
    s32 unk90;
    s32 unk94;
    u8 pad98[0xc];
    UnkChild_022e6538 *child;
} UnkWork_022e6538;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e6538 *work;
} UnkTask_022e6538;

extern UnkTask_022e6538 *gTaskCurrent;

void ZoneMap_Unk_ov001_022e5a48(s32 v);
void ZoneMap_Unk_ov001_022e5f04(void);
void ZoneMap_Action_ShowMsgBox11(void);
void ZoneMap_Unk_ov001_022e5e28(s32 a, s32 b, s32 c);
void ZoneMap_Unk_ov001_022e5db0(s32 a);
void ZoneMap_Unk_ov001_022e3180(s32 a, s32 b);
void NaviMsg_StartIntro(void);
void ZoneMap_State_SelectNode(void);

#pragma thumb on
void ZoneMap_Unk_ov001_022e6538(void) {
    UnkWork_022e6538 *w = gTaskCurrent->work;

    switch (w->state) {
    case 0:
        w->unk94 = 1;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 1:
        w->unk94 = 3;
        NaviMsg_StartIntro();
        ZoneMap_Unk_ov001_022e5a48(0xf0);
        break;
    case 3:
        w->unk94 = 4;
        ZoneMap_Action_ShowMsgBox11();
        break;
    case 4:
        w->unk94 = 5;
        ZoneMap_Unk_ov001_022e5e28(0, 1, 2);
        break;
    case 5:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 6:
        w->unk94 = 7;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 7:
        w->unk94 = 8;
        ZoneMap_Unk_ov001_022e5e28(9, 2, 0xf);
        break;
    case 8:
        w->unk94 = 9;
        ZoneMap_Unk_ov001_022e5db0(0xf);
        break;
    case 9:
        w->unk94 = 10;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 10:
        w->unk94 = 0xb;
        ZoneMap_Unk_ov001_022e5e28(1, 2, 3);
        break;
    case 11:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 12:
        w->unk94 = 0xd;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 13:
        w->unk94 = 0xe;
        ZoneMap_Unk_ov001_022e5e28(10, 3, 0x10);
        break;
    case 14:
        w->unk94 = 0xf;
        ZoneMap_Unk_ov001_022e5db0(0x10);
        break;
    case 15:
        w->unk94 = 0x10;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 16:
        w->unk94 = 0x11;
        ZoneMap_Unk_ov001_022e5e28(2, 3, 6);
        break;
    case 17:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 18:
        w->unk94 = 0x13;
        ZoneMap_Unk_ov001_022e5e28(3, 6, 7);
        break;
    case 19:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 20:
        w->child->unk6f0 = 2;
        w->unk94 = 0x15;
        ZoneMap_Unk_ov001_022e5e28(4, 7, 8);
        break;
    case 21:
        w->unk94 = 0x16;
        ZoneMap_Unk_ov001_022e5db0(8);
        break;
    case 22:
        w->unk90 = 1;
        ZoneMap_Unk_ov001_022e3180(w->child->unk1cc4, 8);
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 23:
        w->unk94 = 0x18;
        ZoneMap_Unk_ov001_022e5e28(5, 8, 9);
        break;
    case 24:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 25:
        w->child->unk710 = 2;
        w->unk94 = 0x1a;
        ZoneMap_Unk_ov001_022e5e28(6, 9, 0xc);
        break;
    case 26:
        w->unk94 = 0x1b;
        ZoneMap_Unk_ov001_022e5db0(0xc);
        break;
    case 27:
        w->unk90 = 1;
        ZoneMap_Unk_ov001_022e3180(w->child->unk1cc4, 9);
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 28:
        w->unk94 = 0x1d;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 29:
        w->unk94 = 0x1e;
        ZoneMap_Unk_ov001_022e5e28(7, 0xc, 0xd);
        break;
    case 30:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 31:
        w->unk94 = 0x20;
        ZoneMap_Unk_ov001_022e5f04();
        break;
    case 32:
        w->unk94 = 0x21;
        ZoneMap_Unk_ov001_022e5e28(8, 0xd, 0xe);
        break;
    case 33:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    case 34:
        w->unk90 = 1;
        gTaskCurrent->func = ZoneMap_State_SelectNode;
        break;
    }
}
#pragma thumb reset
