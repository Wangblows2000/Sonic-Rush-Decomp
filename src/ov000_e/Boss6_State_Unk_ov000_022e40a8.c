#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD5E - 0xD0C];
    u16 unkD5E;
    u8 padD60[0xF50 - 0xD60];
    s32 unkF50;
    u8 padF54[0x1068 - 0xF54];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
} UnkStruct_Boss;

typedef struct UnkStruct_Player UnkStruct_Player;
struct UnkStruct_Player {
    u8 pad[0x32c];
    u32 unk32C;
    u8 pad330[0x338 - 0x330];
    void (*unk338)(UnkStruct_Player *p);
    u8 pad33C[0x420 - 0x33C];
    u16 unk420;
};

extern UnkStruct_Player *gPlayers;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void BossEffect_CreateLand(s32 a, s32 b, s32 c, s32 d);
void func_ov000_022e0294(UnkStruct_Boss *w);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_State_Unk_ov000_022e3f50(void);

void Boss6_State_Unk_ov000_022e40a8(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    UnkStruct_Player *p;

    if (!(m->unk10C & 2))
        return;
    w->base.unk68 = Boss6_State_Unk_ov000_022e3f50;
    Model_SetAnimation(m, 0, w->unkD08, 9, 0);
    m->unk114 = 0x1000;
    ScreenShake_Start(0xc);
    w->unkF50 = 0x5a;
    BossEffect_CreateLand(1, w->unk1068 >> 4, 0x20000, w->unk1070 >> 4);
    func_ov000_022e0294(w);
    p = gPlayers;
    if (!(p->unk32C & 0x400)) {
        if ((w->unkD5E != 0 && (w->unk20C & 0x100)) || (w->unkD5E == 0 && !(w->unk20C & 0x100))) {
            p->unk420 = 0x10;
            gPlayers->unk338(gPlayers);
        } else {
            p->unk420 = 0x20;
            gPlayers->unk338(gPlayers);
        }
    }
    Sound_PlaySeqArc(0, 0, 0x86);
}
