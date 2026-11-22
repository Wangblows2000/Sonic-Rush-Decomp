/*
 * sp_core.c - SpecialStage_* top level (src/game/SpecialStage/SpecialStage_*.c),
 * SpecialStageData_* (stage scripts), SpecialStageTrick_* (trick sequences) and
 * the module API (special.h).
 *
 * Task model: the DS runs, per frame and in priority order, the update task
 * (SpecialStage_State_IntroUpdate / _PlayUpdate / _Exit), the draw task
 * (SpecialStage_State_IntroDraw / _PlayDraw), SpecialStageIntroBg_Main and the
 * HUD display tasks.  special_run_frame() runs them in that order; the draw
 * task records draw commands that special_draw() submits to the PVR.
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpWork *gSpecialStageWork;
SpData *gSpecialStageData;
SpTrick *gSpecialStageTrick;
s32 data_ov001_0231f698;
u32 gGameState_unk28;
SpTrace g_sp_trace;
SpecialHooks special_hooks;
int g_sp_fog = -1;
int g_sp_verbose;
#define SPT(m) do { if (g_sp_verbose) printf("special: %s\n", m); } while (0)               /* PlayDraw fog density (0..127), -1 off */
static int s_paused, s_result, s_active, s_force_quota;
void special_debug_force_quota(int on) { s_force_quota = on; }
static SpecialResult s_res;
extern int g_sp_layer_main[4], g_sp_layer_sub[4];
void SpecialStageIntroBg_Main(void);
void SpecialStageIntroBg_Destructor(void);
void SpecialStage_RelocateCurrentSsdHeader(void);

void special_set_emerald_flags(uint32_t flags) { gGameState_unk28 = flags; }

/* ------------------------------------------------------------ small helpers */
s32 SpecialStage_GetStageIndex(void) { return gSpecialStageWork->unk4; }
u32 SpecialStage_GetEmeraldFlags(void) { return gSpecialStageWork->unk8; }
SpObj *SpecialStage_GetPlayer(void) { return gSpecialStageWork->unk18; }
SpObjList **SpecialStage_GetGimmickManager(void) { return gSpecialStageWork->unk1C; }
u16 SpecialStage_GetRingQuota(void) { return ((SpSst *)gSpecialStageData->unk14)->quota1; }

s32 SpecialStage_PipeAngleToX(u16 ang) { return FX_Mul(SIN(ang), 0x1e000); }

u16 SpecialStage_PipeXToAngle(s32 x) {
    s32 r = (s32)FX_Sqrt(0x38400 - (s32)(((s64)(x >> 2) * (x >> 2) + 0x800) >> 12));
    if ((r << 2) == 0) {
        if (x > 0)
            return 0x4000;
        if (x < 0)
            return 0xc000;
        return 0;
    }
    return FX_Atan2(x, r << 2);
}

s32 SpecialStage_TouchToPipeAngle(s32 x, s32 y) {
    VecFx32 c = *SP_ROM(0x0231eeb4, VecFx32);
    s32 dy = (y << 12) - c.y, dx = (x << 12) - c.x, r, len, a;
    if (dy <= 0)
        r = dx > 0 ? 0x1e000 : -0x1e000;
    else {
        VecFx32 v;
        v.x = dx;
        v.y = dy;
        v.z = 0;
        len = VEC_Mag(&v);
        if (len == 0)
            r = 0;
        else
            r = FX_Mul(FX_Div(0x1e000, len), dx);
        if (r > 0x1e000)
            r = 0x1e000;
        else if (r < -0x1e000)
            r = -0x1e000;
    }
    a = SpecialStage_PipeXToAngle(r);
    if (a > 0x8000)
        a -= 0x10000;
    return a;
}

u16 SpecialStage_ArcLengthToAngle(s32 a, s32 b) {
    s32 t = (s32)(((s64)a * 0x3244 + 0x800) >> 12);
    s32 d = FX_Div(0x8000000, t);
    return (u16)((s32)(((s64)d * b + 0x800) >> 12) >> 12);
}

BOOL SpecialStage_IsPointInCircle(const s32 *a, const VecFx32 *b, s32 x, s32 y) {
    VecFx32 p, q;
    p.x = b->x + a[0];
    p.y = b->y + a[1];
    p.z = 0;
    q.x = x;
    q.y = y;
    q.z = 0;
    return VEC_Distance(&p, &q) < a[2];
}

void SpecialStage_RotateMtxXAroundPivot(MtxFx43 *m, s32 d, u16 ang) {
    MtxFx43 t;
    MTX_TransApply43(m, m, 0, d, 0);
    MTX_RotX43_(&t, SIN(ang), COS(ang));
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, 0, -d, 0);
}

void SpecialStage_RotateMtxYAroundPivot(MtxFx43 *m, s32 d, u16 ang) {
    MtxFx43 t;
    MTX_TransApply43(m, m, d, 0, 0);
    MTX_RotY43_(&t, SIN(ang), COS(ang));
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, -d, 0, 0);
}

/* ------------------------------------------------------------ stage data (SST) */
void SpecialStage_LoadStageData(s32 idx) {
    SpData *g;
    char rel[32];
    gSpecialStageData = (SpData *)calloc(1, sizeof(SpData));
    g = gSpecialStageData;
    g->idx = idx;
    g->unk8 = 4;
    /* gSpecialStageFileTable[idx] = "ss/ss_<idx+1>.sst" */
    snprintf(rel, sizeof(rel), "ss/ss_%d.sst", (int)idx + 1);
    g->unk14 = (u8 *)sp_file_load(rel, NULL);
    /* SpecialStage_RelocateSstHeader: "SST" check; the offset is resolved on use */
}

void SpecialStage_ReleaseStageData(void) {
    if (gSpecialStageData == NULL)
        return;
    free(gSpecialStageData->unk14);
    free(gSpecialStageData);
    gSpecialStageData = NULL;
}

void SpecialStageData_AddStep(void) {
    gSpecialStageData->unk8++;
    gSpecialStageData->unkC++;
}

static const SpSstCmd *sst_cmds(const SpData *p) {
    const SpSst *s = (const SpSst *)p->unk14;
    return (const SpSstCmd *)(p->unk14 + s->offset);
}
static s32 sst_count(const SpData *p) { return ((const SpSst *)p->unk14)->count; }

BOOL SpecialStageData_RunCommand(SpData *p) {
    const SpSstCmd *tbl = sst_cmds(p), *e;
    do {
        e = &tbl[p->unk18];
        switch (e->type) {
        case 0:
            p->unk10 = SpecialStageMap_AddSegment(e->a);
            break;
        case 1:
            SpecialStageGimmick_Spawn(SpecialStage_GetGimmickManager(), p->unk10, e->a, e->b, NULL, e->c, e->d, e->e);
            break;
        }
        p->unk18++;
        if (p->unk18 >= sst_count(p))
            return FALSE;
    } while (tbl[p->unk18].type != 0);
    return TRUE;
}

void SpecialStageData_Update(void) {
    SpData *p = gSpecialStageData;
    if (p->unk18 >= sst_count(p))
        return;
    while (p->unk8 > 0) {
        if (!SpecialStageData_RunCommand(p)) {
            p->unk8 = 0;
            break;
        }
        p->unk8--;
    }
    p->unk4++;
}

/* ------------------------------------------------------------ tricks (SSD) */
void SpecialStageTrick_Reset(void) {
    memset(gSpecialStageTrick->unk0, 0xff, 0x100);
    gSpecialStageTrick->unk100[0] = 0;
    gSpecialStageTrick->unk100[1] = 0;
    gSpecialStageTrick->unk100[2] = 0;
}
void SpecialStageTrick_Init(void) {
    gSpecialStageTrick = (SpTrick *)calloc(1, sizeof(SpTrick));
    SpecialStageTrick_Reset();
    /* Touch_Enable: sp_input.c */
}
void SpecialStageTrick_Shutdown(void) {
    free(gSpecialStageTrick);
    gSpecialStageTrick = NULL;
}
void SpecialStageTrick_SetSequences(SpTrickSeq *a, s32 b) {
    gSpecialStageTrick->unk10C = a;
    gSpecialStageTrick->unk110 = b;
}
SpTrickSeq *SpecialStageTrick_GetSequence(s32 idx) {
    SpTrick *g = gSpecialStageTrick;
    if (idx < 0 || idx >= g->unk110)   /* the original test (idx < 0 && idx >= count) never fires */
        idx = 0;
    return &g->unk10C[idx];
}
s32 SpecialStageTrick_GetRating(s32 idx, s32 n) {
    SpTrickSeq *e = SpecialStageTrick_GetSequence(idx);
    s32 third = e->unk20 / 3, d = e->unk20 - n, thr = third * 2, i;
    for (i = 2; i >= 0; i--) {
        if (thr <= d)
            return i;
        thr -= third;
    }
    return 0;
}
s32 SpecialStageTrick_CheckTouch(s32 idx, s32 n, s32 cur) {
    SpTrickSeq *e = &gSpecialStageTrick->unk10C[idx < 0 ? 0 : idx];
    if (e->unk20 < n)
        return -1;
    /* the DS tests the touch point against the targets from `cur` on (circle 0x02317e80, r 0x10):
     * the current target advances, a later one fails; the controller scheme applies the same
     * rule to the button presses of this frame (sp_qte_step, sp_input.c) */
    return sp_qte_step(cur, e->count, g_sp_pad.qte);
}
s32 SpecialStagePlayer_Unk_ov001_02314694(s32 i) { return gSpecialStageTrick->unk10C[i < 0 ? 0 : i].unk24; }

/* ------------------------------------------------------------ init / states */
void SpecialStage_Start(void) {
    data_ov001_0231f698 = 0;
    SpecialStage_Init(s_res.stage);
}

void SpecialStage_Init(s32 arg) {
    u8 *ssd;
    gSpecialStageWork = (SpWork *)calloc(1, sizeof(SpWork));
    gSpecialStageWork->unk4 = arg;
    gSpecialStageWork->unk8 = gGameState_unk28;
    data_ov001_0231f5c4 = (SpRes *)calloc(1, sizeof(SpRes));
    memset(&gSpDisplayMain, 0, sizeof(gSpDisplayMain));
    memset(&gSpDisplaySub, 0, sizeof(gSpDisplaySub));
    gSpDisplayMain.bright = 0x10;
    gSpDisplaySub.bright = 0x10;
    {
        int i;
        for (i = 0; i < 4; i++)
            g_sp_layer_main[i] = g_sp_layer_sub[i] = -1;
    }
    /* lights: 0 (0,-1,0) 0x7fff, 1 (0xec7,-0x600,0) 0x4631, 2 (0,0x620,0xec7) 0x2108 (sp_draw.c) */
    SPT("SpecialStage_LoadHudAssets");
    SpecialStage_LoadHudAssets(data_ov001_0231f5c4);
    SpecialStage_RelocateCurrentSsdHeader();
    SPT("SpecialStage_CreateIntroTasks");
    SpecialStage_CreateIntroTasks();
    SpecialStageTrick_Init();
    SPT("SpecialStageHud_Create");
    SpecialStageHud_Create();
    SPT("SpecialStage_LoadStageData");
    SpecialStage_LoadStageData(arg);
    SPT("SpecialStageCamera_Init");
    SpecialStageCamera_Init();
    SPT("SpecialStage_LoadPlayerArchive");
    SpecialStage_LoadPlayerArchive();
    SPT("SpecialStage_LoadMapModels");
    SpecialStage_LoadMapModels();
    SPT("SpecialStage_LoadGimmickModels");
    SpecialStage_LoadGimmickModels();
    SPT("SpecialStage_LoadActArchive");
    SpecialStage_LoadActArchive();
    /* SpecialStage_InitDummy */
    ssd = data_ov001_0231f5c4->unk2C;
    {
        u32 ofs, cnt;
        memcpy(&ofs, ssd + 8, 4);
        memcpy(&cnt, ssd + 12, 4);
        SpecialStageTrick_SetSequences((SpTrickSeq *)(ssd + ofs), (s32)cnt);
    }
    gSpecialStageWork->unk18 = SpecialStagePlayer_Create(0);
    gSpecialStageWork->unk1C = SpecialStageGimmick_CreateManager(0x80);
    /* DISPCNT main 0x1500 (BG0 3D, BG2, OBJ), sub 0x1400 (BG2, OBJ) */
    gSpDisplayMain.bg_enable = 0x15;
    gSpDisplaySub.bg_enable = 0x14;
    Sound_PlaySeq_SS(0x1f);
}

void SpecialStage_State_IntroUpdate(void) {
    SpWork *g = gSpecialStageWork;
    SpSprite *a = &g->spr[0], *b = &g->spr[1];
    s16 v;
    if (g->timer <= 0x10) {
        gSpDisplayMain.bright = (s16)(0x10 - g->timer);
        gSpDisplaySub.bright = (s16)(0x10 - g->timer);
    }
    if (g->timer > 0x5c) {
        v = (s16)(g->timer - 0x5c);
        if (v > 0x10)
            v = 0x10;
        gSpDisplaySub.bright = v;
    }
    if (g->timer <= 0xa) {
        a->x = (s16)((g->timer << 8) / 10 - 0x100);
        a->y = 0x20;
        AnimatorSprite_ProcessAnimation(a, 0, 0);
    } else if (g->timer < 0x14) {
        AnimatorSprite_ProcessAnimation(a, 0, 0);
    } else if (g->timer >= 0x14) {
        b->x = 0;
        b->y = 0x20;
        AnimatorSprite_ProcessAnimation(b, 0, 0);
        g->unk30++;
    }
    if (g->timer >= 0) {
        if (g->timer > 0x5c)
            g->unk34 = 0x20;
        else
            g->unk34++;
    }
    if (g->unk18 != NULL)
        SpecialStagePlayer_Update(g->unk18);
    SpecialStageCamera_Update();
    if (g->timer == 0x5c) {
        if (!g_sp_pad.begin)   /* the DS waits for a touch (gTouchState.flags & 0x40) */
            return;
        Sound_PlaySeqArc_SS(0x10, 0x14);
    }
    switch (g->timer) {
    case 0x28:
        SpecialStageCamera_SetMode(0xa);
        break;
    case 0x5c:
        SpecialStageCamera_SetMode(0xb);
        break;
    case 0xf0:
        Sound_StopBGM_SS(0x3c);
        break;
    case 0x12c:
        SpecialStage_StartPlay();
        break;
    }
    g->timer++;
}

void SpecialStage_State_IntroDraw(void) {
    SpWork *g = gSpecialStageWork;
    s32 n;
    SpecialStageCamera_Apply();
    if (g->timer <= 0x14)
        sp_draw_sprite(&g->spr[0], 0x1000, 0x1000, 0);
    else {
        sp_draw_sprite(&g->spr[1], 0x1000, 0x1000, 0);
        gSpDisplayMain.bg[1].x = (s16)(g->unk30 - 10);
        gSpDisplayMain.bg[1].y = (s16)0xffe8;
    }
    if (g->unk34 > 0 && (g->unk34 & 0x20)) {
        g->spr[2].x = 0x12;
        g->spr[2].y = 0x4b;
        sp_draw_sprite(&g->spr[2], 0x1000, 0x1000, 0);
    }
    if (g->unk18 != NULL)
        SpecialStagePlayer_Draw(g->unk18);
    if (g->timer <= 0x5c)
        return;
    n = g->timer - 0x5c;
    if (n > 0x1f)
        n = 0x1f;
    SpecialStage_DrawFadeQuad(n);
}

void SpecialStage_DrawFadeQuad(s32 n) {
    /* blend main BG0 (3D) over the backdrop, white quad 0x64 x 0x64 units 0x14 in front of the camera */
    gSpDisplayMain.blend_on = 1;
    gSpDisplayMain.eva = 0xf;
    gSpDisplayMain.evb = 8;
    SpecialStageCamera_Apply();
    sp_draw_fadequad(n);
}

void SpecialStage_StartPlay(void) {
    SpWork *g = gSpecialStageWork;
    gSpDisplayMain.blend_on = 0;
    gSpDisplayMain.bg[1].x = 0;
    gSpDisplayMain.bg[1].y = 0;
    gSpDisplayMain.eva = 0;
    gSpDisplayMain.evb = 0;
    g->task14 = 0;   /* Task_Destroy(IntroBg) -> SpecialStageIntroBg_Destructor */
    SpecialStageIntroBg_Destructor();
    SpecialStagePlayer_Action_Start(g->unk18);
    SpecialStage_Unk_ov001_023099ec();
    Sound_PlaySeq_SS(0x1e);
    g->taskC = 1;
    g->task10 = 1;
}

void SpecialStage_State_PlayUpdate(void) {
    SpWork *g = gSpecialStageWork;
    if ((gPad[2] & 8) && !SpecialStage_Unk_ov001_02309894()) {
        /* PauseMenu_Create(1) */
        s_paused = 1;
        Sound_PlaySeqArc_SS(0x10, 0x12);   /* SE_Pause */
        return;
    }
    if (g->unk20 != 0)
        return;
    SpecialStageData_Update();
    if (g->unk18 != NULL)
        SpecialStagePlayer_Update(g->unk18);
    SpecialStageCamera_Update();
    SpecialStageMap_Update();
    SpecialStageGimmick_Update(g->unk1C);
    SpecialStage_UpdateObjects();
    SpecialStage_CheckGimmickCollisions();
    SpecialStageHud_Update();
    g->unk28 += 1;
}

void SpecialStage_State_PlayDraw(void) {
    SpWork *g = gSpecialStageWork;
    SpecialStageCamera_Apply();
    SpecialStageMap_Draw();
    SpecialStageGimmick_Draw(g->unk1C);
    SpecialStage_DrawObjects();
    if (g->unk18 != NULL)
        SpecialStagePlayer_Draw(g->unk18);
    SpecialStageHud_Draw();
    if (g->unk28 < 0x78)
        g_sp_fog = (0x7f - g->unk28 * 0x7f / 120) & 0x7f;   /* G3X_SetFogTable, colour 0x7fff */
    else
        g_sp_fog = -1;
}

void SpecialStage_RequestExit(s32 v) {
    gSpecialStageWork->unk24 = v;
    gSpecialStageWork->taskC = 2;
}

void SpecialStage_State_Exit(void) {
    SpWork *g = gSpecialStageWork;
    /* SpecialStage_Shutdown runs in special_shutdown(); its result part is applied here */
    if (g->unk24 != 0)
        gGameState_unk28 |= 1u << g->unk4;
    s_res.cleared = g->unk24 != 0;
    s_res.emerald_flags = gGameState_unk28;
    s_res.rings = g->unk18 ? g->unk18->unk148 : 0;
    s_res.frames = g->unk28;
    s_result = g->unk24 != 0 ? 1 : 2;
    /* gGameState.unk18 & 0x40: SceneChange_ToStageFromSpecialStage (the caller returns to the act) */
}

void SpecialStage_Shutdown(void) {
    SpWork *g = gSpecialStageWork;
    if (!g)
        return;
    SpecialStageGimmick_DestroyManager(g->unk1C);
    g->unk1C = NULL;
    if (g->unk18 != NULL) {
        SpecialStagePlayer_Destroy(g->unk18);
        g->unk18 = NULL;
    }
    SpecialStage_ReleaseActArchive();
    SpecialStage_ReleaseGimmickModels();
    SpecialStage_ReleaseMapModels();
    SpecialStage_ReleasePlayerArchive();
    SpecialStageCamera_Shutdown();
    SpecialStage_ReleaseStageData();
    SpecialStageHud_Destroy();
    SpecialStageTrick_Shutdown();
    SpecialStage_ReleaseHudAssets(data_ov001_0231f5c4);
    free(data_ov001_0231f5c4);
    data_ov001_0231f5c4 = NULL;
    free(gSpecialStageWork);
    gSpecialStageWork = NULL;
    g_sp_fog = -1;
}

/* ------------------------------------------------------------ API */
int special_init(int stage_index, int character) {
    if (s_active)
        special_shutdown();
    if (character != 0 || stage_index < 0 || stage_index >= SPECIAL_NUM_STAGES)
        return -2;
    if (sp_data_load() != 0)
        return -1;
    memset(&g_sp_trace, 0, sizeof(g_sp_trace));
    memset(&s_res, 0, sizeof(s_res));
    s_res.stage = stage_index;
    {
        /* test builds: <special dir>/../debug/spquota.txt keeps the rings at the goal quota
         * (special_debug_force_quota): the stage is cleared without a player */
        extern char g_sp_path[128];
        char p[160];
        const char *slash = strrchr(g_sp_path, '/');
        FILE *f;
        snprintf(p, sizeof(p), "%.*s/debug/spquota.txt", slash ? (int)(slash - g_sp_path) : 0, g_sp_path);
        f = fopen(p, "rb");
        if (f) {
            s_force_quota = 1;
            fclose(f);
            printf("special: debug quota on\n");
        }
    }
    s_paused = 0;
    s_result = 0;
    sp_input_reset();
    SPT("bank");
    {
        int sp_audio_load_bank(void);
        sp_audio_load_bank();
    }
    SPT("render");
    if (sp_render_init() != 0)
        return -1;
    SPT("start");
    SpecialStage_Start();
    SPT("started");
    if (!gSpecialStageData->unk14 || !data_ov001_0231f5c4->unk2C || !gSpecialStagePlayerResources->mdl[0].loaded) {
        special_shutdown();
        return -1;
    }
    s_res.quota_1 = SpecialStage_GetRingQuota();
    s_res.quota_2 = SpecialStageHud_Unk_ov001_02314368();
    s_active = 1;
    return 0;
}

int special_paused(void) { return s_paused; }

int special_run_frame(const InputState *in) {
    SpWork *g = gSpecialStageWork;
    if (!s_active || !g)
        return s_result ? s_result : 2;
    if (s_result)
        return s_result;
    sp_input_update(in);
    if (s_paused) {
        /* PauseMenu (special stage): CONTINUE; GO BACK only when this emerald is already owned */
        if (gPad[2] & 9) {            /* START / A: continue */
            s_paused = 0;
            Sound_PlaySeqArc_SS(0x10, 0x14);
        } else if ((gPad[2] & 2) && (g->unk8 & (1u << g->unk4))) {
            s_paused = 0;             /* B: GO BACK -> SpecialStage_RequestExit(0) */
            SpecialStage_RequestExit(0);
        }
        return 0;
    }
    if (s_force_quota && g->unk18 && g->unk18->unk148 < SpecialStageHud_Unk_ov001_02314368())
        g->unk18->unk148 = SpecialStageHud_Unk_ov001_02314368();
    switch (g->taskC) {
    case 0:
        SpecialStage_State_IntroUpdate();
        break;
    case 1:
        SpecialStage_State_PlayUpdate();
        break;
    case 2:
        SpecialStage_State_Exit();
        return s_result;
    }
    if (s_paused)   /* START this frame: the last frame's draw commands stay (the frozen frame) */
        return 0;
    sp_draw_reset();
    switch (g->task10) {
    case 0:
        SpecialStage_State_IntroDraw();
        break;
    case 1:
        SpecialStage_State_PlayDraw();
        break;
    }
    if (g->task14)
        SpecialStageIntroBg_Main();
    sp_hud_tasks();
    return 0;
}

void special_draw(void) {
    if (!s_active)
        return;
    sp_render_frame();
}

void special_shutdown(void) {
    sp_render_shutdown();   /* PVR textures of the sprites / BGs first (their pixels are freed below) */
    if (gSpecialStageWork) {
        if (s_result == 0)
            s_res.rings = gSpecialStageWork->unk18 ? gSpecialStageWork->unk18->unk148 : 0;
        SpecialStage_Shutdown();
    }
    s_active = 0;
}

void special_result(SpecialResult *r) {
    *r = s_res;
    r->emerald_flags = gGameState_unk28;
}
