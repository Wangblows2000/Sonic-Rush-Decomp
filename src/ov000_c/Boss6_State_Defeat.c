#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x78 - 0x60];
    s32 unk78;
    s32 unk7C;
    u8 pad80[0x84 - 0x80];
    s32 unk84;
    s32 unk88;
    u8 pad8C[0x8E - 0x8C];
    u16 unk8E;
    u16 unk90;
} UnkStruct_Cam;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x32C - 0x20C];
    u32 unk32C;
} UnkStruct_Player;

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    u8 pad0[0x248];
    s32 unk248;
} UnkStruct_Fx;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x960 - 0x20C];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xF50 - 0xD0C];
    s32 unkF50;
    u8 padF54[0x1118 - 0xF54];
    s32 unk1118;
    s32 unk111C;
} UnkStruct_Boss;

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Cam data_ov000_0236a48c;
extern UnkStruct_Player *gPlayers;
extern s32 data_ov000_023658cc[][3];
extern s32 data_ov000_023658d0[][3];
extern s32 data_ov000_023658d4[][3];

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_Unk_ov000_022e237c(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void StageObj_ClearPause(void);
UnkStruct_Fx *BossEffect_CreateBlast(s32 a, s32 x, s32 y, s32 z);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Boss6_State_DefeatFinish(void);

static inline u32 GetVisiblePlane(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetVisiblePlane(u32 p) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (p << 8);
}

void Boss6_State_Defeat(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    UnkStruct_Cam *cam;
    UnkStruct_Fx *fx;

    w->unkF50--;
    if (gDisplayMain.unk48 > 0)
        gDisplayMain.unk48 -= 2;
    if (w->unkF50 > 0)
        return;
    w->unk1118++;
    if (w->unk1118 >= 3) {
        w->base.unk68 = Boss6_State_DefeatFinish;
        SetVisiblePlane(GetVisiblePlane() & ~4);
        cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
        Model_SetAnimation(m, 0, w->unkD08, 0x19, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        StageObj_ClearPause();
        w->base.unk54 &= ~0x20;
        gPlayers->base.unk5C |= 0x20;
        gPlayers->unk32C |= 0x200000;
        gDisplayMain.unk48 = 0x10;
        cam->unk54 = data_ov000_023658cc[w->unk111C][0] << 12;
        cam->unk58 = -data_ov000_023658d0[w->unk111C][0] << 12;
        cam->unk5C = data_ov000_023658d4[w->unk111C][0] << 12;
        cam->unk78 = cam->unk7C = 0x12c000;
        cam->unk84 = cam->unk88 = -0x80000;
        cam->unk8E = cam->unk90 = 0;
        fx = BossEffect_CreateBlast(0, data_ov000_023658cc[w->unk111C][0] << 8, data_ov000_023658d0[w->unk111C][0] << 8, data_ov000_023658d4[w->unk111C][0] << 8);
        fx->unk248 = 0x800;
        Sound_PlaySeqArc(0, 0, 0x7d);
        w->unkF50 = 300;
        NaviMsg_SetFaceAnim(0xd);
        NaviMsg_ShowMessage(0x13, 0xf0);
    } else {
        w->unkF50 = 0x1e;
        Boss6_Unk_ov000_022e237c(w->unk111C, w->unk1118);
        gDisplayMain.unk48 = 0x10;
        Sound_PlaySeqArc(0, 0, 0x7c);
    }
}
