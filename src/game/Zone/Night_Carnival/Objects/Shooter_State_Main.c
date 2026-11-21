#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x58 - 1];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x420 - 0x5E];
    u16 unk420;
} UnkStruct_023171b4_P;

typedef struct {
    u8 pad0[4];
    UnkStruct_023171b4_P *unk4;
    UnkStruct_023171b4_P *unk8;
} UnkStruct_023171b4_L;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_023171b4_S;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x36 - 0x32];
    s16 unk36;
    u8 pad38[0x48 - 0x38];
    s16 unk48;
    u8 pad4A[0x58 - 0x4A];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x130 - 0x64];
    UnkStruct_023171b4_L *unk130;
    u8 pad134[0x1B8 - 0x134];
    void *unk1B8;
    u8 pad1BC[0x1DC - 0x1BC];
    void *unk1DC;
    UnkStruct_ObjParam *unk1E0;
} UnkStruct_023171b4;

extern UnkStruct_023171b4_S gCamera;

void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Player_Action_SpringLaunch(UnkStruct_023171b4_P *p, s16 a, s16 b);
void Player_SetSourceMapObject(UnkStruct_023171b4_P *p, UnkStruct_023171b4 *w);
void NNS_SndPlayerStopSeq(void *p, s32 a);
s32 ObjShift_Set(s32 v, s32 a, s32 b, s32 c, s32 d);

void Shooter_State_Main(UnkStruct_023171b4 *w) {
    s32 flag;
    UnkStruct_023171b4_P *p;
    s32 old;
    s32 v;
    s16 s;

    flag = 0;
    if (w->unk1B8 != NULL && (p = w->unk130->unk8) != NULL && p->unk0 == 1) {
        if (w->unk1E0->unk2 == 0xbb) {
            gCamera.unkC8 |= 0x40000;
            if (w->unk130->unk4 == p) {
                if (p->unk420 & 0x80) {
                    if (w->unk8 == 0)
                        Sound_PlaySeqArcEx(w->unk1DC, 0, 0x73);
                    flag = 1;
                    w->unk8++;
                    if (w->unk8 > 0x40)
                        w->unk8 = 0x40;
                    w->unk30 = 0x100;
                    if (w->unkC > 0x400)
                        w->unk30 = 0;
                } else {
                    if (w->unk8 > 8) {
                        s = -(s16)(w->unk8 << 6);
                        w->unk30 = s;
                        Player_Action_SpringLaunch(p, 0, s);
                        Player_SetSourceMapObject(p, w);
                        w->unk62 = 0x40;
                        w->unk8 = 0;
                        w->unk1B8 = NULL;
                    }
                    w->unk8 = 0;
                }
            }
        } else if (w->unk130->unk4 != p) {
            if ((p->unk58 & 0x2000000) &&
                ((!(w->unk5C & 1) && !(p->unk5C & 1)) || ((w->unk5C & 1) && (p->unk5C & 1)))) {
                if (w->unk8 == 0)
                    Sound_PlaySeqArcEx(w->unk1DC, 0, 0x73);
                flag = 1;
                w->unk8++;
                if (w->unk8 > 0x40)
                    w->unk8 = 0x40;
                w->unk58 |= 0x4000000;
                v = w->unkC;
                if (v < 0)
                    v = -v;
                if (v > 0x400)
                    w->unk48 = 0;
                else
                    w->unk48 = 0x100;
            } else {
                w->unk2E = 0;
                if (w->unk8 > 8) {
                    s = w->unk8 << 6;
                    if (!(w->unk5C & 1))
                        s = -s;
                    w->unk2E = s;
                    Player_Action_SpringLaunch(p, s, 0);
                    Player_SetSourceMapObject(p, w);
                    w->unk62 = 0x40;
                    w->unk8 = 0;
                    w->unk1B8 = NULL;
                }
                w->unk8 = 0;
            }
        }
    } else {
        if (w->unk1E0->unk2 == 0xbb)
            gCamera.unkC8 &= ~0x40000;
        w->unk8 = 0;
    }
    if (!flag) {
        old = w->unkC;
        NNS_SndPlayerStopSeq(w->unk1DC, 0);
        w->unkC = ObjShift_Set(w->unkC, 0, 2, 0x200, 0x80);
        if (w->unkC == 0)
            w->unk1B8 = w;
        if (w->unk1E0->unk2 == 0xbb)
            w->unk30 = w->unkC - old;
        else
            w->unk2E = w->unkC - old;
    } else if (w->unk1E0->unk2 == 0xbb) {
        w->unkC += w->unk30;
    } else {
        w->unkC += w->unk2E + w->unk36;
    }
}
