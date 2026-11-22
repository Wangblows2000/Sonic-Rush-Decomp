/*
 * sp_gimmick.c - special-stage object lists, gimmicks (rings, dash panels,
 * trick ramps, switches, enemies, bombs, emerald), act objects (effects,
 * lost rings, target cursor) and the per-object update / collision code:
 * SpecialStageObjList_*, SpecialStageGimmick_*, SpecialStageActObj_*,
 * SpecialStage_UpdateObject(s), SpecialStage_CheckGimmickCollisions,
 * SpecialStage_TestCollision, SpecialStage_ActObj*, SpecialStageTrickReward_Update.
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpGimmickModels *gSpecialStageGimmickModels;
SpActWork *data_ov001_0231f5b8;
int sp_mdl_load(SpMdlRes *r, const char *file);
void sp_mdl_free(SpMdlRes *r);
int sp_anm_load(SpAnmRes *r, const char *file, int kind);
void sp_anm_free(SpAnmRes *r);
void sp_anm_timing(SpAnmRes *r, int kind, u16 nframes);

/* ------------------------------------------------------------ resources */
/* a converted material / pattern / SRT set (missing: an empty timing object of 1 frame) */
static void load_matanim(SpAnmRes *r, const char *file, int kind) {
    if (sp_anm_load(r, file, kind) != 0)
        sp_anm_timing(r, kind, 1);
}

void SpecialStage_LoadGimmickModels(void) {
    SpGimmickModels *g;
    s32 i;
    static const char *const names[5] = {"mod__ss_dash_p__ss_dash_p.dcm", "mod__ss_switch01__ss_switch01.dcm",
                                         "mod__ss_switch02__ss_switch02.dcm", "mod__ss_eme_light__ss_eme_light.dcm",
                                         NULL};
    char eme[64];
    g = (SpGimmickModels *)calloc(1, sizeof(SpGimmickModels));
    gSpecialStageGimmickModels = g;
    g->unk0 = (SpSprRes *)calloc(1, sizeof(SpSprRes));
    sp_sprres_load(g->unk0, "act__ac_spst_gmk");          /* act/ac_spst_gmk.bac */
    load_matanim(&g->unkEC4, "mod__ss.dcp", 3); /* mod/ss.nsbtp */
    /* Model_InitSprite3D(&objs[k], gmk, anim): 0:0 1:2 5:7 6:8 3:4 4:3 7:5 8:6 2:1 */
    Model_InitSprite3D(&g->objs[0], g->unk0, 0);
    Model_InitSprite3D(&g->objs[1], g->unk0, 2);
    Model_InitSprite3D(&g->objs[5], g->unk0, 7);
    Model_InitSprite3D(&g->objs[6], g->unk0, 8);
    Model_InitSprite3D(&g->objs[3], g->unk0, 4);
    Model_InitSprite3D(&g->objs[4], g->unk0, 3);
    Model_InitSprite3D(&g->objs[7], g->unk0, 5);
    Model_InitSprite3D(&g->objs[8], g->unk0, 6);
    Model_InitSprite3D(&g->objs[2], g->unk0, 1);
    for (i = 0; i < 9; i++) {
        Model_Animate(&g->objs[i]);
        g->objs[i].spr.flags |= 0x14;   /* unkBC */
        g->objs[i].unkDC |= 0x8000;
    }
    snprintf(eme, sizeof(eme), "mod__ss_eme%02d__ss_eme%02d.dcm", (int)SpecialStage_GetStageIndex(),
             (int)SpecialStage_GetStageIndex());
    for (i = 0; i < 5; i++) {
        sp_mdl_load(&g->res[i], names[i] ? names[i] : eme);
        Model_Init(&g->anims[i], 0);
        Model_SetResource(&g->anims[i], &g->res[i]);
    }
    Model_SetAnimation(&g->anims[3], 2, &g->unkEC4, 0);
    g->anims[3].flags[2] |= 4;   /* unk10E */
}

void SpecialStage_ReleaseGimmickModels(void) {
    SpGimmickModels *g = gSpecialStageGimmickModels;
    int i;
    if (!g)
        return;
    for (i = 0; i < 5; i++)
        sp_mdl_free(&g->res[i]);
    sp_anm_free(&g->unkEC4);
    sp_sprres_free(g->unk0);
    free(g->unk0);
    free(g);
    gSpecialStageGimmickModels = NULL;
}

void SpecialStage_LoadActArchive(void) {
    SpActWork *w;
    s32 i;
    w = (SpActWork *)calloc(1, sizeof(SpActWork));
    data_ov001_0231f5b8 = w;
    /* data_ov001_0231f5c4 + 0x28: act/ac_spst_efct.bac */
    for (i = 0; i < 32; i++) {
        Model_InitSprite3D(&w->obj[i], &data_ov001_0231f5c4->spr[3], 0);
        w->obj[i].unkDC |= 0x8000;
    }
    for (i = 0; i < 32; i++)
        Model_Init(&w->obj2[i], 0);
    /* narc/ss_act_lz7.narc: 0 eff_clash.nsbmd 1 .nsbca 2 .nsbma 3 eff_shoot_bomb.nsbmd 4 .nsbca 5 .nsbma 6 .nsbtp */
    sp_mdl_load(&w->res0, "narc__ss_act_lz7__eff_clash__eff_clash.dcm");
    sp_anm_load(&w->res1, "narc__ss_act_lz7__eff_clash.dca", 0);
    sp_mdl_load(&w->res3, "narc__ss_act_lz7__eff_shoot_bomb__eff_shoot_bomb.dcm");
    sp_anm_load(&w->res4, "narc__ss_act_lz7__eff_shoot_bomb.dca", 0);
    load_matanim(&w->res2, "narc__ss_act_lz7__eff_clash.dcc", 2);
    load_matanim(&w->res5, "narc__ss_act_lz7__eff_shoot_bomb.dcc", 2);
    load_matanim(&w->res6, "narc__ss_act_lz7__eff_shoot_bomb.dcp", 3);
    w->task = SpecialStageObjList_Create(0x20);
    w->task->unk1DC = SpecialStage_ActObjDestroyWhenDone;
    w->task->unk1E0 = SpecialStage_ActObjReleaseSlots;
}

void SpecialStage_ReleaseActArchive(void) {
    SpActWork *w = data_ov001_0231f5b8;
    if (!w)
        return;
    SpecialStageObjList_Destroy(w->task);
    sp_mdl_free(&w->res0);
    sp_mdl_free(&w->res3);
    sp_anm_free(&w->res1);
    sp_anm_free(&w->res2);
    sp_anm_free(&w->res4);
    sp_anm_free(&w->res5);
    sp_anm_free(&w->res6);
    free(w);
    data_ov001_0231f5b8 = NULL;
}

/* ------------------------------------------------------------ object list */
SpObjList *SpecialStageObjList_Create(s32 n) {
    SpObjList *g;
    s32 i;
    g = (SpObjList *)calloc(1, sizeof(SpObjList));
    g->ents = (SpObj *)calloc((size_t)n, sizeof(SpObj));
    g->ptrs = (SpObj **)calloc((size_t)n, sizeof(SpObj *));
    g->count = (u16)n;
    g->unk2 = 0;
    g->unk4 = 0;
    g->unk6 = 0;
    g->head.next = &g->tail;   /* unkEC = unkF4 */
    g->head.prev = NULL;
    g->tail.next = NULL;
    g->tail.prev = &g->head;   /* unk1D4 = unk10 */
    for (i = 0; i < g->count; i++)
        g->ptrs[i] = &g->ents[i];
    return g;
}

void SpecialStageObjList_Destroy(SpObjList *g) {
    SpObj *n, *nx;
    if (!g)
        return;
    for (n = g->head.next; n != &g->tail; n = nx) {
        nx = n->next;
        SpecialStageObjList_Free(g, n);
    }
    free(g->ptrs);
    free(g->ents);
    free(g);
}

SpObj *SpecialStageObjList_Alloc(SpObjList *g) {
    SpObj *n;
    if (g->unk2 >= g->count)
        return NULL;
    g->unk2++;
    n = g->ptrs[g->unk4];
    g->unk4++;
    if (g->unk4 >= g->count)
        g->unk4 = 0;
    memset(n, 0, 0xE4 > sizeof(SpObj) ? sizeof(SpObj) : sizeof(SpObj));
    n->unk70 = 99999;
    n->next = &g->tail;
    n->prev = g->tail.prev;
    g->tail.prev->next = n;
    g->tail.prev = n;
    return n;
}

void SpecialStageObjList_Free(SpObjList *g, SpObj *n) {
    if (g->unk1E0 != NULL)
        g->unk1E0(n);
    n->prev->next = n->next;
    n->next->prev = n->prev;
    g->ptrs[g->unk6] = n;
    g->unk6++;
    if (g->unk6 >= g->count)
        g->unk6 = 0;
    g->unk2--;
}

void SpecialStageObjList_Update(SpObjList *g) {
    SpObj *o, *r, *nx;
    for (o = g->head.next; o != &g->tail; o = o->next)
        SpecialStage_UpdateObject(g, o);
    for (o = g->head.next; o != &g->tail; o = nx) {
        int freed = 0;
        nx = o->next;
        if ((o->unk64 & 2) && o->unkD4 != NULL && (o->unkD4->unk60 & 1)) {
            SpecialStageObjList_Free(g, o);
            freed = 1;
        }
        /* the original tests the second rule on the node it may just have freed (links kept) */
        if ((o->unk64 & 0x20) && (r = o->unkD8) != NULL && (r != o->unkD4 || !(o->unk64 & 2)) &&
            (r->unk60 & 1) && !freed)
            SpecialStageObjList_Free(g, o);
    }
    for (o = g->head.next; o != &g->tail; o = nx) {
        nx = o->next;
        if (o->unk60 & 1)
            SpecialStageObjList_Free(g, o);
    }
}

BOOL SpecialStageObjList_IsCulled(SpObj *p) {
    s32 sx, sy;
    VecFx32 out;
    if (p->pos.z < -0x258000)
        return TRUE;
    if (p->pos.x > 0x64000 || p->pos.x < -0x64000)
        return TRUE;
    MTX_MultVec43(&p->pos, &g_sp_view, &out);
    if (out.z != 0 && !NNS_G3dWorldPosToScrPos(&p->pos, &sx, &sy))
        return FALSE;
    if (p->pos.z > 0x19000)
        return TRUE;
    if (p->pos.y > 0xc8000 || p->pos.y < -0xc8000)
        return TRUE;
    return FALSE;
}

static void set_rot(MtxFx33 *rot, const SpObj *e) {
    MtxFx33 a, b, c;
    s32 idx;
    idx = (s32)(e->unk12 & 0xffff) >> 4;
    MTX_RotY33_(&a, SIN_IDX(idx), COS_IDX(idx));
    idx = (s32)(e->unk10 & 0xffff) >> 4;
    MTX_RotX33_(&b, SIN_IDX(idx), COS_IDX(idx));
    idx = (s32)(((e->unk64 & 0x2000) ? e->unk16 : e->unk14) & 0xffff) >> 4;
    MTX_RotZ33_(&c, SIN_IDX(idx), COS_IDX(idx));
    MTX_Concat33(&a, &b, rot);
    MTX_Concat33(rot, &c, rot);
}

void SpecialStageObjList_Draw(SpObjList *p) {
    SpObj *e;
    s32 idx;
    for (e = p->head.next; e != &p->tail; e = e->next) {
        if (e->unk60 & 1)
            continue;
        if (e->unk60 & 4)
            continue;
        if (SpecialStageObjList_IsCulled(e) == 1)
            continue;
        if (e->unkAC) {
            e->unkAC->scale[0] = e->scale.x;
            e->unkAC->scale[1] = e->scale.y;
            e->unkAC->scale[2] = e->scale.z;
            if (!(e->unk60 & 0x40)) {
                e->unkAC->trans[0] = e->pos.x;
                e->unkAC->trans[1] = e->pos.y;
                e->unkAC->trans[2] = e->pos.z;
                set_rot(&e->unkAC->rot, e);
            }
            sp_draw_model(e->unkAC, 0);
        }
        if (e->unkB0) {
            if (e->unk64 & 0x4000) {
                e->unkB0->cb[0] = 0x13;
                e->unkB0->cb[1] = 3;
                e->unkB0->cb[2] = 0;
            }
            e->unkB0->scale[0] = e->scale.x;
            e->unkB0->scale[1] = e->scale.y;
            e->unkB0->scale[2] = e->scale.z;
            if (!(e->unk60 & 0x40)) {
                idx = (s32)(((e->unk64 & 0x2000) ? e->unk16 : e->unk14) & 0xffff) >> 4;
                MTX_RotZ33_(&e->unkB0->rot, SIN_IDX(idx), COS_IDX(idx));
                e->unkB0->trans[0] = e->pos.x;
                e->unkB0->trans[1] = e->pos.y;
                e->unkB0->trans[2] = e->pos.z;
            }
            sp_draw_model(e->unkB0, 0);
            /* the cb change stays on the shared model (as on the DS) */
        }
    }
}

BOOL SpecialStageObj_IsInReach(SpObj *unused, SpObj *p) {
    (void)unused;
    if (p->unk60 & 0x100)
        return FALSE;
    if (p->pos.z < -0x82000)
        return FALSE;
    if (p->pos.z > -0xa000)
        return FALSE;
    if (p->pos.x > 0x1e000)
        return FALSE;
    if (p->pos.x < -0x1e000)
        return FALSE;
    return p->pos.y >= -0x19000;
}

void SpecialStageObj_MarkDestroyed(SpObj *p) { p->unk60 |= 1; }

/* ------------------------------------------------------------ gimmick manager */
SpObjList **SpecialStageGimmick_CreateManager(s32 a) {
    SpObjList **w = (SpObjList **)calloc(1, sizeof(SpObjList *));
    *w = SpecialStageObjList_Create(a);
    (*w)->unk1DC = (void (*)(SpObj *))sp_func(0x0230f0f4);   /* SpecialStageGimmick_Unk_ov001_0230f0f4 */
    return w;
}

void SpecialStageGimmick_DestroyManager(SpObjList **w) {
    if (!w)
        return;
    SpecialStageObjList_Destroy(*w);
    free(w);
}

void SpecialStageGimmick_Draw(SpObjList **p) { SpecialStageObjList_Draw(*p); }

void SpecialStageGimmick_Update(SpObjList **p) {
    SpGimmickModels *g = gSpecialStageGimmickModels;
    s32 i;
    for (i = 0; i < 9; i++)
        Model_Animate(&g->objs[i]);
    for (i = 0; i < 5; i++)
        Model_Animate(&g->anims[i]);
    SpecialStageObjList_Update(*p);
}

void SpecialStageGimmick_Destroy(SpObj *p) { SpecialStageObj_MarkDestroyed(p); }

/* gimmick type table (gSpecialStageGimmickTypeTable, 0x40 per type) */
#define GT(type) (0x023172cc + (u32)(type) * 0x40)

/* the RAM tables data_ov001_0231f670[9] (= &objs[i]) and data_ov001_0231f65c[5] (= &anims[i]) */
static SpModel *model_slot(u32 ptr) {
    SpGimmickModels *g = gSpecialStageGimmickModels;
    if (ptr >= 0x0231f670 && ptr < 0x0231f670 + 9 * 4)
        return &g->objs[(ptr - 0x0231f670) / 4];
    if (ptr >= 0x0231f65c && ptr < 0x0231f65c + 5 * 4)
        return &g->anims[(ptr - 0x0231f65c) / 4];
    return NULL;
}
/* 3D circles {x, y, z, r} (16 bytes); the tap circles @0xC4 are {x, y, r} (12 bytes) and are read
 * as s32[3] through the same pointer */
static const SpCircle *circle(u32 ptr) { return ptr ? (const SpCircle *)sp_rom(ptr, 12) : NULL; }

BOOL SpecialStageGimmick_Unk_ov001_02310144(SpObj *p) {
    if (p->kind == 2) {
        switch (p->unk68) {
        case 9:
        case 10:
            return TRUE;
        }
    }
    return FALSE;
}

void SpecialStageGimmick_CalcPipeMatrix(MtxFx43 *m, s32 mode, s32 flag, s32 force, u32 ang, s32 y, s32 z) {
    MtxFx43 t, t2;
    s32 idx;
    if (force == 1 || (flag != 0 && (ang < 0x38e3 || ang > 0xc71c))) {
        MTX_Identity43_(m);
        idx = (s32)(ang & 0xffff) >> 4;
        MTX_RotZ43_(&t, SIN_IDX(idx), COS_IDX(idx));
        MTX_Concat43(&t, m, m);
        MTX_TransApply43(m, m, 0, y - 0x1e000, 0);
    } else {
        MTX_Identity43_(m);
        MTX_TransApply43(m, m, 0, y, 0);
        idx = (s32)(ang & 0xffff) >> 4;
        MTX_RotZ43_(&t, SIN_IDX(idx), COS_IDX(idx));
        MTX_Concat43(&t, m, m);
        MTX_TransApply43(m, m, 0, -0x1e000, 0);
    }
    switch (mode) {
    case 0:
    case 5:
    case 6:
        MTX_TransApply43(m, m, 0, 0, -z);
        break;
    case 1:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxYAroundPivot(&t2, -0xbf000, SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    case 2:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxYAroundPivot(&t2, 0xbf000, (u16)-SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    case 3:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxXAroundPivot(&t2, 0xbf000, SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    case 4:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxXAroundPivot(&t2, -0xbf000, (u16)-SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    }
}

SpObj *SpecialStageGimmick_Spawn(SpObjList **g, void *parent, s32 type, s32 arg, SpObj *link, u16 ang, s32 y, s32 z) {
    SpObj *n = SpecialStageObjList_Alloc(*g);
    u32 d = GT(type);
    s32 flag;
    if (n == NULL)
        return NULL;   /* the original writes through NULL here (list full never happens) */
    n->kind = 2;
    n->unk16 = ang;
    n->unk18 = y;
    n->unk1C = z;
    if (type != 0) {
        if (type == 0x16)
            n->unk18 += 0x800;
    } else
        n->unk18 += 0x1800;
    n->scale = *SP_ROM(d + 4, VecFx32);
    n->unk5C = sp_rom_s16(d + 0);
    n->unk5E = sp_rom_u16(d + 2);
    n->unk68 = (s16)type;
    n->unk6A = (s16)arg;
    n->unk70 = -1;
    if (z >= 0x12c000) {
        n->unk78 = ((SpSeg *)parent)->next;   /* parent->unk38 */
        n->unk1C -= 0x12c000;
    } else
        n->unk78 = parent;
    n->unkB0 = model_slot(sp_rom_u32(d + 0x14));
    n->unkAC = model_slot(sp_rom_u32(d + 0x18));
    n->unk60 = 0x10;
    n->unk64 = (u16)sp_rom_u32(d + 0x10);
    if (n->unk78 != NULL) {
        flag = (n->unk64 & 0x2000) ? 1 : 0;
        SpecialStageGimmick_CalcPipeMatrix(&n->unk7C, ((SpSeg *)n->unk78)->type, flag,
                                           SpecialStageGimmick_Unk_ov001_02310144(n), n->unk16, n->unk18, n->unk1C);
    } else
        n->unk60 |= 0x20;
    n->unkB4 = circle(sp_rom_u32(d + 0x1c));
    n->unkB8 = sp_rom_s32(d + 0x20);
    n->unkBC = circle(sp_rom_u32(d + 0x24));
    n->unkC0 = sp_rom_s32(d + 0x28);
    n->unkC4 = circle(sp_rom_u32(d + 0x2c));
    n->unkC8 = sp_rom_s32(d + 0x30);
    n->unkCC = (void (*)(SpObj *))sp_func(sp_rom_u32(d + 0x34));
    n->unkD4 = link;
    if (n->unkD4 != NULL)
        n->unkD8 = link->unkD8;
    else
        n->unkD8 = n;
    switch (type) {
    case 4:
        n->unk12 = 0;
        break;
    case 5:
        n->unk12 = 0x71c;
        break;
    case 6:
        n->unk12 = 0xf8e4;
        break;
    case 0x17:
        n->unk10 = 0xc000;
        break;
    }
    return n;
}

SpObj *SpecialStageGimmick_SpawnActObjAt(void *parent, s32 type, u16 ang, s32 y, s32 z) {
    SpObj *n = SpecialStageActObj_CreateByType(parent, (u32)type);
    s32 flag;
    if (n == NULL)
        return NULL;
    n->unk16 = ang;
    n->unk18 = y;
    n->unk1C = z;
    if (z >= 0x12c000) {
        n->unk78 = ((SpSeg *)parent)->next;
        n->unk1C -= 0x12c000;
    }
    if (parent != NULL) {
        flag = (n->unk64 & 0x2000) ? 1 : 0;
        SpecialStageGimmick_CalcPipeMatrix(&n->unk7C, ((SpSeg *)n->unk78)->type, flag,
                                           SpecialStageGimmick_Unk_ov001_02310144(n), n->unk16, n->unk18, n->unk1C);
    }
    return n;
}

void SpecialStageGimmick_SpawnTrickReward(SpObjList **a, s32 x, s32 y, s32 z) {
    SpObj *o;
    (void)a;
    o = SpecialStageGimmick_Spawn(SpecialStage_GetGimmickManager(), NULL, 0, 0, NULL, 0, 0, 0);
    if (o == NULL)
        return;
    o->pos.x = x;
    o->pos.y = y;
    o->pos.z = z;
    o->scale.x = 0x155;
    o->scale.y = 0x155;
    o->scale.z = 0x155;
    o->unk64 |= 0x4000;
    o->unkCC = SpecialStageTrickReward_Update;
    o->unkBC = SP_ROM(0x0231ea5c, SpCircle);
}

void SpecialStageGimmick_OnAttacked(SpObj *p) {
    u32 e;
    SpFn init, fn;
    p->unk5C -= 1;
    if (p->unk5C > 0) {
        SpecialStageActObj_SpawnAt(0, p->pos.x, p->pos.y, p->pos.z);
        p->unk74 = 0x1e;
        Sound_PlaySeqArc_SS(0x10, 0x18);
        return;
    }
    e = GT(p->unk68);
    init = sp_func(sp_rom_u32(e + 0x38));
    fn = sp_func(sp_rom_u32(e + 0x3c));
    if (init != NULL)
        init(p);
    if (fn != NULL) {
        p->unkCC = (void (*)(SpObj *))fn;
        return;
    }
    SpecialStageGimmick_Destroy(p);
}

void SpecialStageGimmick_OnDestroyGiveRings(SpObj *p) {
    SpecialStageActObj_SpawnAt(1, p->pos.x, p->pos.y, p->pos.z);
    Sound_PlaySeqArc_SS(0x10, 0x11);
    SpecialStage_GetPlayer()->unk148 += p->unk5E;
    g_sp_trace.attacks++;
}

SpObj *SpecialStageGimmick_FindTouched(SpObj *a, SpObjList *list, s32 x, s32 y) {
    SpObj *o;
    VecFx32 p;
    s32 sx, sy;
    for (o = list->head.next; o != &list->tail; o = o->next) {
        if ((o->unk60 & 9) == 0 && o->unkC4 != NULL && (o->unk60 & 0x100) == 0 && SpecialStageObj_IsInReach(a, o)) {
            NNS_G3dWorldPosToScrPos(&o->pos, &sx, &sy);
            p.x = sx << 12;
            p.y = sy << 12;
            p.z = 0;
            /* SpecialStageGimmick_TestTouchCircle */
            if (SpecialStage_IsPointInCircle((const s32 *)o->unkC4, &p, x << 12, y << 12))
                return o;
        }
    }
    return NULL;
}

/* controller scheme (SpecialStagePlayer_CheckTouchTarget): the gimmick A attacks.  The same
 * gimmicks FindTouched accepts (not destroyed, with a touch circle, attackable, in reach) and on
 * screen; of those the nearest one (largest z: the course comes towards the camera), then the
 * one closest to Sonic across the pipe */
SpObj *SpecialStageGimmick_FindAttackTarget(SpObj *a, SpObjList *list) {
    SpObj *o, *best = NULL;
    s32 sx, sy, dx, bdx = 0;
    for (o = list->head.next; o != &list->tail; o = o->next) {
        if ((o->unk60 & 9) != 0 || o->unkC4 == NULL || (o->unk60 & 0x100) != 0 || !SpecialStageObj_IsInReach(a, o))
            continue;
        if (NNS_G3dWorldPosToScrPos(&o->pos, &sx, &sy) != 0)
            continue;
        dx = o->pos.x - a->pos.x;
        if (dx < 0)
            dx = -dx;
        if (!best || o->pos.z > best->pos.z || (o->pos.z == best->pos.z && dx < bdx)) {
            best = o;
            bdx = dx;
        }
    }
    return best;
}

/* SpecialStageGimmick_Unk_ov001_0230e634: free fall (bomb / enemy defeat) */
void SpecialStageGimmick_Unk_ov001_0230e634(SpObj *p) {
    VecFx32 *v = &p->mvel, *a = &p->macc;
    v->x += a->x;
    v->y += a->y;
    v->z += a->z;
    p->pos.x += v->x;
    p->pos.y += v->y;
    p->pos.z += v->z;
    if (p->pos.z > 0xf000)
        SpecialStageGimmick_Destroy(p);
}
void SpecialStageGimmick_Unk_ov001_0230e78c(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e6c0(SpObj *p) {
    p->pos.x += p->mvel.x;
    p->pos.y += p->mvel.y;
    p->pos.z += p->mvel.z;
    if (p->unkD0 >= 0x14) {
        p->unkCC = SpecialStageGimmick_Unk_ov001_0230e78c;
        p->unkD0 = 0;
    }
}
void SpecialStageGimmick_Unk_ov001_0230e710(SpObj *p) {
    p->pos.x += p->mvel.x;
    p->pos.y += p->mvel.y;
    p->pos.z += p->mvel.z;
    if (p->unkD0 < 0x14)
        return;
    p->mvel.x *= -1;
    p->mvel.y *= -1;
    p->mvel.z *= -1;
    p->unkCC = SpecialStageGimmick_Unk_ov001_0230e6c0;
    p->unkD0 = 0;
}
/* rewritten from the asm fallback (src/asm_fallback/SpecialStageGimmick_Unk_ov001_0230e78c.c):
 * hover on a sine (y += sin(timer * 0x10000 / 120) * 0.3f, float), then after 120 frames
 * dive at the player (1/20 of the distance per frame), after 300 frames fall away */
void SpecialStageGimmick_Unk_ov001_0230e78c(SpObj *p) {
    float f = (float)p->pos.y;
    s32 a = (s32)((s32)(p->unkD0 << 16) / 120);
    f = f + (float)SIN((u32)a & 0xffff) * 0.3f;
    p->pos.y = (s32)f;
    if (p->unk6C > 0x12c) {
        p->mvel.x = 0;
        p->mvel.y = 0;
        p->mvel.z = 0;
        p->macc.x = 0;
        p->macc.y = 0x200;
        p->macc.z = 0x200;
        p->unkCC = SpecialStageGimmick_Unk_ov001_0230e634;
        p->unkD0 = 0;
        return;
    }
    if (p->unkD0 < 0x78)
        return;
    {
        SpObj *pl = SpecialStage_GetPlayer();
        p->mvel.x = (pl->unkE4.x - p->pos.x) / 20;
        p->mvel.y = (pl->unkE4.y - p->pos.y) / 20;
        p->mvel.z = (pl->unkE4.z - p->pos.z) / 20;
        p->unkCC = SpecialStageGimmick_Unk_ov001_0230e710;
        p->unkD0 = 0;
    }
}
void SpecialStageGimmick_Unk_ov001_0230e8fc(SpObj *p) {
    p->pos.x += p->mvel.x;
    p->pos.y += p->mvel.y;
    p->pos.z += p->mvel.z;
    if (p->unkD0 >= 0x14) {
        p->unkCC = SpecialStageGimmick_Unk_ov001_0230e78c;
        p->unkD0 = 0;
    }
}
/* rewritten from the asm fallback (src/asm_fallback/SpecialStageGimmick_Unk_ov001_0230e94c.c):
 * when its segment becomes the current one, leave the pipe and fly to (0, -0xa000, -0x32000)
 * in 20 frames */
void SpecialStageGimmick_Unk_ov001_0230e94c(SpObj *p) {
    if ((void *)SpecialStageMap_GetCurrentSegment() != p->unk78)
        return;
    if (p->pos.z <= 0)
        return;
    p->unk60 |= 0x20;
    p->unk78 = NULL;
    p->mvel.x = (-p->pos.x) / 20;
    p->mvel.y = (-0xa000 - p->pos.y) / 20;
    p->mvel.z = (-0x32000 - p->pos.z) / 20;
    p->unkCC = SpecialStageGimmick_Unk_ov001_0230e8fc;
    p->unkD0 = 0;
    p->unk6C = 0;
}
void SpecialStageGimmick_SwayRotation(SpObj *p) {
    s32 d = p->unkD0 - p->unk1C / 0x5000;
    u16 a;
    if (d < 0)
        return;
    a = (u16)(d * 800);
    p->unk16 += COS(a) / 8;
}
void SpecialStageGimmick_SwayX(SpObj *p) {
    s32 d = p->unkD0 - p->unk1C / 0x5000;
    u16 a;
    if (d < 0)
        return;
    a = (u16)(d * 800);
    p->unk18 += COS(a);
}
void SpecialStageGimmick_Unk_ov001_0230ed48(SpObj *p) {
    SpecialStageGimmick_Unk_ov001_02312dc8(SpecialStage_GetPlayer(), p);
    if (p->unkD0 % 17 == 1) {
        u16 r1 = Rand(), r2, r3;
        s32 x = p->pos.x + r1 % 0x4000 - 0x2000;
        r2 = Rand();
        r3 = Rand();
        SpecialStageActObj_SpawnAt(2, x, p->pos.y + r2 % 0x4000 - 0x2000, p->pos.z + r3 % 0x4000 - 0x2000);
    }
}
void SpecialStageGimmick_Unk_ov001_0230ee44(SpObj *p) {
    SpObj *r;
    u16 v;
    float f;
    v = Rand();
    Rand();
    f = (float)p->unk1C;
    r = SpecialStageGimmick_SpawnActObjAt(p->unk78, 2, (u16)(p->unk16 + (u16)(gRandSeed >> 16) % 0x2000 - 0x1000), 0,
                                          (s32)(f + 143360.0f * ((float)v / 65535.0f) - 69632.0f));
    if (r == NULL)
        return;
    r->vel.y = 0x800;
    r->unk64 &= ~0x1000;
    r->unk64 &= ~0x2000;
    r->unk60 &= ~0x20;
}
void SpecialStageGimmick_Unk_ov001_0230ef64(SpObj *p) {
    p->unk60 |= 4;
    if (p->pos.z <= 0)
        return;
    SpecialStageGimmick_Unk_ov001_02309994();
    SpecialStageGimmick_Destroy(p);
}
void SpecialStageGimmick_Unk_ov001_0230ef9c(SpObj *p) {
    p->unk60 |= 4;
    if (p->pos.z <= 0)
        return;
    SpecialStageGimmick_Unk_ov001_023099c4();
    SpecialStageGimmick_Destroy(p);
}
void SpecialStageGimmick_Unk_ov001_0230f0f4(SpObj *p) {
    SpObj *r;
    if (p->unkC4 != 0) {
        if (SpecialStageObj_IsInReach(SpecialStage_GetPlayer(), p) == 1) {
            if (!(p->unk62 & 1) && (r = SpecialStageActObj_SpawnAt(5, 0, 0, 0)) != NULL) {
                r->unkD4 = p;
                r->unkD8 = p->unkD8;
                r->unk64 |= 2;
            }
            p->unk62 |= 1;
        } else
            p->unk62 &= ~1;
    }
    if (p->unk5C <= 0)
        return;
    if (p->unk60 & 0x100) {
        if (p->unk6C & 1)
            p->unk60 |= 4;
        else
            p->unk60 &= ~4;
        p->unk60 |= 8;
    } else {
        p->unk60 &= ~4;
        p->unk60 &= ~8;
    }
}
void SpecialStageGimmick_Unk_ov001_02312dc8(SpObj *p, SpObj *v) { p->unk11C = v; }

void SpecialStageTrickReward_Update(SpObj *p) {
    VecFx32 d;
    s32 len;
    VEC_Subtract(&SpecialStage_GetPlayer()->pos, &p->pos, &d);
    len = VEC_Mag(&d);
    if (len > 0x3000) {
        d.x = FX_Div(d.x, len);
        d.y = FX_Div(d.y, len);
        d.z = FX_Div(d.z, len);
        d.x = FX_Mul(d.x, 0x3000);
        d.y = FX_Mul(d.y, 0x3000);
        d.z = FX_Mul(d.z, 0x3000);
    }
    p->pos.x += d.x;
    p->pos.y += d.y;
    p->pos.z += d.z;
}

/* ------------------------------------------------------------ act objects */
SpObj *SpecialStageActObj_CreateByType(void *arg, u32 type) {
    SpActWork *g = data_ov001_0231f5b8;
    SpModel *o = NULL, *an = NULL;
    SpObj *n;
    u32 e = 0x02316fe4 + type * 0x18;   /* data_ov001_02316fe4 */
    s32 i;
    if (type <= 1) {
        for (i = 0; i < 0x20; i++)
            if (g->used2[i] == 0) {
                an = &g->obj2[i];
                g->used2[i] = 1;
                an->speed = 0x1000;
                break;
            }
    } else {
        for (i = 0; i < 0x20; i++)
            if (g->used1[i] == 0) {
                o = &g->obj[i];
                g->used1[i] = 1;
                break;
            }
    }
    if (i == 0x20)
        return NULL;
    n = SpecialStageObjList_Alloc(g->task);
    if (n == NULL)
        return NULL;
    n->kind = 1;
    n->scale = *SP_ROM(e, VecFx32);
    n->unk68 = (s16)type;
    n->unk70 = sp_rom_s16(e + 0x12);
    n->unk78 = arg;
    n->unkB0 = o;
    if (o != NULL) {
        AnimatorSprite_SetAnimation(&o->spr, sp_rom_u16(e + 0x10));
        o->spr.flags &= ~0x10u;
        Model_Animate(o);
        o->spr.flags |= 0x10;
        if (sp_rom_s16(e + 0x12) > 0)
            o->spr.flags |= 4;
    }
    n->unkAC = an;
    n->unk60 = 8;
    n->unk64 = (u16)sp_rom_u32(e + 0xc);
    n->unkCC = (void (*)(SpObj *))sp_func(sp_rom_u32(e + 0x14));
    n->unkD8 = n;
    switch (type) {
    case 0:
        Model_SetResource(n->unkAC, &g->res0);
        Model_SetAnimation(n->unkAC, 0, &g->res1, 0);
        Model_SetAnimation(n->unkAC, 1, &g->res2, 0);
        break;
    case 1:
        Model_SetResource(n->unkAC, &g->res3);
        Model_SetAnimation(n->unkAC, 0, &g->res4, 0);
        Model_SetAnimation(n->unkAC, 1, &g->res5, 0);
        Model_SetAnimation(n->unkAC, 2, &g->res6, 0);
        break;
    case 4:
        n->mvel.x = Rand() % 0x2000;
        n->mvel.x -= 0x1000;
        n->mvel.y = Rand() % 0x1000;
        n->mvel.y += 0x800;
        n->macc.y = -0xd0;
        n->mvel.z = -0x800;
        n->macc.z = 0;
        break;
    }
    return n;
}

SpObj *SpecialStageActObj_SpawnAt(s32 type, s32 x, s32 y, s32 z) {
    SpObj *t = SpecialStageActObj_CreateByType(NULL, (u32)type);
    if (t == NULL)
        return NULL;
    t->pos.x = x;
    t->pos.y = y;
    t->pos.z = z;
    t->unk60 |= 0x20;
    return t;
}

void SpecialStageActObj_Destroy(SpObj *p) { SpecialStageObj_MarkDestroyed(p); }

void SpecialStage_ActObjBounceMove(SpObj *p) {
    VecFx32 v;
    p->mvel.x += p->macc.x;
    p->mvel.y += p->macc.y;
    p->mvel.z += p->macc.z;
    p->pos.x += p->mvel.x;
    p->pos.y += p->mvel.y;
    p->pos.z += p->mvel.z;
    v.x = p->pos.x;
    v.y = p->pos.y;
    v.z = 0;
    if (VEC_Mag(&v) >= 0x1c000 && p->pos.y <= 0) {
        p->mvel.x = (s32)((float)p->mvel.x * -0.98f);
        p->mvel.y = (s32)((float)p->mvel.y * -0.95f);
        p->pos.x += p->mvel.x;
        p->pos.y += p->mvel.y;
        p->pos.z += p->mvel.z;
    }
    if (p->unk70 < 0x1e) {
        if (p->unk70 & 2)
            p->unk60 |= 4;
        else
            p->unk60 &= ~4;
        return;
    }
    if (p->unk70 < 0x3c) {
        if (p->unk70 & 4)
            p->unk60 |= 4;
        else
            p->unk60 &= ~4;
    }
}

void SpecialStage_ActObjDestroyWhenDone(SpObj *p) {
    if (sp_rom_s16(0x02316ff6 + (u32)p->unk68 * 0x18) <= 0) {
        if (p->unkB0 != NULL && (p->unkB0->spr.flags & 2)) {
            SpecialStageActObj_Destroy(p);
            return;
        }
        if (p->unkAC == NULL)
            return;
        if (!(p->unkAC->flags[0] & 2))
            return;
        SpecialStageActObj_Destroy(p);
        return;
    }
    if (p->unk70 > 1)
        return;
    SpecialStageActObj_Destroy(p);
}

void SpecialStage_ActObjFollowParent(SpObj *p) {
    SpObj *q = p->unkD4;
    if (q != NULL) {
        if (q->unk62 & 1) {
            p->pos = q->pos;
            p->pos.z += 0x1000;
        } else
            SpecialStageActObj_Destroy(p);
        return;
    }
    if (!SpecialStageObj_IsInReach(SpecialStage_GetPlayer(), p))
        SpecialStageActObj_Destroy(p);
}

void SpecialStage_ActObjReleaseSlots(SpObj *o) {
    SpActWork *g = data_ov001_0231f5b8;
    s32 i;
    if (o->unkB0 != NULL)
        for (i = 0; i < 0x20; i++)
            if (&g->obj[i] == o->unkB0) {
                g->used1[i] = 0;
                break;
            }
    if (o->unkAC != NULL)
        for (i = 0; i < 0x20; i++)
            if (&g->obj2[i] == o->unkAC) {
                g->used2[i] = 0;
                return;
            }
}

void SpecialStage_ActObjSpawnGimmick(SpObj *p) {
    SpObj *r;
    if (p->unk6C != 0xa)
        return;
    r = SpecialStageGimmick_Spawn(SpecialStage_GetGimmickManager(), p->unk78, 0, 0, NULL, p->unk16, p->unk18, p->unk1C);
    if (r)
        SpecialStage_UpdateObject(*SpecialStage_GetGimmickManager(), r);
}

void SpecialStage_UpdateObjects(void) { SpecialStageObjList_Update(data_ov001_0231f5b8->task); }
void SpecialStage_DrawObjects(void) { SpecialStageObjList_Draw(data_ov001_0231f5b8->task); }

/* ------------------------------------------------------------ per-object update */
void SpecialStage_UpdateObject(SpObjList *c, SpObj *w) {
    s32 v0, n;
    if (w->unk60 & 1)
        return;
    if (w->unk60 & 2)
        return;
    if (w->unk70 == 0) {
        SpecialStageObj_MarkDestroyed(w);
        return;
    }
    if (c != NULL && c->unk1D8 != NULL)
        c->unk1D8(w);
    if (w->unk74 > 0) {
        w->unk74--;
        w->unk60 |= 0x100;
    } else
        w->unk60 &= ~0x100;
    if (w->unkCC != NULL)
        w->unkCC(w);
    if (w->unk64 & 0x100) {
        v0 = w->vel.x;
        n = v0 + w->acc.x;
        if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.x = 0;
            w->acc.x = 0;
        }
    }
    if (w->unk64 & 0x200) {
        v0 = w->vel.y;
        n = v0 + w->acc.y;
        if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.y = 0;
            w->acc.y = 0;
        }
    }
    if (w->unk64 & 0x400) {
        v0 = w->vel.z;
        n = v0 + w->acc.z;
        if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.z = 0;
            w->acc.z = 0;
        }
    }
    if (!(w->unk64 & 0x1000)) {
        w->vel.x += w->acc.x;
        w->vel.y += w->acc.y;
        w->vel.z += w->acc.z;
        if ((w->unk64 & 1) && w->unkD4 != 0) {
        } else if (w->unkD8 == w->unkD4 && (w->unk64 & 1)) {
        } else if ((w->unk64 & 0x10) && w->unkD8 != 0) {
        } else {
            w->unk16 += w->vel.x;
            w->unk18 += w->vel.y;
            w->unk1C += w->vel.z;
        }
    }
    if (!(w->unk60 & 0x10)) {
        if (w->unkAC != NULL)
            Model_Animate(w->unkAC);
        if (w->unkB0 != NULL)
            Model_Animate(w->unkB0);
    }
    if (c != NULL && c->unk1DC != NULL)
        c->unk1DC(w);
    w->unk6C++;
    w->unkD0++;
    w->unk70--;
    if (w->unk78 != NULL && (((SpSeg *)w->unk78)->flags & 1))
        SpecialStageObj_MarkDestroyed(w);
    if (w->unk60 & 0x20)
        return;
    if (w->unk78 != NULL) {
        VecFx32 v = {0, 0, 0};
        MtxFx43 m;
        s32 flag = 0;
        if (!(w->unk64 & 0x1000)) {
            if (w->unk64 & 0x2000)
                flag = 1;
            SpecialStageGimmick_CalcPipeMatrix(&w->unk7C, ((SpSeg *)w->unk78)->type, flag,
                                               SpecialStageGimmick_Unk_ov001_02310144(w), w->unk16, w->unk18, w->unk1C);
        }
        MTX_Concat43(&w->unk7C, &((SpSeg *)w->unk78)->mtx, &m);
        MTX_MultVec43(&v, &m, &v);
        w->pos = v;
    } else {
        u16 a = (u16)(w->unk16 - 0x4000);
        w->pos.x = FX_Mul(COS(a), 0x1e000);
        w->pos.y = FX_Mul(SIN(a), 0x1e000);
        w->pos.x -= FX_Mul(w->unk18, SIN(w->unk16));
        w->pos.y += FX_Mul(w->unk18, COS(w->unk16));
    }
}

BOOL SpecialStage_TestCollision(const SpCircle *a, const VecFx32 *pa, u16 angA, const SpCircle *b, const VecFx32 *pb,
                                u16 angB) {
    VecFx32 va = *pa, vb = *pb;
    s32 idx, c, s;
    idx = (s32)(angA & 0xffff) >> 4;
    c = COS_IDX(idx);
    s = SIN_IDX(idx);
    va.x += FX_Mul(a->x, c);
    va.y += FX_Mul(a->x, s);
    va.x -= FX_Mul(a->y, s);
    va.y += FX_Mul(a->y, c);
    va.z += a->z;
    idx = (s32)(angB & 0xffff) >> 4;
    c = COS_IDX(idx);
    s = SIN_IDX(idx);
    vb.x += FX_Mul(b->x, c);
    vb.y += FX_Mul(b->x, s);
    vb.x -= FX_Mul(b->y, s);
    vb.y += FX_Mul(b->y, c);
    vb.z += b->z;
    return VEC_Distance(&va, &vb) <= a->r + b->r;
}

void SpecialStage_CheckGimmickCollisions(void) {
    SpObjList **w = SpecialStage_GetGimmickManager();
    SpObj *p = SpecialStage_GetPlayer();
    SpObj *e;
    u16 ang;
    if (p->unk12C != NULL) {
        for (e = (*w)->head.next; e != &(*w)->tail; e = e->next) {
            ang = 0;
            if (p->unk12C == NULL)
                break;
            if (e->unk60 & 9)
                continue;
            if (e->unkBC == NULL)
                continue;
            if ((e->unk64 & 0x2000) && !(e->unk64 & 0x4000))
                ang = e->unk16;
            if (SpecialStage_TestCollision(p->unk12C, &p->pos, p->unk16, e->unkBC, &e->pos, ang)) {
                SpecialStage_Unk_ov001_0230f284(e, p);
                SpecialStagePlayer_OnTouchGimmick(p, e);
            }
        }
    }
    if (p->unkB4 != NULL) {
        for (e = (*w)->head.next; e != &(*w)->tail; e = e->next) {
            ang = 0;
            if (p->unkB4 == NULL)
                break;
            if (e->unk60 & 9)
                continue;
            if (e->unk60 & 0x100)
                continue;
            if (e->unkBC == NULL)
                continue;
            if ((e->unk64 & 0x2000) && !(e->unk64 & 0x4000))
                ang = e->unk16;
            if (SpecialStage_TestCollision(p->unkB4, &p->pos, p->unk16, e->unkBC, &e->pos, ang)) {
                SpecialStageGimmick_OnAttacked(e);
                SpecialStagePlayer_OnAttackHit(p);
            }
        }
    }
    if (p->unkBC != NULL) {
        for (e = (*w)->head.next; e != &(*w)->tail; e = e->next) {
            ang = 0;
            if (p->unkBC == NULL)
                break;
            if (e->unk60 & 9)
                continue;
            if (e->unkB4 == NULL)
                continue;
            if ((e->unk64 & 0x2000) && !(e->unk64 & 0x4000))
                ang = e->unk16;
            if (SpecialStage_TestCollision(p->unkBC, &p->pos, p->unk16, e->unkB4, &e->pos, ang))
                SpecialStagePlayer_OnHurt(p, e);
        }
    }
}

/* ------------------------------------------------------------ touched-gimmick reactions */
void SpecialStage_Unk_ov001_0230e3c8(SpObj *p, void *unused, u16 ang) {
    SpObj *w;
    s32 i;
    (void)unused;
    if (p->unk78 == NULL)
        return;
    w = SpecialStageGimmick_SpawnActObjAt(p->unk78, 6, ang, 0x1000, p->unk1C + 0x78000);
    if (w == NULL)
        return;
    SpecialStage_UpdateObject(NULL, w);
    for (i = 0; i < 4; i++) {
        w = SpecialStageGimmick_SpawnActObjAt(p->unk78, 6, (u16)(ang + 0x38e), 0x1000, p->unk1C + 0x8c000 + (i << 12) * 0x14);
        if (w == NULL)
            return;
        SpecialStage_UpdateObject(NULL, w);
        w = SpecialStageGimmick_SpawnActObjAt(p->unk78, 6, (u16)(ang - 0x38e), 0x1000, p->unk1C + 0x8c000 + (i << 12) * 0x14);
        if (w == NULL)
            return;
        SpecialStage_UpdateObject(NULL, w);
    }
    w = SpecialStageGimmick_SpawnActObjAt(p->unk78, 6, ang, 0x1000, p->unk1C + 0x8c000 + (i << 12) * 0x14);
    if (w == NULL)
        return;
    SpecialStage_UpdateObject(NULL, w);
    p->unkAC = &gSpecialStageGimmickModels->anims[2];   /* data_ov001_0231f65c.unk8 */
    p->unk60 |= 8;
}

void SpecialStage_Unk_ov001_0230ea6c(SpObj *s) {
    SpObj *o = SpecialStage_GetPlayer();
    s->unkAC->unk60 = o->unk1F8;
    if (s->unkD0 == 0x2a) {
        s->unk60 |= 0x20;
        s->unk60 |= 0x40;
        s->unk78 = NULL;
        s->unkAC->cb[0] = 4;
        s->unkAC->cb[1] = 0x10;
        s->unkAC->cb[2] = 0xd;
        MTX_Identity33_(&s->unkAC->rot);
        s->unkAC->trans[0] = 0;
        s->unkAC->trans[1] = 0;
        s->unkAC->trans[2] = 0;
        s->unkAC->trans2[0] = -0x18d2;
        s->unkAC->trans2[1] = -0x7fd;
        s->unkAC->trans2[2] = -0x5f1;
        Sound_PlaySeqArc_SS(9, 0);   /* data_ov001_0231f5c8 archive, SE_Shake_hands */
    }
    if (s->unkD0 > 0x2a) {
        if (s->unkD0 % 17 == 1) {
            u16 r1 = Rand(), r2, r3;
            s32 x = o->unk1F8.m[9] + r1 % 0x4000 - 0x2000;
            r2 = Rand();
            r3 = Rand();
            SpecialStageActObj_SpawnAt(2, x, o->unk1F8.m[10] + r2 % 0x5000 - 0x1000, o->unk1F8.m[11] + r3 % 0x4000 - 0x2000);
        }
    } else if (s->unkD0 % 17 == 1) {
        u16 r1 = Rand(), r2, r3;
        s32 x = s->pos.x + r1 % 0x4000 - 0x2000;
        r2 = Rand();
        r3 = Rand();
        SpecialStageActObj_SpawnAt(2, x, s->pos.y + r2 % 0x4000 - 0x2000, s->pos.z + r3 % 0x4000 - 0x2000);
    }
}

void SpecialStage_Unk_ov001_0230f284(SpObj *p, void *q) {
    SpGimmickModels *g = gSpecialStageGimmickModels;
    switch (p->unk68) {
    case 0:
    case 1:
        p->unkB0 = &g->objs[1];
        p->unk60 |= 8;
        p->unkCC = NULL;
        p->unk70 = 0x14;
        g_sp_trace.rings_collected++;
        break;
    case 2:
        p->unkB0 = &g->objs[1];
        p->unk60 |= 8;
        break;
    case 3:
        p->unkB0 = &g->objs[4];
        p->unk60 |= 8;
        break;
    case 4:
    case 5:
    case 6:
        p->unk60 |= 8;
        break;
    case 0xf:
        SpecialStage_Unk_ov001_0230e3c8(p, q, 0xd556);
        break;
    case 0x10:
        SpecialStage_Unk_ov001_0230e3c8(p, q, 0xe000);
        break;
    case 0x11:
        SpecialStage_Unk_ov001_0230e3c8(p, q, 0);
        break;
    case 0x12:
        SpecialStage_Unk_ov001_0230e3c8(p, q, 0x2000);
        break;
    case 0x13:
        SpecialStage_Unk_ov001_0230e3c8(p, q, 0x2aaa);
        break;
    case 0x17:
        p->unk60 |= 8;
        p->unkCC = SpecialStage_Unk_ov001_0230ea6c;
        p->unkD0 = 0;
        break;
    }
}
