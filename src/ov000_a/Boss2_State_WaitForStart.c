#include <nitro/types.h>

typedef struct UnkStruct_022d49e4_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x90 - 0x54];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0xf8 - 0xe8];
    s32 *unkF8;
    u8 padFC[0x10c - 0xfc];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x12c - 0x118];
    s16 unk12C;
    u8 pad4[0x138 - 0x12e];
} UnkStruct_022d49e4_a;

typedef struct UnkStruct_022d49e4_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d49e4_h;

typedef struct UnkStruct_022d49e4_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d49e4_q;

typedef struct UnkStruct_022d49e4_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d49e4_q unk134;
} UnkStruct_022d49e4_o;

typedef struct UnkStruct_022d49e4 UnkStruct_022d49e4;
struct UnkStruct_022d49e4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d49e4 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d49e4_a unkA9C;
    UnkStruct_022d49e4_a unkBD4;
    UnkStruct_022d49e4_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d49e4_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    s32 unk12F8;
    s16 unk12FC;
    s16 unk12FE;
    u16 unk1300;
    u16 unk1302;
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d49e4_o *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1450 - 0x13a8];
    s32 unk1450;
    u8 pad5d[0x1458 - 0x1454];
    s32 unk1458;
    s32 unk145C;
    s32 unk1460;
    s32 unk1464;
    s32 unk1468;
    s16 unk146C;
    u8 pad7[0x1470 - 0x146e];
    s32 unk1470;
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

typedef struct UnkStruct_022d49e4_7 {
    u32 unk0;
} UnkStruct_022d49e4_7;

typedef struct UnkStruct_022d49e4_m {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_022d49e4_m;

typedef struct UnkStruct_022d49e4_c {
    u32 unk0;
} UnkStruct_022d49e4_c;

typedef struct UnkStruct_022d49e4_t {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} UnkStruct_022d49e4_t;

typedef struct UnkStruct_022d49e4_p {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x70 - 0x5e];
    void (*unk70)(void);
} UnkStruct_022d49e4_p;

typedef struct UnkStruct_022d49e4_9 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_022d49e4_9;

extern UnkStruct_022d49e4_7 gBossStatus;
extern UnkStruct_022d49e4_m data_ov000_0236a524;
extern UnkStruct_022d49e4_c data_ov000_0236a444;
extern void *gBossStage2Models[];
extern UnkStruct_022d49e4_t gBoss2ModelColorsDefault[];
extern UnkStruct_022d49e4_p *gPlayers;
extern UnkStruct_022d49e4_9 gCamera;

void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, void *a, s32 b);
void func_ov000_022cd5e4(void);
u8 *NNS_G3dGetMdlSet(void *p);
void NNS_G3dMdlSetMdlDiffAll(void *s, u32 v);
void NNS_G3dMdlSetMdlAmbAll(void *s, u32 v);
void NNS_G3dMdlSetMdlEmiAll(void *s, u32 v);
void func_ov000_022d43f8(void);
void Boss2_Draw_Unk_ov000_022cebc8(void);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void Boss2_CreatePart_ov000_022cf8b0(UnkStruct_022d49e4 *s);

void Boss2_State_WaitForStart(UnkStruct_022d49e4 *s) {
    UnkStruct_022d49e4_m *m;
    s32 i;
    u8 *r;
    u8 *q;
    if (!(gBossStatus.unk0 & 1))
        return;
    s->unk5C &= ~0x20;
    m = (UnkStruct_022d49e4_m *)(u32)&data_ov000_0236a524;
    s->unk20C &= ~1;
    *(vu16 *)0x4000304 |= 0x8000;
    data_ov000_0236a444.unk0 |= 1;
    Model_Init(m, 0);
    Model_SetResource(m, gBossStage2Models[6], 0);
    m->unk20 = 0x1355b50;
    m->unk1C = m->unk20;
    m->unk18 = m->unk1C;
    func_ov000_022cd5e4();
    s->unk20C |= 0x20;
    for (i = 0; i < 11; i++) {
        r = NNS_G3dGetMdlSet(gBossStage2Models[i]);
        q = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
        NNS_G3dMdlSetMdlDiffAll(q, gBoss2ModelColorsDefault[i].unk0);
        NNS_G3dMdlSetMdlAmbAll(q, gBoss2ModelColorsDefault[i].unk2);
        NNS_G3dMdlSetMdlEmiAll(q, gBoss2ModelColorsDefault[i].unk4);
    }
    func_ov000_022d43f8();
    gPlayers->unk70 = Boss2_Draw_Unk_ov000_022cebc8;
    gPlayers->unk5C |= 0x2040;
    func_020537b4(9);
    func_020533e0(0xf, 0xf0);
    Boss2_CreatePart_ov000_022cf8b0(s);
    gCamera.unk62 = 0x19a;
}
