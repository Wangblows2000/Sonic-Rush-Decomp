#include <nitro/types.h>

typedef struct UnkStruct_02311bcc_o {
    u8 pad0[0x28];
    s32 unk28;
} UnkStruct_02311bcc_o;

typedef struct UnkStruct_02311bcc {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[2];
    u16 unk16;
    u8 pad18[0x44 - 0x18];
    s32 unk44;
    u8 pad48[0x114 - 0x48];
    s32 unk114;
    u8 pad118[4];
    UnkStruct_02311bcc_o *unk11C;
    u8 pad120[0x134 - 0x120];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    u8 pad140[4];
    s32 unk144;
    u8 pad148[0x1f0 - 0x148];
    s32 unk1F0;
} UnkStruct_02311bcc;

typedef struct UnkStruct_02311bcc_k {
    u16 held;
    u16 trig;
} UnkStruct_02311bcc_k;

extern UnkStruct_02311bcc_k gTouchState;
extern u32 data_ov001_023178d8[];

BOOL SpecialStagePlayer_Unk_ov001_023121f0(u16 a, u16 b);
void SpecialStagePlayer_UpdateTouchSteer(UnkStruct_02311bcc *p);
void SpecialStagePlayer_ChangeState(UnkStruct_02311bcc *p, s32 v);
void SpecialStage_Unk_ov001_02312dd0(UnkStruct_02311bcc *p, s32 v);
void SpecialStagePlayer_Unk_ov001_02312d7c(UnkStruct_02311bcc *p);
void SpecialStagePlayer_UpdateTurn(UnkStruct_02311bcc *p);
BOOL SpecialStagePlayer_CheckTouchTarget(UnkStruct_02311bcc *p);
s32 SpecialStagePlayer_Unk_ov001_02312250(UnkStruct_02311bcc *p);

static inline BOOL IsLeft_02311bcc(u16 a) {
    if (a < 0x8000)
        return TRUE;
    return FALSE;
}

void SpecialStagePlayer_State_Run(UnkStruct_02311bcc *p) {
    p->unk12 = 0x8000;
    if (p->unk44 >= 0x708 || p->unk44 <= -0x708 || p->unk144 > 0)
        p->unk1F0 = 1;
    else
        p->unk1F0 = 0;
    if (p->unk114 != 0) {
        if (p->unk144 <= 0 && !SpecialStagePlayer_Unk_ov001_023121f0(gTouchState.held, gTouchState.trig))
            SpecialStagePlayer_UpdateTouchSteer(p);
    } else {
        p->unk134 = 0;
        p->unk138 = 0x100;
        if (p->unk11C != NULL && p->unk11C->unk28 > -0x6e000) {
            SpecialStagePlayer_ChangeState(p, 0x18);
            SpecialStage_Unk_ov001_02312dd0(p, 0x1e);
            SpecialStagePlayer_Unk_ov001_02312d7c(p);
            return;
        }
    }
    SpecialStagePlayer_UpdateTurn(p);
    if (SpecialStagePlayer_CheckTouchTarget(p))
        return;
    if (SpecialStagePlayer_Unk_ov001_02312250(p) && (u16)p->unk134 == p->unk16 && (p->unk16 == (u16)data_ov001_023178d8[0] || p->unk16 == (u16)data_ov001_023178d8[2])) {
        p->unk44 = 0x4b0;
        p->unk44 = p->unk44 * (IsLeft_02311bcc(p->unk16) > 0 ? 1 : -1);
        p->unk13C = 0;
        SpecialStagePlayer_ChangeState(p, 6);
        return;
    }
    if (p->unk16 == (u16)p->unk134) {
        p->unk44 = 0;
        p->unk13C = 0;
    }
}
