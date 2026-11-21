#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x32C - 0x36];
    u32 unk32C;
} UnkStruct_0230cea8_Pl;

extern UnkStruct_0230cea8_Pl *gPlayers;
extern s32 data_ov000_0236a00c;

s32 FX_DivS32(s32 a, s32 b);
void ScreenShake_Start(s32 a);
BOOL NNS_SndPlayerCountPlayingSeqBySeqArcIdx(s32 a, s32 b);
void NNS_SndHandleReleaseSeq(s32 a);
void Sound_PlaySeqArcBankPrio(s32 a, s32 b, s32 c, s32 d, s32 e);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
s32 Sound_CalcPan(s32 a, s32 b);
void NNS_SndPlayerSetTrackPan(s32 a, s32 b, s32 c);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void IronBall_State_Roll(UnkStruct_StageObj *w) {
    UnkStruct_0230cea8_Pl *p;
    s16 spd;
    s16 d;
    u32 f;

    if ((w->unk54 & 0x10) && (w->unk58 & 1)) {
        p = gPlayers;
        if (ABS(p->unk34) > ABS(p->unk2E))
            spd = ABS(p->unk34);
        else
            spd = ABS(p->unk2E);
        if (spd < 0x900) {
            w->unk3E = 0xa00;
        } else {
            d = ABS(w->unk10 - p->unk10) >> 8;
            if (d > 0x80) {
                w->unk3C = 0x140;
                w->unk3E = spd + 0x800;
            } else if (d <= 0x40) {
                w->unk3C = 0xc;
                w->unk3E = spd - 0x300;
            } else {
                w->unk3C = 0xc;
                w->unk3E = spd + ((FX_DivS32((d - 0x40) << 8, 0x40) * 0x280) >> 8);
            }
        }
    }
    if (data_ov000_0236a00c != 0 || (gPlayers->unk32C & 0x400)) {
        w->unk54 &= ~0x10;
        w->unk3C = 0xc;
        w->unk3E = 0xa00;
    }
    if (!(w->unk1F4 & 1)) {
        f = w->unk58;
        if ((f & 0x10) && (f & 1)) {
            ScreenShake_Start(4);
            w->unk58 |= 0x10;
            w->unk30 = -0x280;
            w->unk8 = (w->unk8 & 0xffff) | (w->unk34 << 16);
            w->unk34 = 0;
            w->unk62 = 4;
            w->unk1F4 |= 1;
            w->unkC = 0x100;
        } else if (f & 1) {
            w->unk58 &= ~0x10;
        } else {
            w->unk58 |= 0x10;
        }
    } else {
        if (w->unkC != 0) {
            w->unkC--;
            if (w->unkC == 0)
                w->unk1F4 &= ~1;
        }
        if (w->unk58 & 1) {
            w->unk58 &= ~0x10;
            if (w->unk1F4 & 1) {
                w->unk62 = 4;
                ScreenShake_Start(4);
                w->unk34 = (u32)w->unk8 >> 16;
                w->unkC = 0;
            }
            w->unk1F4 &= ~1;
        } else {
            w->unk58 |= 0x10;
        }
    }
    if (w->unk58 & 1) {
        if (!NNS_SndPlayerCountPlayingSeqBySeqArcIdx(0, 0x50)) {
            NNS_SndHandleReleaseSeq(w->unk1DC);
            Sound_PlaySeqArcBankPrio(w->unk1DC, 0, 0x50, 0x20, -1);
        }
    } else {
        NNS_SndPlayerStopSeq(w->unk1DC, 0);
    }
    NNS_SndPlayerSetTrackPan(w->unk1DC, 0xffff, Sound_CalcPan(w->unk10, gPlayers->unk10));
    w->unk8 = (w->unk8 & 0xffff0000) | w->unk1;
}
