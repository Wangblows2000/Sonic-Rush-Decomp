/*
 * Extra Zone boss: the weapon boss (Egg Salamander, bsex_p_body: Super Sonic's rounds 0, 2 and 4).
 * ExtraBoss_CreateWeaponBoss, ExtraBoss_WeaponBoss_*, ExtraBossObj_State_WeaponBoss*,
 * ExtraBossObj_Draw_WeaponBoss(Debris), ExtraBoss_WeaponNodeCallback, and the movement helpers shared
 * with the hand boss.  Translated from src/game/Zone/Extra_Zone/Boss/ (overlay ov001).
 *
 * Node matrices: the DS stores the matrices of the nodes p_body / core / weapon_l / weapon_r in GE
 * matrix-stack slots 0x1b..0x1e from a NODEDESC callback and reads them back with
 * NNS_G3dGetCurrentMtx; natively the joint world matrices of the posed model are written to the same
 * slots (g_boss_mtx_slot, used by MTX_RESTORE models such as the sword) and read back in fx32.
 */
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"

ExObj *data_ov001_0231f06c;   /* the weapon boss */
s32 data_ov001_0231f064;

/* ---------------------------------------------------------- node matrices */
void ex_set_node_callback(BossModel *m, void (*fn)(BossModel *));
int ex_model_index(const BossModel *m);
void ex_get_slot_mtx(int slot, ExMtx43 *out);

/* ExtraBoss_StoreNodeMtxIfMatch (substantially rewritten: the node is looked up by name in the
 * converter's joint table and its world matrix stored in matrix slot v) */
static BOOL ExtraBoss_StoreNodeMtxIfMatch(BossModel *p, u32 v, u32 name_ptr) {
    dcm_mtx_t m;
    int j = ex_joint(ex_model_index(p), ex_rom_str(name_ptr));
    if (j < 0)
        return FALSE;
    boss_model_joint_world(p, j, &m);
    g_boss_mtx_slot[v & 31] = m;
    return TRUE;
}

/* ExtraBoss_WeaponNodeCallback (substantially rewritten: called once after the pose; the four
 * NODEDESC visits of one draw store p_body 0x1b, core 0x1c, weapon_l 0x1d, weapon_r 0x1e) */
static void ExtraBoss_WeaponNodeCallback(BossModel *p) {
    data_ov001_0231f064 = 0;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1b, EX_DATA(0x0231d430, const u32 *)[0]))
        data_ov001_0231f064++;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1c, EX_DATA(0x0231d430, const u32 *)[1]))
        data_ov001_0231f064++;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1d, EX_DATA(0x0231d430, const u32 *)[2]))
        data_ov001_0231f064++;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1e, EX_DATA(0x0231d430, const u32 *)[3]))
        data_ov001_0231f064 = 0;
}

/* ExtraBoss_HandNodeCallback (used by the hand boss and the weapon boss' defeat): p_body 0x1d,
 * l_hand0a 0x1c, body_hand 0x1e, core 0x1b */
void ExtraBoss_HandNodeCallback(BossModel *p) {
    data_ov001_0231f064 = 0;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1d, EX_DATA(0x0231d440, const u32 *)[0]))
        data_ov001_0231f064++;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1c, EX_DATA(0x0231d440, const u32 *)[1]))
        data_ov001_0231f064++;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1e, EX_DATA(0x0231d440, const u32 *)[2]))
        data_ov001_0231f064++;
    if (ExtraBoss_StoreNodeMtxIfMatch(p, 0x1b, EX_DATA(0x0231d440, const u32 *)[3]))
        data_ov001_0231f064 = 0;
}

/* ---------------------------------------------------------- movement helpers */
void ExtraBoss_StopAndFaceCenter(ExObj *p) {
    p->unk60 = 0;
    p->unk5C = p->unk60;
    if (p->unk18 < 0xbb000)
        p->unk54 = 0x1000;
    else
        p->unk54 = -0x1000;
}
void ExtraBoss_StopAndFaceCenter2(ExObj *p) {
    p->unk60 = 0;
    p->unk5C = p->unk60;
    if (p->unk18 < 0xbb000)
        p->unk54 = 0x1000;
    else
        p->unk54 = -0x1000;
}
void ExtraBoss_StepXTowardCameraTarget(ExObj *p) {
    s32 v, x, d;
    d = (s32)(((s64)FX_Div(ExtraBoss_GetWork()->unk31C - 0x80000, 0x80000) * 0x53000 + 0x800) >> 12) + 0x100000;
    x = p->unk18;
    v = d - x;
    if (v > 0) {
        if (v > 0x1000)
            v = 0x1000;
        if (x + v > 0x153000)
            v = 0x153000 - x;
    } else if (v < 0) {
        if (v < -0x1000)
            v = -0x1000;
        if (x + v < 0x100000)
            v = 0x100000 - x;
    }
    p->unk50 = v;
    if (p->unk1C < 0x65000)
        p->unk54 = 0x1000;
    else if (p->unk1C > 0x111000)
        p->unk54 = -0x1000;
}
void ExtraBoss_StepXTowardRightEdge(ExObj *p) {
    s32 x, v, d;
    x = p->unk18;
    d = 0x153000 - x;
    v = d;
    if (d > 0) {
        if (d > 0x1000)
            v = 0x1000;
        if (x + v > 0x153000)
            v = d;
    } else if (d < 0) {
        if (d < -0x1000)
            v = -0x1000;
        if (x + v < 0x100000)
            v = 0x100000 - x;
    }
    p->unk50 = v;
    if (p->unk1C < 0x65000)
        p->unk54 = 0x1000;
    else if (p->unk1C > 0x111000)
        p->unk54 = -0x1000;
}

/* ---------------------------------------------------------------- drawing */
static fx32 *ex_tr(ExBossWork *s, int k) { return &s->mtx[k].m[9]; }

static void ExtraBossObj_Draw_WeaponBoss(ExObj *w) {
    MtxFx33 m;
    ExBossWork *s = (ExBossWork *)w->unk1DC;
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
    ex_get_slot_mtx(0x1b, &s->mtx[EXM_SLOT_1B]);
    ex_get_slot_mtx(0x1c, &s->mtx[EXM_SLOT_1C]);
    ex_get_slot_mtx(0x1d, &s->mtx[EXM_SLOT_1D]);
    ex_get_slot_mtx(0x1e, &s->mtx[EXM_SLOT_1E]);
    c = w->unk9C[0];
    c->unk28[0] = ex_tr(s, EXM_SLOT_1C)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_SLOT_1C)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_SLOT_1C)[2];
    c = w->unk9C[1];
    c->unk28[0] = ex_tr(s, EXM_SLOT_1B)[0] - data_ov001_0231f524;
    c->unk28[1] = -(ex_tr(s, EXM_SLOT_1B)[1] + data_ov001_0231f520);
    c->unk28[2] = ex_tr(s, EXM_SLOT_1B)[2];
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

static void ExtraBossObj_Draw_WeaponBossDebris(ExObj *w) {
    MtxFx33 m;
    ExMtx43 cur;
    ExObj *p = w->unk7C;
    BossModel *a = p->unk84;
    a->trans[0] = data_ov001_0231f524 + (p->unk18 + p->unk30);
    a->trans[1] = -(data_ov001_0231f520 + (p->unk1C + p->unk34));
    a->trans[2] = p->unk20 + p->unk38;
    MTX_RotX33(&a->rot, SIN(p->unk3C), COS(p->unk3C));
    MTX_RotY33(&m, SIN(p->unk3E), COS(p->unk3E));
    MTX_Concat33(&a->rot, &m, &a->rot);
    MTX_RotZ33(&m, SIN(p->unk40), COS(p->unk40));
    MTX_Concat33(&a->rot, &m, &a->rot);
    if (!(w->unk14 & 0x10000))
        Model_Animate(a);
    ex_draw_model(a, 0, NULL);
    ex_get_slot_mtx(0x1e, &cur);
    a = w->unk84;
    w->unk18 = cur.m[9];
    w->unk1C = -cur.m[10];
    w->unk20 = cur.m[11];
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
}

/* ---------------------------------------------------------------- actions */
static void ExtraBossObj_State_WeaponBossEnter(ExObj *p);
static void ExtraBossObj_State_WeaponBossWait(ExObj *p);
static void ExtraBossObj_State_WeaponBossBeamUnitAim(ExObj *p);
static void ExtraBossObj_State_WeaponBossBeamUnitWaitClear(ExObj *p);
static void ExtraBossObj_State_WeaponBossSwordWindUp(ExObj *p);
static void ExtraBossObj_State_WeaponBossBeamCharge(ExObj *p);
static void ExtraBossObj_State_WeaponBossMissileWindUp(ExObj *p);
static void ExtraBossObj_State_WeaponBossHurt(ExObj *p);
static void ExtraBossObj_State_WeaponBossDefeat(ExObj *p);
static void ExtraBoss_WeaponBoss_SelectNextAttack(ExObj *w);

static void ExtraBoss_WeaponBoss_Action_Enter(ExObj *p) {
    BossModel *q = p->unk84;
    p->unk74 = ExtraBossObj_State_WeaponBossEnter;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0);
    q->flags[0] |= 4;
    q->speed = 0x1000;
    p->unk1C8 = 0;
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
}
static void ExtraBoss_WeaponBoss_Action_Wait(ExObj *p) {
    BossModel *q = p->unk84;
    p->unk74 = ExtraBossObj_State_WeaponBossWait;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0);
    q->flags[0] |= 4;
    q->speed = 0x1000;
    {   /* ARM order: table entry from the first Rand(), offset from the second */
        s32 a = EX_S32(0x0231693c, Rand() & 3);
        p->unk1C8 = a + (Rand() & 0x3f);
    }
    ExtraBoss_StopAndFaceCenter2(p);
}
static void ExtraBoss_WeaponBoss_Action_BeamUnitAttack(ExObj *p) {
    BossModel *q = p->unk84;
    p->unk1CC = 2;
    if (ExtraBoss_CountActiveBeamUnits() != 0) {
        p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitWaitClear;
        Model_SetAnimation(q, 0, &data_ov001_0231f068, 0);
        q->flags[0] |= 4;
        q->speed = 0x1000;
        ExtraBoss_StopAndFaceCenter2(p);
        return;
    }
    p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitAim;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 5);
    q->flags[0] &= ~4;
    q->speed = 0x1000;
}
static void ExtraBoss_WeaponBoss_Action_SwordAttack(ExObj *p) {
    BossModel *r = p->unk84;
    ExBossWork *q = (ExBossWork *)p->unk1DC;
    ExObj *t;
    BossModel *s;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, (u32)EX_S32(0x0231d4c0, 1));
    r->flags[0] &= ~4;
    r->speed = 0x1000;
    p->unk1CC = 2;
    t = ExtraBoss_SpawnSwordFx(p);
    q->u10.sword = t;
    if (t != NULL) {
        s = t->unk84;
        s->unk8 = 0x1d;
        s->cb[0] = MOP_FLUSHVP;   /* unkA = 4 */
        s->cb[1] = MOP_RESTORE;   /* unkB = 2 */
        s->cb[2] = MOP_ROTATE;    /* unkC = 0xb */
        t->unk3C = 0x4000;
        t->unk3E = 0x8000;
    }
    p->unk74 = ExtraBossObj_State_WeaponBossSwordWindUp;
    ExtraBoss_StopAndFaceCenter2(p);
}
static void ExtraBoss_WeaponBoss_Action_HomingBeamAttack(ExObj *p) {
    BossModel *q = p->unk84;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0x10);
    q->flags[0] &= ~4;
    q->speed = 0x1000;
    p->unk1CC = 4;
    p->unk74 = ExtraBossObj_State_WeaponBossBeamCharge;
    ExtraBoss_StopAndFaceCenter2(p);
}
static void ExtraBoss_WeaponBoss_Action_MissileAttack(ExObj *p) {
    BossModel *q = p->unk84;
    p->unk74 = ExtraBossObj_State_WeaponBossMissileWindUp;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0x13);
    q->flags[0] &= ~4;
    q->speed = 0x1000;
    p->unk1CC = EX_S32(0x0231694c, ex_level() * 5 + ExtraBoss_GetWork()->unk40);
    p->unk1D4 = (u16)FX_DivS32(0x5555, p->unk1CC - 1);
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
    Sound_PlaySeqArc(0, 0, 0xa5);
}
static void ExtraBoss_WeaponBoss_Action_Hurt(ExObj *p) {
    ExBossWork *a = (ExBossWork *)p->unk1DC;
    BossModel *q = p->unk84;
    a->unk8 = a->unk0;
    a->unk0 = 2;
    p->unk74 = ExtraBossObj_State_WeaponBossHurt;
    q->flags[0] |= 0x40;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 1);
    q->flags[0] &= ~4;
    q->blendSpeed = 0x400;
    q->speed = 0x1000;
    p->unk50 = 0x6000;
    p->unk60 = 0;
    p->unk5C = p->unk60;
    p->unk54 = p->unk5C;
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
    p->unkB0[EX_COLL_E8].unk8 |= 0x200;
    p->unk1C8 = 0;
    p->unk1CC = 0;
}
static void ExtraBoss_WeaponBoss_Action_Defeat(ExObj *p0) {
    ExBossWork *t0 = (ExBossWork *)p0->unk1DC;
    BossModel *t1 = p0->unk84;
    ExWork *t2 = ExtraBoss_GetWork();
    ExObj *t4;
    ExPlayerWork *t5;
    t0->unk8 = t0->unk0;
    t0->unk0 = 3;
    p0->unk74 = ExtraBossObj_State_WeaponBossDefeat;
    t1->flags[0] |= 0x40;
    Model_SetAnimation(t1, 0, &data_ov001_0231f068, 2);
    t1->flags[0] &= ~4;
    t1->blendSpeed = 0x400;
    t1->speed = 0x1000;
    p0->unk50 = 0x4000;
    p0->unk5C = 0xe0;
    p0->unk60 = 0;
    p0->unk54 = p0->unk60;
    p0->unkB0[EX_COLL_B0].unk8 |= 0x200;
    p0->unkB0[EX_COLL_E8].unk8 |= 0x200;
    p0->unk1C8 = 0;
    p0->unk1CC = 0;
    t2->unk318 = 1;
    t4 = ExtraBoss_GetPlayer();
    t5 = (ExPlayerWork *)t4->unk1DC;
    t4->unkB0[EX_COLL_B0].unk8 |= 0x200;
    t4->unkB0[EX_COLL_E8].unk8 |= 0x200;
    t4->unk14 &= ~0x20000u;
    t5->unk0 &= ~2u;
    t5->unk10 = 0;
    ExtraBoss_GetWork()->unk0 |= 2;
    ExtraBoss_SetScrollSpawnEnabled(0);
    ExtraBoss_SetRingPickupEnabled(0);
}

static void ExtraBoss_WeaponBoss_OnHit(ExCollider *self, ExCollider *other) {
    ExObj *s = self->owner;
    ExObj *p = other->owner;
    ExWork *c = ExtraBoss_GetWork();
    ExBossWork *w = (ExBossWork *)p->unk1DC;
    c->unk308--;
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
    p->unkB0[EX_COLL_E8].unk8 |= 0x200;
    w->unkC = 0;
    ExtraBoss_SpawnDamageFx(NULL, s->unk18, s->unk1C, 0);
    w->flash = 0;
    if (w->unk0 == 5)
        ExtraBoss_CancelSwordFx();
    if (c->unk308 > 0) {
        ExtraBoss_WeaponBoss_Action_Hurt(p);
        Sound_PlaySeqArc(0, 0, 0x7b);
    } else {
        ExtraBoss_WeaponBoss_Action_Defeat(p);
        Sound_PlaySeqArc(0, 0, 0xba);
    }
    Sound_PlayVoice(0, 0, EX_S32(0x02316910, Rand() & 1));
    g_bossex_stats.boss_hits++;
}

static void ExtraBoss_WeaponBoss_SelectNextAttack(ExObj *w) {
    ExWork *p = ExtraBoss_GetWork();
    ExBossWork *s = (ExBossWork *)w->unk1DC;
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

/* ----------------------------------------------------------------- states */
static void ExtraBossObj_State_WeaponBossEnter(ExObj *p) {
    s32 i, v;
    p->unk1C8 = (s16)(p->unk1C8 + 0x22);
    if (p->unk1C8 >= 0x1000) {
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        ExtraBoss_WeaponBoss_SelectNextAttack(p);
        return;
    }
    i = 2;
    v = 0x1c0000;
    do {
        v += FX_Mul(0xdc000 - v, p->unk1C8);
    } while (i-- != 0);
    p->unk18 = v;
    i = 2;
    v = 0xc0000;
    do {
        v += FX_Mul(0xc0000 - v, p->unk1C8);
    } while (i-- != 0);
    p->unk1C = v;
}
static void ExtraBossObj_State_WeaponBossWait(ExObj *p0) {
    p0->unk1C8 = p0->unk1C8 - 1;
    if (p0->unk1C8 > 0) {
        ExtraBoss_StepXTowardCameraTarget(p0);
        return;
    }
    ExtraBoss_WeaponBoss_SelectNextAttack(p0);
}

/* homing beams */
static void ExtraBossObj_State_WeaponBossBeamFire(ExObj *p);
static void ExtraBossObj_State_WeaponBossBeamCharge(ExObj *p) {
    BossModel *r = p->unk84;
    ExBossWork *q;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(r->flags[0] & 2))
        return;
    q = (ExBossWork *)p->unk1DC;
    p->unk74 = ExtraBossObj_State_WeaponBossBeamFire;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, 0x11);
    p->unk1D0 = 3;
    ExtraBoss_SpawnHomingBeam(p, ex_tr(q, EXM_SLOT_1D)[0] - data_ov001_0231f524,
                              -(ex_tr(q, EXM_SLOT_1D)[1] + data_ov001_0231f520), 0);
}
static void ExtraBossObj_State_WeaponBossBeamRefire(ExObj *p) {
    BossModel *r;
    ExBossWork *q;
    ExtraBoss_StepXTowardCameraTarget(p);
    p->unk1C8 -= 1;
    if (p->unk1C8 > 0)
        return;
    r = p->unk84;
    q = (ExBossWork *)p->unk1DC;
    p->unk74 = ExtraBossObj_State_WeaponBossBeamFire;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, 0x11);
    ExtraBoss_SpawnHomingBeam(p, ex_tr(q, EXM_SLOT_1D)[0] - data_ov001_0231f524,
                              -(ex_tr(q, EXM_SLOT_1D)[1] + data_ov001_0231f520), 0);
}
static void ExtraBossObj_State_WeaponBossBeamPause(ExObj *p) {
    BossModel *q;
    p->unk1C8 -= 1;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (p->unk1C8 > 0)
        return;
    q = p->unk84;
    p->unk74 = ExtraBossObj_State_WeaponBossBeamCharge;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0x10);
    q->flags[0] &= ~4;
}
static void ExtraBossObj_State_WeaponBossBeamRecover(ExObj *p) {
    BossModel *q = p->unk84;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(q->flags[0] & 2))
        return;
    p->unk1CC -= 1;
    if (p->unk1CC <= 0) {
        ExtraBoss_WeaponBoss_SelectNextAttack(p);
        return;
    }
    p->unk74 = ExtraBossObj_State_WeaponBossBeamPause;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0);
    q->flags[0] |= 4;
    p->unk1C8 = 0x28;
}
static void ExtraBossObj_State_WeaponBossBeamFire(ExObj *p) {
    BossModel *q = p->unk84;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(q->flags[0] & 2))
        return;
    p->unk1D0 -= 1;
    if (p->unk1D0 > 0) {
        p->unk74 = ExtraBossObj_State_WeaponBossBeamRefire;
        p->unk1C8 = 0xa;
        return;
    }
    p->unk74 = ExtraBossObj_State_WeaponBossBeamRecover;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0x12);
}

/* beam units */
static void ExtraBossObj_State_WeaponBossBeamUnitLaunch(ExObj *w);
static void ExtraBossObj_State_WeaponBossBeamUnitEnd(ExObj *p0) {
    if ((p0->unk84->flags[0] & 2) == 0)
        return;
    ExtraBoss_WeaponBoss_SelectNextAttack(p0);
}
static void ExtraBossObj_State_WeaponBossBeamUnitResetArm(ExObj *p) {
    BossModel *q = p->unk84;
    s32 v = q->speed;
    if (v > 0) {
        if (q->anm[0]->frame + v < 0x64000)
            return;
    } else {
        if (q->anm[0]->frame + v > 0x64000)
            return;
    }
    p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitEnd;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 7);
    q->flags[0] &= ~4;
    q->speed = 0x1000;
}
static u16 ex_beam_unit_angle(s32 frame) { return (u16)((((frame >> 13) << 16) / 360) - 0xa38f); }
static void ExtraBossObj_State_WeaponBossBeamUnitTurnArm(ExObj *p) {
    BossModel *o = p->unk84;
    ExBossWork *w;
    if (o->speed >= 0) {
        if (p->unk1D0 > o->anm[0]->frame + o->speed)
            return;
    } else {
        if (p->unk1D0 < o->anm[0]->frame + o->speed)
            return;
    }
    w = (ExBossWork *)p->unk1DC;
    p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitLaunch;
    o->anm[0]->frame = p->unk1D0;
    o->speed = 0;
    ExtraBoss_SpawnBeamUnit(p, ex_tr(w, EXM_SLOT_1E)[0] - data_ov001_0231f524,
                            -(ex_tr(w, EXM_SLOT_1E)[1] + data_ov001_0231f520), 0, ex_beam_unit_angle(p->unk1D0));
    p->unk1C8 = 0;
    p->unk1D4 = 0;
}
static s32 ex_turn_speed(void) {
    ExWork *q = ExtraBoss_GetWork();
    return FX_DivS32(q->unk308 * 0xa000 + ((q->unk30C - q->unk308) << 16), q->unk30C);
}
static void ExtraBossObj_State_WeaponBossBeamUnitLaunch(ExObj *w) {
    BossModel *a = w->unk84;
    ExObj *t;
    ExBossWork *s;
    u16 ang;
    s32 acc, i, v;
    if ((s16)w->unk1C8 < 0x1000) {
        w->unk1C8 = (s16)(w->unk1C8 + 0x199);
        if ((s16)w->unk1C8 > 0x1000)
            w->unk1C8 = 0x1000;
    } else {
        w->unk1D4 = (s16)(w->unk1D4 + 0x44);
        if ((s16)w->unk1D4 > 0x1000)
            w->unk1D4 = 0x1000;
    }
    if ((s16)w->unk1D4 >= 0x1000) {
        w->unk30 = 0;
        w->unk1CC--;
        if (w->unk1CC <= 0) {
            w->unk74 = ExtraBossObj_State_WeaponBossBeamUnitResetArm;
            if (a->anm[0]->frame > 0x64000) {
                a->speed = -0x10000;
                return;
            }
            if (a->anm[0]->frame < 0x64000) {
                a->speed = 0x10000;
                return;
            }
            w->unk74 = ExtraBossObj_State_WeaponBossBeamUnitEnd;
            Model_SetAnimation(a, 0, &data_ov001_0231f068, 7);
            a->flags[0] &= ~4;
            a->speed = 0x1000;
            return;
        }
        t = ExtraBoss_GetPlayer();
        w->unk74 = ExtraBossObj_State_WeaponBossBeamUnitTurnArm;
        ang = FX_Atan2(t->unk1C - w->unk1C, t->unk18 - w->unk18);
        if (ang < 0x5c71)
            ang = 0x5c71;
        else if (ang > 0xb1c7)
            ang = 0xb1c7;
        w->unk1D0 = FX_DivS32((u16)(ang + 0xa38f), 0xb6) << 13;
        if (a->anm[0]->frame == w->unk1D0) {
            s = (ExBossWork *)w->unk1DC;
            a->speed = 0;
            w->unk74 = ExtraBossObj_State_WeaponBossBeamUnitLaunch;
            a->speed = 0;
            ExtraBoss_SpawnBeamUnit(w, ex_tr(s, EXM_SLOT_1E)[0] - data_ov001_0231f524,
                                    -(ex_tr(s, EXM_SLOT_1E)[1] + data_ov001_0231f520), 0, ex_beam_unit_angle(w->unk1D0));
            w->unk1C8 = 0;
            w->unk1D4 = 0;
            return;
        }
        a->speed = ex_turn_speed();
        if (w->unk1D0 < a->anm[0]->frame)
            a->speed = -a->speed;
        return;
    }
    if ((s16)w->unk1D4 == 0) {
        v = (s16)w->unk1C8;
        i = 5;
        acc = 0;
        do {
            acc += FX_Mul(0x18000 - acc, v);
        } while (i-- != 0);
        w->unk30 = acc;
    } else {
        v = (s16)w->unk1D4;
        i = 3;
        acc = 0x18000;
        do {
            acc += FX_Mul(-acc, v);
        } while (i-- != 0);
        w->unk30 = acc;
    }
}
static void ExtraBossObj_State_WeaponBossBeamUnitAim(ExObj *p) {
    BossModel *a = p->unk84;
    ExObj *t;
    ExBossWork *q;
    u16 ang;
    if (!(a->flags[0] & 2))
        return;
    t = ExtraBoss_GetPlayer();
    p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitTurnArm;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, 6);
    a->flags[0] &= ~4;
    a->anm[0]->frame = 0x64000;
    ang = FX_Atan2(t->unk1C - p->unk1C, t->unk18 - p->unk18);
    if (ang == 0x8000) {
        q = (ExBossWork *)p->unk1DC;
        p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitLaunch;
        a->speed = 0;
        p->unk1D0 = 0x64000;
        ExtraBoss_SpawnBeamUnit(p, ex_tr(q, EXM_SLOT_1E)[0] - data_ov001_0231f524,
                                -(ex_tr(q, EXM_SLOT_1E)[1] + data_ov001_0231f520), 0, ex_beam_unit_angle(p->unk1D0));
        p->unk1C8 = 0;
        p->unk1D4 = 0;
        return;
    }
    if (ang < 0x5c71)
        ang = 0x5c71;
    else if (ang > 0xb1c7)
        ang = 0xb1c7;
    p->unk1D0 = FX_DivS32((u16)(ang + 0xa38f), 0xb6) << 13;
    a->speed = ex_turn_speed();
    if (p->unk1D0 < 0x64000)
        a->speed = -a->speed;
}
static void ExtraBossObj_State_WeaponBossBeamUnitWaitClear(ExObj *p) {
    BossModel *q;
    if (ExtraBoss_CountActiveBeamUnits() != 0) {
        ExtraBoss_StepXTowardCameraTarget(p);
        return;
    }
    q = p->unk84;
    p->unk74 = ExtraBossObj_State_WeaponBossBeamUnitAim;
    p->unk54 = 0;
    p->unk50 = p->unk54;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 5);
    q->flags[0] &= ~4;
    q->speed = 0x1000;
}

/* sword */
static void ExtraBossObj_State_WeaponBossSwordSlash(ExObj *p);
static void ExtraBossObj_State_WeaponBossSwordFollowThrough(ExObj *p0);
static void ExtraBossObj_State_WeaponBossSwordRecover(ExObj *p);
static s32 ex_swing_speed(void) {
    ExWork *q = ExtraBoss_GetWork();
    return FX_DivS32(q->unk308 * 0x1333 + (q->unk30C - q->unk308) * 0x1800, q->unk30C);
}
static void ExtraBossObj_State_WeaponBossSwordWindUp(ExObj *p) {
    BossModel *r = p->unk84;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(r->flags[0] & 2))
        return;
    p->unk74 = ExtraBossObj_State_WeaponBossSwordSlash;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, (u32)EX_S32(0x0231d4c0, p->unk1CC) + 1);
    r->speed = ex_swing_speed();
    ExtraBoss_AdvanceSwordFxStep(((ExBossWork *)p->unk1DC)->u10.sword);
    p->unk60 = 0;
    p->unk5C = p->unk60;
    p->unk54 = p->unk5C;
    p->unk50 = p->unk54;
    p->unkB0[EX_COLL_B0].unk8 |= 0x200;
}
static void ExtraBossObj_State_WeaponBossSwordSlash(ExObj *p) {
    BossModel *r = p->unk84;
    if (!(r->flags[0] & 2))
        return;
    p->unk74 = ExtraBossObj_State_WeaponBossSwordFollowThrough;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, (u32)EX_S32(0x0231d4c0, p->unk1CC) + 2);
    if (p->unk1CC == 1)
        p->unk50 = -0x6000;
    ExtraBoss_AdvanceSwordFxStep(((ExBossWork *)p->unk1DC)->u10.sword);
    ExtraBossObj_State_WeaponBossSwordFollowThrough(p);
    Sound_PlaySeqArc(0, 0, 0xad);
    Sound_PlayVoice(0, 0, 0xd0);
}
static void ExtraBossObj_State_WeaponBossSwordFollowThrough(ExObj *p0) {
    ExBossWork *t1 = (ExBossWork *)p0->unk1DC;
    ExCollider *t0 = &t1->u10.sword->unkB0[EX_COLL_E8];
    BossModel *t2 = p0->unk84;
    int k = p0->unk1CC == 2 ? EXM_SLOT_1D : EXM_SLOT_1E;
    t0->unk28[0] = ex_tr(t1, k)[0] - data_ov001_0231f524;
    t0->unk28[1] = -(ex_tr(t1, k)[1] + data_ov001_0231f520);
    t0->unk28[2] = ex_tr(t1, k)[2];
    if ((t2->flags[0] & 2) == 0)
        return;
    p0->unk74 = ExtraBossObj_State_WeaponBossSwordRecover;
    Model_SetAnimation(t2, 0, &data_ov001_0231f068, (u32)EX_S32(0x0231d4c0, p0->unk1CC) + 3);
    t2->speed = 0x1000;
    ExtraBoss_AdvanceSwordFxStep(t1->u10.sword);
    p0->unk50 = 0;
}
static void ExtraBossObj_State_WeaponBossSwordEnd(ExObj *p0) {
    p0->unk1C8 = p0->unk1C8 - 1;
    if (p0->unk1C8 <= 0)
        ExtraBoss_WeaponBoss_SelectNextAttack(p0);
    ExtraBoss_StepXTowardCameraTarget(p0);
}
static void ExtraBossObj_State_WeaponBossSwordNextSwing(ExObj *p) {
    BossModel *r = p->unk84;
    ExBossWork *q;
    ExObj *t;
    ExtraBoss_StepXTowardCameraTarget(p);
    p->unk1C8 -= 1;
    if (p->unk1C8 > 0)
        return;
    q = (ExBossWork *)p->unk1DC;
    p->unk74 = ExtraBossObj_State_WeaponBossSwordWindUp;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, (u32)EX_S32(0x0231d4c0, p->unk1CC));
    r->flags[0] &= ~4;
    t = ExtraBoss_SpawnSwordFx(p);
    q->u10.sword = t;
    if (t != NULL) {
        BossModel *s = t->unk84;
        s->unk8 = 0x1e;
        s->cb[0] = MOP_FLUSHVP;
        s->cb[1] = MOP_RESTORE;
    }
    ExtraBoss_StopAndFaceCenter2(p);
}
static void ExtraBossObj_State_WeaponBossSwordRecover(ExObj *p) {
    BossModel *a = p->unk84;
    if (!(a->flags[0] & 2))
        return;
    p->unk1CC--;
    if (p->unk1CC <= 0) {
        p->unk74 = ExtraBossObj_State_WeaponBossSwordEnd;
        ExtraBoss_StopAndFaceCenter2(p);
        p->unk1C8 = 0x5a;
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        return;
    }
    p->unk74 = ExtraBossObj_State_WeaponBossSwordNextSwing;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, 0);
    a->flags[0] |= 4;
    p->unk1C8 = EX_S32(0x02316974, Rand() & 0xf);
    p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    ExtraBoss_StopAndFaceCenter2(p);
}

/* missiles */
static void ExtraBossObj_State_WeaponBossMissileFire(ExObj *p);
static void ExtraBossObj_State_WeaponBossMissileEnd(ExObj *p0) {
    if ((p0->unk84->flags[0] & 2) == 0)
        return;
    ExtraBoss_WeaponBoss_SelectNextAttack(p0);
    p0->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
}
static void ExtraBossObj_State_WeaponBossMissileRecover(ExObj *p) {
    BossModel *q;
    p->unk1C8 -= 1;
    if (p->unk1C8 > 0)
        return;
    q = p->unk84;
    p->unk74 = ExtraBossObj_State_WeaponBossMissileEnd;
    Model_SetAnimation(q, 0, &data_ov001_0231f068, 0x17);
    q->flags[0] &= ~4;
}
static void ex_fire_missile(ExObj *p) {
    ExBossWork *q = (ExBossWork *)p->unk1DC;
    ExtraBoss_SpawnHomingMissile(p, ex_tr(q, EXM_SLOT_1B)[0] - data_ov001_0231f524,
                                 -(ex_tr(q, EXM_SLOT_1B)[1] + data_ov001_0231f520), ex_tr(q, EXM_SLOT_1B)[2],
                                 (u16)((p->unk1CC - 1) * (u16)p->unk1D4 + 0x9555));
}
static void ExtraBossObj_State_WeaponBossMissileFire(ExObj *p) {
    BossModel *a = p->unk84;
    if (!(a->flags[0] & 2))
        return;
    p->unk1CC--;
    if (p->unk1CC <= 0) {
        p->unk74 = ExtraBossObj_State_WeaponBossMissileRecover;
        Model_SetAnimation(a, 0, &data_ov001_0231f068, 0x16);
        a->flags[0] |= 4;
        p->unk1C8 = 0xf0;
        return;
    }
    Model_SetAnimation(a, 0, &data_ov001_0231f068, 0x15);
    ex_fire_missile(p);
}
static void ExtraBossObj_State_WeaponBossMissileReady(ExObj *p) {
    BossModel *r = p->unk84;
    if (!(r->flags[0] & 2))
        return;
    p->unk74 = ExtraBossObj_State_WeaponBossMissileFire;
    Model_SetAnimation(r, 0, &data_ov001_0231f068, 0x15);
    ex_fire_missile(p);
    Sound_PlayVoice(0, 0, 0xd2);
}
static void ExtraBossObj_State_WeaponBossMissileWindUp(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    if ((t0->flags[0] & 2) == 0)
        return;
    p0->unk74 = ExtraBossObj_State_WeaponBossMissileReady;
    Model_SetAnimation(t0, 0, &data_ov001_0231f068, 0x14);
}

/* hurt */
static void ExtraBossObj_State_WeaponBossHurtRecover(ExObj *p) {
    p->unk1D0--;
    if (p->unk1D0 <= 0) {
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
        ExtraBoss_WeaponBoss_SelectNextAttack(p);
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316918, 0x02316924, 5);
        p->unk50 = p->unk54 = 0;
        return;
    }
    p->unk1C8 = (s16)(p->unk1C8 + 1);
    if ((p->unk1C8 & 7) == 1)
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316930, 0x02316924, 5);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316918, 0x02316924, 5);
}
static void ExtraBossObj_State_WeaponBossHurt(ExObj *p) {
    BossModel *o = p->unk84;
    s32 x, i;
    p->unk1CC = (s16)(p->unk1CC + 0x43);
    if ((s16)p->unk1CC > 0x1000)
        p->unk1CC = 0x1000;
    if (o->flags[0] & 2) {
        p->unk74 = ExtraBossObj_State_WeaponBossHurtRecover;
        Model_SetAnimation(o, 0, &data_ov001_0231f068, 0);
        o->flags[0] |= 4;
        p->unk50 = FX_DivS32(0x100000 - p->unk18, 0xb4);
        p->unk54 = FX_DivS32(0xc0000 - p->unk1C, 0xb4);
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
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316930, 0x02316924, 5);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316918, 0x02316924, 5);
}

/* defeat */
static void ExtraBossObj_State_WeaponBossDefeatFinish(ExObj *p0) {
    p0->unk14 ^= 0x30000;
    if ((u16)p0->unk1C8 == 0)
        return;
    ExtraBoss_GetWork()->unk0 |= 0x400;
    p0->unk74 = NULL;
}
static void ExtraBossObj_State_WeaponBossDefeatFadeOut(ExObj *p) {
    p->unk14 ^= 0x30000;
    p->unk1C8 -= 1;
    if (p->unk1C8 <= 0) {
        p->unk1C8 = 0;
        BrightnessLerp_Create(0x20, 0, 0x10, 3, &p->unk1C8);
        p->unk74 = ExtraBossObj_State_WeaponBossDefeatFinish;
        return;
    }
    if (p->unk1C8 == 0xc1)
        Sound_PlayVoice(0, 0, 0x2c);
}
static void ExtraBossObj_State_WeaponBossDefeatWaitAnim(ExObj *p) {
    BossModel *q = p->unk84;
    p->unk14 ^= 0x30000;
    p->unk1C8 -= 1;
    if (p->unk1C8 == 0)
        Sound_PlayVoice(0, 0, 0x23);
    if (q->flags[0] & 2) {
        p->unk74 = ExtraBossObj_State_WeaponBossDefeatFadeOut;
        p->unk1C8 = 0xfd;
    }
}
static void ExtraBossObj_State_WeaponBossDefeatPartBlink(ExObj *p) {
    p->unk14 ^= 0x30000;
    p->unk1C8--;
    if (p->unk1C8 > 0)
        return;
    p->unk7C->unk84->speed = 0x2000;
    p->unk84->speed = 0x2000;
}
static ExObj *ex_defeat_part(s32 model, u16 id) {
    ExObj *o = ExtraBossObj_Create(2, id);
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
static void ExtraBoss_WeaponBoss_FinishDefeat(ExObj *w) {
    BossModel *a;
    ExObj *o, *o3;
    ExtraBoss_StartCharacterSwitch();
    BrightnessLerp_Create(0x20, 0x10, 0, 3, NULL);
    ex_set_node_callback(w->unk84, NULL);   /* NNS_G3dRenderObjResetCallBack */
    w->unk18 = w->unk1C = w->unk20 = 0;
    w->unk50 = w->unk54 = w->unk58 = 0;
    w->unk5C = w->unk60 = w->unk64 = 0;
    a = w->unk84;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, 3);
    a->flags[0] &= ~4;
    a->speed = 0x2000;
    w->unk74 = ExtraBossObj_State_WeaponBossDefeatWaitAnim;
    w->unk1C8 = 0xb4;

    o = ex_defeat_part(1, 0x2f01);
    if (o != NULL) {
        o->unk74 = ExtraBossObj_State_Blink2;
        o->unk14 |= 0x30000;
    }
    o = ex_defeat_part(4, 0x2f01);
    if (o != NULL)
        o->unk74 = ExtraBossObj_State_Blink2;
    o3 = ex_defeat_part(2, 0x2f01);
    if (o3 != NULL) {
        a = o3->unk84;
        a->cb[0] = MOP_FLUSHVP;
        ex_set_node_callback(a, ExtraBoss_HandNodeCallback);
        Model_SetAnimation(a, 0, &data_ov001_0231f068, 0x1c);
        a->flags[0] &= ~4;
        a->speed = 0;
        o3->unk74 = NULL;
        o3->unk1C8 = 0x119;
        o3->unk14 |= 0x30000;
    }
    o = ex_defeat_part(3, 0x2f01);
    if (o != NULL) {
        a = o->unk84;
        a->cb[0] = MOP_FLUSHVP;
        Model_SetAnimation(a, 0, &data_ov001_0231f068, 0x35);
        a->flags[0] &= ~4;
        a->speed = 0;
        o->unk74 = ExtraBossObj_State_WeaponBossDefeatPartBlink;
        o->unk1C8 = 0x119;
        o->unk78 = ExtraBossObj_Draw_WeaponBossDebris;
        o->unk7C = o3;
    }
}
static void ExtraBossObj_State_WeaponBossDefeat(ExObj *p) {
    BossModel *o = p->unk84;
    (void)ExtraBoss_GetCurrentCharacterSlot();
    p->unk1C8 = (s16)(p->unk1C8 + 1);
    p->unk1CC = (s16)(p->unk1CC + 0x43);
    if ((s16)p->unk1CC > 0x1000)
        p->unk1CC = 0x1000;
    if ((s16)p->unk1C8 == 0x77)
        BrightnessLerp_Create(0x10, 0, 0x10, 3, NULL);
    if (o->flags[0] & 2) {
        if (ExtraBoss_GetWork()->unk40 == 4) {
            ExtraBoss_GetWork()->unk0 |= 0x100;
            p->unk74 = NULL;
            return;
        }
        ExtraBoss_WeaponBoss_FinishDefeat(p);
        ExtraBoss_GetWork()->unk318 = 0;
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316918, 0x02316924, 5);
        return;
    }
    if ((p->unk1C8 & 7) == 1)
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316930, 0x02316924, 5);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(0x0231d518, 0x02316918, 0x02316924, 5);
}

/* ------------------------------------------------------------- creation */
ExObj *ExtraBoss_CreateWeaponBoss(void) {
    ExObj *o;
    ExBossWork *d;
    ExWork *q;
    BossModel *a;
    s32 r;
    q = ExtraBoss_GetWork();
    o = ExtraBossObj_Create(2, 0x2f00);
    if (o == NULL)
        return NULL;
    data_ov001_0231f06c = o;
    d = (ExBossWork *)calloc(1, sizeof(ExBossWork));
    o->unk1DC = d;
    a = (BossModel *)calloc(1, sizeof(BossModel));
    Model_Init(a, 0);
    Model_SetResource(a, &g_ex_bsex[0]);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x34cc;
    o->unk84 = a;
    Model_SetAnimation(a, 0, &data_ov001_0231f068, 0);
    a->flags[0] |= 4;
    a->speed = 0x1000;
    o->unk14 |= 0x40000;
    ex_set_node_callback(a, ExtraBoss_WeaponNodeCallback);
    a->cb[0] = MOP_FLUSHVP;
    o->unk78 = ExtraBossObj_Draw_WeaponBoss;
    ExtraBoss_WeaponBoss_Action_Enter(o);
    d->unk8 = -1;
    d->unk4 = 4;
    d->unkC = 3;
    o->unk18 = 0x1c0000;
    o->unk1C = ExtraBoss_GetPlayer()->unk1C;
    if (o->unk1C > 0x111000)
        o->unk1C = 0xc0000;
    else if (o->unk1C < 0x65000)
        o->unk1C = 0x65000;
    o->unk20 = 0;
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_B0], o, 4, 0, 0x14000, 0x14000, 0x1e000, -0xa000, 0, 0, 0);
    o->unkB0[EX_COLL_B0].unk4 = ExtraBoss_WeaponBoss_OnHit;
    o->unkB0[EX_COLL_B0].unk8 |= 0x3000;
    o->unk9C[0] = &o->unkB0[EX_COLL_B0];
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_E8], o, 1, 1, 0x1e000, 0x32000, 0x20000, 0, 0, 0, 0);
    o->unkB0[EX_COLL_E8].unk8 |= 0x3000;
    o->unk9C[1] = &o->unkB0[EX_COLL_E8];
    if (q->unk40 == 4) {
        r = Rand() & 0x1fff;
        if (r < 0x1000)
            d->unkC = 1;
        else if (r < 0x1999)
            d->unkC = 2;
        else
            d->unkC = 3;
    }
    g_bossex_stats.boss_spawned[0]++;
    return o;
}

const char *ex_weapon_state_name(ExFn fn) {
#define N(f) if (fn == (ExFn)f) return #f;
    N(ExtraBossObj_State_WeaponBossEnter) N(ExtraBossObj_State_WeaponBossWait) N(ExtraBossObj_State_WeaponBossBeamCharge)
    N(ExtraBossObj_State_WeaponBossBeamFire) N(ExtraBossObj_State_WeaponBossBeamPause) N(ExtraBossObj_State_WeaponBossBeamRecover)
    N(ExtraBossObj_State_WeaponBossBeamRefire) N(ExtraBossObj_State_WeaponBossBeamUnitAim)
    N(ExtraBossObj_State_WeaponBossBeamUnitEnd) N(ExtraBossObj_State_WeaponBossBeamUnitLaunch)
    N(ExtraBossObj_State_WeaponBossBeamUnitResetArm) N(ExtraBossObj_State_WeaponBossBeamUnitTurnArm)
    N(ExtraBossObj_State_WeaponBossBeamUnitWaitClear) N(ExtraBossObj_State_WeaponBossDefeat)
    N(ExtraBossObj_State_WeaponBossDefeatFadeOut) N(ExtraBossObj_State_WeaponBossDefeatFinish)
    N(ExtraBossObj_State_WeaponBossDefeatPartBlink) N(ExtraBossObj_State_WeaponBossDefeatWaitAnim)
    N(ExtraBossObj_State_WeaponBossHurt) N(ExtraBossObj_State_WeaponBossHurtRecover) N(ExtraBossObj_State_WeaponBossMissileEnd)
    N(ExtraBossObj_State_WeaponBossMissileFire) N(ExtraBossObj_State_WeaponBossMissileReady)
    N(ExtraBossObj_State_WeaponBossMissileRecover) N(ExtraBossObj_State_WeaponBossMissileWindUp)
    N(ExtraBossObj_State_WeaponBossSwordEnd) N(ExtraBossObj_State_WeaponBossSwordFollowThrough)
    N(ExtraBossObj_State_WeaponBossSwordNextSwing) N(ExtraBossObj_State_WeaponBossSwordRecover)
    N(ExtraBossObj_State_WeaponBossSwordSlash) N(ExtraBossObj_State_WeaponBossSwordWindUp)
#undef N
    return NULL;
}
