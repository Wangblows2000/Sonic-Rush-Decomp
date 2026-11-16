#include <nitro/types.h>

typedef struct UnkStruct_022d4654_a {
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
} UnkStruct_022d4654_a;

typedef struct UnkStruct_022d4654_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d4654_h;

typedef struct UnkStruct_022d4654_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d4654_q;

typedef struct UnkStruct_022d4654_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d4654_q unk134;
} UnkStruct_022d4654_o;

typedef struct UnkStruct_022d4654 UnkStruct_022d4654;
struct UnkStruct_022d4654 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d4654 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d4654_a unkA9C;
    UnkStruct_022d4654_a unkBD4;
    UnkStruct_022d4654_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d4654_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    s32 unk12F8;
    s16 unk12FC;
    s16 unk12FE;
    u16 unk1300;
    u16 unk1302;
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d4654_o *unk131C;
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

typedef struct UnkStruct_022d4654_7 {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
} UnkStruct_022d4654_7;

extern UnkStruct_022d4654 *gBoss2;
extern UnkStruct_022d4654_7 gBossStatus;
extern u32 gRandSeed;
extern u16 data_ov000_02362f58[][16];
extern u16 data_ov000_02362f98[][16];

void func_ov000_022d3b8c(void);
void func_ov000_022d2730(void);
void func_ov000_022d12fc(void);
void func_ov000_022d43f8(void);
void func_ov000_022cf38c(UnkStruct_022d4654 *s);
void func_ov000_022cf074(UnkStruct_022d4654 *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022d4654(u16 mode) {
    UnkStruct_022d4654 *s = gBoss2;
    s32 k = gBossStatus.unk8;
    s->unk1302 = mode;
    switch (mode) {
    case 0:
        switch (s->unk1300) {
        case 1:
            func_ov000_022d3b8c();
            gBossStatus.unk4 = 8;
            s->unk1300 = 0;
            return;
        case 2:
            func_ov000_022d2730();
            gBossStatus.unk4 = 9;
            s->unk1300 = 0;
            return;
        case 3:
            func_ov000_022d12fc();
            gBossStatus.unk4 = 10;
            s->unk1300 = 0;
            return;
        }
        return;
    case 1:
    case 2:
        if (k >= 6) {
            if (mode == 1) {
                s->unk1300 = 2;
            } else if ((Rand() & 0xf) < 2) {
                s->unk1300 = 2;
            } else {
                s->unk1300 = 1;
            }
        } else if (k >= 3) {
            s->unk1300 = data_ov000_02362f58[s->unk12FE][s->unk12FC];
            s->unk12FC++;
            if (s->unk12FC >= 16) {
                s->unk12FE = Rand() & 1;
                s->unk12FC = 0;
            }
            if (s->unk1300 == 3)
                func_ov000_022cf38c(s);
        } else {
            s->unk1300 = data_ov000_02362f98[s->unk12FE][s->unk12FC];
            s->unk12FC++;
            if (s->unk12FC >= 16) {
                s->unk12FE = Rand() & 1;
                s->unk12FC = 0;
            }
            switch (s->unk1300) {
            case 1:
                if (++s->unk12F8 <= 2)
                    break;
                s->unk1300 = 3;
            case 3:
                func_ov000_022cf38c(s);
            case 2:
                s->unk12F8 = 0;
                break;
            }
        }
        func_ov000_022d43f8();
        gBossStatus.unk4 = 7;
        break;
    case 3:
        func_ov000_022cf074(s);
        if (k >= 6) {
            s->unk1300 = 1;
        } else if (k >= 3) {
            if ((Rand() & 0xff) < 0x20)
                s->unk1300 = 2;
            else
                s->unk1300 = 1;
        } else {
            if ((Rand() & 0xff) < 0x40)
                s->unk1300 = 2;
            else
                s->unk1300 = 1;
        }
        func_ov000_022d43f8();
        gBossStatus.unk4 = 7;
        break;
    }
}
