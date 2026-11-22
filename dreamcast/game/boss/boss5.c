/*
 * Boss5 (Huge Crisis act 3, stage z53): the Egg Hammer of boss1.c with the
 * zone-5 variant (gGameState.unk1C == 4: battery turrets that fire missiles).
 * This file is the native translation of src/game/Zone/Huge_Crisis/Boss/:
 * Boss5PartD3 (the missile, map object 0xd3) and BossStage5_* (model load).
 * Boss1_NodeCallback_Batteries and the zone-5 resource set are in boss1.c.
 *
 * Every function keeps its original name and body; the work struct offsets
 * are in the field names.  DS services are replaced as in boss3.c
 * (Task_Create -> a pool of missiles run after the hammer; Model_* -> boss_model.c;
 * the explosion's material / SRT / pattern animations are timing only).
 */
#include <stdio.h>
#include <string.h>
#include "boss_internal.h"

void rect_set_box2d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb); /* ObjRect_SetBox2D */

typedef struct Boss5PartD3 {  /* 0x3ac bytes (Boss5PartD3_Create) */
    DcPlayer h;               /* timer @0x0C = explosion counter */
    BossFn func;              /* @0x68 */
    ObjBox unk134, unk160;    /* @0x134 (unused) / @0x160 explosion box (atk 0x13), flags = unk16A */
    BossModel unk20C;         /* @0x20C missile, then explosion (rot unk230, flags[0] unk318) */
    s32 unk378, unk37C, unk380; /* @0x378 start (fx32 world) */
    s32 unk384, unk388, unk38C; /* @0x384 target */
    s16 unk390;               /* @0x390 speed (progress per frame, 0x1000 = arrived) */
    s16 unk392;               /* @0x392 progress */
    const BossModelRes *unk394; /* archive 0x3d eff_missile.nsbmd */
    const BossModelRes *unk398; /* 0x41 eff_bomb_up.nsbmd */
    const BossAnimRes *unk39C, *unk3A0, *unk3A4, *unk3A8; /* 0x42 .nsbca, 0x43 .nsbma, 0x44 .nsbta, 0x45 .nsbtp */
    int alive;
} Boss5PartD3;

#define B5_MISSILES 8
static Boss5PartD3 s_d3[B5_MISSILES];
static BossModelRes s_missile, s_bomb_up;
static BossAnimRes s_bomb_ca, s_bomb_ma, s_bomb_ta, s_bomb_tp;

static void Boss5PartD3_State_Fly(void *p);
static void Boss5PartD3_State_Explode(void *p);
static void Boss5PartD3_State_Exploding(void *p);

/* Boss5PartD3_Action_Launch (copied) */
void Boss5PartD3_Action_Launch(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s16 g) {
    Boss5PartD3 *s = (Boss5PartD3 *)p;
    s->unk378 = a;
    s->unk37C = b;
    s->unk380 = c;
    s->unk390 = g;
    s->unk384 = d;
    s->unk388 = e;
    s->unk38C = f;
    s->unk390 = g;
    s->unk392 = 0;
    s->func = Boss5PartD3_State_Fly;
    s->func(s);
}

/* Boss5PartD3_State_Fly (copied) */
static void Boss5PartD3_State_Fly(void *p) {
    Boss5PartD3 *s = (Boss5PartD3 *)p;
    u16 ang;
    u16 a2;
    s32 sn;
    s->unk392 += s->unk390;
    if (s->unk392 >= 0x1000) {
        s->unk392 = 0x1000;
        s->func = Boss5PartD3_State_Explode;
    }
    s->h.x = (s->unk378 + FX_Mul(s->unk384 - s->unk378, s->unk392)) >> 4;
    s->h.y = -(s->unk37C + FX_Mul(s->unk388 - s->unk37C, s->unk392)) >> 4;
    s->h.z = (s->unk380 + FX_Mul(s->unk38C - s->unk380, s->unk392)) >> 4;
    ang = (u16)(s->unk392 << 3);
    sn = SIN(ang);
    a2 = ang - 0x4000;
    s->h.y -= (s32)(((s64)sn * 0x12c000 + 0x800) >> 12) >> 4;
    MTX_RotX33(&s->unk20C.rot, SIN(a2), COS(a2)); /* unk230 */
}

/* Boss5PartD3_State_Explode (modified: model release / set-up on the converted resources) */
static void Boss5PartD3_State_Explode(void *p) {
    Boss5PartD3 *s = (Boss5PartD3 *)p;
    BossModel *a = &s->unk20C;
    boss_model_release(a);
    Model_Init(a, 0);
    Model_SetResource(a, s->unk398);
    Model_SetAnimation(a, 0, s->unk39C, 0);
    Model_SetAnimation(a, 1, s->unk3A0, 0);
    Model_SetAnimation(a, 3, s->unk3A4, 0);
    Model_SetAnimation(a, 2, s->unk3A8, 0); /* pattern animation (NNS_G3dGetTex): timing only */
    a->cb[0] = MOP_SETCAMERAROT + 1;        /* unkA = 0x14 */
    a->cb[1] = MOP_FLUSHP;                  /* unkB = 3 */
    a->cb[2] = 0;                           /* unkC */
    a->scale[0] = a->scale[1] = a->scale[2] = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x9d);
    s->h.timer = 0;                         /* unkC */
    s->unk160.flags |= 4;                   /* unk16A */
    s->func = Boss5PartD3_State_Exploding;
    g_boss_stats.missile_explosions++;
}

/* Boss5PartD3_State_Exploding (copied; unk318 = flags[0] of the model) */
static void Boss5PartD3_State_Exploding(void *p) {
    Boss5PartD3 *s = (Boss5PartD3 *)p;
    if (s->h.timer < 0xa) {
        s->h.timer++;
        if (s->h.timer == 0xa)
            s->unk160.flags &= ~4;
    }
    if (s->unk20C.flags[0] & 2)
        s->h.obj_flags |= 4;
}

/* Boss5PartD3_Draw (modified: Model_Draw -> boss_model_pose) */
static void Boss5PartD3_Draw(void *p) {
    Boss5PartD3 *o = (Boss5PartD3 *)p;
    BossModel *m = &o->unk20C;
    m->posed = 0;
    if (o->h.disp_flags & 0x20)
        return;
    m->trans[0] = o->h.x << 4;
    m->trans[1] = -(o->h.y << 4);
    m->trans[2] = o->h.z << 4;
    Model_Animate(m);
    boss_model_pose(m, NULL);
}

/* MapObject_Collide_Default (the explosion box has an owner) */
static void Boss5PartD3_Collide(void *p) {
    Boss5PartD3 *o = (Boss5PartD3 *)p;
    if (o->h.obj_flags & 0xc)
        return;
    if (o->unk160.owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->unk160, 0);
}

/* Boss5PartD3_Destructor (modified) */
static void Boss5PartD3_Destructor(void *p) {
    Boss5PartD3 *s = (Boss5PartD3 *)p;
    boss_model_release(&s->unk20C);
    rect_forget(&s->unk134);
    rect_forget(&s->unk160);
}

/* Boss5PartD3_Create (modified: pool slot; archive files -> converted packages) */
void *Boss5PartD3_Create(const ObjParam *a, s32 x, s32 y) {
    Boss5PartD3 *s = NULL;
    ObjBox *h;
    BossModel *p;
    int i;
    for (i = 0; i < B5_MISSILES; i++)
        if (!s_d3[i].alive) {
            s = &s_d3[i];
            break;
        }
    if (s == NULL)
        return NULL;
    boss_model_release(&s->unk20C);
    memset(s, 0, sizeof(*s)); /* MI_CpuFill16(s, 0, 0x3ac) */
    s->alive = 1;
    s->unk394 = &s_missile;   /* "bs5" archive: 0x3d */
    s->unk398 = &s_bomb_up;   /* 0x41 */
    s->unk39C = &s_bomb_ca;   /* 0x42 */
    s->unk3A0 = &s_bomb_ma;   /* 0x43 */
    s->unk3A4 = &s_bomb_ta;   /* 0x44 */
    s->unk3A8 = &s_bomb_tp;   /* 0x45 */
    /* MapObject_Init */
    s->h.obj_flags |= 0x40;
    s->h.type = (a != NULL && a->type < 0x40) ? 2 : 3;
    s->h.gravity = 0x2a;
    s->h.fall_max = 0xf00;
    s->h.obj_flags |= 1;
    s->h.disp_flags |= 1;
    s->h.move_flags |= 0x200;
    s->unk134.mask = s->unk160.mask = 0x12;
    rect_set_atkdef(&s->unk134, 9, 0xa);
    rect_set_atkdef(&s->unk160, 0x13, 0x13);
    s->unk134.flags &= ~4;
    s->unk160.flags &= ~4;
    s->func = NULL;
    s->h.obj_flags |= 0x10;
    s->h.move_flags |= 0x3300;
    s->h.x = x;
    s->h.y = y;
    s->h.z = 0;
    h = &s->unk134;
    h->flags &= ~4;
    h = &s->unk160;
    rect_set_box2d(h, -0x10, -0x20, 0x10, 0);
    h->owner = s;
    h->owner_kind = OBJ_OWNER_OBJECT;
    h->flags &= ~4;
    p = &s->unk20C;
    Model_Init(p, 0);
    Model_SetResource(p, s->unk394);
    p->trans[0] = s->h.x << 4;
    p->trans[1] = -(s->h.y << 4);
    p->trans[2] = s->h.z << 4;
    p->scale[0] = p->scale[1] = p->scale[2] = 0x34cc;
    g_boss_stats.missiles_fired++;
    return s;
}

static const BossObjOps s_ops_d3 = {Boss5PartD3_Draw, Boss5PartD3_Collide, Boss5PartD3_Destructor};

void boss5_update(void) {
    int i;
    for (i = 0; i < B5_MISSILES; i++)
        if (s_d3[i].alive)
            boss_obj_main(&s_d3[i].h, &s_d3[i].func, &s_ops_d3, &s_d3[i], &s_d3[i].alive);
}

int boss5_draw(int list) {
    int i, n = 0;
    for (i = 0; i < B5_MISSILES; i++)
        if (s_d3[i].alive)
            n += boss_model_draw(&s_d3[i].unk20C, list);
    return n;
}

int boss5_missiles_alive(void) {
    int i, n = 0;
    for (i = 0; i < B5_MISSILES; i++)
        n += s_d3[i].alive;
    return n;
}

void boss5_destroy(void) {
    int i;
    for (i = 0; i < B5_MISSILES; i++)
        if (s_d3[i].alive) {
            Boss5PartD3_Destructor(&s_d3[i]);
            s_d3[i].alive = 0;
        }
}

/* BossStage5_OnModelLoaded is merged into boss1_load (data_02074388 = bs5_base, bs1_neck,
 * bs1_hammer, bs5_sky); here: the missile packages of z53_act_lz7.narc */
int boss5_load(void) {
    int r = 0;
    if (!s_missile.loaded)
        r |= boss_res_load_model(&s_missile, "narc__z53_act_lz7__eff_missile__eff_missile.dcm");
    if (!s_bomb_up.loaded)
        r |= boss_res_load_model(&s_bomb_up, "narc__z53_act_lz7__eff_bomb_up__eff_bomb_up.dcm");
    if (!s_bomb_ca.loaded)
        r |= boss_res_load_anim(&s_bomb_ca, "narc__z53_act_lz7__eff_bomb_up.dca");
    memset(&s_bomb_ma, 0, sizeof(s_bomb_ma));
    memset(&s_bomb_ta, 0, sizeof(s_bomb_ta));
    memset(&s_bomb_tp, 0, sizeof(s_bomb_tp));
    if (boss_anim_lengths("eff_bomb_up.nsbma", &s_bomb_ma.nframes, &s_bomb_ma.count) ||
        boss_anim_lengths("eff_bomb_up.nsbta", &s_bomb_ta.nframes, &s_bomb_ta.count) ||
        boss_anim_lengths("eff_bomb_up.nsbtp", &s_bomb_tp.nframes, &s_bomb_tp.count)) {
        printf("boss: eff_bomb_up timing tables missing in bossdata.bin\n");
        r = -1;
    }
    return r ? -1 : 0;
}

/* BossStage5_FreeModels (+ the missile packages) */
void boss5_unload(void) {
    boss5_destroy();
    boss_res_free_model(&s_missile);
    boss_res_free_model(&s_bomb_up);
    boss_res_free_anim(&s_bomb_ca);
}
