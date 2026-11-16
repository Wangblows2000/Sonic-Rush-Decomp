#include <nitro/types.h>

typedef struct UnkStruct_02346b10 UnkStruct_02346b10;

struct UnkStruct_02346b10 {
    u8 pad0[0x2E];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x324 - 0x5E];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x350 - 0x334];
    s16 unk350;
    u8 pad352[0x368 - 0x352];
    s16 unk368;
    u8 pad36A[0x384 - 0x36A];
    u8 unk384[0x38C - 0x384];
    u8 unk38C[0x3B6 - 0x38C];
    s16 unk3B6;
    u8 pad3B8[0x3D1 - 0x3B8];
    u8 unk3D1;
    u8 pad3D2[0x420 - 0x3D2];
    u16 unk420;
};

extern u32 gObjManagerFlags;
extern UnkStruct_02346b10 *gPlayers[];

#define ABS(x) ((x) < 0 ? -(x) : (x))

void func_ov000_02326f48(UnkStruct_02346b10 *w, s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void ScreenShake_Start(s32 a);
void EffBoostBurst_Create(UnkStruct_02346b10 *w, s32 a, s32 b);
void EffBoostAura_Create(UnkStruct_02346b10 *w, s32 a, s32 b);
void AttackHitbox_Create(UnkStruct_02346b10 *w, s32 l, s32 t, s32 r, s32 b, s32 c);
void AttackHitbox_SetBoostState(void);
void Player_AddTension(UnkStruct_02346b10 *w, s32 a);
void Hud_SetTensionGauge(u16 a, s32 b);
void Player_StartHighSpeed(UnkStruct_02346b10 *w);

void Player_StartBoost(UnkStruct_02346b10 *w) {
    if (!(w->unk32C & 0x80)) {
        if (!(gObjManagerFlags & 0x100)) {
            w->unk58 |= 0x4000;
        }
        if (w->unk324 == 0 && !(w->unk330 & 0x100)) {
            func_ov000_02326f48(w, 0x18, 0xa, 0x7ca5, 0x7ca5);
        }
        if (w->unk324 != 1) {
            Sound_PlaySeqArcEx(w->unk384, 0, 0xa);
        } else {
            Sound_PlaySeqArcEx(w->unk384, 0, 0x17);
        }
        NNS_SndPlayerStopSeq(w->unk38C, 0);
        w->unk3D1 = 0xc;
        w->unk32C |= 0x80;
        ScreenShake_Start(4);
        if (w->unk420 & 0x20) {
            w->unk5C |= 1;
        } else if (w->unk420 & 0x10) {
            w->unk5C &= ~1;
        }
        EffBoostBurst_Create(w, 0, 0);
        EffBoostAura_Create(w, 0, 0);
        if (w->unk58 & 0x10) {
            if (ABS(w->unk2E) < w->unk368) {
                if (w->unk5C & 1) {
                    w->unk2E = -w->unk368;
                } else {
                    w->unk2E = w->unk368;
                }
            }
        } else {
            if (ABS(w->unk34) < w->unk368) {
                if (w->unk5C & 1) {
                    w->unk34 = -w->unk368;
                } else {
                    w->unk34 = w->unk368;
                }
            }
        }
        AttackHitbox_Create(w, -0x20, -0x20, 0x20, 0x20, 0);
        AttackHitbox_SetBoostState();
        if (w->unk3B6 != 0) {
            w->unk32C |= 0x40000;
        } else {
            Player_AddTension(w, -0x70);
            Hud_SetTensionGauge(gPlayers[0]->unk350 >> 4, 1);
        }
    }
    Player_StartHighSpeed(w);
}
