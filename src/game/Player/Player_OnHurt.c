#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*unk68)(void);
    u8 pad6C[0x2C4 - 0x6C];
    u32 unk2C4;
    u8 pad2C8[0x324 - 0x2C8];
    u8 unk324;
    u8 pad325[3];
    s16 unk328;
    u8 pad32A[2];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x350 - 0x334];
    s16 unk350;
    u8 pad352[0x376 - 0x352];
    s16 unk376;
    u8 pad378[0x37C - 0x378];
    s16 unk37C;
    u8 pad37E[0x384 - 0x37E];
    u8 unk384[0x3AE - 0x384];
    s16 unk3AE;
    u8 pad3B0[2];
    s16 unk3B2;
    u8 pad3B4[0x3C4 - 0x3B4];
    s16 unk3C4;
} UnkStruct_023492f0_Player;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_023492f0_Player *unk18;
} UnkStruct_023492f0;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023492f0_Arg;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Player_AddTension(UnkStruct_023492f0_Player *w, s32 a);
void Hud_SetTensionGauge(u32 a, s32 b);
void RingManager_ScatterRings(UnkStruct_023492f0_Player *w);
void Player_Action_Die(UnkStruct_023492f0_Player *w);
void Player_InitState(UnkStruct_023492f0_Player *w);
void Player_SetAnimation(UnkStruct_023492f0_Player *w, s32 a);
void ScreenShake_Start(s32 a);
void Player_State_Hurt(void);

#define ABS_023492f0(x) ((x) < 0 ? -(x) : (x))

void Player_OnHurt(UnkStruct_023492f0 *o, UnkStruct_023492f0_Arg *arg) {
    UnkStruct_023492f0_Player *w = o->unk18;
    s32 v;
    u32 old;

    if (w->unk3C4 != 0) {
        return;
    }
    if (w->unk62 & 0xf) {
        return;
    }
    v = (w->unk58 & 0x8000) ? w->unk2E : w->unk34;
    if (arg->unk20 & 0x40) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x43);
    }
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        NaviMsg_SetFaceAnim(0xc);
        NaviMsg_ShowMessage(0x11, 0x96);
    }
    if (!(w->unk32C & 0x10000000)) {
        Player_AddTension(w, -0x4b0);
        Hud_SetTensionGauge((u32)(w->unk350 << 12) >> 16, 1);
    }
    if (arg->unk20 & 0x20) {
        if (w->unk3AE != 0) {
            Sound_PlaySeqArcEx(w->unk384, 0, 5);
            RingManager_ScatterRings(w);
        }
        Player_Action_Die(w);
        return;
    }
    if (!(w->unk32C & 0x10000000) && w->unk3AE == 0) {
        Player_Action_Die(w);
        return;
    }
    old = w->unk330;
    Player_InitState(w);
    w->unk330 |= old & 0x40000;
    if (ABS_023492f0(v) >= w->unk37C) {
        Player_SetAnimation(w, 0x10);
    } else {
        Player_SetAnimation(w, 0xf);
    }
    if (!(w->unk32C & 0x10000000)) {
        Sound_PlaySeqArcEx(w->unk384, 0, 5);
        RingManager_ScatterRings(w);
    }
    w->unk32C &= ~0x30000000;
    ScreenShake_Start(3);
    w->unk3B2 = w->unk376;
    w->unk2C4 |= 5;
    if (ABS_023492f0(v) < 0x180) {
        v = 0;
    }
    w->unk2E = 0x180;
    w->unk30 = -0x300;
    w->unk34 = 0;
    if (v >= 0) {
        if (v > 0) {
            w->unk2E = -w->unk2E;
        } else if (!(w->unk5C & 1)) {
            w->unk2E = -w->unk2E;
        }
    }
    if (w->unk328 == 0x10) {
        w->unk2E = v >> 1;
    }
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 &= ~1;
    w->unk68 = Player_State_Hurt;
    if (w->unk324 != 1) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x21);
    } else {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x2a);
    }
}
