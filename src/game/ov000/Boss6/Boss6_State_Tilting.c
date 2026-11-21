#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xF50 - 0x20C];
    s32 unkF50;
    u8 padF54[0xF6A - 0xF54];
    s16 unkF6A;
    s16 unkF6C;
    u8 padF6E[0x1124 - 0xF6E];
    s32 unk1124;
} UnkStruct_Boss;

extern s32 data_ov000_0236375c[];
extern s32 data_ov000_023638e8[][3];
extern s32 data_ov000_023638ec[][3];
extern s32 data_ov000_023638f0[][3];

void Boss6_ApplyTiltToPlayer(UnkStruct_Boss *w);
void Boss6_CreatePiece(s32 a, s32 x, s32 y, s32 z);
void Boss6_SpawnDebrisTimed(UnkStruct_Boss *w);
void Boss6_Unk_ov000_022e1f10(UnkStruct_Boss *w);
void Boss6_State_TiltReturn(void);

void Boss6_State_Tilting(UnkStruct_Boss *w) {
    s32 i;

    w->unkF50--;
    Boss6_ApplyTiltToPlayer(w);
    if (w->unkF50 == data_ov000_0236375c[w->unk1124]) {
        i = Rand() & 3;
        w->unk1124++;
        Boss6_CreatePiece(0, data_ov000_023638e8[i][0], data_ov000_023638ec[i][0], data_ov000_023638f0[i][0]);
    }
    Boss6_SpawnDebrisTimed(w);
    Boss6_Unk_ov000_022e1f10(w);
    if (w->unkF50 > 0)
        return;
    w->base.unk68 = Boss6_State_TiltReturn;
    w->unkF6A = -0x79;
    w->unkF6C = 0;
}
