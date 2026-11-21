#include <nitro/types.h>

typedef struct UnkStruct_022f6450_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} UnkStruct_022f6450_s;

typedef struct UnkStruct_022f6450 {
    u8 pad0[0x3c];
    u16 unk3C;
    u16 unk3E;
    u16 unk40;
    u8 pad42[0x50 - 0x42];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0x1dc - 0x5c];
    UnkStruct_022f6450_s *unk1DC;
} UnkStruct_022f6450;

typedef struct UnkStruct_022f6450_p {
    u8 pad0[0x40];
    s32 unk40;
    u8 pad44[0x308 - 0x44];
    s32 unk308;
} UnkStruct_022f6450_p;

extern u32 gRandSeed;

UnkStruct_022f6450_p *ExtraBoss_GetWork(void);
void ExtraBoss_WeaponBoss_Action_BeamUnitAttack(UnkStruct_022f6450 *w);
void ExtraBoss_WeaponBoss_Action_SwordAttack(UnkStruct_022f6450 *w);
void ExtraBoss_WeaponBoss_Action_HomingBeamAttack(UnkStruct_022f6450 *w);
void ExtraBoss_WeaponBoss_Action_MissileAttack(UnkStruct_022f6450 *w);
void ExtraBoss_WeaponBoss_Action_Wait(UnkStruct_022f6450 *w);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void ExtraBoss_WeaponBoss_SelectNextAttack(UnkStruct_022f6450 *w) {
    UnkStruct_022f6450_p *p = ExtraBoss_GetWork();
    UnkStruct_022f6450_s *s = w->unk1DC;
    s32 r;

    w->unk50 = w->unk54 = w->unk58 = 0;
    w->unk3C = w->unk3E = w->unk40 = 0;
    if (s->unk0 == 2)
        s->unk0 = s->unk8;
    s->unk8 = s->unk0;
    if (p->unk40 != 4) {
        switch (s->unk0) {
        case 0:
        case 1:
            switch (s->unk4) {
            case 4:
                ExtraBoss_WeaponBoss_Action_BeamUnitAttack(w);
                s->unk0 = 4;
                break;
            case 5:
                ExtraBoss_WeaponBoss_Action_SwordAttack(w);
                s->unk0 = 5;
                break;
            case 6:
                ExtraBoss_WeaponBoss_Action_HomingBeamAttack(w);
                s->unk0 = 6;
                break;
            case 7:
                ExtraBoss_WeaponBoss_Action_MissileAttack(w);
                s->unk0 = 7;
                break;
            }
            s->unk4 = 1;
            break;
        case 4:
            s->unkC--;
            if (s->unkC <= 0) {
                if (p->unk308 == 1) {
                    if ((Rand() & 0x1fff) < 0x999)
                        s->unk4 = 6;
                    else
                        s->unk4 = 5;
                } else {
                    s->unk4 = 6;
                }
            } else {
                s->unk4 = 4;
            }
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        case 5:
            if (p->unk308 == 1) {
                s->unk4 = 7;
            } else {
                s->unk4 = 4;
                s->unkC = 2;
            }
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        case 6:
            if (p->unk308 == 1)
                s->unk4 = 7;
            else
                s->unk4 = 5;
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        case 7:
            s->unk4 = 4;
            s->unkC = 1;
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        }
    } else {
        switch (s->unk0) {
        case 0:
        case 1:
            switch (s->unk4) {
            case 4:
                ExtraBoss_WeaponBoss_Action_BeamUnitAttack(w);
                s->unk0 = 4;
                break;
            case 5:
                ExtraBoss_WeaponBoss_Action_SwordAttack(w);
                s->unk0 = 5;
                break;
            case 6:
                ExtraBoss_WeaponBoss_Action_HomingBeamAttack(w);
                s->unk0 = 6;
                break;
            case 7:
                ExtraBoss_WeaponBoss_Action_MissileAttack(w);
                s->unk0 = 7;
                break;
            }
            s->unk4 = 1;
            break;
        case 4:
            s->unkC--;
            if (s->unkC <= 0)
                s->unk4 = 5;
            else
                s->unk4 = 4;
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        case 5:
            s->unk4 = 7;
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        case 6:
            s->unk4 = 4;
            r = Rand() & 0x1fff;
            if (r < 0x1000)
                s->unkC = 1;
            else if (r < 0x1999)
                s->unkC = 2;
            else
                s->unkC = 3;
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        case 7:
            s->unk4 = 6;
            ExtraBoss_WeaponBoss_Action_Wait(w);
            s->unk0 = 1;
            break;
        }
    }
}
