#include <nitro/types.h>

typedef struct UnkStruct_022edd80_3 {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022edd80_3;

typedef struct UnkStruct_022edd80 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *func;
    u8 pad1[0x210 - 0x6c];
    s32 unk210;
    UnkStruct_022edd80_3 *unk214;
    u8 pad2[0x248 - 0x218];
    s32 unk248;
} UnkStruct_022edd80;

typedef struct UnkStruct_022edd80_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022edd80_v;

typedef struct UnkStruct_022edd80_c {
    u8 pad[0x18];
    UnkStruct_022edd80_v unk18;
    UnkStruct_022edd80_v unk24;
    u8 pad1[0x54 - 0x30];
    UnkStruct_022edd80_v unk54;
    UnkStruct_022edd80_v unk60;
} UnkStruct_022edd80_c;

typedef struct UnkStruct_022edd80_a {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x114 - 0x24];
    s32 unk114;
} UnkStruct_022edd80_a;

typedef struct {
    u8 pad[0x70];
    s32 unk70;
} UnkStruct_022edd80_pl;

typedef struct {
    u8 pad[4];
} UnkStruct_022edd80_m;

typedef struct UnkStruct_022edd80_6 {
    u32 unk0;
} UnkStruct_022edd80_6;

extern UnkStruct_022edd80_6 gBossStatus;
extern UnkStruct_022edd80_6 data_ov000_0236a444;
extern UnkStruct_022edd80_a data_ov000_0236a524;
extern UnkStruct_022edd80_c data_ov000_0236a48c;
extern UnkStruct_022edd80_pl *gPlayers;
extern void *gBossF;

void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void func_ov000_022ee380(void);
void func_ov000_022ee17c(void);
void Boss7_State_Unk_ov000_022edcd8(UnkStruct_022edd80 *s);
void Model_SetAnimation(void *s, s32 a, s32 b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void func_ov000_022f898c(void);
void BossF_Action_Unk_ov000_022f8310(void *p);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);

void Boss7_State_WaitForStart(UnkStruct_022edd80 *s) {
    UnkStruct_022edd80_a *a;
    UnkStruct_022edd80_c *c;
    UnkStruct_022edd80_3 *p1;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = (UnkStruct_022edd80_a *)(u32)&data_ov000_0236a524;
    p1 = s->unk214;
    s->unk5C &= ~0x20;
    p1->unk5C &= ~0x20;
    data_ov000_0236a444.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, s->unk248, 0);
    a->unk18 = a->unk1C = a->unk20 = 0x1355b50;
    c = (UnkStruct_022edd80_c *)(u32)&data_ov000_0236a48c;
    func_ov000_022ee380();
    s->unk210 = 1;
    c->unk24.x = 0x9b800;
    c->unk24.y = -0x1a0000;
    c->unk24.z = 0xc8000;
    c->unk18 = c->unk24;
    c->unk60.x = 0x9b800;
    c->unk60.y = -0x1a8000;
    c->unk60.z = 0;
    c->unk54 = c->unk60;
    func_020537b4(9);
    func_020533e0(0xf, 0xf0);
    func_ov000_022ee17c();
    s->func = Boss7_State_Unk_ov000_022edcd8;
}
