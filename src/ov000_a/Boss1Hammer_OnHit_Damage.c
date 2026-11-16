#include <nitro/types.h>

typedef struct UnkStruct_022c6bc0_m {
    s32 m[9];
} UnkStruct_022c6bc0_m;

typedef struct UnkStruct_022c6bc0_4 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022c6bc0_4;

typedef struct UnkStruct_022c6bc0_5 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022c6bc0_5;

typedef struct UnkStruct_022c6bc0_2 {
    u8 unk0;
} UnkStruct_022c6bc0_2;

typedef struct UnkStruct_022c6bc0 {
    s32 unk0;
    s32 unk4;
    u8 pad[0x10];
    UnkStruct_022c6bc0_2 *unk18;
} UnkStruct_022c6bc0;

extern u32 gRandSeed;
extern s16 data_0206e470[];
extern UnkStruct_022c6bc0_4 gBossStatus;
extern UnkStruct_022c6bc0_5 *gBoss1;

void Boss1_Action_HurtAll(void);
void Boss1_Action_DefeatAll(void);
u8 *BossEffect_CreateHit(s32 a, s32 b, s32 c, s32 d);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Sound_PlaySeq(s32 a, s32 b);
void Player_Action_AttackBounce(UnkStruct_022c6bc0_2 *p);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss1Hammer_OnHit_Damage(void *a, UnkStruct_022c6bc0 *b) {
    UnkStruct_022c6bc0_m mtx;
    UnkStruct_022c6bc0_2 *p = b->unk18;
    u8 *obj;
    u16 ang;
    if (gBossStatus.unk8 == 0)
        return;
    gBossStatus.unk8--;
    if (gBossStatus.unk8 != 0) {
        Boss1_Action_HurtAll();
        obj = BossEffect_CreateHit(1, b->unk0 << 8, b->unk4 << 8, 0);
        ang = (Rand() & 0xfff) + 0xaaa;
        MTX_RotX33_(obj + 0x158, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        ang = Rand();
        MTX_RotY33_(&mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        MTX_Concat33(obj + 0x158, &mtx, obj + 0x158);
        *(s32 *)(obj + 0x248) = 0x1000;
        if (gBossStatus.unk8 == 2) {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            Sound_PlaySeq(0, 0xf);
            gBoss1->unk20C |= 2;
        } else {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x10, 0xf0);
        }
        if (p == NULL)
            return;
        if (p->unk0 != 1)
            return;
        Player_Action_AttackBounce(b->unk18);
    } else {
        Boss1_Action_DefeatAll();
    }
}
