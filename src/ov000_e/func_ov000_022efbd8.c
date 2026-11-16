#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x32c - 0x20c];
    u32 unk32C;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x21C - 0x210];
    void *unk21C;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
} UnkStruct_022efbd8;

typedef struct {
    u8 pad[0x40];
    s32 unk40;
    u8 pad44[0x50 - 0x44];
    s32 unk50;
    u8 pad54[0x58 - 0x54];
    s32 unk58;
    u8 pad5C[0x78 - 0x5C];
    s32 unk78;
    u8 pad7C[0x84 - 0x7C];
    s32 unk84;
} UnkStruct_0236a48c;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *gBossF;
extern UnkStruct_0236a48c data_ov000_0236a48c;
extern UnkStruct_Player *gPlayers;
extern UnkStruct_02369ac0 gBossStatus;

void func_ov000_022f4ce4(UnkStruct_022efbd8 *s, s32 a, s32 b, s32 c);
void func_ov000_022f9044(UnkStruct_022efbd8 *s);
void func_ov000_022f4cbc(UnkStruct_022efbd8 *s);
void Player_Action_Blank(UnkStruct_Player *p);
void Player_SetAnimation(UnkStruct_Player *p, s32 a);
void BossHelp_Destroy(void);
void BossFPartD8_State_Unk_ov000_022efb08(void);

void func_ov000_022efbd8(UnkStruct_022efbd8 *s) {
    UnkStruct_Boss *b = gBossF;
    UnkStruct_Player *p;
    UnkStruct_0236a48c *c;

    func_ov000_022f4ce4(s, 0x23, 0x1000, 0);
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->base.unk1B8 = NULL;
    c = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;
    c->unk40 = 0;
    c->unk58 -= 0x64000;
    c->unk50 = 0;
    c->unk84 = 0x28000;
    c->unk78 = 0x258000;
    p = gPlayers;
    if (!(p->unk32C & 0x400)) {
        p->base.unk70 = gBossF->unk21C;
        Player_Action_Blank(gPlayers);
        Player_SetAnimation(gPlayers, 2);
        gPlayers->base.unk34 = 0;
        gPlayers->base.unk2E = 0;
        gPlayers->base.unk30 = 0;
        gPlayers->base.unk58 |= 0x10;
        gPlayers->base.unk58 &= ~0x80;
    }
    s->unk434 = 0;
    BossHelp_Destroy();
    b->unk20C &= ~0x40;
    gBossStatus.unk4 = 0x3a;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022efb08;
}
