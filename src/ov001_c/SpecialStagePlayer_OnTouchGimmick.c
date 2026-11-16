#include <nitro/types.h>

typedef struct UnkStruct_02312f24_e {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x20 - 0x18];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x5e - 0x2c];
    u16 unk5E;
    u8 pad60[0x68 - 0x60];
    s16 unk68;
    s16 unk6A;
} UnkStruct_02312f24_e;

typedef struct UnkStruct_02312f24 {
    u8 pad0[0x20];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x134 - 0x2c];
    s32 unk134;
    s32 unk138;
    u8 pad13C[0x148 - 0x13c];
    s32 unk148;
    u8 pad14C[0x150 - 0x14c];
    s16 unk150;
    u8 pad152[0x22c - 0x152];
    void (*unk22C)(void);
} UnkStruct_02312f24;

void SpecialStagePlayer_State_Trick(void);
void SpecialStagePlayer_State_TrickInput(void);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void SpecialStagePlayer_Action_StartTrick(UnkStruct_02312f24 *p, s32 v);
void SpecialStagePlayer_ChangeState(UnkStruct_02312f24 *p, s32 v);
void SpecialStageMap_SetScrollSpeed(s32 v);
void SpecialStageCamera_SetMode(s32 v);
void SpecialStagePlayer_StartDash(UnkStruct_02312f24 *p, UnkStruct_02312f24_e *e);
void func_ov001_02311ec8(UnkStruct_02312f24 *p);

void SpecialStagePlayer_OnTouchGimmick(UnkStruct_02312f24 *p, UnkStruct_02312f24_e *e) {
    if (p->unk22C == SpecialStagePlayer_State_Trick || p->unk22C == SpecialStagePlayer_State_TrickInput) {
        switch (e->unk68) {
        case 0:
        case 1:
            p->unk148 += e->unk5E;
            Sound_PlaySeqArc(0, 0x10, 3);
            break;
        }
        return;
    }
    switch (e->unk68) {
    case 0:
    case 1:
        p->unk148 += e->unk5E;
        Sound_PlaySeqArc(0, 0x10, 3);
        break;
    case 2:
        SpecialStagePlayer_Action_StartTrick(p, e->unk6A);
        break;
    case 3:
        p->unk150 = e->unk6A;
        SpecialStagePlayer_ChangeState(p, 9);
        SpecialStageMap_SetScrollSpeed(0x800);
        SpecialStageCamera_SetMode(1);
        p->unk20 = e->unk20;
        p->unk24 = e->unk24;
        p->unk28 = e->unk28;
        break;
    case 4:
        SpecialStagePlayer_StartDash(p, e);
        p->unk134 = e->unk16;
        func_ov001_02311ec8(p);
        p->unk138 = 0x800;
        break;
    case 5:
        SpecialStagePlayer_StartDash(p, e);
        p->unk134 = e->unk16 - 0x4000;
        func_ov001_02311ec8(p);
        p->unk138 = 0x111;
        break;
    case 6:
        SpecialStagePlayer_StartDash(p, e);
        p->unk134 = e->unk16 + 0x4000;
        func_ov001_02311ec8(p);
        p->unk138 = 0x111;
        break;
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
        Sound_PlaySeqArc(0, 0x10, 0x15);
        break;
    case 0x17:
        SpecialStageCamera_SetMode(6);
        break;
    }
}
