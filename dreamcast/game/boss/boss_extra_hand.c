/*
 * Extra Zone boss: the hand boss (bsex_n_body + the catch hand bsex_n_r_hand: Burning Blaze's
 * rounds 1 and 3).  ExtraBoss_CreateHandBoss, ExtraBoss_HandBoss_*, ExtraBossObj_State_HandBoss*,
 * ExtraBossObj_State_HandCatch*, ExtraBossObj_Draw_HandBoss / _HandCatch and the catch-hand node
 * callback.  Translated from src/game/Zone/Extra_Zone/Boss/ (overlay ov001).
 */
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"

ExObj *data_ov001_0231f078[2];   /* hand boss, catch hand */

void ex_set_node_callback(BossModel *m, void (*fn)(BossModel *));
int ex_model_index(const BossModel *m);
void ex_get_slot_mtx(int slot, ExMtx43 *out);

#define HANIM(i) ((u32)EX_U16(0x02316840, (i)))  /* gExtraBossHandBossAnimTable[i] */
static fx32 *ex_tr(ExBossWork *s, int k) { return &s->mtx[k].m[9]; }
static ExBossWork *ex_hw(const ExObj *p) { return (ExBossWork *)p->unk1DC; }
static BossModel *ex_catch_model(const ExObj *p) { return ex_hw(p)->unk18->unk84; }

/* sets the body (a) and catch hand (b) animations: table entries base .. base + 3 */
static void ex_hand_anims(BossModel *a, BossModel *b, int base) {
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(base));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(base + 1));
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(base + 2));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(base + 3));
}

/* ExtraBoss_HandCatchNodeCallback: hand_catch -> matrix slot 0x1d */
static void ExtraBoss_HandCatchNodeCallback(BossModel *p) {
    dcm_mtx_t m;
    int j = ex_joint(ex_model_index(p), ex_rom_str(EX_DATA(0x0231d404, const u32 *)[0]));
    if (j >= 0) {
        boss_model_joint_world(p, j, &m);
        g_boss_mtx_slot[0x1d] = m;
        data_ov001_0231f064 = 0;
    }
}

/* -------------------------------------------------------------- drawing */
static void ExtraBossObj_Draw_HandBoss(ExObj *w) {
    MtxFx33 m;
    ExBossWork *s = ex_hw(w);
    BossModel *a = w->unk84;
    ExCollider *c;
    s32 t;
    a->trans[0] = data_ov001_0231f524 + (w->unk18 + w->unk30);
    a->trans[1] = -(data_ov001_0231f520 + (w->unk1C + w->unk34));
    a->trans[2] = w->unk20 + w->unk38;
    MTX_RotX33(&a->rot, SIN(w->unk3C), COS(w->unk3C));
    MTX_RotY33(&m, SIN(w->unk3E), COS(w->unk3E));
    MTX_Concat33(&a->rot, &m, &a->rot);
    MTX_RotZ33(&m, SIN(w->unk40), COS(w->unk40));
    MTX_Concat33(&a->rot, &m, &a->rot);
    if (!(w->unk14 & 0x10000))
        Model_Animate(a);
    ex_draw_model(a, 0, NULL);
    ex_get_slot_mtx(0x1d, &s->mtx[EXM_SLOT_1D]);
    ex_get_slot_mtx(0x1b, &s->mtx[EXM_SLOT_1B]);
    ex_get_slot_mtx(0x1c, &s->mtx[EXM_SLOT_1C]);
    ex_get_slot_mtx(0x1e, &s->mtx[EXM_SLOT_1E]);
    c = w->unk9C[0];
    c->unk28[0] = ex_tr(s, EXM_SLOT_1B)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_SLOT_1B)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_SLOT_1B)[2];
    c = w->unk9C[1];
    c->unk28[0] = ex_tr(s, EXM_SLOT_1D)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_SLOT_1D)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_SLOT_1D)[2];
    c = w->unk9C[2];
    c->unk28[0] = ex_tr(s, EXM_SLOT_1C)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_SLOT_1C)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_SLOT_1C)[2];
    c = w->unk9C[3];
    c->unk28[0] = ex_tr(s, EXM_SLOT_1C)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_SLOT_1C)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_SLOT_1C)[2];
    if (!(w->unkB0[EX_COLL_B0].unk8 & 0x200)) {
        s->flash++;
        t = s->flash & 7;
        if (t == 1)
            ExtraBoss_LoadModelPalettes(0x0231d408, 0x0231670c, 0x02316714, 2);
        else if (t == 5)
            ExtraBoss_LoadModelPalettes(0x0231d408, 0x02316710, 0x02316714, 2);
    } else if (s->flash != 0) {
        ExtraBoss_LoadModelPalettes(0x0231d408, 0x02316710, 0x02316714, 2);
        s->flash = 0;
    }
}

static void ExtraBossObj_Draw_HandCatch(ExObj *w) {
    MtxFx33 m;
    ExBossWork *s = ex_hw(w->unk7C);
    BossModel *a = w->unk84;
    ExCollider *c;
    ExObj *t;
    if (s->unk14 == 0) {
        w->unk18 = ex_tr(s, EXM_SLOT_1E)[0];
        w->unk1C = -ex_tr(s, EXM_SLOT_1E)[1];
        w->unk20 = ex_tr(s, EXM_SLOT_1E)[2];
    }
    a->trans[0] = w->unk18 + w->unk30;
    a->trans[1] = -(w->unk1C + w->unk34);
    a->trans[2] = w->unk20 + w->unk38;
    MTX_RotX33(&a->rot, SIN(w->unk3C), COS(w->unk3C));
    MTX_RotY33(&m, SIN(w->unk3E), COS(w->unk3E));
    MTX_Concat33(&a->rot, &m, &a->rot);
    MTX_RotZ33(&m, SIN(w->unk40), COS(w->unk40));
    MTX_Concat33(&a->rot, &m, &a->rot);
    if (!(w->unk14 & 0x10000))
        Model_Animate(a);
    ex_draw_model(a, 0, NULL);
    ex_get_slot_mtx(0x1d, &s->mtx[EXM_CATCH]);
    c = w->unk9C[1];
    c->unk28[0] = ex_tr(s, EXM_CATCH)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_CATCH)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_CATCH)[2];
    c = w->unk9C[2];
    c->unk28[0] = ex_tr(s, EXM_CATCH)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_CATCH)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_CATCH)[2];
    if (s->unk16 == 0)
        return;
    t = ExtraBoss_GetPlayer();
    t->unk18 = ex_tr(s, EXM_CATCH)[0] - data_ov001_0231f524;
    t->unk1C = -(ex_tr(s, EXM_CATCH)[1] + data_ov001_0231f520);
}

/* --------------------------------------------------------------- actions */
static void ExtraBossObj_State_HandBossEnter(ExObj *p);
static void ExtraBossObj_State_HandBossWait(ExObj *p);
static void ExtraBossObj_State_HandBossClawDelay(ExObj *p);
static void ExtraBossObj_State_HandBossCatchMove(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionExit(ExObj *p);
static void ExtraBossObj_State_HandBossHurt(ExObj *p);
static void ExtraBossObj_State_HandBossDefeat(ExObj *p);
static void ExtraBossObj_State_HandCatchPullPlayer(ExObj *p);
static void ExtraBoss_HandBoss_SelectNextAttack(ExObj *p);

static void ExtraBoss_HandBoss_Action_Enter(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    ExObj *t1 = ex_hw(p0)->unk18;
    BossModel *t2 = t1->unk84;
    p0->unk74 = ExtraBossObj_State_HandBossEnter;
    t1->unk74 = ExtraBossObj_State_None;
    Model_SetAnimation(t0, 0, &data_ov001_0231f068, HANIM(0));
    Model_SetAnimation(t0, 4, &data_ov001_0231f074, HANIM(1));
    t0->flags[0] |= 4;
    t0->flags[4] |= 4;
    t0->speed = 0x1000;
    Model_SetAnimation(t2, 0, &data_ov001_0231f068, HANIM(2));
    Model_SetAnimation(t2, 4, &data_ov001_0231f074, HANIM(3));
    t2->flags[0] |= 4;
    t2->flags[4] |= 4;
    t2->speed = 0x1000;
    p0->unk1C8 = (u16)0;
    p0->unkB0[EX_COLL_B0].unk8 |= 0x200;
}
static void ExtraBoss_HandBoss_Action_Wait(ExObj *p) {
    ExObj *q = ex_hw(p)->unk18;
    BossModel *a = p->unk84;
    BossModel *b = q->unk84;
    p->unk74 = ExtraBossObj_State_HandBossWait;
    q->unk74 = ExtraBossObj_State_None;
    p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
    p->unkB0[EX_COLL_120].unk8 |= 0x200;
    p->unkB0[EX_COLL_158].unk8 |= 0x200;
    q->unkB0[EX_COLL_E8].unk8 |= 0x200;
    q->unkB0[EX_COLL_120].unk8 |= 0x200;
    a->flags[0] |= 0x40;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(1));
    a->blendSpeed = 0x400;
    a->flags[0] |= 4;
    a->flags[4] |= 4;
    a->speed = 0x1000;
    b->flags[0] |= 0x40;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(2));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(3));
    b->blendSpeed = 0x400;
    b->flags[0] |= 4;
    b->flags[4] |= 4;
    b->speed = 0x1000;
    {   /* ARM order: the first Rand() picks the table entry, the second the 0..63 offset */
        s32 t = EX_S32(0x02316810, Rand() & 3);
        p->unk1C8 = (Rand() & 0x3f) + t;
    }
    ExtraBoss_StopAndFaceCenter(p);
}
static void ExtraBoss_HandBoss_Action_ClawAttack(ExObj *p) {
    ExObj *q = ex_hw(p)->unk18;
    s32 r;
    p->unk74 = ExtraBossObj_State_HandBossClawDelay;
    q->unk74 = ExtraBossObj_State_None;
    p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
    p->unkB0[EX_COLL_120].unk8 |= 0x200;
    p->unkB0[EX_COLL_158].unk8 |= 0x200;
    q->unkB0[EX_COLL_E8].unk8 |= 0x200;
    q->unkB0[EX_COLL_120].unk8 |= 0x200;
    r = Rand() & 0xff;
    if (r < 0x4d)
        p->unk1C8 = 0x1e;
    else if (r < 0xcd)
        p->unk1C8 = 0xb4;
    else
        p->unk1C8 = 0x12c;
    ExtraBoss_StopAndFaceCenter(p);
}
static void ExtraBoss_HandBoss_Action_CatchAttack(ExObj *p) {
    ExObj *o = ex_hw(p)->unk18;
    u16 a;
    p->unk74 = ExtraBossObj_State_HandBossCatchMove;
    o->unk74 = ExtraBossObj_State_None;
    p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
    p->unkB0[EX_COLL_120].unk8 |= 0x200;
    p->unkB0[EX_COLL_158].unk8 |= 0x200;
    o->unkB0[EX_COLL_E8].unk8 |= 0x200;
    o->unkB0[EX_COLL_120].unk8 |= 0x200;
    a = FX_Atan2(0xc0000 - p->unk1C, 0x162000 - p->unk18);
    p->unk50 = FX_Mul(COS(a), 0x5000);
    p->unk54 = FX_Mul(SIN(a), 0x5000);
    g_bossex_stats.catches++;
}
static void ExtraBoss_HandBoss_Action_SuctionAttack(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    ExObj *t1 = ex_hw(p0)->unk18;
    BossModel *t2 = t1->unk84;
    p0->unk74 = ExtraBossObj_State_HandBossSuctionExit;
    t1->unk74 = ExtraBossObj_State_None;
    p0->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    p0->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
    p0->unkB0[EX_COLL_120].unk8 |= 0x200;
    p0->unkB0[EX_COLL_158].unk8 |= 0x200;
    t1->unkB0[EX_COLL_E8].unk8 |= 0x200;
    t1->unkB0[EX_COLL_120].unk8 |= 0x200;
    t0->flags[0] |= 0x40;
    Model_SetAnimation(t0, 0, &data_ov001_0231f068, HANIM(0x44));
    Model_SetAnimation(t0, 4, &data_ov001_0231f074, HANIM(0x45));
    t0->blendSpeed = 0x400;
    t0->flags[0] |= 4;
    t0->flags[4] |= 4;
    t0->speed = 0x1000;
    t2->flags[0] |= 0x40;
    Model_SetAnimation(t2, 0, &data_ov001_0231f068, HANIM(0x46));
    Model_SetAnimation(t2, 4, &data_ov001_0231f074, HANIM(0x47));
    t2->blendSpeed = 0x400;
    t2->flags[0] |= 4;
    t2->flags[4] |= 4;
    t2->speed = 0x1000;
    p0->unk50 = -0x2000;
    p0->unk54 = 0;
    p0->unk5C = -256;
    p0->unk60 = 0;
    Sound_PlaySeqArc(0, 0, 0xa6);
}
static void ExtraBoss_HandBoss_Action_Hurt(ExObj *p) {
    ExBossWork *s = ex_hw(p);
    BossModel *a = p->unk84;
    BossModel *b = s->unk18->unk84;
    s->unk8 = s->unk0;
    s->unk0 = 2;
    p->unk74 = ExtraBossObj_State_HandBossHurt;
    a->flags[0] |= 0x40;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(4));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(5));
    a->blendSpeed = 0x400;
    a->flags[0] &= ~4;
    a->flags[4] &= ~4;
    a->speed = 0x1000;
    b->flags[0] |= 0x40;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(6));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(7));
    b->blendSpeed = 0x400;
    b->flags[0] &= ~4;
    b->flags[4] &= ~4;
    b->speed = 0x1000;
    p->unk50 = 0x6000;
    p->unk60 = 0;
    p->unk5C = p->unk60;
    p->unk54 = p->unk5C;
    p->unk1C8 = 0;
    p->unk1CC = 0;
}
static void ExtraBoss_HandBoss_Action_Defeat(ExObj *p0) {
    ExBossWork *t0 = ex_hw(p0);
    ExObj *t1 = t0->unk18;
    ExWork *t2 = ExtraBoss_GetWork();
    BossModel *t3 = t1->unk84;
    BossModel *t4 = p0->unk84;
    ExObj *t23;
    ExPlayerWork *t24;
    t0->unk8 = t0->unk0;
    t0->unk0 = 3;
    p0->unk74 = ExtraBossObj_State_HandBossDefeat;
    t4->flags[0] |= 0x40;
    Model_SetAnimation(t4, 0, &data_ov001_0231f068, HANIM(8));
    Model_SetAnimation(t4, 4, &data_ov001_0231f074, HANIM(9));
    t4->blendSpeed = 0x400;
    t4->flags[0] &= ~4;
    t4->flags[4] &= ~4;
    t4->speed = 0x1000;
    t3->flags[0] |= 0x40;
    Model_SetAnimation(t3, 0, &data_ov001_0231f068, HANIM(0xa));
    Model_SetAnimation(t3, 4, &data_ov001_0231f074, HANIM(0xb));
    t3->blendSpeed = 0x400;
    t3->flags[0] &= ~4;
    t3->flags[4] &= ~4;
    t3->speed = 0x1000;
    p0->unk50 = 0x4000;
    p0->unk5C = 0xf0;
    p0->unk60 = 0;
    p0->unk54 = p0->unk60;
    p0->unkB0[EX_COLL_B0].unk8 |= 0x200;
    p0->unkB0[EX_COLL_E8].unk8 |= 0x200;
    p0->unk1C8 = 0;
    p0->unk1CC = 0;
    t2->unk318 = 1;
    t23 = ExtraBoss_GetPlayer();
    t24 = (ExPlayerWork *)t23->unk1DC;
    t23->unkB0[EX_COLL_B0].unk8 |= 0x200;
    t23->unkB0[EX_COLL_E8].unk8 |= 0x200;
    t23->unk14 &= ~0x20000u;
    t24->unk0 &= ~2u;
    t24->unk10 = 0;
    ExtraBoss_GetWork()->unk0 |= 2;
    ExtraBoss_SetScrollSpawnEnabled(0);
    ExtraBoss_SetRingPickupEnabled(0);
}

/* ExtraBoss_Unk_ov001_022eed0c: the second claw box touched the player: off, background shake */
static void ExtraBoss_Unk_ov001_022eed0c(ExCollider *p0, ExCollider *p1) {
    ExObj *t0 = p0->owner;
    if (p1->owner->unk4 != 0)
        return;
    t0->unkB0[EX_COLL_158].unk8 |= 0x200;
    ExtraBoss_StartBgShake(0x3c, 4);
    Sound_PlaySeqArc(0, 0, 0xb6);
}

static void ExtraBoss_HandBoss_OnHit(ExCollider *self, ExCollider *other) {
    ExObj *s = self->owner;
    ExObj *p = other->owner;
    ExBossWork *w = ex_hw(p);
    ExObj *o = w->unk18;
    ExWork *c = ExtraBoss_GetWork();
    c->unk308--;
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
    p->unkB0[EX_COLL_E8].unk8 |= 0x200;
    p->unkB0[EX_COLL_120].unk8 |= 0x200;
    p->unkB0[EX_COLL_158].unk8 |= 0x200;
    o->unkB0[EX_COLL_158].unk8 |= 0x200;
    o->unkB0[EX_COLL_120].unk8 |= 0x200;
    w->unkC = 0;
    w->unk16 = 0;
    ExtraBoss_SpawnDamageFx(NULL, s->unk18, s->unk1C, 0);
    w->flash = 0;
    if (c->unk308 > 0) {
        ExtraBoss_HandBoss_Action_Hurt(p);
        Sound_PlaySeqArc(0, 0, 0x7b);
    } else {
        ExtraBoss_HandBoss_Action_Defeat(p);
        Sound_PlaySeqArc(0, 0, 0xba);
    }
    Sound_PlayVoice(0, 0, EX_S32(0x023167e4, Rand() & 1));
    g_bossex_stats.boss_hits++;
}

static void ExtraBoss_HandBoss_SelectNextAttack(ExObj *p) {
    ExWork *g = ExtraBoss_GetWork();
    ExBossWork *s = ex_hw(p);
    s32 st;
    p->unk50 = p->unk54 = p->unk58 = 0;
    p->unk3C = p->unk3E = p->unk40 = 0;
    if (s->unk0 == 2)
        s->unk0 = s->unk8;
    st = s->unk0;
    if (g->unk40 == 1) {
        switch (st) {
        case 0:
        case 1:
            switch (s->unk4) {
            case 4:
                ExtraBoss_HandBoss_Action_ClawAttack(p);
                s->unk0 = 4;
                break;
            case 5:
                ExtraBoss_HandBoss_Action_CatchAttack(p);
                s->unk0 = 5;
                break;
            case 6:
                ExtraBoss_HandBoss_Action_SuctionAttack(p);
                s->unk0 = 6;
                break;
            }
            s->unk4 = 1;
            break;
        case 4:
            s->unkC--;
            if (s->unkC > 0)
                s->unk4 = 4;
            else if (s->u10.unk10 == 2)
                s->unk4 = 6;
            else
                s->unk4 = 5;
            ExtraBoss_HandBoss_Action_Wait(p);
            s->unk0 = 1;
            break;
        case 5:
            s->u10.unk10 = 2;
            s->unkC = s->u10.unk10;
            s->unk4 = 4;
            ExtraBoss_HandBoss_Action_Wait(p);
            s->unk0 = 1;
            break;
        case 6:
            s->u10.unk10 = 1;
            s->unkC = s->u10.unk10;
            s->unk4 = 4;
            ExtraBoss_HandBoss_Action_Wait(p);
            s->unk0 = 1;
            break;
        }
    } else {
        switch (st) {
        case 0:
        case 1:
            switch (s->unk4) {
            case 4:
                ExtraBoss_HandBoss_Action_ClawAttack(p);
                s->unk0 = 4;
                break;
            case 5:
                ExtraBoss_HandBoss_Action_CatchAttack(p);
                s->unk0 = 5;
                break;
            case 6:
                ExtraBoss_HandBoss_Action_SuctionAttack(p);
                s->unk0 = 6;
                break;
            }
            s->unk4 = 1;
            break;
        case 4:
        case 5:
            s->unk4 = 6;
            ExtraBoss_HandBoss_Action_Wait(p);
            s->unk0 = 1;
            break;
        case 6:
            if (s->unk8 == 4)
                s->unk4 = 5;
            else
                s->unk4 = 4;
            ExtraBoss_HandBoss_Action_Wait(p);
            s->unk0 = 1;
            break;
        }
    }
    if (st != 1)
        s->unk8 = st;
}

/* ---------------------------------------------------------------- states */
static void ExtraBossObj_State_HandBossEnter(ExObj *p) {
    s32 i, v;
    p->unk1C8 = (s16)(p->unk1C8 + 0x22);
    if ((s16)p->unk1C8 >= 0x1000) {
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        ExtraBoss_HandBoss_SelectNextAttack(p);
        return;
    }
    i = 2;
    v = 0x1c0000;
    do {
        v += FX_Mul(0xdc000 - v, (s16)p->unk1C8);
    } while (i-- != 0);
    p->unk18 = v;
    i = 2;
    v = 0xc0000;
    do {
        v += FX_Mul(0xc0000 - v, (s16)p->unk1C8);
    } while (i-- != 0);
    p->unk1C = v;
}
static void ExtraBossObj_State_HandBossWait(ExObj *p0) {
    p0->unk1C8 = p0->unk1C8 - 1;
    if (p0->unk1C8 > 0) {
        ExtraBoss_StepXTowardRightEdge(p0);
        return;
    }
    ExtraBoss_HandBoss_SelectNextAttack(p0);
}

/* claw */
static void ExtraBossObj_State_HandBossClawWindUp(ExObj *p);
static void ExtraBossObj_State_HandBossClawSwipe1(ExObj *p);
static void ExtraBossObj_State_HandBossClawWindUp2(ExObj *p);
static void ExtraBossObj_State_HandBossClawSwipe2(ExObj *p);
static void ExtraBossObj_State_HandBossClawRecover(ExObj *p);
static void ExtraBossObj_State_HandBossClawReturn(ExObj *p);
static void ExtraBossObj_State_HandBossClawEnd(ExObj *p);
static s32 ex_claw_speed(s32 slow, s32 fast) {
    ExWork *q = ExtraBoss_GetWork();
    return FX_DivS32(q->unk308 * slow + (q->unk30C - q->unk308) * fast, q->unk30C);
}
static void ExtraBossObj_State_HandBossClawDelay(ExObj *p0) {
    BossModel *t1, *t3;
    ExObj *t2;
    p0->unk1C8 = p0->unk1C8 - 1;
    if (p0->unk1C8 > 0) {
        ExtraBoss_StepXTowardRightEdge(p0);
        return;
    }
    t1 = p0->unk84;
    t2 = ex_hw(p0)->unk18;
    t3 = t2->unk84;
    p0->unk60 = 0;
    p0->unk5C = p0->unk60;
    p0->unk54 = p0->unk5C;
    p0->unk50 = p0->unk54;
    p0->unk74 = ExtraBossObj_State_HandBossClawWindUp;
    p0->unkB0[EX_COLL_B0].unk8 |= 0x200;
    p0->unkB0[EX_COLL_120].unk8 &= ~0x200u;
    t2->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
    t1->flags[0] |= 0x40;
    Model_SetAnimation(t1, 0, &data_ov001_0231f068, HANIM(0x14));
    Model_SetAnimation(t1, 4, &data_ov001_0231f074, HANIM(0x15));
    t1->blendSpeed = 0x400;
    t1->flags[0] &= ~4;
    t1->flags[4] &= ~4;
    t1->speed = ex_claw_speed(0xccc, 0x1000);
    t3->flags[0] |= 0x40;
    Model_SetAnimation(t3, 0, &data_ov001_0231f068, HANIM(0x16));
    Model_SetAnimation(t3, 4, &data_ov001_0231f074, HANIM(0x17));
    t3->blendSpeed = 0x400;
    t3->flags[0] &= ~4;
    t3->flags[4] &= ~4;
    t3->speed = t1->speed;
    p0->unk60 = 0;
    p0->unk5C = p0->unk60;
    p0->unk54 = p0->unk5C;
    p0->unk50 = p0->unk54;
}
static void ExtraBossObj_State_HandBossClawWindUp(ExObj *p) {
    BossModel *a = p->unk84, *b;
    if (!(a->flags[0] & 2))
        return;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossClawSwipe1;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x18));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x19));
    a->speed = ex_claw_speed(0x1333, 0x1800);
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x1a));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x1b));
    b->speed = a->speed;
    p->unk1CC = p->unk18;
    p->unk1D0 = 0;
    p->unk1D4 = FX_Div(0x88, a->speed);
    ExtraBoss_SpawnClawFx(NULL, p->unk18 - 0x46000, p->unk1C, 0x28000, 0);
}
static void ExtraBossObj_State_HandBossClawSwipe1(ExObj *p) {
    BossModel *a = p->unk84, *b;
    s32 v, acc, i;
    p->unk1D0 += p->unk1D4;
    if (p->unk1D0 > 0x1000)
        p->unk1D0 = 0x1000;
    i = 2;
    v = p->unk1D0;
    acc = p->unk1CC;
    do {
        acc += FX_Mul(0xc8000 - acc, v);
    } while (i-- != 0);
    p->unk18 = acc;
    if (!(a->flags[0] & 2))
        return;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossClawWindUp2;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x1c));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x1d));
    a->speed = 0x1000;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x1e));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x1f));
    b->speed = 0x1000;
}
static void ExtraBossObj_State_HandBossClawWindUp2(ExObj *p) {
    BossModel *a = p->unk84, *b;
    if (!(a->flags[0] & 2))
        return;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossClawSwipe2;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x20));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x21));
    a->speed = ex_claw_speed(0x1333, 0x1800);
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x22));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x23));
    b->speed = a->speed;
    p->unk1CC = p->unk18;
    p->unk1D0 = 0;
    p->unk1D4 = FX_Div(0xcc, a->speed);
    ExtraBoss_SpawnClawFx(NULL, p->unk18, p->unk1C, 0x28000, 1);
}
static void ExtraBossObj_State_HandBossClawSwipe2(ExObj *p) {
    BossModel *a = p->unk84, *b;
    s32 v, acc, i;
    p->unk1D0 += p->unk1D4;
    if (p->unk1D0 > 0x1000)
        p->unk1D0 = 0x1000;
    i = 2;
    v = p->unk1D0;
    acc = p->unk1CC;
    do {
        acc += FX_Mul(0x8c000 - acc, v);
    } while (i-- != 0);
    p->unk18 = acc;
    if (!(a->flags[0] & 2))
        return;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossClawRecover;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x24));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x25));
    a->speed = 0x1000;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x26));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x27));
    b->speed = 0x1000;
}
static void ExtraBossObj_State_HandBossClawRecover(ExObj *p0) {
    BossModel *t0 = p0->unk84, *t2;
    ExObj *t1;
    if ((t0->flags[0] & 2) == 0)
        return;
    t1 = ex_hw(p0)->unk18;
    t2 = t1->unk84;
    p0->unk74 = ExtraBossObj_State_HandBossClawReturn;
    p0->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    p0->unkB0[EX_COLL_120].unk8 |= 0x200;
    t1->unkB0[EX_COLL_E8].unk8 |= 0x200;
    Model_SetAnimation(t0, 0, &data_ov001_0231f068, HANIM(0x28));
    Model_SetAnimation(t0, 4, &data_ov001_0231f074, HANIM(0x29));
    t0->flags[0] |= 4;
    t0->flags[4] |= 4;
    Model_SetAnimation(t2, 0, &data_ov001_0231f068, HANIM(0x2a));
    Model_SetAnimation(t2, 4, &data_ov001_0231f074, HANIM(0x2b));
    t2->flags[0] |= 4;
    t2->flags[4] |= 4;
    p0->unk50 = 0x5000;
}
static void ExtraBossObj_State_HandBossClawReturn(ExObj *p) {
    BossModel *a = p->unk84, *b;
    if (p->unk18 + 0x5000 < 0x153000)
        return;
    b = ex_catch_model(p);
    p->unk18 = 0x153000;
    p->unk50 = 0;
    p->unk74 = ExtraBossObj_State_HandBossClawEnd;
    a->flags[0] |= 0x40;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x2c));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x2d));
    a->blendSpeed = 0x400;
    a->flags[0] &= ~4;
    a->flags[4] &= ~4;
    b->flags[0] |= 0x40;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x2e));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x2f));
    b->blendSpeed = 0x400;
    b->flags[0] &= ~4;
    b->flags[4] &= ~4;
}
static void ExtraBossObj_State_HandBossClawEnd(ExObj *p0) {
    if ((p0->unk84->flags[0] & 2) == 0)
        return;
    ExtraBoss_HandBoss_SelectNextAttack(p0);
}

/* catch */
static void ExtraBossObj_State_HandBossCatchReady(ExObj *p);
static void ExtraBossObj_State_HandBossCatchAim(ExObj *p);
static void ExtraBossObj_State_HandBossCatchResetArm(ExObj *p);
static void ExtraBossObj_State_HandBossCatchTurnArm(ExObj *p);
static void ExtraBossObj_State_HandBossCatchRewind(ExObj *p);
static void ExtraBossObj_State_HandCatchFlyOut(ExObj *p);
static void ExtraBossObj_State_HandCatchReturn(ExObj *p);
static void ExtraBossObj_State_HandBossGrabHold(ExObj *p);
static void ExtraBossObj_State_HandBossGrabEnd(ExObj *p);

static void ExtraBossObj_State_HandBossCatchMove(ExObj *p) {
    BossModel *a, *b;
    if (p->unk18 + p->unk50 < 0x162000)
        return;
    a = p->unk84;
    b = ex_catch_model(p);
    p->unk18 = 0x162000;
    p->unk1C = 0xc0000;
    p->unk54 = 0;
    p->unk50 = p->unk54;
    p->unk74 = ExtraBossObj_State_HandBossCatchReady;
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
    a->flags[0] |= 0x40;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x30));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x31));
    a->blendSpeed = 0x400;
    a->flags[0] &= ~4;
    a->flags[4] &= ~4;
    a->speed = 0x1000;
    b->flags[0] |= 0x40;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x32));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x33));
    b->blendSpeed = 0x400;
    b->flags[0] &= ~4;
    b->flags[4] &= ~4;
    b->speed = 0x1000;
}
static void ExtraBossObj_State_HandBossCatchReady(ExObj *p) {
    BossModel *a = p->unk84, *b;
    s32 r;
    if (!(a->flags[0] & 2))
        return;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossCatchAim;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x34));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x35));
    a->flags[0] &= ~4;
    a->flags[4] |= 4;
    a->speed = 0;
    if (a->anm[0])
        a->anm[0]->frame = 0xb4000;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x36));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x37));
    b->flags[0] &= ~4;
    b->flags[4] |= 4;
    b->speed = 0;
    if (b->anm[0])
        b->anm[0]->frame = 0xb4000;
    r = Rand() & 0xff;
    if (r < 0x4d)
        p->unk1C8 = 0x1e;
    else if (r < 0xcd)
        p->unk1C8 = 0x78;
    else
        p->unk1C8 = 0xd2;
    boss_sfx_handle(1, 0xb3);   /* Sound_PlaySeqArc(work + 0x33c, 0, 0xb3) */
    Sound_PlayVoice(0, 0, 0xd1);
}
static void ExtraBossObj_State_HandBossCatchAim(ExObj *p) {
    ExObj *t = ExtraBoss_GetPlayer();
    ExBossWork *q = ex_hw(p);
    ExObj *o2 = q->unk18;
    BossModel *a = p->unk84;
    BossModel *b = o2->unk84;
    u16 ang;
    s32 d, idx;
    ang = FX_Atan2(t->unk1C - (p->unk1C + 0x19d50), t->unk18 - (p->unk18 - 0x10d60));
    if (ang < 0x4000)
        ang = 0x4000;
    else if (ang > 0xc000)
        ang = 0xc000;
    p->unk1C8--;
    if (p->unk1C8 > 0) {
        ang = (u16)FX_DivS32((u16)(ang - 0x4000), 0xb6);
        d = (ang << 13) - a->anm[0]->frame;
        if (d > 0) {
            if (d >= 0x4000)
                d = 0x4000;
        } else if (d < 0) {
            if (d <= -0x4000)
                d = -0x4000;
        }
        a->anm[0]->frame += d;
        b->anm[0]->frame += d;
        return;
    }
    ExtraBoss_SpawnJumpFx(NULL, ex_tr(q, EXM_SLOT_1E)[0] - data_ov001_0231f524,
                          -(ex_tr(q, EXM_SLOT_1E)[1] + data_ov001_0231f520), ex_tr(q, EXM_SLOT_1E)[2]);
    p->unk74 = ExtraBossObj_State_HandBossCatchWaitHand;
    o2->unk74 = ExtraBossObj_State_HandCatchFlyOut;
    ang = (u16)((a->anm[0]->frame >> 13) * 0xb6);
    idx = (s32)((u16)(ang + 0x4000) & 0xffff) >> 4;
    o2->unk1D0 = ex_tr(q, EXM_SLOT_1E)[0] + FX_Mul(COS_IDX(idx), 0x320000);
    o2->unk1D4 = FX_Mul(SIN_IDX(idx), 0x320000) - ex_tr(q, EXM_SLOT_1E)[1];
    o2->unk1D8 = 0;
    o2->unk18 = ex_tr(q, EXM_SLOT_1E)[0];
    o2->unk1C = -ex_tr(q, EXM_SLOT_1E)[1];
    o2->unk20 = ex_tr(q, EXM_SLOT_1E)[2];
    q->unk14 = 1;
    o2->unkB0[EX_COLL_120].unk8 &= ~0x200u;
    boss_sfx_stop(1);   /* NNS_SndPlayerStopSeq(work + 0x33c, 0) */
    Sound_PlaySeqArc(0, 0, 0xb4);
}
void ExtraBossObj_State_HandBossCatchWaitHand(ExObj *p) {
    if (ex_hw(p)->unk14 == 0)
        p->unk74 = ExtraBossObj_State_HandBossCatchResetArm;
}
static void ExtraBossObj_State_HandBossCatchResetArm(ExObj *p) {
    BossModel *a = p->unk84;
    BossModel *b = ex_catch_model(p);
    if (a->anm[0]->frame > 0xb4000) {
        p->unk1CC = -0x4000;
        p->unk74 = ExtraBossObj_State_HandBossCatchTurnArm;
        return;
    }
    if (a->anm[0]->frame < 0xb4000) {
        p->unk1CC = 0x4000;
        p->unk74 = ExtraBossObj_State_HandBossCatchTurnArm;
        return;
    }
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x30));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x31));
    a->flags[0] &= ~4;
    a->flags[4] &= ~4;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x32));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x33));
    b->flags[0] &= ~4;
    b->flags[4] &= ~4;
    b->speed = 0;
    a->speed = b->speed;
    b->anm[0]->frame = 0x3000;
    a->anm[0]->frame = b->anm[0]->frame;
    p->unk74 = ExtraBossObj_State_HandBossCatchRewind;
}
static void ExtraBossObj_State_HandBossCatchTurnArm(ExObj *p) {
    s32 d = p->unk1CC;
    BossModel *a = p->unk84;
    BossModel *b = ex_catch_model(p);
    if ((d < 0 && a->anm[0]->frame + d <= 0xb4000) || (d > 0 && a->anm[0]->frame + d >= 0xb4000)) {
        Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x30));
        Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x31));
        a->flags[0] &= ~4;
        a->flags[4] &= ~4;
        Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x32));
        Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x33));
        b->flags[0] &= ~4;
        b->flags[4] &= ~4;
        b->speed = 0;
        a->speed = b->speed;
        b->anm[0]->frame = 0x33000;
        a->anm[0]->frame = b->anm[0]->frame;
        p->unk74 = ExtraBossObj_State_HandBossCatchRewind;
        return;
    }
    a->anm[0]->frame += d;
    b->anm[0]->frame += p->unk1CC;
}
static void ExtraBossObj_State_HandBossCatchRewind(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    BossModel *t1 = ex_catch_model(p0);
    t0->anm[0]->frame -= 0x1000;
    t1->anm[0]->frame -= 0x1000;
    if (t0->anm[0]->frame > 0)
        return;
    ExtraBoss_HandBoss_SelectNextAttack(p0);
}
static s32 ex_approach(s32 t, s32 target, s32 v) {
    s32 i = 2;
    do {
        v = target + FX_Mul(v - target, t);
    } while (i-- != 0);
    return v;
}
static void ExtraBossObj_State_HandCatchFlyOut(ExObj *p) {
    ExObj *q = p->unk7C;
    ExBossWork *w = ex_hw(q);
    p->unk1D8 = (s16)(p->unk1D8 + 0x5b);
    if ((s16)p->unk1D8 >= 0x1000) {
        p->unk1D8 = 0x1000;
        p->unk74 = ExtraBossObj_State_HandCatchReturn;
        q->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        p->unkB0[EX_COLL_120].unk8 |= 0x200;
    }
    p->unk18 = ex_approach((s16)p->unk1D8, ex_tr(w, EXM_SLOT_1E)[0], p->unk1D0);
    p->unk1C = ex_approach((s16)p->unk1D8, -ex_tr(w, EXM_SLOT_1E)[1], p->unk1D4);
}
static void ExtraBossObj_State_HandCatchReturn(ExObj *p) {
    ExBossWork *w = ex_hw(p->unk7C);
    p->unk1D8 = (s16)(p->unk1D8 - 0x16);
    if ((s16)p->unk1D8 <= 0) {
        p->unk1D8 = 0;
        p->unk18 = ex_tr(w, EXM_SLOT_1E)[0];
        p->unk1C = -ex_tr(w, EXM_SLOT_1E)[1];
        p->unk20 = ex_tr(w, EXM_SLOT_1E)[2];
        w->unk14 = 0;
        p->unk74 = ExtraBossObj_State_None;
    }
    p->unk18 = ex_approach((s16)p->unk1D8, ex_tr(w, EXM_SLOT_1E)[0], p->unk1D0);
    p->unk1C = ex_approach((s16)p->unk1D8, -ex_tr(w, EXM_SLOT_1E)[1], p->unk1D4);
}
static void ExtraBossObj_State_HandBossGrabTurnArm(ExObj *p) {
    ExObj *q = ex_hw(p)->unk18;
    s32 d = p->unk1CC;
    BossModel *a = p->unk84;
    BossModel *b = q->unk84;
    if ((d < 0 && a->anm[0]->frame + d <= 0xb4000) || (d > 0 && a->anm[0]->frame + d >= 0xb4000)) {
        Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x3c));
        Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x3d));
        a->flags[0] &= ~4;
        a->flags[4] &= ~4;
        Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x3e));
        Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x3f));
        b->flags[0] &= ~4;
        b->flags[4] &= ~4;
        b->speeds[0] = 0x1000;
        b->speed = 0x1000;
        a->speed = b->speed;
        p->unk74 = ExtraBossObj_State_HandBossGrabHold;
        q->unk74 = ExtraBossObj_State_None;
        p->unkB0[EX_COLL_158].unk8 &= ~0x200u;
        return;
    }
    a->anm[0]->frame += d;
    b->anm[0]->frame += p->unk1CC;
}
static void ExtraBossObj_State_HandCatchPullPlayer(ExObj *p) {
    ExObj *o = p->unk7C;
    ExBossWork *q = ex_hw(o);
    BossModel *a, *b;
    p->unk1D8 = (s16)(p->unk1D8 - 0x5b);
    if ((s16)p->unk1D8 <= 0) {
        p->unk1D8 = 0;
        p->unk18 = ex_tr(q, EXM_SLOT_1E)[0];
        p->unk1C = -ex_tr(q, EXM_SLOT_1E)[1];
        p->unk20 = ex_tr(q, EXM_SLOT_1E)[2];
        q->unk14 = 0;
        a = o->unk84;
        b = p->unk84;
        if (a->anm[0]->frame > 0xb4000) {
            o->unk1CC = -0x4000;
            o->unk74 = ExtraBossObj_State_HandBossGrabTurnArm;
            p->unk74 = ExtraBossObj_State_None;
            return;
        }
        if (a->anm[0]->frame < 0xb4000) {
            o->unk1CC = 0x4000;
            o->unk74 = ExtraBossObj_State_HandBossGrabTurnArm;
            p->unk74 = ExtraBossObj_State_None;
            return;
        }
        Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x3c));
        Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x3d));
        a->flags[0] &= ~4;
        a->flags[4] &= ~4;
        Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x3e));
        Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x3f));
        b->flags[0] &= ~4;
        b->flags[4] &= ~4;
        b->speeds[0] = 0x1000;
        b->speed = 0x1000;
        a->speed = b->speed;
        o->unk74 = ExtraBossObj_State_HandBossGrabHold;
        p->unk74 = ExtraBossObj_State_None;
        o->unkB0[EX_COLL_158].unk8 &= ~0x200u;
    } else {
        p->unk18 = ex_approach((s16)p->unk1D8, ex_tr(q, EXM_SLOT_1E)[0], p->unk1D0);
        p->unk1C = ex_approach((s16)p->unk1D8, -ex_tr(q, EXM_SLOT_1E)[1], p->unk1D4);
    }
}
static void ExtraBossObj_State_HandBossGrabHold(ExObj *p) {
    ExBossWork *s = ex_hw(p);
    BossModel *a = p->unk84;
    BossModel *b = s->unk18->unk84;
    BossModel *r;
    if (!(a->flags[0] & 2))
        return;
    r = ExtraBoss_GetPlayer()->unk84;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x40));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x41));
    a->flags[0] &= ~4;
    a->flags[4] &= ~4;
    Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x42));
    Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x43));
    b->flags[0] &= ~4;
    b->flags[4] &= ~4;
    b->speed = 0x1000;
    a->speed = b->speed;
    p->unk74 = ExtraBossObj_State_HandBossGrabEnd;
    r->cb[0] = MOP_FLUSHP;   /* unkA = 3 */
    r->cb[1] = MOP_NONE;     /* unkB = 0 */
    ExtraBossPlayer_Action_Hurt();
    p->unkB0[EX_COLL_158].unk8 |= 0x200;
    s->unk16 = 0;
}
static void ExtraBossObj_State_HandBossGrabEnd(ExObj *p0) {
    if ((p0->unk84->flags[0] & 2) == 0)
        return;
    ExtraBoss_HandBoss_SelectNextAttack(p0);
}
static void ExtraBoss_HandCatch_OnGrabPlayer(ExCollider *pa, ExCollider *pb) {
    ExObj *a = pa->owner;
    ExObj *b = pb->owner;
    ExObj *c = a->unk7C;
    ExBossWork *d = ex_hw(c);
    BossModel *e, *f;
    if (b->unk4 != 0)
        return;
    e = a->unk84;
    f = b->unk84;
    f->unk8 = 0x1d;
    f->cb[0] = MOP_FLUSHVP; /* 4 */
    f->cb[1] = 0xc;         /* Model_MatrixOp_Translate */
    f->cb[2] = MOP_RESTORE; /* 2 */
    b->unk18 = 0;
    b->unk1C = 0x14000;
    b->unk20 = 0;
    d->unk16 = 1;
    c->unk74 = ExtraBossObj_State_HandBossCatchWaitHand;
    a->unk74 = ExtraBossObj_State_HandCatchPullPlayer;
    Model_SetAnimation(e, 4, &data_ov001_0231f074, HANIM(0x3b));
    e->flags[4] &= ~4;
    e->speed = 0x1000;
    e->speeds[0] = 0;
    a->unkB0[EX_COLL_120].unk8 |= 0x200;
    ExtraBoss_SpawnCatchFx(NULL, ex_tr(d, EXM_CATCH)[0], -ex_tr(d, EXM_CATCH)[1], 0);
    Sound_PlaySeqArc(0, 0, 0xb5);
    g_bossex_stats.grabs++;
}

/* suction (space beam) */
static void ExtraBossObj_State_HandBossSuctionEnter(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionReady(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionAim(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionFire(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionHold(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionWaitEnd(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionLeave(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionReturn(ExObj *p);
static void ExtraBossObj_State_HandBossSuctionExit(ExObj *p) {
    BossModel *a, *b;
    if (p->unk18 <= -0xc8000) {
        a = p->unk84;
        b = ex_catch_model(p);
        p->unk60 = 0;
        p->unk5C = p->unk60;
        p->unk54 = p->unk5C;
        p->unk50 = p->unk54;
        p->unk18 = -0xc8000;
        p->unk1C = 0xc0000;
        p->unk20 = -0x78000;
        p->unk74 = ExtraBossObj_State_HandBossSuctionEnter;
        ex_hand_anims(a, b, 0x48);
        p->unk1CC = 0;
        ExtraBoss_SetCameraParams(0, 0, 0, 0, 0x2000);
        p->unkB0[EX_COLL_B0].unk8 |= 0x200;
    }
}
static void ExtraBossObj_State_HandBossSuctionEnter(ExObj *p) {
    BossModel *a, *b;
    s32 v, acc, i;
    p->unk1CC = (s16)(p->unk1CC + 0x44);
    v = (s16)p->unk1CC;
    if (v >= 0x1000) {
        a = p->unk84;
        b = ex_catch_model(p);
        p->unk74 = ExtraBossObj_State_HandBossSuctionReady;
        ex_hand_anims(a, b, 0x4c);
        a->flags[0] &= ~4;
        a->flags[4] &= ~4;
        a->speed = 0x1000;
        b->flags[0] &= ~4;
        b->flags[4] &= ~4;
        b->speed = 0x1000;
        return;
    }
    i = 2;
    acc = -0xc8000;
    do {
        acc += FX_Mul(0xc0000 - acc, v);
    } while (i-- != 0);
    p->unk18 = acc;
}
static void ExtraBossObj_State_HandBossSuctionReady(ExObj *p) {
    BossModel *a = p->unk84, *b;
    s32 r;
    if (!(a->flags[0] & 2))
        return;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossSuctionAim;
    ex_hand_anims(a, b, 0x50);
    a->flags[0] |= 4;
    a->flags[4] |= 4;
    a->speed = 0x1000;
    b->flags[0] |= 4;
    b->flags[4] |= 4;
    b->speed = 0x1000;
    r = Rand() & 0xff;
    if (r < 0x4d)
        p->unk1C8 = 0x3c;
    else if (r < 0xcd)
        p->unk1C8 = 0x78;
    else
        p->unk1C8 = 0xd2;
}
static void ExtraBossObj_State_HandBossSuctionAim(ExObj *p) {
    BossModel *a, *b;
    ExObj *t;
    s32 d;
    p->unk1C8--;
    if (p->unk1C8 <= 0) {
        a = p->unk84;
        b = ex_catch_model(p);
        p->unk50 = p->unk54 = 0;
        p->unk74 = ExtraBossObj_State_HandBossSuctionFire;
        a->flags[0] |= 0x40;
        Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0x54));
        Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0x55));
        a->blendSpeed = 0x400;
        a->flags[0] &= ~4;
        a->flags[4] &= ~4;
        a->speed = 0x1000;
        b->flags[0] |= 0x40;
        Model_SetAnimation(b, 0, &data_ov001_0231f068, HANIM(0x56));
        Model_SetAnimation(b, 4, &data_ov001_0231f074, HANIM(0x57));
        b->blendSpeed = 0x400;
        b->flags[0] &= ~4;
        b->flags[4] &= ~4;
        b->speed = 0x1000;
        ExtraBoss_SpawnSpacebFx(p, p->unk1C, -0xa000);
        Sound_PlaySeqArc(0, 0, 0xb7);
        Sound_PlayVoice(0, 0, 0xd2);
        return;
    }
    t = ExtraBoss_GetPlayer();
    d = t->unk1C - p->unk1C;
    if (d > 0) {
        if (d > 0x1000)
            d = 0x1000;
        if (p->unk1C + d > 0x111000)
            d = 0x111000 - p->unk1C;
    } else if (d < 0) {
        if (d < -0x1000)
            d = -0x1000;
        if (p->unk1C + d < 0x65000)
            d = 0x65000 - p->unk1C;
    }
    p->unk54 = d;
}
static void ExtraBossObj_State_HandBossSuctionFire(ExObj *p0) {
    BossModel *t0 = p0->unk84, *t1;
    if ((t0->flags[0] & 2) == 0)
        return;
    t1 = ex_catch_model(p0);
    p0->unk74 = ExtraBossObj_State_HandBossSuctionHold;
    ex_hand_anims(t0, t1, 0x58);
    t0->flags[0] &= ~4;
    t0->flags[4] &= ~4;
    t0->speed = 0x1000;
    t1->flags[0] &= ~4;
    t1->flags[4] &= ~4;
    t1->speed = 0x1000;
}
static void ExtraBossObj_State_HandBossSuctionHold(ExObj *p0) {
    BossModel *t0 = p0->unk84, *t1;
    if ((t0->flags[0] & 2) == 0)
        return;
    t1 = ex_catch_model(p0);
    p0->unk74 = ExtraBossObj_State_HandBossSuctionWaitEnd;
    ex_hand_anims(t0, t1, 0x5c);
    t0->flags[0] |= 4;
    t0->flags[4] |= 4;
    t0->speed = 0x1000;
    t1->flags[0] |= 4;
    t1->flags[4] |= 4;
    t1->speed = 0x1000;
}
static void ExtraBossObj_State_HandBossSuctionWaitEnd(ExObj *p) {
    BossModel *a, *b;
    if (!(ExtraBoss_GetWork()->unk0 & 0x40)) {
        a = p->unk84;
        b = ex_catch_model(p);
        p->unk74 = ExtraBossObj_State_HandBossSuctionLeave;
        ex_hand_anims(a, b, 0x60);
        a->flags[0] |= 4;
        a->flags[4] |= 4;
        a->speed = 0x1000;
        b->flags[0] |= 4;
        b->flags[4] |= 4;
        b->speed = 0x1000;
        p->unk50 = 0x2000;
    }
}
static void ExtraBossObj_State_HandBossSuctionLeave(ExObj *p) {
    BossModel *a, *b;
    if (p->unk18 < 0x248000)
        return;
    a = p->unk84;
    b = ex_catch_model(p);
    p->unk74 = ExtraBossObj_State_HandBossSuctionReturn;
    p->unk50 = -0x1000;
    p->unk20 = 0;
    ex_hand_anims(a, b, 0x64);
    a->flags[0] |= 4;
    a->flags[4] |= 4;
    a->speed = 0x1000;
    b->flags[0] |= 4;
    b->flags[4] |= 4;
    b->speed = 0x1000;
    ExtraBoss_SetCameraParams(0, 0x20000, -0x20000, 0, 0x2000);
    p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
}
static void ExtraBossObj_State_HandBossSuctionReturn(ExObj *p0) {
    if (p0->unk18 + p0->unk50 > 0x153000)
        return;
    p0->unk18 = 0x153000;
    p0->unk54 = 0;
    p0->unk50 = p0->unk54;
    ExtraBoss_HandBoss_SelectNextAttack(p0);
}

/* hurt */
static void ExtraBossObj_State_HandBossHurtRecover(ExObj *p) {
    p->unk1D0--;
    if (p->unk1D0 <= 0) {
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
        ExtraBoss_HandBoss_SelectNextAttack(p);
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x02316804, 0x023167f8, 6);
        p->unk50 = p->unk54 = 0;
        return;
    }
    p->unk1C8 = (s16)(p->unk1C8 + 1);
    if ((p->unk1C8 & 7) == 1)
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x023167ec, 0x023167f8, 6);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x02316804, 0x023167f8, 6);
}
static void ExtraBossObj_State_HandBossHurt(ExObj *p) {
    BossModel *o = p->unk84, *o2;
    u16 r;
    s32 x, i;
    p->unk1CC = (s16)(p->unk1CC + 0x43);
    if ((s16)p->unk1CC > 0x1000)
        p->unk1CC = 0x1000;
    if (o->flags[0] & 2) {
        o2 = ex_catch_model(p);
        p->unk74 = ExtraBossObj_State_HandBossHurtRecover;
        o->flags[0] |= 0x40;
        Model_SetAnimation(o, 0, &data_ov001_0231f068, HANIM(0));
        Model_SetAnimation(o, 4, &data_ov001_0231f074, HANIM(1));
        o->blendSpeed = 0x400;
        o->flags[0] |= 4;
        o->flags[4] |= 4;
        o->speed = 0x1000;
        o2->flags[0] |= 0x40;
        Model_SetAnimation(o2, 0, &data_ov001_0231f068, HANIM(2));
        Model_SetAnimation(o2, 4, &data_ov001_0231f074, HANIM(3));
        o2->blendSpeed = 0x400;
        o2->flags[0] |= 4;
        o2->flags[4] |= 4;
        o2->speed = 0x1000;
        r = Rand();
        p->unk50 = FX_DivS32(0x100000 - p->unk18, 0xb4);
        p->unk54 = FX_DivS32(EX_S32(0x02316820, r & 7) + 0xc0000 - p->unk1C, 0xb4);
        p->unk1D0 = 0xb4;
    } else {
        i = 1;
        x = 0;
        do {
            x = FX_Mul(x - 0x6000, (s16)p->unk1CC) + 0x6000;
        } while (i-- != 0);
        p->unk50 = x;
    }
    p->unk1C8 = (s16)(p->unk1C8 + 1);
    if ((p->unk1C8 & 7) == 1)
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x023167ec, 0x023167f8, 6);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x02316804, 0x023167f8, 6);
}

/* defeat */
static void ExtraBossObj_State_HandBossDefeatFinish(ExObj *p0) {
    p0->unk14 ^= 0x30000;
    if ((u16)p0->unk1C8 == 0)
        return;
    ExtraBoss_GetWork()->unk0 |= 0x400;
    p0->unk74 = NULL;
}
static void ExtraBossObj_State_HandBossDefeatFadeOut(ExObj *p) {
    p->unk14 ^= 0x30000;
    p->unk1C8 -= 1;
    if (p->unk1C8 <= 0) {
        p->unk1C8 = 0;
        BrightnessLerp_Create(0x20, 0, 0x10, 3, &p->unk1C8);
        p->unk74 = ExtraBossObj_State_HandBossDefeatFinish;
        return;
    }
    if (p->unk1C8 == 0xc1)
        Sound_PlayVoice(0, 0, 0x24);
}
static void ExtraBossObj_State_HandBossDefeatWaitAnim(ExObj *p) {
    BossModel *q = p->unk84;
    p->unk14 ^= 0x30000;
    p->unk1C8 -= 1;
    if (p->unk1C8 == 0)
        Sound_PlayVoice(0, 0, 0x2d);
    if (q->flags[0] & 2) {
        p->unk74 = ExtraBossObj_State_HandBossDefeatFadeOut;
        p->unk1C8 = 0xfd;
    }
}
static void ExtraBossObj_State_HandBossDefeatPartBlink(ExObj *p0) {
    p0->unk14 ^= 0x30000;
    p0->unk1C8 = p0->unk1C8 - 1;
    if (p0->unk1C8 <= 0)
        p0->unk84->speed = 0x2000;
}
static ExObj *ex_defeat_part(s32 model) {
    ExObj *o = ExtraBossObj_Create(2, 0x2f01);
    BossModel *a;
    if (o == NULL)
        return NULL;
    a = (BossModel *)calloc(1, sizeof(BossModel));
    Model_Init(a, 0);
    Model_SetResource(a, &g_ex_bsex[model]);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x34cc;
    o->unk84 = a;
    return o;
}
static void ExtraBoss_HandBoss_FinishDefeat(ExObj *w) {
    ExBossWork *d = ex_hw(w);
    BossModel *a = w->unk84;
    ExObj *o2 = d->unk18;
    BossModel *a2 = o2->unk84;
    ExObj *o;
    ExtraBoss_StartCharacterSwitch();
    BrightnessLerp_Create(0x20, 0x10, 0, 3, NULL);
    ex_set_node_callback(a2, NULL);   /* NNS_G3dRenderObjResetCallBack */
    d->unk14 = d->unk16 = 0;
    o2->unk14 |= 0x30000;
    w->unk18 = w->unk1C = w->unk20 = 0;
    w->unk50 = w->unk54 = w->unk58 = 0;
    w->unk5C = w->unk60 = w->unk64 = 0;
    a->flags[0] |= 0x40;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0xc));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(0xd));
    a->blendSpeed = 0x400;
    a->flags[0] &= ~4;
    a->flags[4] &= ~4;
    a->speed = 0x2000;
    a2->flags[0] |= 0x40;
    Model_SetAnimation(a2, 0, &data_ov001_0231f068, HANIM(0xe));
    Model_SetAnimation(a2, 4, &data_ov001_0231f074, HANIM(0xf));
    a2->blendSpeed = 0x400;
    a2->flags[0] &= ~4;
    a2->flags[4] &= ~4;
    a2->speed = 0x2000;
    w->unk74 = ExtraBossObj_State_HandBossDefeatWaitAnim;
    o2->unk74 = ExtraBossObj_State_Blink;
    w->unk1C8 = 0xb4;
    o = ex_defeat_part(1);
    if (o != NULL)
        o->unk74 = ExtraBossObj_State_Blink;
    o = ex_defeat_part(4);
    if (o != NULL) {
        o->unk74 = ExtraBossObj_State_Blink;
        o->unk14 |= 0x30000;
    }
    o = ex_defeat_part(0);
    if (o != NULL) {
        BossModel *na3 = o->unk84;
        na3->cb[0] = MOP_FLUSHVP;
        Model_SetAnimation(na3, 0, &data_ov001_0231f068, 4);
        na3->flags[0] &= ~4;
        na3->speed = 0;
        o->unk74 = ExtraBossObj_State_HandBossDefeatPartBlink;
        o->unk1C8 = 0x119;
    }
}
static void ExtraBossObj_State_HandBossDefeat(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    (void)ExtraBoss_GetCurrentCharacterSlot();
    p0->unk1C8 = (s16)(p0->unk1C8 + 1);
    p0->unk1CC = (s16)(p0->unk1CC + 0x43);
    if ((s16)p0->unk1CC > 0x1000)
        p0->unk1CC = 0x1000;
    if ((s16)p0->unk1C8 == 0x77)
        BrightnessLerp_Create(0x10, 0, 0x10, 3, NULL);
    if (t0->flags[0] & 2) {
        ExtraBoss_HandBoss_FinishDefeat(p0);
        ExtraBoss_GetWork()->unk318 = 0;
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x02316804, 0x023167f8, 6);
        return;
    }
    if ((p0->unk1C8 & 7) == 1)
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x023167ec, 0x023167f8, 6);
    else if ((p0->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(0x0231d4ac, 0x02316804, 0x023167f8, 6);
}

/* -------------------------------------------------------------- creation */
ExObj *ExtraBoss_CreateHandBoss(void) {
    ExObj *o, *o2;
    ExBossWork *d;
    BossModel *a, *a2;
    o = ExtraBossObj_Create(2, 0x2f00);
    if (o == NULL)
        return NULL;
    data_ov001_0231f078[0] = o;
    d = (ExBossWork *)calloc(1, sizeof(ExBossWork));
    o->unk1DC = d;
    a = (BossModel *)calloc(1, sizeof(BossModel));
    Model_Init(a, 0);
    Model_SetResource(a, &g_ex_bsex[2]);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x34cc;
    o->unk84 = a;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, HANIM(0));
    Model_SetAnimation(a, 4, &data_ov001_0231f074, HANIM(1));
    a->flags[0] |= 4;
    a->flags[4] |= 4;
    a->speed = 0x1000;
    o->unk14 |= 0x40000;
    ex_set_node_callback(a, ExtraBoss_HandNodeCallback);
    a->cb[0] = MOP_FLUSHVP;
    o->unk78 = ExtraBossObj_Draw_HandBoss;
    o->unk18 = 0x1c0000;
    o->unk1C = ExtraBoss_GetPlayer()->unk1C;
    if (o->unk1C > 0x111000)
        o->unk1C = 0xc0000;
    else if (o->unk1C < 0x65000)
        o->unk1C = 0x65000;
    o->unk20 = 0;
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_B0], o, 4, 0, 0xd000, 0xd000, 0x1e000, -0x14000, 0x5000, 0, 0);
    o->unkB0[EX_COLL_B0].unk4 = ExtraBoss_HandBoss_OnHit;
    o->unkB0[EX_COLL_B0].unk8 |= 0x3000;
    o->unk9C[0] = &o->unkB0[EX_COLL_B0];
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_E8], o, 1, 1, 0x1e000, 0x32000, 0x20000, 0, 0, 0, 0);
    o->unkB0[EX_COLL_E8].unk8 |= 0x3000;
    o->unk9C[1] = &o->unkB0[EX_COLL_E8];
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_120], o, 1, 1, 0x32000, 0x1e000, 0x20000, 0, 0, 0, 0);
    o->unkB0[EX_COLL_120].unk8 |= 0x3200;
    o->unk9C[2] = &o->unkB0[EX_COLL_120];
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_158], o, 1, 1, 0x1e000, 0x32000, 0x20000, 0, 0, 0, 0);
    o->unkB0[EX_COLL_158].unk4 = ExtraBoss_Unk_ov001_022eed0c;
    o->unkB0[EX_COLL_158].unk8 |= 0x3250;
    o->unk9C[3] = &o->unkB0[EX_COLL_158];

    o2 = ExtraBossObj_Create(2, 0x2f01);
    data_ov001_0231f078[1] = o2;
    a2 = (BossModel *)calloc(1, sizeof(BossModel));
    Model_Init(a2, 0);
    Model_SetResource(a2, &g_ex_bsex[3]);
    a2->scale[0] = a2->scale[1] = a2->scale[2] = 0x34cc;
    o2->unk84 = a2;
    Model_SetAnimation(a2, 0, &data_ov001_0231f068, HANIM(2));
    Model_SetAnimation(a2, 4, &data_ov001_0231f074, HANIM(3));
    a2->flags[0] |= 4;
    a2->flags[4] |= 4;
    a2->speed = 0x1000;
    ex_set_node_callback(a2, ExtraBoss_HandCatchNodeCallback);
    a2->cb[0] = MOP_FLUSHVP;
    o2->unk78 = ExtraBossObj_Draw_HandCatch;
    o2->unk7C = data_ov001_0231f078[0];
    d->unk18 = o2;
    o2->unk14 |= 0x40000;
    o2->unk18 = o2->unk1C = o2->unk20 = 0;
    ExtraBoss_InitCollider(&o2->unkB0[EX_COLL_E8], o2, 1, 1, 0x32000, 0x1e000, 0x20000, 0, 0, 0, 0);
    o2->unkB0[EX_COLL_E8].unk8 |= 0x3200;
    o2->unk9C[1] = &o2->unkB0[EX_COLL_E8];
    ExtraBoss_InitCollider(&o2->unkB0[EX_COLL_120], o2, 1, 1, 0xf000, 0xf000, 0x20000, 0, 0, 0, 0);
    o2->unkB0[EX_COLL_120].unk4 = ExtraBoss_HandCatch_OnGrabPlayer;
    o2->unkB0[EX_COLL_120].unk8 |= 0x3220;
    o2->unk9C[2] = &o2->unkB0[EX_COLL_120];
    ExtraBoss_HandBoss_Action_Enter(data_ov001_0231f078[0]);
    d->unk8 = -1;
    d->unk4 = 4;
    d->unkC = d->u10.unk10 = 2;
    g_bossex_stats.boss_spawned[1]++;
    return o2;
}

const char *ex_hand_state_name(ExFn fn) {
#define N(f) if (fn == (ExFn)f) return #f;
    N(ExtraBossObj_State_HandBossEnter) N(ExtraBossObj_State_HandBossWait) N(ExtraBossObj_State_HandBossClawDelay)
    N(ExtraBossObj_State_HandBossClawWindUp) N(ExtraBossObj_State_HandBossClawSwipe1) N(ExtraBossObj_State_HandBossClawWindUp2)
    N(ExtraBossObj_State_HandBossClawSwipe2) N(ExtraBossObj_State_HandBossClawRecover) N(ExtraBossObj_State_HandBossClawReturn)
    N(ExtraBossObj_State_HandBossClawEnd) N(ExtraBossObj_State_HandBossCatchMove) N(ExtraBossObj_State_HandBossCatchReady)
    N(ExtraBossObj_State_HandBossCatchAim) N(ExtraBossObj_State_HandBossCatchWaitHand) N(ExtraBossObj_State_HandBossCatchResetArm)
    N(ExtraBossObj_State_HandBossCatchTurnArm) N(ExtraBossObj_State_HandBossCatchRewind) N(ExtraBossObj_State_HandBossGrabTurnArm)
    N(ExtraBossObj_State_HandBossGrabHold) N(ExtraBossObj_State_HandBossGrabEnd) N(ExtraBossObj_State_HandBossSuctionExit)
    N(ExtraBossObj_State_HandBossSuctionEnter) N(ExtraBossObj_State_HandBossSuctionReady) N(ExtraBossObj_State_HandBossSuctionAim)
    N(ExtraBossObj_State_HandBossSuctionFire) N(ExtraBossObj_State_HandBossSuctionHold) N(ExtraBossObj_State_HandBossSuctionWaitEnd)
    N(ExtraBossObj_State_HandBossSuctionLeave) N(ExtraBossObj_State_HandBossSuctionReturn) N(ExtraBossObj_State_HandBossHurt)
    N(ExtraBossObj_State_HandBossHurtRecover) N(ExtraBossObj_State_HandBossDefeat) N(ExtraBossObj_State_HandBossDefeatWaitAnim)
    N(ExtraBossObj_State_HandBossDefeatFadeOut) N(ExtraBossObj_State_HandBossDefeatFinish) N(ExtraBossObj_State_HandBossDefeatPartBlink)
    N(ExtraBossObj_State_HandCatchFlyOut) N(ExtraBossObj_State_HandCatchReturn) N(ExtraBossObj_State_HandCatchPullPlayer)
#undef N
    return NULL;
}
