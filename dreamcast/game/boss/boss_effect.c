/*
 * Boss effects: BossEffect_Create (src/asm_fallback/BossEffect_Create.c, read
 * from its ARM listing), BossEffect_State_Main, BossEffect_Destructor, the
 * BossEffect_Create<Name> wrappers Boss1 uses, EffPebble_Create /
 * EffPebble_State_Lifetime, and their StageObj_Draw3D step.
 *
 * BossEffect_Create(name, mask, flags, x, y, z) loads "<name>.nsbmd" from the
 * zone act archive and, for each set bit of mask, the animation
 * <name>{.nsbca,.nsbma,.nsbtp,.nsbta,.nsbva}; .nsbtp is kept for later
 * (@0x2AC) and bound when the 2-frame delay ends.  Only .nsbca (joint
 * animation) is converted for the Dreamcast, so material/pattern/SRT
 * animations show their first state (MODELS.md section 8).  Every converted
 * effect's .nsbca has the same length as its other animations, so the
 * "all animations ended" lifetime test gives the same frame count.
 */
#include <string.h>
#include "boss_internal.h"

#define BEFF_MAX 48 /* effect task pool */
#define BEFF_RES_MAX 24

/* converted resources of the zone act archive (gZoneActArchive), by name;
 * the table (stage, name, .dcm, .dca) is in bossdata.bin (tools/bossdata.py) */
struct BossEffRes {
    char name[24];
    BossModelRes mdl;
    BossAnimRes anim;
};
static BossEffRes s_res[BEFF_RES_MAX];
static int s_nres;
static BossEffect s_eff[BEFF_MAX];
static BossEffect s_dummy; /* Task_Create failed (OS_GetArenaLo): the DS callers write into it anyway */

void boss_effects_init(void) {
    int i;
    for (i = 0; i < BEFF_MAX; i++)
        if (s_eff[i].used)
            boss_model_release(&s_eff[i].unk134);
    memset(s_eff, 0, sizeof(s_eff));
}

/* the DS sets the effect resources up per stage (Boss1_Create: NNS_G3dResDefaultSetup of
 * exc:eff_*.nsbmd); here every effect package of the stage is loaded at boss_init */
int boss_effects_load(int stage) {
    const char *name, *model, *anim;
    int i, r = 0;
    for (i = 0; s_nres < BEFF_RES_MAX && boss_effect_file(stage, i, &name, &model, &anim) == 0; i++) {
        BossEffRes *e = &s_res[s_nres];
        memset(e, 0, sizeof(*e));
        strncpy(e->name, name, sizeof(e->name) - 1);
        r |= boss_res_load_model(&e->mdl, model);
        if (anim)
            r |= boss_res_load_anim(&e->anim, anim);
        s_nres++;
    }
    return r ? -1 : 0;
}

void boss_effects_shutdown(void) {
    int i;
    boss_effects_init();
    for (i = 0; i < s_nres; i++) {
        boss_res_free_model(&s_res[i].mdl);
        boss_res_free_anim(&s_res[i].anim);
    }
    s_nres = 0;
}

static const BossEffRes *find_res(const char *name) {
    int i;
    for (i = 0; i < s_nres; i++)
        if (!strcmp(s_res[i].name, name))
            return &s_res[i];
    return NULL;
}

static BossEffect *eff_alloc(void) {
    int i;
    for (i = 0; i < BEFF_MAX; i++)
        if (!s_eff[i].used) {
            memset(&s_eff[i], 0, sizeof(s_eff[i]));
            s_eff[i].used = 1;
            return &s_eff[i];
        }
    memset(&s_dummy, 0, sizeof(s_dummy));
    boss_log("effect_pool_full", 0, 0);
    return NULL;
}

static void BossEffect_State_Main(void *self);

/* BossEffect_Create (rewritten from the ARM listing; resources by name) */
BossEffect *BossEffect_Create(const char *name, u32 anim_mask, u32 flags, s32 x, s32 y, s32 z) {
    const BossEffRes *res = find_res(name);
    BossEffect *w;
    u32 i;
    if (res == NULL) /* not in this zone's archive (StageObj_LoadModel finds no file) */
        boss_log("effect_missing", 0, x >> 8);
    w = eff_alloc();
    if (w == NULL)
        return &s_dummy;
    w->res = res;
    w->unk2A0 = flags;
    w->h.disp_flags |= 0x30;
    w->unk8 = 2;
    Model_Init(&w->unk134, 0); /* StageObj_LoadModel */
    Model_SetResource(&w->unk134, res && res->mdl.loaded ? &res->mdl : NULL);
    for (i = 0; i < 5; i++) {
        if (anim_mask & (1u << i)) {
            if (i != 2)
                Model_SetAnimation(&w->unk134, i, i == 0 && res && res->anim.loaded ? &res->anim : NULL, 0);
            else
                w->unk2AC = 1;
        }
    }
    w->h.x = x;
    w->h.y = y;
    w->h.z = z;
    w->unk134.scale[0] = w->unk134.scale[1] = w->unk134.scale[2] = 0x34cc;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->state = BossEffect_State_Main;
    g_boss_stats.effects_spawned++;
    boss_log("effect", x >> 8, y >> 8);
    return w;
}

/* BossEffect_State_Main (copied) */
static void BossEffect_State_Main(void *self) {
    BossEffect *w = (BossEffect *)self;
    BossModel *m = &w->unk134;
    u32 i;
    if (w->unk8 == 0) {
        if (w->unk2A4 != NULL)
            w->unk2A4(w);
        if (w->unkC == 0) {
            for (i = 0; i < 5; i++) {
                if (m->anm[i] != NULL && !(m->flags[i] & 2))
                    break;
            }
            if (i == 5)
                w->h.obj_flags |= 4;
        } else if (w->unkC > 0) {
            w->unkC--;
            if (w->unkC == 0)
                w->h.obj_flags |= 4;
        }
    } else {
        w->unk8--;
        if (w->unk8 != 0)
            return;
        /* Model_SetAnimation(m, 2, w->unk2AC, ...): pattern animation not converted */
        w->h.disp_flags &= ~0x30;
    }
}

/* BossEffect_Create<Name> wrappers (copied; argument order flags, x, y, z).  Ops 0x13 / 0x14:
 * Model_MatrixOp_SetCameraRot / SetCameraRotBillboard, then FlushP. */
static BossEffect *with_ops(BossEffect *s, u8 op) {
    s->unk134.cb[0] = op;          /* unk13e */
    s->unk134.cb[1] = MOP_FLUSHP;  /* unk13f = 3 */
    s->unk134.cb[2] = 0;           /* unk140 */
    return s;
}
#define W(N, name, mask) \
    BossEffect *BossEffect_Create##N(s32 a, s32 b, s32 c, s32 d) { return BossEffect_Create(name, mask, (u32)a, b, c, d); }
#define WO(N, name, mask, op) \
    BossEffect *BossEffect_Create##N(s32 a, s32 b, s32 c, s32 d) { return with_ops(BossEffect_Create(name, mask, (u32)a, b, c, d), op); }
W(BaseDest, "eff_base_dest", 1)
W(BeamFinB, "eff_beam_fin_b", 7)
W(Blast, "eff_blast", 7)
W(Blast1, "eff_blast1", 7)
W(Blast3, "eff_blast3", 7)
W(Blast4, "eff_blast4", 7)
W(Blast5, "eff_blast5", 7)
W(BlastF, "eff_blastf", 7)
WO(BombBall, "eff_bomb_ball", 7, 0x13)
W(Boost, "eff_boost", 11)
W(Catch, "eff_catch", 7)
WO(Clash, "eff_clash", 3, 0x13)
WO(DmgHand, "eff_dmg_hand", 3, 0x13)
W(Hit, "eff_hit", 7)
W(Homing, "eff_homing", 3)
W(ImpactBig, "eff_impact_big", 11)
W(ImpactPr, "eff_impact_pr", 11)
W(ImpactSl, "eff_impact_sl", 11)
W(ImpactSmall, "eff_impact_small", 11)
W(ImpactSt, "eff_impact_st", 11)
WO(Jump, "eff_jump", 3, 0x14)
WO(Land, "eff_land", 3, 0x14)
W(LaserFinB, "eff_laser_fin_b", 6)
WO(Launch, "eff_launch", 3, 0x13)
W(Parts, "eff_parts", 3)
W(Pick, "eff_pick", 11)
WO(Pop, "eff_pop", 3, 0x14)
WO(Produce, "eff_produce", 3, 0x13)
WO(ShootBomb, "eff_shoot_bomb", 7, 0x13)
WO(StrikeBig, "eff_strike_big", 3, 0x13)
WO(StrikeFin, "eff_strike_fin", 3, 0x13)
WO(StrikeHit, "eff_strike_hit", 3, 0x13)
W(Suction, "eff_suction", 8)
W(Suction2, "eff_suction2", 10)
W(Tackle, "eff_tackle", 8)
#undef W
#undef WO

/* EffPebble_State_Lifetime (copied) */
static void EffPebble_State_Lifetime(void *self) {
    BossEffect *s = (BossEffect *)self;
    if (s->h.timer-- <= 0)
        s->h.obj_flags |= 4;
}

/* EffPebble_Create (copied; unkC is the StageObj timer @0x0C) */
BossEffect *EffPebble_Create(s32 x, s32 y, s32 z, s16 a, s16 b, s16 c, s32 d) {
    const BossEffRes *res = find_res("eff_pebble"); /* data_ov000_02367c2c, model 0 (eff_pebble01) */
    BossEffect *w = eff_alloc();
    if (w == NULL)
        return &s_dummy;
    w->res = res;
    Model_Init(&w->unk134, 0); /* StageObj_LoadModelSimple: Model_InitShape */
    w->unk134.type = 1;
    Model_SetResource(&w->unk134, res && res->mdl.loaded ? &res->mdl : NULL);
    w->h.timer = d;
    w->h.x = x;
    w->h.y = y;
    w->h.z = z;
    w->h.unk44 = 0x100;
    w->h.unk46 = 0x100;
    w->h.vel_x = a;
    w->h.vel_y = b;
    w->h.vel_z = c;
    w->h.gravity = 0x2a;
    w->h.fall_max = 0xf00;
    w->unk134.scale[0] = w->unk134.scale[1] = w->unk134.scale[2] = 0x34cc;
    w->h.move_flags |= 0x180;
    w->h.obj_flags |= 2;
    w->state = EffPebble_State_Lifetime;
    g_boss_stats.pebbles_spawned++;
    return w;
}

/* StageObj_Draw3D -> StageObj_Draw3DEx (the parts that apply to boss effects:
 * integer-pixel translation, loop flag from disp 4, pause from disp 0x10,
 * hidden with disp 0x20), then Model_Draw */
static void eff_draw(void *self) {
    BossEffect *w = (BossEffect *)self;
    BossModel *m = &w->unk134;
    u16 flags = w->h.disp_flags;
    u32 i;
    m->flag &= ~2u;
    for (i = 0; i < 5; i++)
        m->flags[i] &= ~5;
    if (flags & 4)
        for (i = 0; i < 5; i++)
            m->flags[i] |= 4;
    m->trans[0] = (w->h.x >> 8) << 12;
    m->trans[1] = -(w->h.y >> 8) << 12;
    m->trans[2] = (w->h.z >> 8) << 12;
    if (flags & 0x10)
        m->flag |= 2;
    if (!(flags & 0x1000))
        Model_Animate(m);
    if (flags & 0x20) {
        m->posed = 0;
        return;
    }
    boss_model_pose(m, NULL);
}

static const BossObjOps s_eff_ops = {eff_draw, NULL, NULL};

void boss_effects_update(void) {
    int i;
    for (i = 0; i < BEFF_MAX; i++) {
        BossEffect *w = &s_eff[i];
        int alive = 1;
        if (!w->used)
            continue;
        boss_obj_main(&w->h, &w->state, &s_eff_ops, w, &alive);
        if (!alive) {
            boss_model_release(&w->unk134);
            w->used = 0;
        }
    }
}

int boss_effects_draw(int list) {
    int i, n = 0;
    for (i = 0; i < BEFF_MAX; i++)
        if (s_eff[i].used && s_eff[i].unk134.posed)
            n += boss_model_draw(&s_eff[i].unk134, list);
    return n;
}

int boss_effects_live(void) {
    int i, n = 0;
    for (i = 0; i < BEFF_MAX; i++)
        n += s_eff[i].used;
    return n;
}
