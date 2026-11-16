#include <nitro/types.h>

typedef struct UnkStruct_022f8320_3 {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022f8320_3;

typedef struct UnkStruct_022f8320 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x210 - 0x5e];
    u8 pad1b[0x218 - 0x210];
    UnkStruct_022f8320_3 *unk218;
    s32 unk21C;
    u8 pad2[0x224 - 0x220];
    s32 unk224;
    u8 pad2b[0x22c - 0x228];
    s32 unk22C;
    s32 unk230;
    u8 pad2c[0x238 - 0x234];
    s32 unk238;
} UnkStruct_022f8320;

typedef struct UnkStruct_022f8320_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022f8320_v;

typedef struct UnkStruct_022f8320_c {
    u8 pad[0x18];
    UnkStruct_022f8320_v unk18;
    UnkStruct_022f8320_v unk24;
    u8 pad1[0x54 - 0x30];
    UnkStruct_022f8320_v unk54;
    UnkStruct_022f8320_v unk60;
} UnkStruct_022f8320_c;

typedef struct UnkStruct_022f8320_a {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x114 - 0x24];
    s32 unk114;
} UnkStruct_022f8320_a;

typedef struct {
    u8 pad[0x70];
    s32 unk70;
} UnkStruct_022f8320_pl;

typedef struct {
    u8 pad[4];
} UnkStruct_022f8320_m;

typedef struct UnkStruct_022f8320_6 {
    u32 unk0;
} UnkStruct_022f8320_6;

extern UnkStruct_022f8320_6 gBossStatus;
extern UnkStruct_022f8320_6 data_ov000_0236a444;
extern UnkStruct_022f8320_a data_ov000_0236a524;
extern UnkStruct_022f8320_c data_ov000_0236a48c;
extern UnkStruct_022f8320_pl *gPlayers;
extern void *gBossF;

void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void BossF_InitCamera(void);
void Model_SetAnimation(void *s, s32 a, s32 b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void BossF_Unk_ov000_022f898c(void);
void BossF_Action_Unk_ov000_022f8310(void *p);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);

void BossF_State_WaitForStart(UnkStruct_022f8320 *s) {
    UnkStruct_022f8320_a *a;
    UnkStruct_022f8320_c *c;
    UnkStruct_022f8320_3 *p1;
    if (!(gBossStatus.unk0 & 1))
        return;
    a = (UnkStruct_022f8320_a *)(u32)&data_ov000_0236a524;
    p1 = s->unk218;
    s->unk5C &= ~0x20;
    p1->unk5C &= ~0x20;
    data_ov000_0236a444.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, s->unk238, 0);
    Model_SetAnimation(a, 0, s->unk224, 0, 0);
    Model_SetAnimation(a, 1, s->unk230, 0, 0);
    Model_SetAnimation(a, 3, s->unk22C, 0, 0);
    a->unk114 = 0x800;
    a->unk18 = a->unk1C = a->unk20 = 0x1355b50;
    c = (UnkStruct_022f8320_c *)(u32)&data_ov000_0236a48c;
    BossF_InitCamera();
    c->unk24.x = 0x180000;
    c->unk24.y = -0x2e8000;
    c->unk24.z = 0x1c2000;
    c->unk18 = c->unk24;
    c->unk60.x = 0x180000;
    c->unk60.y = -0x2c0000;
    c->unk60.z = 0;
    c->unk54 = c->unk60;
    s->unk21C = gPlayers->unk70;
    NaviMsg_SetFaceAnim(9);
    NaviMsg_ShowMessage(0xf, 0xf0);
    ScreenShake_Start(0xc);
    BossF_Unk_ov000_022f898c();
    BossF_Action_Unk_ov000_022f8310(gBossF);
}
