#include <nitro/types.h>

typedef struct UnkStruct_0230ea6c_m {
    s32 m[12];
} UnkStruct_0230ea6c_m;

typedef struct UnkStruct_0230ea6c_p {
    u8 pad0[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 padD[0x24 - 0xd];
    u8 mtx[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    UnkStruct_0230ea6c_m unk60;
} UnkStruct_0230ea6c_p;

typedef struct UnkStruct_0230ea6c {
    u8 pad0[0x20];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x60 - 0x2c];
    u16 unk60;
    u8 pad62[0x78 - 0x62];
    s32 unk78;
    u8 pad7C[0xac - 0x7c];
    UnkStruct_0230ea6c_p *unkAC;
    u8 padB0[0xd0 - 0xb0];
    s32 unkD0;
} UnkStruct_0230ea6c;

typedef struct UnkStruct_0230ea6c_o2 {
    u8 pad0[0x1f8];
    UnkStruct_0230ea6c_m unk1F8;
} UnkStruct_0230ea6c_o2;

extern u32 gRandSeed;
extern u8 data_ov001_0231f5c8[];
extern u8 gSoundArchive[];

UnkStruct_0230ea6c_o2 *SpecialStage_GetPlayer(void);
void MTX_Identity33_(void *m);
void NNS_SndArcSetCurrent(void *p);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
s32 SpecialStageActObj_SpawnAt(s32 p0, s32 p1, s32 p2, s32 p3);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov001_0230ea6c(UnkStruct_0230ea6c *s) {
    UnkStruct_0230ea6c_o2 *o = SpecialStage_GetPlayer();

    s->unkAC->unk60 = o->unk1F8;
    if (s->unkD0 == 0x2a) {
        s->unk60 |= 0x20;
        s->unk60 |= 0x40;
        s->unk78 = 0;
        s->unkAC->unkA = 4;
        s->unkAC->unkB = 0x10;
        s->unkAC->unkC = 0xd;
        MTX_Identity33_(s->unkAC->mtx);
        s->unkAC->unk48 = 0;
        s->unkAC->unk4C = 0;
        s->unkAC->unk50 = 0;
        s->unkAC->unk54 = -0x18d2;
        s->unkAC->unk58 = -0x7fd;
        s->unkAC->unk5C = -0x5f1;
        NNS_SndArcSetCurrent(data_ov001_0231f5c8);
        Sound_PlaySeqArc(0, 9, 0);
        NNS_SndArcSetCurrent(gSoundArchive);
    }
    if (s->unkD0 > 0x2a) {
        if (s->unkD0 % 17 == 1) {
            SpecialStageActObj_SpawnAt(2, o->unk1F8.m[9] + Rand() % 0x4000 - 0x2000, o->unk1F8.m[10] + Rand() % 0x5000 - 0x1000, o->unk1F8.m[11] + Rand() % 0x4000 - 0x2000);
        }
    } else {
        if (s->unkD0 % 17 == 1) {
            SpecialStageActObj_SpawnAt(2, s->unk20 + Rand() % 0x4000 - 0x2000, s->unk24 + Rand() % 0x4000 - 0x2000, s->unk28 + Rand() % 0x4000 - 0x2000);
        }
    }
}
