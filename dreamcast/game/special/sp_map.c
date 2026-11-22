/*
 * sp_map.c - SpecialStageMap_* (the scrolling half-pipe): segment list,
 * scroll/curve maths (fx32 matrices as the original) and the map/sky models.
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpMap *gSpecialStageMap;
int sp_mdl_load(SpMdlRes *r, const char *file);
void sp_mdl_free(SpMdlRes *r);
void sp_mdl_unlit(SpMdlRes *r);
void sp_anm_timing(SpAnmRes *r, int kind, u16 nframes);
void sp_anm_free(SpAnmRes *r);

/* gSpecialStageMapModelList[stage][4]: ss_map_all_00 / st_mid / st_gl / sky0N */
static const char *const k_map_all[5] = {
    "mod__ss_map_all_00__ss_map_st_00.dcm", "mod__ss_map_all_00__ss_map_left_00.dcm",
    "mod__ss_map_all_00__ss_map_right_00.dcm", "mod__ss_map_all_00__ss_map_up_00.dcm",
    "mod__ss_map_all_00__ss_map_down_00.dcm"};

void SpecialStage_LoadMapModels(void) {
    SpMap *sl;
    s32 i, k;
    char sky[64];
    sl = (SpMap *)calloc(1, sizeof(SpMap));
    gSpecialStageMap = sl;
    sl->pool = (SpSeg *)calloc(8, sizeof(SpSeg));      /* @0xA40: 0x200 bytes = 8 segments */
    sl->slots = (SpSeg **)calloc(8, sizeof(SpSeg *));  /* @0xA44 */
    sl->unk38 = 0x12c05e;
    sl->unk0 = 0;
    sl->unk4 = 0;
    sl->unkA10 = 1;
    MTX_Identity33_(&sl->unkA14);
    /* res[0] = ss_map_all_00 (5 models), res[1] = ss_map_st_mid, res[2] = ss_map_st_gl, res[3] = sky */
    for (i = 0; i < 5; i++) {
        sp_mdl_load(&sl->res_all[i], k_map_all[i]);
        sp_mdl_unlit(&sl->res_all[i]);
    }
    sp_mdl_load(&sl->res[1], "mod__ss_map_st_mid__ss_map_st_mid.dcm");
    sp_mdl_load(&sl->res[2], "mod__ss_map_st_gl__ss_map_st_gl.dcm");
    snprintf(sky, sizeof(sky), "mod__ss_map_sky%02d__ss_map_sky%02d.dcm", (int)SpecialStage_GetStageIndex(),
             (int)SpecialStage_GetStageIndex());
    sp_mdl_load(&sl->res[3], sky);
    for (i = 1; i < 4; i++)
        sp_mdl_unlit(&sl->res[i]);
    /* FP(0xa0c) = ss.nsbta (sky texture scroll) */
    {
        u32 n;
        const u8 *b = (const u8 *)sp_blob("sky_srt", &n);
        sp_anm_timing(&sl->nsbta, 4, b ? *(const u16 *)b : 1);
        sl->nsbta.srt = b;
    }
    for (i = 0; i < 7; i++) {
        s32 idx = sp_rom_s32(0x023171ec + i * 4);
        s32 sub = sp_rom_s32(0x02317208 + i * 4);
        SpModel *an = &sl->models[i];
        Model_Init(an, 0);
        Model_SetResource(an, idx == 0 ? &sl->res_all[sub] : &sl->res[idx]);
        an->cb[0] = 4;   /* unk46: FlushVP */
        an->cb[1] = 2;   /* unk47: RestoreMtx */
        an->unk8 = 0x1e; /* unk44 */
    }
    Model_Init(&sl->sky, 0);
    Model_SetResource(&sl->sky, &sl->res[3]);
    Model_SetAnimation(&sl->sky, 3, &sl->nsbta, 0);
    sl->sky.flags[3] |= 4;
    sl->sky.scale[0] = 0x320000;
    sl->sky.scale[1] = 0x320000;
    sl->sky.scale[2] = 0x320000;
    sl->sky.cb[0] = 0x13;
    sl->sky.cb[1] = 3;
    sl->sky.cb[2] = 0;
    sl->cap = 8;
    sl->count = 0;
    sl->rd = 0;
    sl->wr = 0;
    sl->head.type = 0;
    sl->head.next = &sl->tail;
    sl->head.prev = NULL;
    sl->tail.next = NULL;
    sl->tail.prev = &sl->head;
    for (k = 0; k < sl->cap; k++)
        sl->slots[k] = &sl->pool[k];
}

void SpecialStage_ReleaseMapModels(void) {
    SpMap *g = gSpecialStageMap;
    int i;
    if (!g)
        return;
    for (i = 0; i < 5; i++)
        sp_mdl_free(&g->res_all[i]);
    for (i = 1; i < 4; i++)
        sp_mdl_free(&g->res[i]);
    free(g->pool);
    free(g->slots);
    free(g);
    gSpecialStageMap = NULL;
}

SpSeg *SpecialStageMap_AllocSegment(void) {
    SpMap *g = gSpecialStageMap;
    SpSeg *n;
    if (g->count >= g->cap)
        return NULL;
    g->count++;
    n = g->slots[g->rd];
    g->rd++;
    if (g->rd >= g->cap)
        g->rd = 0;
    memset(n, 0, sizeof(*n));
    n->next = &g->tail;
    n->prev = g->tail.prev;
    g->tail.prev->next = n;
    g->tail.prev = n;
    return n;
}

SpSeg *SpecialStageMap_AddSegment(u8 v) {
    SpSeg *n = SpecialStageMap_AllocSegment();
    n->type = v;   /* *SpecialStageMap_AllocSegment() = v */
    return n;
}

void SpecialStageMap_FreeSegment(SpSeg *n) {
    SpMap *g = gSpecialStageMap;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    g->slots[g->wr] = n;
    g->wr++;
    if (g->wr >= g->cap)
        g->wr = 0;
    g->count--;
}

SpSeg *SpecialStageMap_GetCurrentSegment(void) { return gSpecialStageMap->head.next; }
s32 SpecialStageMap_GetCurveLength(void) { return gSpecialStageMap->unk38; }
s32 SpecialStageMap_GetScrollDistance(void) { return gSpecialStageMap->unk0; }
s32 SpecialStageMap_GetScrollSpeed(void) { return gSpecialStageMap->unk4; }
s32 SpecialStageMap_GetStraightLength(void) { return 0x12c000; }
void SpecialStageMap_SetScrollSpeed(s32 v) { gSpecialStageMap->unk4 = v; }
BOOL SpecialStageMap_IsSegmentListEmpty(void) { return gSpecialStageMap->head.next == &gSpecialStageMap->tail; }

/* ---- segment type table (gSpecialStageMapSegmentTypeTable): length, scroll, draw,
 * pivot offset, advance, draw offset.  Types 0, 5, 6 straight (st, st_mid, st_gl),
 * 1/2 left/right curves (Y), 3/4 up/down curves (X). */
static void SpecialStageMap_ScrollStraight(void) {
    MTX_TransApply43(&gSpecialStageMap->mtx, &gSpecialStageMap->mtx, 0, 0, gSpecialStageMap->unk0);
}
static void SpecialStageMap_ScrollCurve1(void) {
    SpMap *g = gSpecialStageMap;
    u16 a = (u16)-SpecialStage_ArcLengthToAngle(0xbf000, g->unk0);
    SpecialStage_RotateMtxYAroundPivot(&g->mtx, -0xbf000, a);
}
static void SpecialStageMap_ScrollCurve2(void) {
    SpMap *g = gSpecialStageMap;
    SpecialStage_RotateMtxYAroundPivot(&g->mtx, 0xbf000, SpecialStage_ArcLengthToAngle(0xbf000, g->unk0));
}
static void SpecialStageMap_ScrollCurve3(void) {
    SpMap *g = gSpecialStageMap;
    u16 a = (u16)-SpecialStage_ArcLengthToAngle(0xbf000, g->unk0);
    SpecialStage_RotateMtxXAroundPivot(&g->mtx, 0xbf000, a);
}
static void SpecialStageMap_ScrollCurve4(void) {
    SpMap *g = gSpecialStageMap;
    SpecialStage_RotateMtxXAroundPivot(&g->mtx, -0xbf000, SpecialStage_ArcLengthToAngle(0xbf000, g->unk0));
}
/* data_0206f470 = sin/cos of 0x4000, data_02071470 = sin/cos of 0xC000 */
#define S90 SIN(0x4000)
#define C90 COS(0x4000)
#define S270 SIN(0xC000)
#define C270 COS(0xC000)
static void SpecialStageMap_StraightAdvance(MtxFx43 *m) { MTX_TransApply43(m, m, 0, 0, -0x12c000); }
static void SpecialStageMap_StraightDrawOffset(MtxFx43 *m) { MTX_TransApply43(m, m, 0, 0, 0x12c000); }
static void SpecialStageMap_StraightPivotOffset(MtxFx43 *m) { (void)m; }
static void SpecialStageMap_Curve1Advance(MtxFx43 *m) {
    MtxFx43 t;
    MTX_RotY43_(&t, S90, C90);
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, 0xbf000, 0, 0);
}
static void SpecialStageMap_Curve1DrawOffset(MtxFx43 *m) {
    MtxFx43 t;
    MTX_TransApply43(m, m, -0xbf000, 0, 0);
    MTX_RotY43_(&t, S270, C270);
    MTX_Concat43(&t, m, m);
}
static void SpecialStageMap_Curve1PivotOffset(MtxFx43 *m) { MTX_TransApply43(m, m, -0xbf000, 0, 0); }
static void SpecialStageMap_Curve2Advance(MtxFx43 *m) {
    MtxFx43 t;
    MTX_RotY43_(&t, S270, C270);
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, -0xbf000, 0, 0);
}
static void SpecialStageMap_Curve2DrawOffset(MtxFx43 *m) {
    MtxFx43 t;
    MTX_TransApply43(m, m, 0xbf000, 0, 0);
    MTX_RotY43_(&t, S90, C90);
    MTX_Concat43(&t, m, m);
}
static void SpecialStageMap_Curve2PivotOffset(MtxFx43 *m) { MTX_TransApply43(m, m, 0xbf000, 0, 0); }
static void SpecialStageMap_Curve3Advance(MtxFx43 *m) {
    MtxFx43 t;
    MTX_RotX43_(&t, S90, C90);
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, 0, -0xbf000, 0);
}
static void SpecialStageMap_Curve3DrawOffset(MtxFx43 *m) {
    MtxFx43 t;
    MTX_TransApply43(m, m, 0, 0xbf000, 0);
    MTX_RotX43_(&t, S270, C270);
    MTX_Concat43(&t, m, m);
}
static void SpecialStageMap_Curve3PivotOffset(MtxFx43 *m) { MTX_TransApply43(m, m, 0, 0xbf000, 0); }
static void SpecialStageMap_Curve4Advance(MtxFx43 *m) {
    MtxFx43 t;
    MTX_RotX43_(&t, S270, C270);
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, 0, 0xbf000, 0);
}
static void SpecialStageMap_Curve4DrawOffset(MtxFx43 *m) {
    MtxFx43 t;
    MTX_TransApply43(m, m, 0, -0xbf000, 0);
    MTX_RotX43_(&t, S90, C90);
    MTX_Concat43(&t, m, m);
}
static void SpecialStageMap_Curve4PivotOffset(MtxFx43 *m) { MTX_TransApply43(m, m, 0, -0xbf000, 0); }

typedef struct {
    s32 (*fn0)(void);
    void (*fn4)(void);
    int draw;                       /* SpecialStageMap_DrawSegmentTypeN: model index */
    void (*fnC)(MtxFx43 *m);        /* pivot offset (Update) / draw offset in Draw: see below */
    void (*fn10)(MtxFx43 *m);
    void (*fn14)(MtxFx43 *m);
} SpSegType;
/* the ROM table (0x02317224): fn0 length, fn4 scroll, fn8 draw, fnC pivot offset,
 * fn10 advance, fn14 draw offset.  (Update calls fnC + fn10 per segment, Draw calls
 * fnC; data_ov001_02317238 = fn14 of the last passed segment type.) */
static const SpSegType k_seg[7] = {
    {SpecialStageMap_GetStraightLength, SpecialStageMap_ScrollStraight, 0, SpecialStageMap_StraightPivotOffset,
     SpecialStageMap_StraightAdvance, SpecialStageMap_StraightDrawOffset},
    {SpecialStageMap_GetCurveLength, SpecialStageMap_ScrollCurve1, 1, SpecialStageMap_Curve1PivotOffset,
     SpecialStageMap_Curve1Advance, SpecialStageMap_Curve1DrawOffset},
    {SpecialStageMap_GetCurveLength, SpecialStageMap_ScrollCurve2, 2, SpecialStageMap_Curve2PivotOffset,
     SpecialStageMap_Curve2Advance, SpecialStageMap_Curve2DrawOffset},
    {SpecialStageMap_GetCurveLength, SpecialStageMap_ScrollCurve3, 3, SpecialStageMap_Curve3PivotOffset,
     SpecialStageMap_Curve3Advance, SpecialStageMap_Curve3DrawOffset},
    {SpecialStageMap_GetCurveLength, SpecialStageMap_ScrollCurve4, 4, SpecialStageMap_Curve4PivotOffset,
     SpecialStageMap_Curve4Advance, SpecialStageMap_Curve4DrawOffset},
    {SpecialStageMap_GetStraightLength, SpecialStageMap_ScrollStraight, 5, SpecialStageMap_StraightPivotOffset,
     SpecialStageMap_StraightAdvance, SpecialStageMap_StraightDrawOffset},
    {SpecialStageMap_GetStraightLength, SpecialStageMap_ScrollStraight, 6, SpecialStageMap_StraightPivotOffset,
     SpecialStageMap_StraightAdvance, SpecialStageMap_StraightDrawOffset},
};
s32 sp_seg_length(s32 type) { return k_seg[type].fn0(); }

void SpecialStageMap_Update(void) {
    SpMap *g = gSpecialStageMap;
    SpSeg *e, *e2, *e3, *nx;
    MtxFx43 m;
    s32 d;
    g->unk0 += g->unk4;
    if (SpecialStageMap_IsSegmentListEmpty())
        return;
    for (e = g->head.next; e != &g->tail; e = nx) {
        nx = e->next;   /* the original reads e->next after freeing: the freed node keeps its links */
        if (e->flags & 1)
            SpecialStageMap_FreeSegment(e);
    }
    e2 = g->head.next;
    d = k_seg[e2->type].fn0();
    if (g->unk0 > d) {
        g->head.type = e2->type;
        g->unk0 -= d;
        e2->flags |= 1;
        SpecialStageData_AddStep();
    }
    if (SpecialStageMap_IsSegmentListEmpty())
        return;
    for (e2 = g->head.next; e2 != &g->tail; e2 = e2->next)
        if (!(e2->flags & 1))
            break;
    MTX_Identity43_(&g->mtx);
    k_seg[e2->type].fn4();
    m = g->mtx;
    for (e3 = g->head.next; e3 != &g->tail; e3 = e3->next) {
        if (e3->flags & 1)
            continue;
        e3->mtx = m;
        k_seg[e3->type].fnC(&m);
        k_seg[e3->type].fn10(&m);
    }
    Model_Animate(&g->sky);
}

void SpecialStageMap_Draw(void) {
    SpMap *g = gSpecialStageMap;
    SpSeg *e;
    MtxFx43 m, m2;
    if (SpecialStageMap_IsSegmentListEmpty())
        return;
    /* the segment just passed (type @0xA48), drawn behind the camera */
    m = g->mtx;
    k_seg[g->head.type].fn14(&m);
    g->head.mtx = m;
    g_sp_mtx_slot[0x1e] = m;
    sp_draw_model(&g->models[k_seg[g->head.type].draw], 0);
    for (e = g->head.next; e != &g->tail; e = e->next) {
        if (e->flags & 1)
            continue;
        m2 = e->mtx;
        k_seg[e->type].fnC(&m2);
        g_sp_mtx_slot[0x1e] = m2;
        sp_draw_model(&g->models[k_seg[e->type].draw], 0);
    }
    sp_draw_model(&g->sky, 0);
}
