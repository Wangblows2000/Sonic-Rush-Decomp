#include <ov000_d/player.h>

BOOL func_ov000_0232bb20(void);
void func_ov000_0232be64(s32 a);

void func_ov000_023497f4(UnkStruct_Player *w) {
    if (!func_ov000_0232bb20()) {
        w->unk3CF = 0;
    }
    if (w->unk3E4 != NULL) {
        w->unk3D0 = w->unk3E4->unkA;
        if (w->unk3E4->unkA < 0xff) {
            w->unk3E4->unkA++;
        }
        w->unk3E4 = NULL;
    }
    if ((w->unk32C & 0x4000) && (w->unk8 & 1)) {
        Player_SetAnimation(w, 0x3b);
        w->unk32C |= 4;
        if (w->unk3D0 == 0) {
            func_ov000_0232be64(0);
        }
        func_ov000_0234307c(w, 0x280 >> w->unk3D0);
        if (w->unk324 != 1) {
            if (Player_Rand() & 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x1f);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x20);
            }
        } else {
            if (Player_Rand() & 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x28);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x29);
            }
        }
        NNS_SndPlayerStopSeq(w->unk390, 0);
        Sound_PlaySeqArcEx(w->unk390, 0, 0x31);
        NNS_SndPlayerSetVolume(w->unk390, 0x7f >> w->unk3D0);
        func_ov000_02345830(w, 200);
    } else if (w->unk8 & 2) {
        if (w->unk328 == 0x39) {
            Player_SetAnimation(w, 0x3a);
            w->unk32C |= 0x4000;
            func_ov000_0234307c(w, 400 >> w->unk3D0);
            if (w->unk324 != 1) {
                if (Player_Rand() & 1) {
                    Sound_PlaySeqArcEx(w->unk384, 0, 0x1e);
                } else {
                    Sound_PlaySeqArcEx(w->unk384, 0, 0x25);
                }
            } else {
                if (Player_Rand() & 1) {
                    Sound_PlaySeqArcEx(w->unk384, 0, 0x27);
                } else {
                    Sound_PlaySeqArcEx(w->unk384, 0, 0x2e);
                }
            }
            Sound_PlaySeqArcEx(w->unk384, 0, 0x2f);
            NNS_SndPlayerSetVolume(w->unk384, 0x7f >> w->unk3D0);
        } else {
            Player_SetAnimation(w, 0x39);
            func_ov000_0234307c(w, 400 >> w->unk3D0);
            if (w->unk324 != 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x1d);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x26);
            }
            Sound_PlaySeqArcEx(w->unk384, 0, 0x30);
            NNS_SndPlayerSetVolume(w->unk384, 0x7f >> w->unk3D0);
        }
    } else {
        return;
    }
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    func_ov000_0232c7b0(w->unk3D0);
    func_ov000_02345830(w, 200);
    w->unk8 = 0;
    w->unk32A = 0;
}
