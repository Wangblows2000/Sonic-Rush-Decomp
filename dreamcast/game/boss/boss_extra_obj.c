/*
 * Extra Zone boss: the object system (ExtraBossObj_*), hit boxes (ExtraBoss_InitCollider,
 * ExtraBoss_CheckCollisionGroup, ExtraBoss_TestCollider*), and the effects / shots
 * (ExtraBossPlayer_CreateFxObj and the ExtraBoss_Spawn* objects with their states and hit callbacks).
 * Translated from src/game/Zone/Extra_Zone/Boss/ (overlay ov001); ExtraBoss_TestColliderRotatedBoxes
 * is rewritten from the ARM listing in src/asm_fallback/.
 */
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"

ExObjManager *gExtraBossPlayerManager;
u8 *data_ov001_0231f094;

/* ---------------------------------------------------------------- tasks */
#define EX_MAX_TASKS 96
static ExTask s_tasks[EX_MAX_TASKS];
static u32 s_task_seq;
ExTask *gTaskCurrent;

ExTask *ex_task_create(ExTaskFn fn, u16 id, void *work) {
    int i;
    for (i = 0; i < EX_MAX_TASKS; i++)
        if (!s_tasks[i].used) {
            ExTask *t = &s_tasks[i];
            memset(t, 0, sizeof(*t));
            t->used = 1;
            t->fn = fn;
            t->id = id;
            t->work = work;
            t->seq = ++s_task_seq;
            return t;
        }
    return NULL;
}
void ex_task_destroy(ExTask *t) {
    if (t != NULL)
        t->dead = 1;
}
static void ex_obj_reap(void *work);
static void ex_obj_reap_all(void);
void ex_tasks_clear(void) {
    ex_obj_reap_all();
    memset(s_tasks, 0, sizeof(s_tasks));
    s_task_seq = 0;
    gTaskCurrent = NULL;
}
/* Task manager main loop: every task once, ascending (id, creation); tasks created during the frame
 * run in the same frame when they sort after the current one (they are linked into the list). */
void ex_tasks_run(void) {
    u32 last_id = 0, last_seq = 0;
    int first = 1;
    for (;;) {
        ExTask *best = NULL;
        int i;
        for (i = 0; i < EX_MAX_TASKS; i++) {
            ExTask *t = &s_tasks[i];
            if (!t->used || t->dead)
                continue;
            if (!first && (t->id < last_id || (t->id == last_id && t->seq <= last_seq)))
                continue;
            if (best == NULL || t->id < best->id || (t->id == best->id && t->seq < best->seq))
                best = t;
        }
        if (best == NULL)
            break;
        first = 0;
        last_id = best->id;
        last_seq = best->seq;
        gTaskCurrent = best;
        if (best->fn != NULL)
            best->fn();
    }
    gTaskCurrent = NULL;
    {
        int i;
        /* Task_Destroy frees the task (and its work: the object) after the frame, as the DS does; until
         * then a destroyed object keeps its contents (hit boxes already in the lists still read it) */
        for (i = 0; i < EX_MAX_TASKS; i++)
            if (s_tasks[i].dead) {
                ex_obj_reap(s_tasks[i].work);
                memset(&s_tasks[i], 0, sizeof(s_tasks[i]));
            }
    }
}

/* ------------------------------------------------------------ objects */
static ExObj s_objs[0x2e];
static void ex_obj_purge_colliders(const ExObj *w);
static void ex_obj_reap(void *work) {
    ExObj *w = (ExObj *)work;
    if (w >= s_objs && w < s_objs + 0x2e) {
        ex_obj_purge_colliders(w);
        memset(w, 0, sizeof(*w));
    }
}
static void ex_obj_reap_all(void) {
    memset(s_objs, 0, sizeof(s_objs));
    ExtraBoss_ClearColliderLists();
}

static void ex_model_free(BossModel *m) {
    if (m == NULL)
        return;
    boss_model_release(m);   /* Model_Release */
    free(m);                 /* Heap_Free */
}

/* ExtraBossObj_Create (modified: object from a static pool instead of the task work) */
ExObj *ExtraBossObj_Create(s32 kind, u16 id) {
    ExObj *w = NULL;
    ExObj **slot;
    s32 n, i;
    ExTask *t;
    for (i = 0; i < 0x2e; i++)
        if (!s_objs[i].used) {
            w = &s_objs[i];
            break;
        }
    if (w == NULL)
        return NULL;
    t = ex_task_create(ExtraBossObj_Main, id, w);
    if (t == NULL)
        return NULL;
    memset(w, 0, sizeof(*w));
    switch (kind) {
    case 0:
        n = 1;
        slot = gExtraBossPlayerManager->unk4;
        break;
    case 1:
        n = 0x1e;
        slot = gExtraBossPlayerManager->unk44;
        break;
    case 2:
        n = 5;
        slot = gExtraBossPlayerManager->unk8;
        break;
    default:
        n = 10;
        slot = gExtraBossPlayerManager->unk1C;
        break;
    }
    for (i = 0; i < n; i++, slot++) {
        if (*slot == NULL)
            break;
    }
    if (i >= n) {
        ex_task_destroy(t);
        return NULL;
    }
    *slot = w;
    w->unkA = (u16)i;
    for (i = 0; i < 0x2e; i++) {
        if (gExtraBossPlayerManager->unkBC[i] == NULL)
            break;
    }
    gExtraBossPlayerManager->unkBC[i] = w;
    w->used = 1;
    w->unk0 = t;
    w->unk8 = (u16)i;
    w->unk4 = kind;
    w->unk4C = 0x1000;
    w->unk48 = w->unk4C;
    w->unk44 = w->unk48;
    return w;
}

BOOL ExtraBossObj_IsAlive(const ExObj *p) { return p != NULL && !(p->unk14 & 8); }

/* ExtraBossObj_RegisterColliders */
static ExCollider *data_ov001_0231f430[3][20], *data_ov001_0231f340[3][20];
static s32 data_ov001_0231f334[3], data_ov001_0231f328[3];

/* native: a released object leaves the hit-box lists.  The lists are rebuilt every frame by the state
 * task, but on the first frame of a round (ExtraBoss_StartBattle runs from the title card, after the
 * 0x1000 task) PostUpdate checks the lists of the previous frame; the DS reads the freed objects there. */
static void ex_obj_purge_colliders(const ExObj *w) {
    s32 k, i, n;
    for (k = 0; k < 3; k++) {
        for (i = n = 0; i < data_ov001_0231f334[k]; i++)
            if (data_ov001_0231f430[k][i]->owner != w)
                data_ov001_0231f430[k][n++] = data_ov001_0231f430[k][i];
        data_ov001_0231f334[k] = n;
        for (i = n = 0; i < data_ov001_0231f328[k]; i++)
            if (data_ov001_0231f340[k][i]->owner != w)
                data_ov001_0231f340[k][n++] = data_ov001_0231f340[k][i];
        data_ov001_0231f328[k] = n;
    }
}

static void ExtraBossObj_RegisterColliders(ExCollider *p) {
    ExObj *o = p->owner;
    s32 i;
    u32 bit;
    if (p->unk8 & 0x100) {
        for (i = 0, bit = 1; i < 3; i++, bit <<= 1) {
            if (p->unk8 & bit) {
                if (!(p->unk8 & 0x1000)) {
                    p->unk28[0] = o->unk18;
                    p->unk28[1] = o->unk1C;
                    p->unk28[2] = o->unk20;
                }
                if (!(p->unk8 & 0x2000))
                    p->unk34 = (u16)(0x10000 - o->unk40);
                if (data_ov001_0231f334[i] < 20) {
                    data_ov001_0231f430[i][data_ov001_0231f334[i]] = p;
                    data_ov001_0231f334[i]++;
                }
            }
        }
    } else {
        for (i = 0, bit = 1; i < 3; i++, bit <<= 1) {
            if (p->unk8 & bit) {
                if (!(p->unk8 & 0x1000)) {
                    p->unk28[0] = o->unk18;
                    p->unk28[1] = o->unk1C;
                    p->unk28[2] = o->unk20;
                }
                if (!(p->unk8 & 0x2000))
                    p->unk34 = (u16)(0x10000 - o->unk40);
                if (data_ov001_0231f328[i] < 20) {
                    data_ov001_0231f340[i][data_ov001_0231f328[i]] = p;
                    data_ov001_0231f328[i]++;
                }
            }
        }
    }
}

/* ExtraBossObj_DrawSprite (no Extra boss object sets a sprite; kept for completeness) */
static void ExtraBossObj_DrawSprite(ExObj *p, ExSprite *o) {
    o->x = (s16)((data_ov001_0231f524 + (p->unk18 + p->unk30)) >> 12);
    o->y = (s16)((data_ov001_0231f520 + (p->unk1C + p->unk34)) >> 12);
    o->flags &= ~0x180u;
    if (!(p->unk14 & 0x40))
        o->flags |= 0x80;
    if (!(p->unk14 & 0x10000))
        ex_sprite_process(o);
    if (p->unk44 != 0x1000 || p->unk48 != 0x1000 || p->unk40 != 0) {
        ex_sprite_draw_rs(o, p->unk44, p->unk48, p->unk40);
        return;
    }
    ex_sprite_draw(o);
}

/* ExtraBossObj_DrawModel (modified: Model_Draw poses the joints and queues the model; the
 * G3i_FrustumW_ of camera flag 1 becomes the queued off-axis frustum) */
void ExtraBossObj_DrawModel(ExObj *w, BossModel *a) {
    MtxFx33 m;
    u32 fl = w->unk14;
    u16 ang;
    BOOL cam;
    float fr[4];
    ang = 0;
    if (fl & 0x20) {
        if (fl & 0x40)
            ang = 0xc000;
        else
            ang = 0x4000;
    }
    a->trans[0] = data_ov001_0231f524 + (w->unk18 + w->unk30);
    a->trans[1] = -(data_ov001_0231f520 + (w->unk1C + w->unk34));
    a->trans[2] = w->unk20 + w->unk38;
    MTX_RotY33(&a->rot, SIN(ang), COS(ang));
    MTX_RotX33(&m, SIN(w->unk3C), COS(w->unk3C));
    MTX_Concat33(&a->rot, &m, &a->rot);
    MTX_RotY33(&m, SIN(w->unk3E), COS(w->unk3E));
    MTX_Concat33(&a->rot, &m, &a->rot);
    MTX_RotZ33(&m, SIN(w->unk40), COS(w->unk40));
    MTX_Concat33(&a->rot, &m, &a->rot);
    cam = fl & 1;
    if (cam) {
        ExCamSlot *c = ExtraBoss_GetCurrentCharacterSlot();
        s32 x = FX_Mul(FX_Div(SIN(c->unk0), COS(c->unk0)), c->unk4);
        s32 y = FX_Mul(x, c->unkC);
        s32 d = FX_Div(c->unk4, c->unk14[2]);
        s32 dy = FX_Mul(d, c->unk14[1] - a->trans[1]);
        s32 dx = FX_Mul(d, c->unk14[0] - a->trans[0]);
        a->trans[0] = c->unk14[0];
        a->trans[1] = c->unk14[1];
        fr[0] = (x + dy) / 4096.0f;   /* G3i_FrustumW_(t, b, l, r, n, f) */
        fr[1] = (dy - x) / 4096.0f;
        fr[2] = (dx - y) / 4096.0f;
        fr[3] = (y + dx) / 4096.0f;
    }
    if (!(fl & 0x10000))
        Model_Animate(a);
    ex_draw_model(a, cam, cam ? fr : NULL);
}

void ExtraBossObj_Draw_Default(ExObj *p) {
    if (p->unk94 != NULL)
        ExtraBossObj_DrawModel(p, p->unk94);
    else if (p->unk84 != NULL)
        ExtraBossObj_DrawModel(p, p->unk84);
    else if (p->unk90 != NULL)
        ExtraBossObj_DrawSprite(p, (ExSprite *)p->unk90);
}

void ExtraBossObj_Main(void) {
    ExObj *p = (ExObj *)gTaskCurrent->work;
    s32 i;
    if (p->unk74 != NULL) {
        p->unk74(p);
        if (!ExtraBossObj_IsAlive(p))
            return;
    }
    if ((p->unk14 & 0x40000) && !(p->unk14 & 0x20000)) {
        if (p->unk78 != NULL)
            p->unk78(p);
        else
            ExtraBossObj_Draw_Default(p);
    }
    if (p->unk14 & 0x1000)
        return;
    for (i = 0; i < 5; i++) {
        if (p->unk9C[i] != NULL)
            ExtraBossObj_RegisterColliders(p->unk9C[i]);
    }
}

void ExtraBossObj_DrawAll(void) {
    s32 i;
    for (i = 0; i < 0x2e; i++) {
        ExObj *o = gExtraBossPlayerManager->unkBC[i];
        if (o != NULL && !(o->unk14 & 0x20000) && !(o->unk14 & 0x40000)) {
            if (o->unk78 != NULL)
                o->unk78(o);
            else
                ExtraBossObj_Draw_Default(o);
        }
    }
}

/* ExtraBossObj_ReleasePlayerModel: NNS_G3dResDefaultRelease + Heap_Free of the e_son / son_easy files */
static void ExtraBossObj_ReleasePlayerModel(void) {
    boss_res_free_model(&data_ov001_0231f540);
    boss_res_free_model(&data_ov001_0231f538);
}

/* ExtraBossObj_ReleaseFxModelRes (modified: the converted resources stay loaded; the DS releases the
 * texture VRAM of a per-object setup, flags & 1 clear) */
void ExtraBossObj_ReleaseFxModelRes(ExObj *p) {
    s32 t0 = p->unk1C8;
    if (gExtraBossFxRes[t0].flags & 1)
        return;
    /* NNS_G3dResDefaultRelease(gExtraBossFxRes[t0].res): nothing to do natively */
}
static void ExtraBossObj_ReleaseFxModelResThunk(ExObj *p) { ExtraBossObj_ReleaseFxModelRes(p); }

/* ExtraBossObj_Destroy (modified: models freed with Model_Release + free, the task marked dead) */
void ExtraBossObj_Destroy(ExObj *w) {
    ExTask *task = w->unk0;
    switch (w->unk4) {
    case 0:
        gExtraBossPlayerManager->unk4[w->unkA] = NULL;
        ExtraBossObj_ReleasePlayerModel();
        break;
    case 1:
        gExtraBossPlayerManager->unk44[w->unkA] = NULL;
        ExtraBossObj_ReleaseFxModelResThunk(w);
        break;
    case 2:
        gExtraBossPlayerManager->unk8[w->unkA] = NULL;
        ExtraBoss_GetWork()->unk304 = 0xffff;
        break;
    case 3:
        gExtraBossPlayerManager->unk1C[w->unkA] = NULL;
        break;
    }
    gExtraBossPlayerManager->unkBC[w->unk8] = NULL;
    /* unk90 (sprite) / unk94 (shape) are never set by the fight */
    if (w->unk84 != NULL) {
        if (w->unk88 != NULL) {
            ex_model_free(w->unk88);
            ex_model_free(w->unk8C);
        } else {
            ex_model_free(w->unk84);
        }
    }
    free(w->unk1DC);
    w->unk1DC = NULL;
    w->unk84 = w->unk88 = w->unk8C = NULL;
    ex_task_destroy(task);   /* the object is released with its task after the frame (ex_tasks_run) */
}

void ExtraBossObj_DestroyFlagged(void) {
    s32 i;
    for (i = 0; i < 0x2e; i++) {
        if (gExtraBossPlayerManager->unkBC[i] != NULL && (gExtraBossPlayerManager->unkBC[i]->unk14 & 8))
            ExtraBossObj_Destroy(gExtraBossPlayerManager->unkBC[i]);
    }
}
void ExtraBossObj_DestroyKind1(void) {
    s32 i;
    for (i = 0; i < 0x1e; i++) {
        if (gExtraBossPlayerManager->unk44[i] != NULL)
            ExtraBossObj_Destroy(gExtraBossPlayerManager->unk44[i]);
    }
}
void ExtraBossObj_DestroyKind2(void) {
    s32 i;
    for (i = 0; i < 5; i++) {
        if (gExtraBossPlayerManager->unk8[i] != NULL)
            ExtraBossObj_Destroy(gExtraBossPlayerManager->unk8[i]);
    }
}
void ExtraBossObj_DestroyKind2And3(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (gExtraBossPlayerManager->unk1C[i] != NULL)
            ExtraBossObj_Destroy(gExtraBossPlayerManager->unk1C[i]);
    }
    for (i = 0; i < 5; i++) {
        if (gExtraBossPlayerManager->unk8[i] != NULL)
            ExtraBossObj_Destroy(gExtraBossPlayerManager->unk8[i]);
    }
}
void ExtraBossObj_DestroyPlayer(void) {
    ExObj *t0 = gExtraBossPlayerManager->unk4[0];
    if (t0 == NULL)
        return;
    ExtraBossObj_Destroy(t0);
}

void ExtraBossObj_UpdateMotionAll(void) {
    s32 i, v, s;
    ExObj *o;
    for (i = 0; i < 0x2e; i++) {
        o = gExtraBossPlayerManager->unkBC[i];
        if (!ExtraBossObj_IsAlive(o))
            continue;
        if (o->unk14 & 0x80000)
            continue;
        o->unk24 = o->unk18;
        o->unk28 = o->unk1C;
        o->unk2C = o->unk20;
        if (!(o->unk14 & 0x200000)) {
            if (o->unk14 & 0x100) {
                v = o->unk50;
                s = v + o->unk5C;
                if ((s >= 0 && v < 0) || (s <= 0 && v > 0) || s == 0) {
                    o->unk50 = 0;
                    o->unk5C = 0;
                    o->unk14 &= ~0x100u;
                }
            }
            if (o->unk14 & 0x200) {
                v = o->unk54;
                s = v + o->unk60;
                if ((s >= 0 && v < 0) || (s <= 0 && v > 0) || s == 0) {
                    o->unk54 = 0;
                    o->unk60 = 0;
                    o->unk14 &= ~0x200u;
                }
            }
            if (o->unk14 & 0x400) {
                v = o->unk58;
                s = v + o->unk64;
                if ((s >= 0 && v < 0) || (s <= 0 && v > 0) || s == 0) {
                    o->unk58 = 0;
                    o->unk64 = 0;
                    o->unk14 &= ~0x400u;
                }
            }
            o->unk50 += o->unk5C;
            o->unk54 += o->unk60;
            o->unk58 += o->unk64;
            o->unk18 += o->unk50;
            o->unk1C += o->unk54;
            o->unk20 += o->unk58;
        }
        if (!(o->unk14 & 0x400000)) {
            o->unk68 += o->unk6E;
            o->unk6A += o->unk70;
            o->unk6C += o->unk72;
            o->unk3C += o->unk68;
            o->unk3E += o->unk6A;
            o->unk40 += o->unk6C;
        }
    }
}

void ExtraBossObj_PropagateParentDestroy(void) {
    s32 i;
    for (i = 0; i < 0x2e; i++) {
        ExObj *o = gExtraBossPlayerManager->unkBC[i];
        if (ExtraBossObj_IsAlive(o) && !(o->unk14 & 0x100000) && (o->unk14 & 0x10)) {
            ExObj *par;
            BOOL kill = FALSE;
            for (par = o->unk7C; par != NULL; par = par->unk7C) {
                if (par->unk14 & 8) {
                    kill = TRUE;
                    break;
                }
                if (!(par->unk14 & 0x10))
                    break;
            }
            if (kill)
                o->unk14 |= 8;
        }
    }
}

BOOL ExtraBossObj_IsOutOfArena(s32 x, s32 y, s32 w, s32 h) {
    if (x - w < -0x40000 || 0x1c0000 < x + w || y - h < -0x40000 || 0x1c0000 < y + h)
        return 1;
    return 0;
}

/* --------------------------------------------------------- generic states */
void ExtraBossObj_State_None(ExObj *p) { (void)p; }
void ExtraBossObj_State_Blink(ExObj *p) { p->unk14 ^= 0x30000; }
void ExtraBossObj_State_Blink2(ExObj *p) { p->unk14 ^= 0x30000; }
void ExtraBossObj_State_DestroyAfterTimer(ExObj *p) {
    p->unk1CC -= 1;
    if (p->unk1CC <= 0)
        p->unk14 |= 8;
}
void ExtraBossObj_State_DestroyWhenAnimsDone(ExObj *p) {
    s32 i;
    BossModel *o = p->unk84;
    for (i = 0; i < 5; i++) {
        if (o->anm[i] != NULL && !(o->flags[i] & 2))
            break;
    }
    if (i == 5)
        p->unk14 |= 8;
}
void ExtraBossObj_State_DestroyWhenOutOfArena(ExObj *p) {
    if (ExtraBossObj_IsOutOfArena(p->unk18, p->unk1C, 0x30000, 0x30000))
        p->unk14 |= 8;
}

/* ------------------------------------------------------------- hit boxes */
void ExtraBoss_InitCollider(ExCollider *c, ExObj *owner, u32 mask, s32 mode, s32 a, s32 b, s32 d, s32 e, s32 f,
                            s32 g, u16 h) {
    c->owner = owner;
    c->unk8 = (mask & 7) | (mode == 1 ? 0x100 : 0);
    c->unk18[0] = a;
    c->unk18[1] = b;
    c->unk18[2] = d;
    c->unkC[0] = e;
    c->unkC[1] = f;
    c->unkC[2] = g;
    c->unk24 = h;
}

void ExtraBoss_ClearColliderLists(void) {
    s32 i;
    for (i = 0; i < 3; i++) {
        data_ov001_0231f334[i] = 0;
        data_ov001_0231f328[i] = 0;
    }
}

static BOOL ExtraBoss_TestColliderBoxes(const ExCollider *a, const ExCollider *b) {
    s32 aminx, amaxx, amaxy, aminy, bminx, bminy, bmaxx, bmaxy, aminz, amaxz, bminz, bmaxz;
    aminz = a->unk28[2] + (a->unkC[2] - a->unk18[2]);
    amaxz = a->unk28[2] + (a->unkC[2] + a->unk18[2]);
    bminz = b->unk28[2] + (b->unkC[2] - b->unk18[2]);
    bmaxz = b->unk28[2] + (b->unkC[2] + b->unk18[2]);
    if (aminz < bminz && amaxz < bminz)
        return FALSE;
    if (bminz < aminz && bmaxz < aminz)
        return FALSE;
    aminx = a->unk28[0] + (a->unkC[0] - a->unk18[0]);
    aminy = a->unk28[1] + (a->unkC[1] - a->unk18[1]);
    amaxx = a->unk28[0] + (a->unkC[0] + a->unk18[0]);
    bminx = b->unk28[0] + (b->unkC[0] - b->unk18[0]);
    amaxy = a->unk28[1] + (a->unkC[1] + a->unk18[1]);
    bminy = b->unk28[1] + (b->unkC[1] - b->unk18[1]);
    bmaxx = b->unk28[0] + (b->unkC[0] + b->unk18[0]);
    bmaxy = b->unk28[1] + (b->unkC[1] + b->unk18[1]);
    if (aminx < bminx && amaxx < bminx)
        return FALSE;
    if (bminx < aminx && bmaxx < aminx)
        return FALSE;
    if (aminy < bminy && amaxy < bminy)
        return FALSE;
    if (bminy < aminy && bmaxy < aminy)
        return FALSE;
    return TRUE;
}

/* ExtraBoss_TestColliderRotatedBoxes (substantially rewritten from the ARM listing in
 * src/asm_fallback/ExtraBoss_TestColliderRotatedBoxes.c; every multiply is the listing's
 * smull + 0x800 >> 12, i.e. FX_Mul).  Steps:
 *   1. z ranges overlap;
 *   2. bounding circles: |dx| and |dy| of the centres <= ra + rb, r = FX_Sqrt(hx^2 + hy^2);
 *   3. the corners of both boxes rotated by rot + owner rot (and, when the owner rotation is set,
 *      the centre offset rotated around the position);
 *   4. any edge of a crossing any edge of b (parametric test, both parameters in [0, den]);
 *   5. a corner inside its own half extents (the listing compares each box's world corners with
 *      +-its own half size, without subtracting the centre; kept as the original does). */
static BOOL ExtraBoss_TestColliderRotatedBoxes(const ExCollider *a, const ExCollider *b) {
    const ExCollider *box[2];
    s32 corner[2][4][2];
    s32 edge[2][4][4];
    s32 ra, rb, r, d, i, j, k;
    {
        s32 amin = a->unk28[2] + (a->unkC[2] - a->unk18[2]);
        s32 amax = a->unk28[2] + (a->unkC[2] + a->unk18[2]);
        s32 bmin = b->unk28[2] + (b->unkC[2] - b->unk18[2]);
        s32 bmax = b->unk28[2] + (b->unkC[2] + b->unk18[2]);
        if (amin < bmin && amax < bmin)
            return FALSE;
        if (bmin < amin && bmax < amin)
            return FALSE;
    }
    rb = FX_Sqrt(FX_Mul(b->unk18[0], b->unk18[0]) + FX_Mul(b->unk18[1], b->unk18[1]));
    ra = FX_Sqrt(FX_Mul(a->unk18[0], a->unk18[0]) + FX_Mul(a->unk18[1], a->unk18[1]));
    r = ra + rb;
    d = (a->unk28[0] + a->unkC[0]) - (b->unk28[0] + b->unkC[0]);
    if (d < 0)
        d = -d;
    if (r < d)
        return FALSE;
    d = (a->unk28[1] + a->unkC[1]) - (b->unk28[1] + b->unkC[1]);
    if (d < 0)
        d = -d;
    if (r < d)
        return FALSE;
    box[0] = a;
    box[1] = b;
    for (i = 0; i < 2; i++) {
        const ExCollider *o = box[i];
        u16 ang = (u16)(o->unk24 + o->unk34);
        s32 s = SIN(ang), c = COS(ang);
        s32 hxc = FX_Mul(o->unk18[0], c), hxs = FX_Mul(o->unk18[0], s);
        s32 hyc = FX_Mul(o->unk18[1], c), hys = FX_Mul(o->unk18[1], s);
        s32 cx = o->unkC[0] + o->unk28[0];
        s32 cy = o->unkC[1] + o->unk28[1];
        corner[i][0][0] = cx - hxc + hys;
        corner[i][0][1] = cy - hxs - hyc;
        corner[i][1][0] = cx - hxc - hys;
        corner[i][1][1] = cy - hxs + hyc;
        corner[i][2][0] = cx + hxc - hys;
        corner[i][2][1] = cy + hxs + hyc;
        corner[i][3][0] = cx + hxc + hys;
        corner[i][3][1] = cy + hxs - hyc;
        if (o->unk34 != 0 && (o->unkC[0] != 0 || o->unkC[1] != 0)) {
            s32 s2 = SIN(o->unk34), c2 = COS(o->unk34);
            s32 ox = o->unkC[0], oy = o->unkC[1];
            s32 dx = FX_Mul(ox, c2) - FX_Mul(oy, s2) - ox;
            s32 dy = FX_Mul(ox, s2) + FX_Mul(oy, c2) - oy;
            for (k = 0; k < 4; k++) {
                corner[i][k][0] += dx;
                corner[i][k][1] += dy;
            }
        }
        for (k = 0; k < 4; k++) {
            edge[i][k][0] = corner[i][k][0];
            edge[i][k][1] = corner[i][k][1];
            edge[i][k][2] = corner[i][(k + 1) & 3][0];
            edge[i][k][3] = corner[i][(k + 1) & 3][1];
        }
    }
    for (i = 0; i < 4; i++) {
        s32 ax0 = edge[0][i][0], ay0 = edge[0][i][1];
        s32 dxa = edge[0][i][2] - ax0, dya = edge[0][i][3] - ay0;
        for (j = 0; j < 4; j++) {
            s32 bx0 = edge[1][j][0], by0 = edge[1][j][1];
            s32 dbx = bx0 - edge[1][j][2], dby = by0 - edge[1][j][3];
            s32 den = FX_Mul(dxa, dby) - FX_Mul(dya, dbx);
            s32 ex, ey, t, u;
            if (den == 0)
                continue;
            ex = bx0 - ax0;
            ey = by0 - ay0;
            t = FX_Mul(ex, dby) - FX_Mul(ey, dbx);
            u = FX_Mul(ey, dxa) - FX_Mul(ex, dya);
            if (den < 0) {
                t = -t;
                den = -den;
                u = -u;
            }
            if (t >= 0 && den >= t && u >= 0 && den >= u)
                return TRUE;
        }
    }
    for (i = 0; i < 2; i++) {
        const ExCollider *o = box[i];
        for (k = 0; k < 4; k++) {
            s32 x = corner[i][k][0], y = corner[i][k][1];
            if (-o->unk18[0] > x || x > o->unk18[0])
                continue;
            if (-o->unk18[1] > y || y > o->unk18[1])
                continue;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL ExtraBoss_TestColliderOverlap(const ExCollider *p0, const ExCollider *p1) {
    if ((u16)(p0->unk24 + p0->unk34) == 0 && p1->unk24 == 0 && p1->unk34 == 0)
        return ExtraBoss_TestColliderBoxes(p0, p1);
    return ExtraBoss_TestColliderRotatedBoxes(p0, p1);
}

static void ExtraBoss_CheckCollisionGroup(s32 k) {
    s32 nA = data_ov001_0231f334[k];
    s32 nB = data_ov001_0231f328[k];
    s32 i, j;
    ExCollider **pa = data_ov001_0231f430[k];
    ExCollider **pb;
    ExCollider *a, *b;
    for (i = 0; i < nA; i++, pa++) {
        for (pb = data_ov001_0231f340[k], j = 0; j < nB; j++, pb++) {
            a = *pa;
            b = *pb;
            if (a->owner->unk14 & 0x1000)
                continue;
            if (b->owner->unk14 & 0x1000)
                continue;
            if (a->owner->unk14 & 8)
                continue;
            if (b->owner->unk14 & 8)
                continue;
            if (a->unk8 & 0x200)
                continue;
            if (b->unk8 & 0x200)
                continue;
            if (!ExtraBoss_TestColliderOverlap(a, b))
                continue;
            if (a->unk4 != NULL) {
                a->unk4(a, b);
            } else if (a->owner->unk4 == 1 && a->owner->unk98 != 0xffff) {
                a->owner->unk98--;
                if (a->owner->unk98 == 0)
                    a->owner->unk14 |= 8;
            }
            if (b->unk4 != NULL) {
                b->unk4(a, b);
            } else if (b->owner->unk4 == 1 && a->owner->unk98 != 0xffff) { /* (sic) a's hit points */
                b->owner->unk98--;
                if (b->owner->unk98 == 0)
                    b->owner->unk14 |= 8;
            }
        }
    }
}

void ExtraBoss_CheckAllCollisions(void) {
    ExtraBoss_CheckCollisionGroup(1);
    ExtraBoss_CheckCollisionGroup(2);
    ExtraBoss_CheckCollisionGroup(0);
}

/* ----------------------------------------------------------- effect models */
/* ExtraBossPlayer_SetupFxModel (modified: NNS_G3dResDefaultSetup / NNS_G3dGetTex are not needed) */
BossModel *ExtraBossPlayer_SetupFxModel(BossModel *o, s32 n) {
    s32 i;
    if (o == NULL) {
        o = (BossModel *)calloc(1, sizeof(BossModel));
        Model_Init(o, 0);
    }
    Model_SetResource(o, &gExtraBossFxRes[n].mdl);
    o->scale[2] = 0x34cc;
    o->scale[1] = o->scale[2];
    o->scale[0] = o->scale[1];
    for (i = 0; i < 5; i++) {
        if (gExtraBossFxRes[n].anm[i].loaded || gExtraBossFxRes[n].anm[i].nframes != NULL)
            Model_SetAnimation(o, (u32)i, &gExtraBossFxRes[n].anm[i], 0);
    }
    return o;
}

void ExtraBoss_SetObjModelAnims(ExObj *p, s32 idx) {
    s32 i;
    s32 ty = p->unk1C8;
    BossModel *obj = p->unk84;
    for (i = 0; i < 5; i++) {
        if (gExtraBossFxRes[ty].anm[i].loaded || gExtraBossFxRes[ty].anm[i].nframes != NULL)
            Model_SetAnimation(obj, (u32)i, &gExtraBossFxRes[ty].anm[i], (u32)idx);
    }
}

void ExtraBoss_SwapObjModel(ExObj *p, s32 n) {
    if (p->unk1C8 == n)
        return;
    ex_model_free(p->unk84);
    ExtraBossObj_ReleaseFxModelRes(p);
    {
        BossModel *r = ExtraBossPlayer_SetupFxModel(NULL, n);
        p->unk1C8 = n;
        p->unk84 = r;
    }
}

ExObj *ExtraBossPlayer_CreateFxObj(s32 kind, ExObj *parent, s32 x, s32 y, s32 z) {
    ExObj *t = ExtraBossObj_Create(1, 0x3200);
    if (t == NULL)
        return NULL;
    t->unk1C8 = kind;
    t->unk84 = ExtraBossPlayer_SetupFxModel(NULL, kind);
    t->unk7C = parent;
    t->unk98 = 1;
    t->unk14 |= 1;
    t->unk18 = x;
    t->unk1C = y;
    t->unk20 = z;
    t->unk74 = ExtraBossObj_State_DestroyWhenAnimsDone;
    return t;
}

void ExtraBoss_CopyParentPosition(ExObj *p) {
    ExObj *q = p->unk7C;
    p->unk18 = q->unk18;
    p->unk1C = q->unk1C;
    p->unk20 = q->unk20;
}
void ExtraBoss_SnapObjToCameraX(ExObj *p) { p->unk18 = ExtraBoss_GetWork()->unk31C; }

/* ExtraBoss_AllocFlagBlock / FreeFlagBlock / ResetFxFlags / CountActiveBeamUnits / CancelSwordFx */
void ExtraBoss_AllocFlagBlock(void) { data_ov001_0231f094 = (u8 *)calloc(1, 0x20); }
void ExtraBoss_FreeFlagBlock(void) {
    free(data_ov001_0231f094);
    data_ov001_0231f094 = NULL;
}
void ExtraBoss_ResetFxFlags(void) {
    data_ov001_0231f094[0] = 0;
    data_ov001_0231f094[1] = 0;
}
s32 ExtraBoss_CountActiveBeamUnits(void) {
    s32 n = 0;
    u8 b = *data_ov001_0231f094;
    if (b & 1)
        n++;
    if (b & 2)
        n++;
    return n;
}
void ExtraBoss_CancelSwordFx(void) { data_ov001_0231f094[1] = 1; }

/* -------------------------------------------------------- player effects */
void ExtraBossPlayer_SpawnBlzShotFx(ExObj *p, s32 x, s32 y, s32 z) {
    ExObj *t0 = ExtraBossPlayer_CreateFxObj(1, p, x, y, z);
    if (t0 != NULL)
        t0->unk14 &= ~1u;
}
void ExtraBossPlayer_SpawnSpecialShot(ExObj *p, s32 b, s32 c, s32 d) {
    ExObj *t = ExtraBossPlayer_CreateFxObj(0, p, b, c, d);
    if (t == NULL)
        return;
    t->unk74 = ExtraBossObj_State_DestroyWhenOutOfArena;
    t->unk14 &= ~1u;
    t->unk80 = p->unk80;
    t->unk84->flags[3] |= 4;
    t->unk50 = 0x5000;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 6, 1, 0x10000, 0x10000, 0x10000, 0, 0, 0, 0);
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    g_bossex_stats.shots++;
}
void ExtraBossPlayer_SpawnClashFx(ExObj *p, s32 x, s32 y, s32 z) {
    ExObj *t0 = ExtraBossPlayer_CreateFxObj(0xb, p, x, y, z);
    if (t0 == NULL)
        return;
    t0->unk80 = ExtraBoss_GetPlayer();
}

/* ------------------------------------------------------------- small effects */
void ExtraBoss_SpawnBombFx(ExObj *a, s32 b, s32 c, s32 d, BOOL flag) {
    ExObj *t = ExtraBossPlayer_CreateFxObj(0xd, a, b, c, d);
    if (t == NULL)
        return;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk98 = 0xffff;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x1e000, 0x1e000, 0x10000, 0, 0, 0, 0);
    if (flag)
        t->unkB0[EX_COLL_E8].unk8 |= 0x10;
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
}
void ExtraBoss_SpawnClawFx(ExObj *a, s32 b, s32 c, s32 d, s16 big) {
    ExObj *t;
    if (big != 0)
        t = ExtraBossPlayer_CreateFxObj(0x11, a, b, c, d);
    else
        t = ExtraBossPlayer_CreateFxObj(0x10, a, b, c, d);
    if (t == NULL)
        return;
    t->unk74 = ExtraBossObj_State_DestroyAfterTimer;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk98 = 0xffff;
    t->unk1CC = 0x3c;
    Sound_PlaySeqArc(0, 0, 0xb2);
    g_bossex_stats.claws++;
}
void ExtraBoss_SpawnCatchFx(ExObj *a, s32 b, s32 c, s32 d) {
    ExObj *t0 = ExtraBossPlayer_CreateFxObj(0x13, a, b, c, d);
    if (t0 != NULL)
        t0->unk14 &= ~1u;
}
void ExtraBoss_SpawnJumpFx(ExObj *a, s32 b, s32 c, s32 d) {
    ExObj *t0 = ExtraBossPlayer_CreateFxObj(0x12, a, b, c, d);
    if (t0 != NULL)
        t0->unk14 &= ~1u;
}
void ExtraBoss_SpawnDamageFx(ExObj *a, s32 b, s32 c, s32 d) {
    ExObj *t0 = ExtraBossPlayer_CreateFxObj(0x16, a, b, c, d);
    if (t0 == NULL)
        return;
    t0->unk14 &= ~1u;
    t0->unk80 = ExtraBoss_GetPlayer();
}

/* ------------------------------------------------------------------ meteors */
static void ExtraBoss_Meteor_OnHit(ExCollider *self, ExCollider *other) {
    ExObj *p = other->owner;
    s32 n = 3;
    (void)self;
    if (p->unk1C8 == 4) {
        ExtraBoss_SpawnRingBurst(p->unk18, p->unk1C, 5);
        n = 5;
    }
    ex_model_free(p->unk84);
    ExtraBossObj_ReleaseFxModelRes(p);
    {
        BossModel *r = ExtraBossPlayer_SetupFxModel(NULL, n);
        p->unk1C8 = n;
        p->unk84 = r;
    }
    p->unk74 = ExtraBossObj_State_DestroyWhenAnimsDone;
    p->unk50 = 0;
    p->unk68 = p->unk6A = p->unk6C = 0;
    p->unk14 |= 0x1000;
    Sound_PlaySeqArc(0, 0, 0xa7);
    g_bossex_stats.meteors_broken++;
}
static void ExtraBoss_SpawnMeteorOfKind(ExObj *p, s32 x, s32 y, s32 z, s32 kind) {
    ExObj *t = ExtraBossPlayer_CreateFxObj(kind, p, x, y, z);
    if (t == NULL)
        return;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk98 = 0xffff;
    t->unk74 = ExtraBossObj_State_DestroyWhenOutOfArena;
    t->unk50 = -0x3000;
    t->unk68 = (u16)((Rand() & 0x1ff) - 0xff);
    t->unk6A = (u16)((Rand() & 0x1ff) - 0xff);
    t->unk6C = (u16)((Rand() & 0x1ff) - 0xff);
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_B0], t, 2, 0, 0xa000, 0xa000, 0xa000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_B0].unk4 = ExtraBoss_Meteor_OnHit;
    t->unkB0[EX_COLL_B0].unk8 |= 0x2000;
    t->unk9C[0] = &t->unkB0[EX_COLL_B0];
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x9000, 0x9000, 0x9000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_E8].unk8 |= 0x2000;
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    g_bossex_stats.meteors++;
}
void ExtraBoss_SpawnMeteor(ExObj *a, s32 b, s32 c, s32 d) { ExtraBoss_SpawnMeteorOfKind(a, b, c, d, 2); }
void ExtraBoss_SpawnMeteorR(ExObj *a, s32 b, s32 c, s32 d) { ExtraBoss_SpawnMeteorOfKind(a, b, c, d, 4); }

/* ------------------------------------------------------------------- big beam */
static void ExtraBossObj_State_BigBeamEnd(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    ExtraBoss_SnapObjToCameraX(p0);
    if (t0->flags[0] & 2)
        p0->unk14 |= 8;
}
static void ExtraBossObj_State_BigBeamFire(ExObj *p) {
    p->unk1CC--;
    ExtraBoss_SnapObjToCameraX(p);
    if (p->unk1CC > 0)
        return;
    p->unk74 = ExtraBossObj_State_BigBeamEnd;
    ex_model_free(p->unk84);
    ExtraBossObj_ReleaseFxModelRes(p);
    {
        BossModel *r = ExtraBossPlayer_SetupFxModel(NULL, 7);
        p->unk1C8 = 7;
        p->unk84 = r;
    }
    p->unkB0[EX_COLL_E8].unk8 |= 0x200;
}
static void ExtraBossObj_State_BigBeamStart(ExObj *p0) {
    BossModel *t0 = p0->unk84;
    ExtraBoss_SnapObjToCameraX(p0);
    if ((t0->flags[0] & 2) == 0)
        return;
    p0->unk74 = ExtraBossObj_State_BigBeamFire;
    p0->unk1CC = 0x3c;
    p0->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
}
static void ExtraBossObj_State_BigBeamDelay(ExObj *p0) {
    ExtraBoss_SnapObjToCameraX(p0);
    p0->unk1CC = p0->unk1CC - 1;
    if (p0->unk1CC <= 0) {
        p0->unk14 &= ~0x10000u;
        p0->unk74 = ExtraBossObj_State_BigBeamStart;
    }
}
void ExtraBoss_SpawnBigBeam(ExObj *p, s32 y, s32 z) {
    ExObj *t;
    s32 x = ExtraBoss_GetWork()->unk31C;
    t = ExtraBossPlayer_CreateFxObj(6, p, x, y, z);
    if (t == NULL)
        return;
    t->unk74 = ExtraBossObj_State_BigBeamDelay;
    t->unk14 |= 0x10000;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk98 = 0xffff;
    t->unk1CC = ex_level() == 1 ? 0x10 : 0x18;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x80000, 0x10000, 0x9000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_E8].unk8 |= 0x2200;
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    Sound_PlaySeqArc(0, 0, 0xa8);
    g_bossex_stats.big_beams++;
}

/* ------------------------------------------------------------- small beam */
static void ExtraBossObj_State_SmallBeam(ExObj *p) {
    p->unk1CC--;
    if (p->unk1CC <= 0 || p->unk7C->unk74 == ExtraBossObj_State_BeamUnitDeflected)
        p->unk14 |= 8;
}
static void ExtraBoss_SpawnSmallBeam(ExObj *p, s32 x, s32 y, s32 z, s32 n, u16 ang, u16 rz) {
    ExObj *t = ExtraBossPlayer_CreateFxObj(0xa, p, x, y, z);
    if (t == NULL)
        return;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk14 |= 0x10;
    t->unk98 = 0xffff;
    t->unk74 = ExtraBossObj_State_SmallBeam;
    t->unk40 = ang;
    t->unk6C = rz;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x5000, 0x200000, 0x9000, 0, 0, 0, 0);
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_120], t, 1, 1, 0x200000, 0x5000, 0x9000, 0, 0, 0, 0);
    t->unk9C[2] = &t->unkB0[EX_COLL_120];
    t->unk1CC = n;
    Sound_PlaySeqArc(0, 0, 0xab);
}

/* -------------------------------------------------------------- beam units */
static void ex_beam_unit_colors(s32 kind, u16 diff, u16 amb, u16 emi) {
    /* NNS_G3dMdlSetMdlDiffAll / AmbAll / EmiAll on the unit's model resource */
    boss_model_set_colors(&gExtraBossFxRes[kind].mdl, diff, amb, emi);
}
static void ExtraBossObj_State_BeamUnitActive(ExObj *p);
static void ExtraBossObj_State_BeamUnitFiring(ExObj *p);
static void ExtraBossObj_State_BeamUnitCharge(ExObj *p) {
    p->unk1CC -= 1;
    if (p->unk1CC > 0)
        return;
    p->unk74 = ExtraBossObj_State_BeamUnitFiring;
    p->unk1CC = 0x14;
    ExtraBoss_SpawnSmallBeam(p, p->unk18, p->unk1C, p->unk20, 0x14, p->unk40, p->unk6C);
}
static void ExtraBossObj_State_BeamUnitFiring(ExObj *p) {
    p->unk1CC -= 1;
    if (p->unk1CC > 0)
        return;
    p->unk1D0 -= 1;
    if (p->unk1D0 <= 0) {
        p->unk74 = ExtraBossObj_State_BeamUnitActive;
        p->unk1CC = 0x78;
        return;
    }
    p->unk74 = ExtraBossObj_State_BeamUnitCharge;
    p->unk54 = 0;
    p->unk50 = p->unk54;
    p->unk1CC = 0x1e;
}
static void ExtraBossObj_State_BeamUnitActive(ExObj *p) {
    u16 ang;
    ExObj *pl;
    s32 dx, dy, idx, dist, adj;
    pl = ExtraBoss_GetPlayer();
    p->unk1D8--;
    if (p->unk1D8 > 0) {
        if (p->unk1D8 & 2)
            p->unk14 |= 0x20000;
        else
            p->unk14 &= ~0x20000u;
    } else if (p->unk1D8 == 0) {
        p->unk14 &= ~0x20000u;
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    }
    p->unk1CC--;
    p->unk1D4--;
    if (p->unk1D4 <= 0) {
        p->unk14 |= 8;
        if (p->unk1C8 == 8)
            *data_ov001_0231f094 &= ~1;
        else
            *data_ov001_0231f094 &= ~2;
        ExtraBoss_SpawnBombFx(NULL, p->unk18, p->unk1C, 0, 0);
        Sound_PlaySeqArc(0, 0, 0xac);
        return;
    }
    if (p->unk1CC <= 0 && p->unk1D4 > 0x78) {
        p->unk74 = ExtraBossObj_State_BeamUnitCharge;
        p->unk54 = 0;
        p->unk50 = p->unk54;
        p->unk1CC = 0x1e;
        if (ExtraBoss_GetWork()->unk40 == 4)
            p->unk1D0 = (Rand() & 1) ? 1 : 3;
        else
            p->unk1D0 = 1;
    } else {
        dx = p->unk18 - pl->unk18;
        dy = p->unk1C - pl->unk1C;
        dist = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
        adj = 0;
        if (dist > 0x30000) {
            adj = -0x1e00;
            if (0x30000 - dist > adj)
                adj = 0x30000 - dist;
        } else if (dist < 0x30000) {
            adj = 0x1e00;
            if (0x30000 - dist < 0x1e00)
                adj = 0x30000 - dist;
        }
        dist += adj;
        ang = FX_Atan2(dy, dx);
        idx = (s32)((u16)(ang + EX_U16(0x0231d5d8, ex_level() * 5 + ExtraBoss_GetWork()->unk40)) & 0xffff) >> 4;
        {
            s32 vx = FX_Mul(dist, COS_IDX(idx));
            s32 vy;
            p->unk50 = vx + pl->unk18 - p->unk18;
            vy = FX_Mul(dist, SIN_IDX(idx));
            p->unk54 = vy + pl->unk1C - p->unk1C;
        }
    }
    if (p->unk1D4 > 0x78)
        return;
    if (p->unk1D4 & 3)
        return;
    if (p->unk1D4 & 4)
        ex_beam_unit_colors(p->unk1C8, 0x7fff, 0, 0);
    else
        ex_beam_unit_colors(p->unk1C8, 0x85f, 0x85f, 0x85f);
}
static void ExtraBossObj_State_BeamUnitDeploy(ExObj *p) {
    s32 ax, ay;
    p->unk1D8--;
    if (p->unk1D8 > 0) {
        if (p->unk1D8 & 2)
            p->unk14 |= 0x20000;
        else
            p->unk14 &= ~0x20000u;
    } else if (p->unk1D8 == 0) {
        p->unk14 &= ~0x20000u;
        p->unkB0[EX_COLL_B0].unk8 &= ~0x200u;
    }
    ay = p->unk54;
    ax = p->unk50;
    if (ay < 0)
        ay = -ay;
    if (ax < 0)
        ax = -ax;
    if (ax + ay > 0x1000)
        return;
    p->unk74 = ExtraBossObj_State_BeamUnitActive;
    p->unk5C = p->unk60 = 0;
    p->unk6C = 0x2d8;
    p->unk1CC = 0x78;
    p->unk1D4 = 600;
    ExtraBossObj_State_BeamUnitActive(p);
}
void ExtraBossObj_State_BeamUnitDeflected(ExObj *p) {
    if (!ExtraBossObj_IsOutOfArena(p->unk18, p->unk1C, 0x30000, 0x30000))
        return;
    p->unk14 |= 8;
    if (p->unk1C8 == 8)
        *data_ov001_0231f094 &= ~1;
    else
        *data_ov001_0231f094 &= ~2;
}
static void ExtraBoss_BeamUnit_OnHit(ExCollider *self, ExCollider *other) {
    ExObj *s = self->owner;
    ExObj *p = other->owner;
    u16 a;
    if (s->unk4 != 0)
        return;
    p->unk9C[0] = NULL;
    p->unk9C[1] = &p->unkB0[EX_COLL_E8];
    p->unk6C = 0;
    p->unk50 = p->unk54 = 0;
    a = FX_Atan2(s->unk1C - s->unk28, s->unk18 - s->unk24);
    p->unk50 = FX_Mul(COS(a), 0x8000);
    p->unk54 = FX_Mul(SIN(a), 0x8000);
    p->unk74 = ExtraBossObj_State_BeamUnitDeflected;
    Sound_PlaySeqArc(0, 0, 0xa9);
    g_bossex_stats.deflects++;
}
static void ExtraBoss_BeamUnit_OnContact(ExCollider *self, ExCollider *other) {
    ExObj *o = other->owner;
    ExObj *p = self->owner;
    if (o->unk4 != 2)
        return;
    p->unk14 |= 8;
    if (p->unk1C8 == 8)
        *data_ov001_0231f094 &= ~1;
    else
        *data_ov001_0231f094 &= ~2;
}
void ExtraBoss_SpawnBeamUnit(ExObj *parent, s32 x, s32 y, s32 z, u16 ang) {
    s32 kind, idx;
    ExObj *t;
    if (*data_ov001_0231f094 & 1) {
        *data_ov001_0231f094 |= 2;
        kind = 9;
    } else {
        *data_ov001_0231f094 |= 1;
        kind = 8;
    }
    t = ExtraBossPlayer_CreateFxObj(kind, parent, x, y, z);
    if (t == NULL)
        return;
    ex_beam_unit_colors(kind, 0x7fff, 0, 0);
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk74 = ExtraBossObj_State_BeamUnitDeploy;
    t->unk18 = x;
    t->unk1C = y;
    t->unk20 = z;
    idx = (s32)(ang & 0xffff) >> 4;
    t->unk50 = FX_Mul(COS_IDX(idx), 0xa000);
    t->unk54 = FX_Mul(SIN_IDX(idx), 0xa000);
    t->unk5C = FX_Mul(COS_IDX(idx), -0xa00);
    t->unk60 = FX_Mul(SIN_IDX(idx), -0xa00);
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_B0], t, 2, 0, 0xa000, 0xa000, 0xa000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_B0].unk4 = ExtraBoss_BeamUnit_OnHit;
    t->unkB0[EX_COLL_B0].unk8 |= 0x2200;
    t->unk9C[0] = &t->unkB0[EX_COLL_B0];
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 4, 1, 0xa000, 0xa000, 0xa000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_E8].unk4 = ExtraBoss_BeamUnit_OnContact;
    t->unkB0[EX_COLL_E8].unk8 |= 0x2000;
    t->unk1D8 = 0x78;
    Sound_PlaySeqArc(0, 0, 0xaa);
    g_bossex_stats.beam_units++;
}

/* ------------------------------------------------------------- homing beam */
static void ExtraBossObj_State_HomingBeamFire(ExObj *p);
static void ExtraBossObj_State_HomingBeamAim(ExObj *p) {
    ExObj *o;
    p->unk1CC--;
    if (p->unk1CC > 0)
        return;
    o = p->unk80;
    p->unk74 = ExtraBossObj_State_HomingBeamFire;
    if (o->unk1C < p->unk1C)
        p->unk54 = -EX_S32(0x0231d5ec, ex_level() * 5 + ExtraBoss_GetWork()->unk40);
    else
        p->unk54 = EX_S32(0x0231d5ec, ex_level() * 5 + ExtraBoss_GetWork()->unk40);
    p->unk1D0 = 0;   /* s16 t */
    Sound_PlaySeqArc(0, 0, 0xaf);
}
static void ExtraBossObj_State_HomingBeamApproach(ExObj *p) {
    ExObj *t = p->unk80;
    s32 cap;
    if (ExtraBossObj_IsOutOfArena(p->unk18, p->unk1C, 0x30000, 0x30000)) {
        p->unk14 |= 8;
        return;
    }
    cap = EX_S32(0x0231d664, ex_level() * 5 + ExtraBoss_GetWork()->unk40);
    if (p->unk50 + p->unk5C >= cap) {
        p->unk50 = cap;
        p->unk5C = 0;
    }
    if (t->unk18 < p->unk18 - 0xa000)
        return;
    if (t->unk18 > p->unk18 + 0xa000)
        return;
    p->unk74 = ExtraBossObj_State_HomingBeamAim;
    p->unk50 = p->unk5C = 0;
    p->unk1CC = 10;
}
static void ExtraBossObj_State_HomingBeamFire(ExObj *p) {
    s32 i, v;
    if ((s16)p->unk1D0 < 0x1000) {
        p->unk1D0 = (s16)(p->unk1D0 + 0x333);
        i = 1;
        v = 0x34cc;
        do {
            v += FX_Mul(0x6998 - v, (s16)p->unk1D0);
        } while (i-- != 0);
        p->unk84->scale[1] = v;
        i = 1;
        v = 0x8000;
        do {
            v += FX_Mul(0x10000 - v, (s16)p->unk1D0);
        } while (i-- != 0);
        p->unkB0[EX_COLL_E8].unk18[1] = v;
    }
    if (ExtraBossObj_IsOutOfArena(p->unk18, p->unk1C, 0x30000, 0x30000))
        p->unk14 |= 8;
}
static void ExtraBoss_HomingBeam_OnHit(ExCollider *pa, ExCollider *pt) {
    ExObj *a = pa->owner;
    ExObj *t = pt->owner;
    u16 ang;
    if (a->unk4 != 0)
        return;
    t->unk9C[0]->unk8 |= 0x200;
    t->unk9C[1]->unk8 |= 0x200;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_120], t, 4, 1, 0x10000, 0x10000, 0x10000, 0, 0, 0, 0);
    t->unk9C[2] = &t->unkB0[EX_COLL_120];
    t->unk98 = 1;
    t->unk84->scale[0] = 0x6998;
    t->unk84->scale[1] = 0x34cc;
    ang = FX_Atan2(a->unk1C - a->unk28, a->unk18 - a->unk24);
    t->unk50 = FX_Mul(COS(ang), 0x8000);
    t->unk54 = FX_Mul(SIN(ang), 0x8000);
    t->unk40 = (u16)(0x10000 - ang);
    t->unk74 = ExtraBossObj_State_DestroyWhenOutOfArena;
    Sound_PlaySeqArc(0, 0, 0xa9);
    g_bossex_stats.deflects++;
}
void ExtraBoss_SpawnHomingBeam(ExObj *parent, s32 x, s32 y, s32 z) {
    ExObj *t = ExtraBossPlayer_CreateFxObj(0xf, parent, x, y, z);
    if (t == NULL)
        return;
    t->unk84->flags[0] |= 4;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk14 |= 0x10;
    t->unk98 = 0xffff;
    t->unk74 = ExtraBossObj_State_HomingBeamApproach;
    t->unk50 = EX_S32(0x0231d63c, ex_level() * 5 + ExtraBoss_GetWork()->unk40);
    t->unk5C = EX_S32(0x0231d614, ex_level() * 5 + ExtraBoss_GetWork()->unk40);
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_B0], t, 2, 0, 0x8000, 0x8000, 0x10000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_B0].unk4 = ExtraBoss_HomingBeam_OnHit;
    t->unkB0[EX_COLL_B0].unk8 |= 0x2000;
    t->unk9C[0] = &t->unkB0[EX_COLL_B0];
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x8000, 0x8000, 0x10000, 0, 0, 0, 0);
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    Sound_PlaySeqArc(0, 0, 0xae);
    g_bossex_stats.homing_beams++;
}

/* ---------------------------------------------------------- homing missile */
static void ExtraBossObj_State_MissileHoming(ExObj *p) {
    ExObj *t;
    s32 dx, dy, dz, idx, vx, vy;
    if (p->unk20 >= 0) {
        p->unk14 |= 8;
        ExtraBoss_SpawnBombFx(NULL, p->unk18, p->unk1C, 0, 1);
        Sound_PlaySeqArc(0, 0, 0xb1);
        return;
    }
    if (p->unk58 + p->unk64 >= 0x4000) {
        p->unk58 = 0x4000;
        p->unk64 = 0;
    }
    t = ExtraBoss_GetPlayer();
    dx = t->unk18 - p->unk18;
    dy = t->unk1C - p->unk1C;
    idx = (s32)(FX_Atan2(dy, dx) & 0xffff) >> 4;
    vx = FX_Mul(p->unk1D0, COS_IDX(idx));
    vy = FX_Mul(p->unk1D0, SIN_IDX(idx));
    if ((vx > 0 && p->unk50 < -vx) || (vx < 0 && p->unk50 > -vx))
        vx = 0;
    if ((vy > 0 && p->unk54 < -vy) || (vy < 0 && p->unk54 > -vy))
        vy = 0;
    p->unk50 = vx;
    p->unk54 = vy;
    if (p->unk50 >= 0 && p->unk50 > dx)
        p->unk50 = dx;
    else if (p->unk50 < 0 && p->unk50 < dx)
        p->unk50 = dx;
    if (p->unk54 >= 0 && p->unk54 > dy)
        p->unk54 = dy;
    else if (p->unk54 < 0 && p->unk54 < dy)
        p->unk54 = dy;
    dz = t->unk20 - p->unk20;
    p->unk3C = FX_Atan2(dy, dz);
    p->unk3E = FX_Atan2(dx, dz);
    p->unk3C >>= 1;
    p->unk3E >>= 1;
}
static void ExtraBossObj_State_MissileLaunch(ExObj *p) {
    s32 i, v;
    p->unk1CC = (u16)((s16)p->unk1CC + 0x44); /* s16 t in the low half; the high half stays 0 */
    if (p->unk1CC >= 0x1000) {
        p->unk74 = ExtraBossObj_State_MissileHoming;
        return;
    }
    i = 2;
    v = 0;
    do {
        v = p->unk1D4 + FX_Mul(v - p->unk1D4, (s16)p->unk1CC);
    } while (i-- != 0);
    p->unk50 = v;
    i = 2;
    v = 0;
    do {
        v = p->unk1D8 + FX_Mul(v - p->unk1D8, (s16)p->unk1CC);
    } while (i-- != 0);
    p->unk54 = v;
}
void ExtraBoss_SpawnHomingMissile(ExObj *parent, s32 x, s32 y, s32 z, u16 ang) {
    ExObj *t;
    s32 r, idx;
    r = Rand() & 0xff;
    t = ExtraBossPlayer_CreateFxObj(0xe, parent, x, y, z);
    if (t == NULL)
        return;
    t->unk14 &= ~1u;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk14 |= 0x10;
    idx = (s32)(ang & 0xffff) >> 4;
    t->unk74 = ExtraBossObj_State_MissileLaunch;
    t->unk58 = -0x2000;
    t->unk64 = 0x88;
    t->unk50 = FX_Mul(COS_IDX(idx), 0x8000);
    t->unk54 = FX_Mul(SIN_IDX(idx), 0x8000);
    t->unk1D4 = t->unk50;
    t->unk1D8 = t->unk54;
    t->unk1D0 = FX_DivS32(r * EX_S32(0x0231d68c, ex_level() * 5 + ExtraBoss_GetWork()->unk40) + ((0x100 - r) << 12),
                          0x100);
    t->unk1CC = 0;
    Sound_PlaySeqArc(0, 0, 0xb0);
    g_bossex_stats.missiles++;
}

/* --------------------------------------------------------- space beam (suction) */
static void ExtraBossObj_State_SuctionFx(ExObj *p) {
    ExtraBoss_SnapObjToCameraX(p);
    if (!(ExtraBoss_GetWork()->unk0 & 0x40))
        p->unk14 |= 8;
}
static void ExtraBoss_SpawnSuctionFx(ExObj *a, s32 b, s32 c) {
    s32 v = ExtraBoss_GetWork()->unk31C;
    ExObj *t = ExtraBossPlayer_CreateFxObj(0x15, a, v, b, c);
    if (t == NULL)
        return;
    t->unk84->flags[3] |= 4;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk74 = ExtraBossObj_State_SuctionFx;
}
static void ExtraBossObj_State_SpacebEnd(ExObj *p) {
    BossModel *q = p->unk84;
    ExtraBoss_SnapObjToCameraX(p);
    p->unk1CC -= 1;
    if (q->flags[0] & 2) {
        ExtraBoss_SetObjModelAnims(p, 0);
        ExtraBoss_GetWork()->unk0 &= ~0x40u;
        ExtraBoss_EndRingSuction();
        p->unk14 |= 8;
        return;
    }
    if (p->unk1CC == 0)
        p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
}
static void ExtraBossObj_State_SpacebActive(ExObj *p) {
    ExtraBoss_SnapObjToCameraX(p);
    p->unk1CC -= 1;
    if (p->unk1CC > 0)
        return;
    p->unk74 = ExtraBossObj_State_SpacebEnd;
    ExtraBoss_SetObjModelAnims(p, 2);
    p->unk84->flags[0] &= ~4;
    p->unk84->flags[1] &= ~4;
    p->unk84->flags[2] &= ~4;
    p->unk1CC = 0xa;
}
static void ExtraBossObj_State_SpacebStart(ExObj *p) {
    BossModel *q = p->unk84;
    ExtraBoss_SnapObjToCameraX(p);
    if (!(q->flags[0] & 2))
        return;
    p->unk74 = ExtraBossObj_State_SpacebActive;
    ExtraBoss_SetObjModelAnims(p, 1);
    p->unk84->flags[0] |= 4;
    p->unk84->flags[1] |= 4;
    p->unk84->flags[2] |= 4;
    p->unk1CC = 0x12c;
    ExtraBoss_GetWork()->unk0 |= 0x40;
    ExtraBoss_GetWork()->unk310 = p->unk1C;
    ExtraBoss_StartRingSuction();
    ExtraBoss_SpawnSuctionFx(NULL, p->unk1C, p->unk20);
    Sound_PlaySeqArc(0, 0, 0xb9);
}
void ExtraBoss_SpawnSpacebFx(ExObj *a, s32 b, s32 c) {
    s32 v = ExtraBoss_GetWork()->unk31C;
    ExObj *t = ExtraBossPlayer_CreateFxObj(0x14, a, v, b, c);
    if (t == NULL)
        return;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk98 = 0xffff;
    t->unk74 = ExtraBossObj_State_SpacebStart;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x100000, 0x20000, 0x10000, 0, 0, 0, 0);
    t->unkB0[EX_COLL_E8].unk8 |= 0x2210;
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    Sound_PlaySeqArc(0, 0, 0xb8);
    g_bossex_stats.suctions++;
}

/* ------------------------------------------------------------------- sword */
static void ExtraBossObj_State_SwordFx(ExObj *p) {
    BossModel *o = p->unk84;
    if ((p->unk1D0 == 3 && (o->flags[4] & 2)) || data_ov001_0231f094[1] != 0)
        p->unk14 |= 8;
}
ExObj *ExtraBoss_SpawnSwordFx(ExObj *p) {
    ExObj *t = ExtraBossPlayer_CreateFxObj(0xc, p, 0, 0, 0);
    if (t == NULL)
        return NULL;
    t->unk14 &= ~1u;
    t->unk80 = ExtraBoss_GetPlayer();
    t->unk14 |= 0x10;
    t->unk98 = 0xffff;
    t->unk74 = ExtraBossObj_State_SwordFx;
    data_ov001_0231f094[1] = 0;
    ExtraBoss_InitCollider(&t->unkB0[EX_COLL_E8], t, 1, 1, 0x3c000, 0x1e000, 0x32000, -0x3c000, 0, 0, 0);
    t->unkB0[EX_COLL_E8].unk8 |= 0x3200;
    t->unk9C[1] = &t->unkB0[EX_COLL_E8];
    t->unk1D0 = 0;
    g_bossex_stats.swords++;
    return t;
}
void ExtraBoss_AdvanceSwordFxStep(ExObj *p) {
    p->unk1D0 += 1;
    switch (p->unk1D0) {
    case 1:
        ExtraBoss_SetObjModelAnims(p, 0);
        p->unkB0[EX_COLL_E8].unk8 |= 0x200;
        break;
    case 2:
        ExtraBoss_SetObjModelAnims(p, 1);
        p->unkB0[EX_COLL_E8].unk8 &= ~0x200u;
        break;
    case 3:
        ExtraBoss_SetObjModelAnims(p, 0);
        p->unkB0[EX_COLL_E8].unk8 |= 0x200;
        break;
    }
}

/* names of the state functions (host trace) */
const char *ex_state_name(ExFn fn) {
#define N(f) if (fn == (ExFn)f) return #f;
    N(ExtraBossObj_State_None) N(ExtraBossObj_State_BeamUnitActive) N(ExtraBossObj_State_BeamUnitDeploy)
    N(ExtraBossObj_State_BeamUnitCharge) N(ExtraBossObj_State_BeamUnitFiring) N(ExtraBossObj_State_BeamUnitDeflected)
    N(ExtraBossObj_State_HomingBeamApproach) N(ExtraBossObj_State_HomingBeamAim) N(ExtraBossObj_State_HomingBeamFire)
    N(ExtraBossObj_State_MissileLaunch) N(ExtraBossObj_State_MissileHoming) N(ExtraBossObj_State_SpacebStart)
    N(ExtraBossObj_State_SpacebActive) N(ExtraBossObj_State_SpacebEnd) N(ExtraBossObj_State_SwordFx)
    N(ExtraBossObj_State_BigBeamDelay) N(ExtraBossObj_State_BigBeamStart) N(ExtraBossObj_State_BigBeamFire)
    N(ExtraBossObj_State_BigBeamEnd) N(ExtraBossObj_State_SmallBeam) N(ExtraBossObj_State_DestroyWhenAnimsDone)
    N(ExtraBossObj_State_DestroyWhenOutOfArena) N(ExtraBossObj_State_DestroyAfterTimer)
    N(ExtraBossObj_State_Blink) N(ExtraBossObj_State_Blink2)
#undef N
    {
        const char *n = ex_weapon_state_name(fn);
        return n != NULL ? n : ex_hand_state_name(fn);
    }
}
