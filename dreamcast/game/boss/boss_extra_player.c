/*
 * Extra Zone boss: the player (Super Sonic / Burning Blaze flight control), ExtraBossPlayer*,
 * ExtraBossPlayerManager, the player's hit callback, the flight effects (eff_son_fly / dash,
 * eff_blz_fly / dash) and the ribbon trail (ExtraBoss_CreatePlayerTrail, ExtraBossObj_Draw_PlayerTrail).
 * Translated from src/game/Zone/Extra_Zone/Boss/ (overlay ov001).
 */
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"

BossModelRes data_ov001_0231f540, data_ov001_0231f538;

ExObj *ExtraBoss_GetPlayer(void) { return gExtraBossPlayerManager->unk4[0]; }

BOOL ExtraBoss_IsPlayerSpawned(void) {
    if (gExtraBossPlayerManager == NULL)
        return FALSE;
    return gExtraBossPlayerManager->unk4[0] != NULL;
}

void ExtraBossPlayerManager_Create(void) { gExtraBossPlayerManager = (ExObjManager *)calloc(1, sizeof(ExObjManager)); }

void ExtraBossPlayerManager_Destroy(void) {
    s32 i;
    if (gExtraBossPlayerManager == NULL)
        return;
    for (i = 0; i < 0x2e; i++) {
        if (gExtraBossPlayerManager->unkBC[i] != NULL)
            ExtraBossObj_Destroy(gExtraBossPlayerManager->unkBC[i]);
    }
    free(gExtraBossPlayerManager);
    gExtraBossPlayerManager = NULL;
}

/* ------------------------------------------------------------ states */
static void ExtraBossPlayer_UpdateInvincibility(ExObj *p) {
    ExPlayerWork *a = (ExPlayerWork *)p->unk1DC;
    if (!(a->unk0 & 2))
        return;
    a->unk10--;
    if (a->unk10 <= 0) {
        a->unk0 &= ~2u;
        p->unk14 &= ~0x20000u;
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
        return;
    }
    if (a->unk10 & 4)
        p->unk14 &= ~0x20000u;
    else
        p->unk14 |= 0x20000;
}

static void ExtraBossPlayer_OnDashHit(ExCollider *pa, ExCollider *pb);
static void ExtraBossPlayer_State_Dash(ExObj *p);

static void ExtraBossPlayer_Action_Dash(ExObj *p) {
    u16 keys = ExtraBoss_GetWork()->unk18;
    u16 ang;
    s32 idx;
    const ExPlayerCfg *cfg = EX_DATA(0x0231da50, const ExPlayerCfg *);
    p->unk14 &= ~0x700u;
    if (ExtraBoss_GetWork()->unk3C == 0)
        Sound_PlaySeqArc(0, 0, 9);
    else
        Sound_PlaySeqArc(0, 0, 0x13);
    if (keys & 0x20) {
        ExtraBossPlayer_ChangeState(p, 4);
        if (keys & 0x40) {
            p->unk40 = 0xe000;
            ang = 0xa000;
        } else if (keys & 0x80) {
            p->unk40 = 0x2000;
            ang = 0x6000;
        } else {
            p->unk40 = 0;
            ang = 0x8000;
        }
    } else {
        ExtraBossPlayer_ChangeState(p, 3);
        if (keys & 0x40) {
            if (keys & 0x10) {
                p->unk40 = 0x2000;
                ang = 0xe000;
            } else {
                p->unk40 = 0x4000;
                ang = 0xc000;
            }
        } else if (keys & 0x80) {
            if (keys & 0x10) {
                p->unk40 = 0xe000;
                ang = 0x2000;
            } else {
                p->unk40 = 0xc000;
                ang = 0x4000;
            }
        } else {
            ang = 0;
            p->unk40 = 0;
        }
    }
    /* data_ov001_0231da50 is read as the first character's entry (the original indexes it without
     * the character: both have the same dash speeds) */
    idx = (s32)(ang & 0xffff) >> 4;
    p->unk50 = FX_Mul(cfg->unk14, COS_IDX(idx));
    p->unk54 = FX_Mul(cfg->unk14, SIN_IDX(idx));
    p->unk5C = FX_Mul(cfg->unk10, COS_IDX(idx));
    p->unk60 = FX_Mul(cfg->unk10, SIN_IDX(idx));
    p->unk14 |= 0x300;
    p->unk1C8 = (s16)p->unk3C;
    ExtraBoss_InitCollider(&p->unkB0[EX_COLL_E8], p, 2, 1, 0x8000, 0x8000, 0x8000, 0, -0x8000, 0, 0);
    p->unkB0[EX_COLL_E8].unk4 = ExtraBossPlayer_OnDashHit;
    p->unk9C[1] = &p->unkB0[EX_COLL_E8];
    p->unk1CC = 2;
    p->unk9C[0] = &p->unkB0[EX_COLL_B0];
    g_bossex_stats.dashes++;
}

static BOOL ExtraBossPlayer_CheckActionInput(ExObj *p) {
    ExPlayerWork *a = (ExPlayerWork *)p->unk1DC;
    u16 held = ExtraBoss_GetWork()->unk18;
    u16 trg = ExtraBoss_GetWork()->unk1C;
    BOOL r = FALSE;
    if (a->unk0 & 4)
        return r;
    if (trg & a->unk8->unk0) {
        ExtraBossPlayer_Action_Dash(p);
        r = TRUE;
    } else if (trg & a->unk8->unk2) {
        ExtraBossPlayer_ChangeState(p, 8);
        r = TRUE;
        p->unk14 &= ~0x700u;
    } else if (!(a->unk4 & 2) && (held & 0xf0)) {
        ExtraBossPlayer_ChangeState(p, 2);
        r = TRUE;
        p->unk14 &= ~0x700u;
    }
    return r;
}

static void ExtraBossPlayer_ApplyMoveInput(ExObj *p) {
    ExPlayerWork *w = (ExPlayerWork *)p->unk1DC;
    u16 held = ExtraBoss_GetWork()->unk18;
    s32 accel = w->unk8->unk4, decel = w->unk8->unk8, max = w->unk8->unkC;
    s32 s;
    p->unk5C = p->unk60 = 0;
    p->unk14 &= ~0x700u;
    if (held & 0x10) {
        p->unk5C = accel;
    } else if (held & 0x20) {
        p->unk5C = -accel;
    } else {
        if (p->unk50 > 0)
            p->unk5C = decel;
        else if (p->unk50 < 0)
            p->unk5C = -decel;
        p->unk14 |= 0x100;
    }
    if (held & 0x40) {
        p->unk60 = -accel;
    } else if (held & 0x80) {
        p->unk60 = accel;
    } else {
        if (p->unk54 > 0)
            p->unk60 = decel;
        else if (p->unk54 < 0)
            p->unk60 = -decel;
        p->unk14 |= 0x200;
    }
    s = p->unk5C + p->unk50;
    if (s > max)
        p->unk5C = max - p->unk50;
    else if (s < -max)
        p->unk5C = -(max + p->unk50);
    s = p->unk60 + p->unk54;
    if (s > max)
        p->unk60 = max - p->unk54;
    else if (s < -max)
        p->unk60 = -(max + p->unk54);
    p->unk3C = (u16)(p->unk54 * 0x1555 / w->unk8->unkC);
}

static void ExtraBossPlayer_ApplyMoveInputHalf(ExObj *p) {
    ExPlayerWork *w = (ExPlayerWork *)p->unk1DC;
    u16 held = ExtraBoss_GetWork()->unk18;
    s32 accel = w->unk8->unk4 >> 1, decel = w->unk8->unk8 >> 1, max = w->unk8->unkC >> 1;
    s32 s;
    p->unk5C = p->unk60 = 0;
    p->unk14 &= ~0x700u;
    if (held & 0x10) {
        p->unk5C = accel;
    } else if (held & 0x20) {
        p->unk5C = -accel;
    } else {
        if (p->unk50 > 0)
            p->unk5C = decel;
        else if (p->unk50 < 0)
            p->unk5C = -decel;
        p->unk14 |= 0x100;
    }
    if (held & 0x40) {
        p->unk60 = -accel;
    } else if (held & 0x80) {
        p->unk60 = accel;
    } else {
        if (p->unk54 > 0)
            p->unk60 = decel;
        else if (p->unk54 < 0)
            p->unk60 = -decel;
        p->unk14 |= 0x200;
    }
    s = p->unk5C + p->unk50;
    if (s > max)
        p->unk5C = max - p->unk50;
    else if (s < -max)
        p->unk5C = -(max + p->unk50);
    s = p->unk60 + p->unk54;
    if (s > max)
        p->unk60 = max - p->unk54;
    else if (s < -max)
        p->unk60 = -(max + p->unk54);
}

static void ExtraBossPlayer_State_None(ExObj *p) { (void)p; }
static void ExtraBossPlayer_State_Idle(ExObj *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    p->unk50 = -0x1000;
    ExtraBossPlayer_CheckActionInput(p);
}
static void ExtraBossPlayer_State_Move(ExObj *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    if (ExtraBossPlayer_CheckActionInput(p))
        return;
    ExtraBossPlayer_ApplyMoveInput(p);
    if (p->unk5C != 0 || p->unk60 != 0 || p->unk50 != 0 || p->unk54 != 0)
        return;
    ExtraBossPlayer_ChangeState(p, 1);
    p->unk14 &= ~0x700u;
}
static void ExtraBossPlayer_State_Dash(ExObj *p) {
    ExPlayerWork *s = (ExPlayerWork *)p->unk1DC;
    s32 half, d, base;
    ExtraBossPlayer_UpdateInvincibility(p);
    if (ExtraBoss_GetWork()->unk3C == 0) {
        p->unk1CC--;
        if (p->unk1CC <= 0)
            p->unk9C[0] = NULL;
    }
    if (p->unk50 == 0 && p->unk54 == 0) {
        if (!ExtraBossPlayer_CheckActionInput(p)) {
            ExtraBossPlayer_ChangeState(p, 1);
            p->unk14 &= ~0x700u;
        }
        p->unk9C[1] = NULL;
        p->unk9C[0] = &p->unkB0[EX_COLL_B0];
        return;
    }
    half = s->unk8->unk14 >> 1;
    d = FX_Sqrt(FX_Mul(p->unk50, p->unk50) + FX_Mul(p->unk54, p->unk54));
    if (half > d) {
        s32 r = (u16)p->unk1C8;
        base = r > 0x8000 ? 0x10000 : 0;
        p->unk3C = (u16)((s32)(r * d) / half + base * (half - d) / half);
    }
    if (FX_Div(d, s->unk8->unk14) >= 0x800)
        return;
    if (s->unk8->unk0 & ExtraBoss_GetWork()->unk1C)
        ExtraBossPlayer_Action_Dash(p);
}
static void ExtraBossPlayer_State_Hurt(ExObj *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    p->unk1C8 -= 1;
    if (p->unk1C8 > 0)
        return;
    p->unk50 = 0;
    if (ExtraBossPlayer_CheckActionInput(p))
        return;
    ExtraBossPlayer_ChangeState(p, 1);
    p->unk14 &= ~0x700u;
}
static void ExtraBossPlayer_State_Defeat(ExObj *p) { (void)p; }
static void ExtraBossPlayer_State_SpecialStart(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    ExtraBossPlayer_UpdateInvincibility(p0);
    if ((t0->flags[0] & 2) == 0)
        return;
    ExtraBossPlayer_ChangeState(p0, 9);
    p0->unk1C8 = 0x3d;
}
static void ExtraBossPlayer_State_SpecialCharge(ExObj *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    p->unk1C8 -= 1;
    if (p->unk1C8 <= 0) {
        ExtraBossPlayer_ChangeState(p, 0xa);
        ExtraBossPlayer_SpawnBlzShotFx(p, p->unk18, p->unk1C, 0);
        ExtraBossPlayer_SpawnSpecialShot(p, p->unk18, p->unk1C, 0);
        Sound_PlaySeqArc(0, 0, 0x12);
    }
    ExtraBossPlayer_ApplyMoveInputHalf(p);
}
static void ExtraBossPlayer_State_SpecialFire(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    ExtraBossPlayer_UpdateInvincibility(p0);
    if ((t0->flags[0] & 2) == 0)
        return;
    ExtraBossPlayer_ChangeState(p0, 0xb);
}
static void ExtraBossPlayer_State_SpecialEnd(ExObj *p) {
    BossModel *q = p->unk84;
    ExtraBossPlayer_UpdateInvincibility(p);
    if (!(q->flags[0] & 2))
        return;
    if (ExtraBossPlayer_CheckActionInput(p))
        return;
    ExtraBossPlayer_ChangeState(p, 1);
    p->unk14 &= ~0x700u;
}
static void ExtraBossPlayer_State_Unk_ov001_02301ba0(ExObj *p) { ExtraBossPlayer_UpdateInvincibility(p); }

/* the player state tables hold ROM function addresses (config/eur/arm9/overlays/ov001/symbols.txt) */
ExFn ex_player_state_fn(u32 a) {
    switch (a) {
    case 0x02301ba0: return ExtraBossPlayer_State_Unk_ov001_02301ba0;
    case 0x02301bac: return ExtraBossPlayer_State_SpecialEnd;
    case 0x02301c10: return ExtraBossPlayer_State_SpecialFire;
    case 0x02301c50: return ExtraBossPlayer_State_SpecialCharge;
    case 0x02301cc8: return ExtraBossPlayer_State_SpecialStart;
    case 0x02301d10: return ExtraBossPlayer_State_Defeat;
    case 0x02301d70: return ExtraBossPlayer_State_Hurt;
    case 0x02301e78: return ExtraBossPlayer_State_Dash;
    case 0x02302298: return ExtraBossPlayer_State_Move;
    case 0x02302320: return ExtraBossPlayer_State_Idle;
    case 0x0230236c: return ExtraBossPlayer_State_None;
    }
    return ExtraBossPlayer_State_None;
}

void ExtraBossPlayer_ChangeState(ExObj *p, s32 n) {
    ExPlayerWork *s = (ExPlayerWork *)p->unk1DC;
    const ExPlayerStateDef *e = &s->unkC[n];
    p->unk84 = e->idx ? p->unk8C : p->unk88;
    if (e->anim0 != 0xffff) {
        Model_SetAnimation(p->unk84, 0, &data_ov001_0231f534, e->anim0);
        p->unk84->flags[0] &= ~4;
        if (e->flags & 0x10)
            p->unk84->flags[0] |= 4;
    }
    if (e->anim1 != 0xffff) {
        Model_SetAnimation(p->unk84, 4, &data_ov001_0231f53c, e->anim1);
        p->unk84->flags[4] &= ~4;
        if (e->flags & 0x10)
            p->unk84->flags[4] |= 4;
    }
    if (e->flags & 1) {
        p->unk50 = e->v8[0];
        p->unk54 = e->v8[1];
        p->unk58 = e->v8[2];
        p->unk5C = e->v14[0];
        p->unk60 = e->v14[1];
        p->unk64 = e->v14[2];
        p->unk68 = (u16)e->a20[0];
        p->unk6A = (u16)e->a20[1];
        p->unk6C = (u16)e->a20[2];
        p->unk6E = (u16)e->a26[0];
        p->unk70 = (u16)e->a26[1];
        p->unk72 = (u16)e->a26[2];
    }
    if (e->flags & 2)
        p->unk3C = 0;
    if (e->flags & 4)
        p->unk3E = 0;
    if (e->flags & 6)
        p->unk40 = 0;
    p->unk74 = ex_player_state_fn(e->fn);
    s->unk4 = n;
}

static void ExtraBossPlayer_OnDashHit(ExCollider *pa, ExCollider *pb) {
    ExObj *a = pa->owner;
    ExObj *b = pb->owner;
    if (a->unk74 != ExtraBossPlayer_State_Dash)
        return;
    a->unk60 = 0;
    a->unk5C = a->unk60;
    a->unk54 = a->unk5C;
    a->unk50 = a->unk54;
    switch (b->unk1C8) {
    case 8:
    case 9:
    case 0xf:
        ExtraBossPlayer_SpawnClashFx(NULL, a->unk18, a->unk1C, a->unk20);
        break;
    }
}

/* ExtraBoss_PlayerOnHurt: the player's body box (B list, group 0) */
static void ExtraBoss_PlayerOnHurt(ExCollider *self, ExCollider *other) {
    ExObj *o = other->owner;
    ExPlayerWork *w = (ExPlayerWork *)o->unk1DC;
    if (self->unk8 & 0x10) {
        ExtraBoss_ScatterPlayerRings(o->unk18, o->unk1C, ExtraBoss_GetWork()->unk40 <= 2 ? 5 : 10);
        Sound_PlaySeqArc(0, 0, 5);
    }
    if (self->unk8 & 0x20) {
        ExtraBossPlayer_ChangeState(o, 0xc);
        return;
    }
    if (self->unk8 & 0x40)
        return;
    other->unk8 |= 0x200;
    o->unk9C[1] = NULL;
    ExtraBossPlayer_ChangeState(o, 5);
    o->unk1C8 = 0x3c;
    w->unk0 |= 2;
    w->unk10 = 0xb4;
    w->unk0 &= ~4u;
    if (ExtraBoss_GetWork()->unk3C == 0)
        Sound_PlayVoice(0, 0, 0x21);
    else
        Sound_PlayVoice(0, 0, 0x2a);
    g_bossex_stats.player_hurts++;
}

void ExtraBossPlayer_Action_Defeat(void) {
    ExObj *t0 = ExtraBoss_GetPlayer();
    ExPlayerWork *t1 = (ExPlayerWork *)t0->unk1DC;
    ExtraBossPlayer_ChangeState(t0, 6);
    t0->unk14 &= 0xfffdf8ffu;
    t1->unk0 &= ~1u;
    t0->unk14 |= 0x1000;
    t0->unk9C[1] = NULL;
}
void ExtraBossPlayer_Action_Hurt(void) {
    ExObj *p = ExtraBoss_GetPlayer();
    ExPlayerWork *a = (ExPlayerWork *)p->unk1DC;
    ExtraBossPlayer_ChangeState(p, 5);
    p->unk9C[0]->unk8 |= 0x200;
    p->unk9C[1] = NULL;
    p->unk1C8 = 0x3c;
    a->unk0 |= 2;
    a->unk10 = 0xb4;
    a->unk0 &= ~4u;
    if (ExtraBoss_GetWork()->unk3C == 0)
        Sound_PlayVoice(0, 0, 0x21);
    else
        Sound_PlayVoice(0, 0, 0x2a);
    g_bossex_stats.player_hurts++;
}
void ExtraBossPlayer_Action_LockControl(void) {
    ExObj *p = ExtraBoss_GetPlayer();
    ExtraBossPlayer_ChangeState(p, 7);
    p->unk14 |= 0x1000;
}
void ExtraBossPlayer_Action_UnlockControl(ExObj *p) {
    ExtraBossPlayer_ChangeState(p, 1);
    p->unk14 &= ~0x1000u;
}

/* ExtraBoss_LoadPlayerModel (modified: the files are the converted packages; the title card's copy
 * of the player model at work + 0x64 belongs to StageTitleCard_CreateExtraBoss and is not kept) */
void ExtraBoss_LoadPlayerModel(s32 idx) {
    ExObj *o;
    BossModel *a;
    ExPlayerWork *d;
    /* File_Load(data_ov001_0231d9f0[idx]) "/mod/e_son.nsbmd" / "/mod/e_blz.nsbmd" and
     * File_Load(data_ov001_0231d9f8[idx]) "/mod/son_easy.nsbmd" / "/mod/blz_easy.nsbmd" */
    boss_res_free_model(&data_ov001_0231f540);
    boss_res_free_model(&data_ov001_0231f538);
    if (boss_res_load_model(&data_ov001_0231f540, ex_model_file(5 + idx)) != 0 ||
        boss_res_load_model(&data_ov001_0231f538, ex_model_file(7 + idx)) != 0)
        boss_log("missing", 0x3000, idx);
    o = ExtraBossObj_Create(0, 0x3000);
    a = (BossModel *)calloc(1, sizeof(BossModel));
    Model_Init(a, 0);
    Model_SetResource(a, &data_ov001_0231f540);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x34cc;
    o->unk84 = a;
    o->unk88 = a;
    a = (BossModel *)calloc(1, sizeof(BossModel));
    Model_Init(a, 0);
    Model_SetResource(a, &data_ov001_0231f538);
    a->scale[0] = a->scale[1] = a->scale[2] = 0x34cc;
    o->unk8C = a;
    o->unk74 = ExtraBossPlayer_State_None;
    o->unk14 |= 0x1b0020;
    d = (ExPlayerWork *)calloc(1, 0x40);
    o->unk1DC = d;
    d->unk8 = EX_DATA(0x0231da50, const ExPlayerCfg *) + idx;
    d->unkC = EX_DATA(EX_S32(0x0231d9e8, idx), const ExPlayerStateDef *);
    ExtraBoss_InitCollider(&o->unkB0[EX_COLL_B0], o, 1, 0, 0x8000, 0x8000, 0x8000, 0, -0x8000, 0, 0);
    o->unkB0[EX_COLL_B0].unk4 = ExtraBoss_PlayerOnHurt;
    o->unkB0[EX_COLL_B0].unk8 |= 0x2000;
    ExtraBossPlayer_ChangeState(o, 1);
    o->unk18 = o->unk24 = 0x40000;
    o->unk1C = o->unk28 = 0xc0000;
    o->unk20 = o->unk2C = 0;
}

/* ------------------------------------------------- clamp to the field, suction */
void ExtraBoss_Unk_ov001_02302658(void) {
    ExWork *q = ExtraBoss_GetWork();
    ExObj *o = ExtraBoss_GetPlayer();
    ExPlayerWork *d = (ExPlayerWork *)o->unk1DC;
    s32 dx, dy, adx, ady, sx, sy;
    if (d->unk0 & 1) {
        if (o->unk18 < q->unk54 + 0x10000)
            o->unk18 = q->unk54 + 0x10000;
        else if (o->unk18 > q->unk58 - 0x10000)
            o->unk18 = q->unk58 - 0x10000;
        if (o->unk1C < q->unk50 + 0x10000)
            o->unk1C = q->unk50 + 0x10000;
        else if (o->unk1C > q->unk5C)
            o->unk1C = q->unk5C;
    }
    if (d->unk4 == 6)
        return;
    if (d->unk0 & 4) {
        o->unk50 = o->unk54 = o->unk5C = o->unk60 = 0;
        o->unk30 = (FX_ModS32(Rand(), 5) - 2) << 12;
        o->unk34 = (FX_ModS32(Rand(), 5) - 2) << 12;
    }
    if (ExtraBoss_GetWork()->unk0 & 0x40) {
        dx = o->unk18 - 0xc0000;
        q = ExtraBoss_GetWork();
        dy = o->unk1C - q->unk310;
        adx = dx < 0 ? -dx : dx;
        ady = dy < 0 ? -dy : dy;
        sx = FX_Div(FX_Mul(adx, 0x3000) + FX_Mul(0xc0000 - adx, 0x5000), 0xc0000);
        sy = FX_Div(FX_Mul(ady, 0x6000) + FX_Mul(0x180000 - ady, 0x6800), 0x180000);
        if (dx < 0) {
            if (adx < sx)
                o->unk18 = 0xc0000;
            else
                o->unk18 += sx;
        } else if (dx > 0) {
            if (adx < sx)
                o->unk18 = 0xc0000;
            else
                o->unk18 -= sx;
        }
        if (dy < 0) {
            if (ady >= sy) {
                o->unk1C += sy;
                return;
            }
            o->unk1C = ExtraBoss_GetWork()->unk310;
            d->unk0 |= 4;
        } else if (dy > 0) {
            if (ady >= sy) {
                o->unk1C -= sy;
                return;
            }
            o->unk1C = ExtraBoss_GetWork()->unk310;
            d->unk0 |= 4;
        }
    } else {
        d->unk0 &= ~4u;
    }
}

/* --------------------------------------------------------- flight effects */
static void ExtraBossObj_State_PlayerFlyFx(ExObj *p);
static void ExtraBossObj_State_PlayerDashFx(ExObj *p);
static void ExtraBossObj_State_PlayerDashFxEnd(ExObj *p);
static void ExtraBossObj_State_PlayerFxHidden(ExObj *p);
static s32 ex_parent_state(const ExObj *p) { return ((const ExPlayerWork *)p->unk7C->unk1DC)->unk4; }

static void ExtraBossObj_State_PlayerFxHidden(ExObj *p) {
    s32 st = ex_parent_state(p);
    s32 a, b;
    if (st == 5 || st == 0xc || st == 6)
        return;
    if (ExtraBoss_GetWork()->unk3C == 0) {
        a = 0x17;
        b = 0x18;
    } else {
        a = 0x19;
        b = 0x1a;
    }
    p->unk14 &= ~0x30000u;
    if (st == 3 || st == 4) {
        p->unk74 = ExtraBossObj_State_PlayerDashFx;
        p->unk1CC = 0;
        ExtraBoss_SwapObjModel(p, b);
        if (ExtraBoss_GetWork()->unk3C == 0) {
            p->unk84->flags[0] &= ~4;
            p->unk84->flags[1] &= ~4;
            p->unk84->flags[3] &= ~4;
        } else {
            p->unk84->flags[0] |= 4;
            p->unk84->flags[1] |= 4;
            p->unk84->flags[3] |= 4;
        }
    } else {
        p->unk74 = ExtraBossObj_State_PlayerFlyFx;
        ExtraBoss_SwapObjModel(p, a);
        p->unk84->flags[0] |= 4;
        p->unk84->flags[1] |= 4;
        p->unk84->flags[3] |= 4;
    }
    ExtraBoss_CopyParentPosition(p);
}
static void ExtraBossObj_State_PlayerFlyFx(ExObj *p) {
    switch (ex_parent_state(p)) {
    case 3:
    case 4:
        p->unk74 = ExtraBossObj_State_PlayerDashFx;
        p->unk1CC = 0;
        ExtraBoss_SwapObjModel(p, ExtraBoss_GetWork()->unk3C == 0 ? 0x18 : 0x1a);
        if (ExtraBoss_GetWork()->unk3C == 0) {
            p->unk84->flags[0] &= ~4;
            p->unk84->flags[1] &= ~4;
            p->unk84->flags[3] &= ~4;
        } else {
            p->unk84->flags[0] |= 4;
            p->unk84->flags[1] |= 4;
            p->unk84->flags[3] |= 4;
        }
        ExtraBoss_CopyParentPosition(p);
        return;
    case 5:
    case 6:
    case 12:
        p->unk74 = ExtraBossObj_State_PlayerFxHidden;
        p->unk14 |= 0x30000;
        return;
    default:
        break;
    }
    ExtraBoss_CopyParentPosition(p);
}
static void ExtraBossObj_State_PlayerDashFxEnd(ExObj *p) {
    if (p->unk84->flags[0] & 2) {
        p->unk74 = ExtraBossObj_State_PlayerFlyFx;
        ExtraBoss_SwapObjModel(p, 0x17);
        p->unk84->flags[0] |= 4;
        p->unk84->flags[1] |= 4;
    }
    switch (ex_parent_state(p)) {
    case 3:
    case 4:
        p->unk74 = ExtraBossObj_State_PlayerDashFx;
        p->unk1CC = 0;
        ExtraBoss_SwapObjModel(p, 0x18);
        p->unk84->flags[0] &= ~4;
        p->unk84->flags[1] &= ~4;
        p->unk84->flags[3] &= ~4;
        ExtraBoss_CopyParentPosition(p);
        return;
    case 5:
    case 6:
    case 12:
        p->unk74 = ExtraBossObj_State_PlayerFxHidden;
        p->unk14 |= 0x30000;
        return;
    default:
        break;
    }
    ExtraBoss_CopyParentPosition(p);
}
static void ExtraBossObj_State_PlayerDashFx(ExObj *p) {
    ExObj *q = p->unk7C;
    BossModel *o = p->unk84;
    s32 st = ((ExPlayerWork *)q->unk1DC)->unk4;
    if (ExtraBoss_GetWork()->unk3C == 0 && p->unk1CC == 0 && (o->flags[0] & 2)) {
        p->unk1CC = 1;
        ExtraBoss_SetObjModelAnims(p, 1);
        o->flags[0] |= 4;
        o->flags[1] |= 4;
    }
    if (st != 3 && st != 4) {
        p->unk40 = 0;
        switch (st) {
        case 5:
        case 6:
        case 0xc:
            p->unk74 = ExtraBossObj_State_PlayerFxHidden;
            p->unk14 |= 0x30000;
            return;
        }
        if (ExtraBoss_GetWork()->unk3C == 0) {
            p->unk74 = ExtraBossObj_State_PlayerDashFxEnd;
            ExtraBoss_SetObjModelAnims(p, 2);
            o->flags[0] &= ~4;
            o->flags[1] &= ~4;
            return;
        }
        p->unk74 = ExtraBossObj_State_PlayerFlyFx;
        ExtraBoss_SwapObjModel(p, 0x19);
        p->unk84->flags[0] |= 4;
        p->unk84->flags[1] |= 4;
        return;
    }
    ExtraBoss_CopyParentPosition(p);
    if (st == 3)
        p->unk40 = q->unk40;
    else
        p->unk40 = (u16)(q->unk40 - 0x8000);
}
static void ExtraBoss_SpawnPlayerFlyFx(ExObj *p) {
    s32 k = 0x17;
    ExObj *t;
    if (ExtraBoss_GetWork()->unk3C == 1)
        k = 0x19;
    t = ExtraBossPlayer_CreateFxObj(k, p, p->unk18, p->unk1C, p->unk20);
    if (t == NULL)
        return;
    t->unk74 = ExtraBossObj_State_PlayerFlyFx;
    t->unk14 |= 0x20;
    t->unk84->flags[0] |= 4;
    t->unk84->flags[1] |= 4;
}

/* --------------------------------------------------------------- trail */
static void ExtraBoss_UpdatePlayerTrailNode(ExTrailNode *out, ExObj *p) {
    MtxFx33 m, r;
    ExTrailWork *s = (ExTrailWork *)p->unk1DC;
    ExObj *o = p->unk7C;
    fx32 a[3], b[3];
    u16 ang;
    s32 w, idx, i;
    ExTrailNode *n;
    w = s->unk0;
    a[0] = 0;
    a[1] = -w / 2;
    a[2] = 0;
    b[0] = 0;
    b[2] = 0;
    b[1] = w / 2;
    ang = FX_Atan2(o->unk1C - o->unk28, o->unk18 - o->unk24);
    MTX_Identity33(&m);
    idx = (s32)(ang & 0xffff) >> 4;
    MTX_RotZ33(&r, SIN_IDX(idx), COS_IDX(idx));
    MTX_Concat33(&m, &r, &m);
    ex_mtx_mult_vec33(a, &m, a);
    out->b[0] = a[0] + (o->unk18 + o->unk30);
    out->b[1] = a[1] + (o->unk1C + o->unk34);
    out->b[2] = o->unk20 + a[2];
    ex_mtx_mult_vec33(b, &m, b);
    out->a[0] = b[0] + (o->unk18 + o->unk30);
    out->a[1] = b[1] + (o->unk1C + o->unk34);
    out->a[2] = o->unk20 + b[2];
    i = 0;
    n = s->unk2A4->next;
    if (s->unk2A8 <= 0)
        return;
    do {
        i++;
        n->a[0] -= 0x6000;
        n->b[0] -= 0x6000;
        n = n->next;
    } while (i < s->unk2A8);
}
static s32 ex_trail_kind(const ExObj *p) { return ((const ExPlayerWork *)p->unk7C->unk1DC)->unk4; }
static void ExtraBossObj_State_PlayerTrailGrow(ExObj *p);
static void ExtraBossObj_State_PlayerTrailHidden(ExObj *p);
static void ExtraBossObj_State_PlayerTrailShrink(ExObj *p) {
    ExTrailWork *a = (ExTrailWork *)p->unk1DC;
    s32 kind = ex_trail_kind(p);
    ExTrailNode *n = a->unk2A4->next;
    a->unk2A4 = n;
    ExtraBoss_UpdatePlayerTrailNode(n, p);
    a->unk2A8--;
    if (a->unk2A8 <= 0) {
        p->unk74 = ExtraBossObj_State_PlayerTrailHidden;
        p->unk14 |= 0x20000;
    }
    if (kind == 5 || kind == 6 || kind == 0xc)
        return;
    p->unk74 = ExtraBossObj_State_PlayerTrailGrow;
    p->unk14 &= ~0x20000u;
    switch (kind) {
    case 3:
    case 4:
        a->unk0 = 0x10000;
        break;
    default:
        a->unk0 = 0xc000;
        break;
    }
}
static void ExtraBossObj_State_PlayerTrailFull(ExObj *p) {
    ExTrailWork *a = (ExTrailWork *)p->unk1DC;
    s32 kind = ex_trail_kind(p);
    ExTrailNode *n = a->unk2A4->next;
    a->unk2A4 = n;
    ExtraBoss_UpdatePlayerTrailNode(n, p);
    switch (kind) {
    case 5:
    case 6:
    case 12:
        p->unk74 = ExtraBossObj_State_PlayerTrailShrink;
        a->unk0 = 0xa000;
        break;
    case 3:
    case 4:
        a->unk0 = 0x10000;
        break;
    default:
        a->unk0 = 0xc000;
        break;
    }
}
static void ExtraBossObj_State_PlayerTrailGrow(ExObj *p) {
    ExTrailWork *a = (ExTrailWork *)p->unk1DC;
    s32 kind = ex_trail_kind(p);
    ExTrailNode *n = a->unk2A4->next;
    a->unk2A4 = n;
    ExtraBoss_UpdatePlayerTrailNode(n, p);
    a->unk2A8++;
    if (a->unk2A8 >= a->unk2AC) {
        a->unk2A8 = a->unk2AC;
        p->unk74 = ExtraBossObj_State_PlayerTrailFull;
    }
    switch (kind) {
    case 5:
    case 6:
    case 12:
        p->unk74 = ExtraBossObj_State_PlayerTrailShrink;
        a->unk0 = 0xa000;
        break;
    case 3:
    case 4:
        a->unk0 = 0x10000;
        break;
    default:
        a->unk0 = 0xc000;
        break;
    }
}
static void ExtraBossObj_State_PlayerTrailHidden(ExObj *p) {
    ExTrailWork *a = (ExTrailWork *)p->unk1DC;
    s32 kind = ex_trail_kind(p);
    ExTrailNode *n = a->unk2A4->next;
    a->unk2A4 = n;
    ExtraBoss_UpdatePlayerTrailNode(n, p);
    if (kind == 5 || kind == 6 || kind == 0xc)
        return;
    p->unk74 = ExtraBossObj_State_PlayerTrailGrow;
    a->unk2A8 = 0;
    p->unk14 &= ~0x20000u;
    switch (kind) {
    case 3:
    case 4:
        a->unk0 = 0x10000;
        break;
    default:
        a->unk0 = 0xc000;
        break;
    }
}
/* ExtraBossObj_Draw_PlayerTrail (modified: the GE vertex commands become a queued strip, see
 * ex_draw_trail in boss_extra_draw.c) */
static void ExtraBossObj_Draw_PlayerTrail(ExObj *p) { ex_draw_trail(p); }

static void ExtraBoss_CreatePlayerTrail(ExObj *arg) {
    ExObj *p;
    ExTrailWork *b;
    s32 i;
    p = ExtraBossObj_Create(1, 0x2f80);
    if (p == NULL)
        return;
    b = (ExTrailWork *)calloc(1, sizeof(ExTrailWork));
    p->unk1DC = b;
    p->unk7C = arg;
    for (i = 0; i < 0x14; i++)
        b->nodes[i].next = &b->nodes[i + 1];
    b->nodes[0x14].next = &b->nodes[0];
    for (i = 0x14; i > 0; i--)
        b->nodes[i].prev = &b->nodes[i - 1];
    b->nodes[0].prev = &b->nodes[0x14];
    b->unk2A4 = &b->nodes[0];
    b->unk0 = 0xc000;
    b->unk2AC = 0x14;
    if (ExtraBoss_GetWork()->unk3C == 0) {
        b->unk2B0 = 0x3ff;
        b->unk2B2 = 0x17ff;
    } else {
        b->unk2B0 = 0x14bf;
        b->unk2B2 = 0x14bf;
    }
    b->unk2B4 = 5;
    b->unk2B8 = 0x12;
    b->unk2BC = 1;
    b->unk2C0 = 0x14;
    b->unk2C4 = 5;
    ExtraBoss_UpdatePlayerTrailNode(&b->nodes[0], p);
    p->unk74 = ExtraBossObj_State_PlayerTrailGrow;
    p->unk78 = ExtraBossObj_Draw_PlayerTrail;
    p->unk30 = 0x2000;
}

void ExtraBoss_ResetPlayerForBattle(void) {
    ExObj *p = ExtraBoss_GetPlayer();
    ExPlayerWork *a = (ExPlayerWork *)p->unk1DC;
    ExtraBossPlayer_Action_UnlockControl(p);
    p->unk14 &= 0xffe4efffu;
    a->unk0 |= 1;
    p->unk24 = 0x40000;
    p->unk18 = p->unk24;
    p->unk28 = 0xc0000;
    p->unk1C = p->unk28;
    p->unk2C = 0;
    p->unk20 = p->unk2C;
    p->unk9C[0] = &p->unkB0[EX_COLL_B0];
    ExtraBoss_SpawnPlayerFlyFx(p);
    ExtraBoss_CreatePlayerTrail(p);
}
