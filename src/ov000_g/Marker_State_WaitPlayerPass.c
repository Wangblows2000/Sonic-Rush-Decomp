#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
} UnkStruct_02310574_P;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    u8 pad30[0x5C - 0x30];
    u16 unk5C;
} UnkStruct_02310574;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
} UnkStruct_02310574_Sys;

extern UnkStruct_02310574_P *gPlayers;
extern UnkStruct_02310574_Sys gGameState;

void CheckpointTimePopup_Create(UnkStruct_02310574_P *p, s32 t);
void CheckpointTimePopup_SetTimeDigits(u16 n, s32 t);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Marker_Action_StageClear(UnkStruct_02310574 *w);

void Marker_State_WaitPlayerPass(UnkStruct_02310574 *w) {
    UnkStruct_02310574_P *p = gPlayers;

    if (w->unk5C & 8)
        w->unk5C |= 0x10;
    if (w->unk14 + 0x2000 <= p->unk14)
        return;
    if (w->unk10 < p->unk10 && w->unk10 >= p->unk1C) {
        w->unk2E++;
        if (w->unk8 < w->unk2E) {
            CheckpointTimePopup_Create(p, gGameState.unk20 - w->unkC);
            CheckpointTimePopup_SetTimeDigits(w->unk2E - 1, gGameState.unk20 - w->unkC);
            w->unkC = gGameState.unk20;
            Sound_PlaySeqArcEx(NULL, 0, 0x44);
            w->unk8 = w->unk2E;
            w->unk5C &= ~0x10;
            if ((u32)w->unk8 >= 5) {
                Marker_Action_StageClear(w);
                return;
            }
        }
    }
    if (w->unk10 > p->unk10 && w->unk10 <= p->unk1C)
        w->unk2E--;
}
