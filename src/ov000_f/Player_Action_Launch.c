#include <nitro/types.h>

typedef struct UnkStruct_02355b44 UnkStruct_02355b44;

struct UnkStruct_02355b44 {
    u8 pad0;
    u8 unk1;
    u8 pad2[5 - 2];
    u8 unk5;
    u8 pad6[0x8 - 6];
    s32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(UnkStruct_02355b44 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x32A - 0x2CC];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x352 - 0x330];
    s16 unk352;
    u8 pad354[0x384 - 0x354];
    u8 unk384[0x3CF - 0x384];
    u8 unk3CF;
    u8 pad3D0[0x3DA - 0x3D0];
    s16 unk3DA;
};

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_02355b44_aad8;

extern s16 data_0206e470[];
extern UnkStruct_02355b44_aad8 gCamera;

#define IDX_02355b44(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02355b44(a) data_0206e470[(u32)(IDX_02355b44(a) << 1)]
#define COS_02355b44(a) data_0206e470[(u32)((IDX_02355b44(a) << 1) + 1)]

void Player_ResetGimmickState(UnkStruct_02355b44 *w);
void Player_Action_LandOnGround(UnkStruct_02355b44 *w, u8 ang);
void Math_RotateVec2S16(s16 *a, s16 *b, u8 c);
void Player_State_InAir(UnkStruct_02355b44 *w);
void Player_SetAnimation(UnkStruct_02355b44 *w, s32 state);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);
void EffAirBubble_Spawn(UnkStruct_02355b44 *w, s32 a, s32 b, u16 c);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_Launch(UnkStruct_02355b44 *w, s16 vx, s16 vy) {
    if (w->unk32C & 0x400) {
        return;
    }
    Player_ResetGimmickState(w);
    if (w->unk58 & 1) {
        w->unk2E = w->unk34;
    }
    Player_Action_LandOnGround(w, 0);
    Math_RotateVec2S16(&vx, &vy, w->unk5);
    if (!(w->unk58 & 0x10)) {
        w->unk3DA = w->y >> 8;
    }
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 &= ~1;
    w->unk68 = Player_State_InAir;
    if (vx != 0) {
        Player_SetAnimation(w, 0x16);
        w->unk2E = vx;
        if (w->unk2E < 0) {
            if (w->unk34 > 0) {
                w->unk34 = 0;
            }
            w->unk5C |= 1;
        } else {
            if (w->unk34 < 0) {
                w->unk34 = 0;
            }
            w->unk5C &= ~1;
        }
    } else {
        Player_SetAnimation(w, 0x13);
        w->unk5C |= 4;
        w->unk2E = (w->unk34 * COS_02355b44((u16)(w->unk1 << 8))) >> 12;
    }
    if (vy != 0) {
        w->unk30 = vy;
    } else {
        w->unk30 = (w->unk34 * SIN_02355b44((u16)(w->unk1 << 8))) >> 12;
    }
    ObjRect_SetAtkDefFlags(w->unk2CC, 0, 0);
    w->unk32C &= ~0xf;
    w->unk32C |= 1;
    w->unk32C |= 2;
    if (vx != 0) {
        w->unk352 = 0x40;
    }
    w->unkC = 0;
    w->unk8 = 0;
    w->unk32A = 0;
    w->unk3CF = 0;
    if (w->unk32C & 0x4000000) {
        w->unk2E = (w->unk2E >> 1) + (w->unk2E >> 2);
        w->unk30 = (w->unk30 >> 1) + (w->unk30 >> 2);
        EffAirBubble_Spawn(w, 0, 0, gCamera.unk62);
    }
    Sound_PlaySeqArcEx(w->unk384, 0, 0x42);
}
