#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x58];
    u32 unk58;
    u8 pad5C[0x3E8 - 0x5C];
    void *unk3E8;
    u8 pad3EC[0x420 - 0x3EC];
    u16 unk420;
} UnkStruct_023156e0_P;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[6];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u8 pad5C[0x1DC - 0x5C];
    void *unk1DC;
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x204 - 0x1E4];
    UnkStruct_023156e0_P *unk204;
} UnkStruct_023156e0;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023156e0_Sys;

extern UnkStruct_023156e0_Sys gGameState;
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void NNS_SndPlayerStopSeq(void *p, s32 a);
void Saucer_Explode(UnkStruct_023156e0 *w);
s32 ObjSpd_UpSet(s32 a, s32 b, s32 c);
s32 ObjSpd_DownSet(s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_02315a48(UnkStruct_023156e0 *w);

void func_ov000_023156e0(UnkStruct_023156e0 *w) {
    s16 acc = 8;
    s16 max = 0x1c0;
    UnkStruct_023156e0_P *p;
    s32 idx;

    if (w->unk1E0->unk4 & 1) {
        acc <<= 1;
        max <<= 1;
    }
    if (w->unk58 & 0xf) {
        NNS_SndPlayerStopSeq(w->unk1DC, 0);
        Saucer_Explode(w);
        return;
    }
    p = w->unk204;
    if (p != NULL) {
        if (p->unk420 & 0x20)
            w->unkC = ObjSpd_UpSet(w->unkC, -0x100, 0x1800);
        else if (p->unk420 & 0x10)
            w->unkC = ObjSpd_UpSet(w->unkC, 0x100, 0x1800);
        else
            w->unkC = ObjSpd_DownSet(w->unkC, 0x300);
        if (!(gGameState.unk20 & 7))
            Sound_PlaySeqArcEx(w->unk1DC, 0, 0x6c);
        if (p->unk3E8 != w) {
            w->unk204 = NULL;
            w->unk30 = 0x200;
            func_ov000_02315a48(w);
            return;
        }
    }
    w->unk34 = ObjSpd_UpSet(w->unk34, acc, max);
    idx = (s32)((u16)((u8)(w->unk1 - 0x40) << 8) & 0xffff) >> 4;
    w->unk2E = ((w->unk34 * data_0206e470[(u32)((idx << 1) + 1)]) >> 12) << 1;
    w->unk30 = (w->unk34 * data_0206e470[(u32)(idx << 1)]) >> 12;
    w->unk30 += w->unk8;
    w->unk8 = ObjSpd_DownSet(w->unk8, 4);
    if (w->unk204 != NULL && (w->unk204->unk58 & 2))
        w->unk30 = 0;
    w->unk1 = w->unkC >> 8;
}
