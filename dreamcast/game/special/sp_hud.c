/*
 * sp_hud.c - SpecialStageHud_* (src/game/SpecialStage/Hud/), the intro sprites
 * (SpecialStage_CreateIntroTasks), the HUD/intro asset loading and the HUD's
 * display tasks.  On the DS the HUD lives on both screens: engine 1 (top
 * screen: ring counter, quota, emeralds, progress bar, results) and engine 0
 * (the 3D touch screen: captions, Go/START!, trick circles, ratings).  The
 * sprites keep their engine; sp_draw.c composes both into the one 640x480 view.
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpHud *gSpecialStageHud;
SpRes *data_ov001_0231f5c4;
SpDisplay gSpDisplayMain, gSpDisplaySub;
float g_sp_hud_palf;       /* SpecialStageHud_Unk_ov001_02308b74 lerp factor (palette slot 8) */
int g_sp_layer_main[4] = {-1, -1, -1, -1}, g_sp_layer_sub[4] = {-1, -1, -1, -1};

/* ------------------------------------------------------------ assets */
static const char *const k_lang[6] = {"jpn", "eng", "fra", "deu", "ita", "spa"};

static u32 GetLang(void) { return (g_sp_language >= 0 && g_sp_language <= 5) ? (u32)g_sp_language : 1; }

void SpecialStage_LoadHudAssets(SpRes *o) {
    /* gSpecialStageBgFileTable[lang] / gSpecialStageSpriteFileTable[lang] */
    static const char *const bgs[7] = {"sp_stdm_base_a", "sp_stdm_base_b", NULL, "sp_fix_base", "sp_fix_touch",
                                       "sp_cldm_base", "sp_cldm_caption"};
    char rel[96];
    s32 i;
    for (i = 0; i < 7; i++) {
        if (bgs[i])
            snprintf(rel, sizeof(rel), "bg/%s.mnbg", bgs[i]);
        else
            snprintf(rel, sizeof(rel), "bg/sp_stdm_caption_%s.mnbg", k_lang[GetLang()]);
        o->bg[i] = (u8 *)sp_file_load(rel, &o->bgsize[i]);
    }
    sp_sprres_load(&o->spr[0], "act__ac_spst");
    snprintf(rel, sizeof(rel), "act__ac_spst_%s", k_lang[GetLang()]);
    sp_sprres_load(&o->spr[1], rel);
    sp_sprres_load(&o->spr[2], "act__ac_spst_ann");
    sp_sprres_load(&o->spr[3], "act__ac_spst_efct");
    /* Dreamcast controller art (tools/padart.py): button targets, PRESS START prompt, legend */
    sp_sprres_load(&o->spr[4], "act__ac_spst_pad");
    o->unk2C = (u8 *)sp_file_load("ss/ss.ssd", NULL);   /* SpecialStage_LoadHudFile(data_ov001_0231df98) */
    /* the sound archive set-up (NNS_SndArcInit, SeqArc 0xe / 9, banks 0x16 / 0x11, group 0x14) is the
     * ss.srb bank: see sp_audio.c */
}

void SpecialStage_ReleaseHudAssets(SpRes *o) {
    s32 i;
    void sp_audio_stop_all(void);
    sp_audio_stop_all();   /* NNS_SndStopSoundAll */
    for (i = 0; i < 7; i++)
        free(o->bg[i]);
    for (i = 0; i < 5; i++)
        sp_sprres_free(&o->spr[i]);
    free(o->unk2C);
    memset(o, 0, sizeof(*o));
}

/* SpecialStage_RelocateSsdHeader: "SSD" check + offset relocation */
BOOL SpecialStage_RelocateSsdHeader(u8 *p) {
    if (!p || memcmp(p, sp_rom(0x0231e6b0, 3), 3) != 0)
        return FALSE;
    return TRUE;
}
void SpecialStage_RelocateCurrentSsdHeader(void) { SpecialStage_RelocateSsdHeader(data_ov001_0231f5c4->unk2C); }

static void Background_Init(SpBg *b, int res, u32 engine, u8 layer) {
    b->res = res;
    b->engine = (int)engine;
    b->layer = layer;
    b->drawn = 0;
}
static void Background_Draw(SpBg *b) {
    b->drawn = 1;
    if (b->engine == 0)
        g_sp_layer_main[b->layer & 3] = b->res;
    else
        g_sp_layer_sub[b->layer & 3] = b->res;
}

/* ------------------------------------------------------------ intro */
void SpecialStage_CreateIntroTasks(void) {
    SpWork *w = gSpecialStageWork;
    SpRes *rs = data_ov001_0231f5c4;
    s32 i;
    w->taskC = 0;   /* SpecialStage_State_IntroUpdate */
    w->task10 = 0;  /* SpecialStage_State_IntroDraw */
    w->task14 = 1;  /* SpecialStageIntroBg_Main */
    for (i = 0; i < 3; i++) {
        u32 e = 0x023171b0 + i * 8;
        Background_Init(&w->obj[i], sp_rom_s16(e + 6), sp_rom_u32(e), sp_rom_u8(e + 4));
    }
    for (i = 0; i < 3; i++) {
        u32 e = 0x023171c8 + i * 12;
        AnimatorSprite_Init(&w->spr[i], &rs->spr[sp_rom_s16(e + 8)], sp_rom_u16(e + 4), 0, (int)sp_rom_u32(e));
        if (sp_rom_s16(e + 8) == 1 && sp_rom_u16(e + 4) == 0 && rs->spr[4].loaded)
            w->spr[i].res = &rs->spr[4];   /* "TOUCH THE TOUCH SCREEN TO CONTINUE" -> "PRESS START TO BEGIN" */
        w->spr[i].flags |= 0x20000;
        w->spr[i].flags &= ~0x40u;
    }
    for (i = 0; i < 3; i++) {
        u32 e = 0x023171c8 + i * 12;
        AnimatorSprite_ProcessAnimation(&w->spr[i], 0, 0);
        w->spr[i].flags |= 0x10;
        if (sp_rom_u8(e + 6) == 0)
            w->spr[i].flags |= 8;
        if (sp_rom_u8(e + 7) != 0)
            w->spr[i].flags |= 4;
        w->spr[i].unk46 = 0;
    }
    Background_Draw(&w->obj[0]);
    Background_Draw(&w->obj[1]);
    Background_Draw(&w->obj[2]);
}

void SpecialStageIntroBg_Main(void) {
    if (gSpecialStageWork->timer == 0x19)
        gSpDisplayMain.bg_enable |= 2;
}
void SpecialStageIntroBg_Destructor(void) {
    gSpDisplayMain.bg_enable &= ~6u;
    g_sp_layer_main[1] = g_sp_layer_main[2] = -1;   /* MIi_CpuClear32(0, 0x6000000, 0x1000) */
}

/* ------------------------------------------------------------ HUD create / destroy */
void SpecialStageHud_Create(void) {
    SpHud *w;
    SpRes *r = data_ov001_0231f5c4;
    s32 i;
    float f1, f2;
    s32 n;
    w = (SpHud *)calloc(1, sizeof(SpHud));
    gSpecialStageHud = w;
    w->unkBC8 = -1;
    w->unkBCC = -1;
    w->unkBD0 = -1;
    w->unkBD4 = -1;
    w->unkBBC = 1;
    w->unkBC4 = 0;   /* not set by the original Create: the memset leaves 0 (start sequence runs) */
    f1 = 34.133335f;
    n = 512;
    f2 = (0.0f - n) / 225.0f;
    for (i = 0; i < 7; i++) {
        w->a[i] = f1;
        w->b[i] = f2;
        w->c[i] = -256.0f + sp_rom_s16(0x02317098 + i * 2);
    }
    for (i = 0; i < 4; i++)
        Background_Init(&w->bg[i], 3 + i, sp_rom_u32(0x023170a8 + i * 4), sp_rom_u8(0x0231708c + i));
    for (i = 0; i < 23; i++) {
        AnimatorSprite_Init(&w->spr[i], &r->spr[0], sp_rom_u16(0x02317114 + i * 2), 0,
                            (int)sp_rom_u32(0x02317144 + i * 4));
        w->spr[i].flags |= 0x20000;
    }
    for (i = 0; i < 23; i++) {
        AnimatorSprite_ProcessAnimation(&w->spr[i], 0, 0);
        w->spr[i].flags |= 0x10;
        w->spr[i].flags &= ~0x40u;
        if (sp_rom_u8(0x023170e4 + i) == 0)
            w->spr[i].flags |= 8;
        if (sp_rom_u8(0x023170fc + i) != 0)
            w->spr[i].flags |= 4;
        w->spr[i].unk46 = 5;
    }
    for (i = 0; i < 3; i++) {
        AnimatorSprite_Init(&w->spr3[i], &r->spr[0], 0x40, 0, 1);
        w->spr3[i].flags |= 4;
    }
    w->spr[2].x = 0x4a;
    w->spr[2].y = 0x74;
    w->spr[3].x = 0x5e;
    w->spr[3].y = 0x74;
    w->spr[4].x = 0x72;
    w->spr[4].y = 0x74;
    w->spr[5].x = 0xa2;
    w->spr[5].y = 0x7d;
    w->spr[6].x = 0xb6;
    w->spr[6].y = 0x7d;
    w->spr[7].x = 0xca;
    w->spr[7].y = 0x7d;
    for (i = 2; i <= 7; i++)
        w->spr[i].flags |= 0x200;
    w->spr[8].x = 0x80;
    w->spr[8].y = 0xac;
    w->spr[9].x = 0x18;
    w->spr[9].y = 0xac;
    w->spr[9].unk46 = 2;
    for (i = 0; i < 7; i++) {
        SpSprite *s = &w->spr[11 + i];
        s->x = (s16)(-256.0f + sp_rom_s16(0x02317098 + i * 2));
        s->y = 0x54;
    }
    w->spr[20].unk46 = 3;
    w->spr[18].unk46 = 8;
    w->spr[19].unk46 = 3;
    w->spr[19].flags |= 0x200;
    /* palettes of anim 0x12 (pal0) and anim 0x13 + stage (pal1): the palette lerp is applied to
     * spr[18] at draw time (g_sp_hud_palf) */
}

void SpecialStageHud_Destroy(void) {
    free(gSpecialStageHud);
    gSpecialStageHud = NULL;
}

/* SpecialStage_Unk_ov001_023099ec: play sprites (StartPlay) */
void SpecialStage_Unk_ov001_023099ec(void) {
    SpHud *hud = gSpecialStageHud;
    SpRes *r = data_ov001_0231f5c4;
    s32 i;
    hud->unkAE8 = 1;
    AnimatorSprite_Init(&hud->spr0, &r->spr[1], 1, 0, 0);
    AnimatorSprite_ProcessAnimation(&hud->spr0, 0, 0);
    hud->spr0.flags |= 0x10;
    hud->spr0.unk46 = 1;
    AnimatorSprite_Init(&hud->spr1, &r->spr[2], 0, 0, 0);
    AnimatorSprite_ProcessAnimation(&hud->spr1, 0, 0);
    hud->spr1.unk46 = 1;
    hud->unkBC0 = 0x1000;
    for (i = 0; i < 10; i++) {
        /* targets 1..8: the controller button art (same anims 0x2a..0x39 / 0x3e in ac_spst_pad) */
        AnimatorSprite_Init(&hud->spr2[i], i < 8 && r->spr[4].loaded ? &r->spr[4] : &r->spr[0],
                            sp_rom_u16(0x023170b8 + i * 2), 0, 0);
        AnimatorSprite_ProcessAnimation(&hud->spr2[i], 0, 0);
        /* the original sets the flags of the i-th object of the *HUD array* through h (stride 0x48):
         * h->spr2[0] with h advancing = spr2[i] */
        hud->spr2[i].flags |= 0x10;
        hud->spr2[i].unk46 = 5;
    }
    hud->spr2[8].x = 0x20;              /* +0xA54 / +0xA56: the timing-bar frame */
    hud->spr2[8].y = 0xB4;
    hud->spr2[9].flags |= 4;            /* +0xAC4: the pen icon loops */
    Background_Draw(&hud->bg[0]);
    Background_Draw(&hud->bg[1]);
    hud->unkAE4 = SpecialStage_GetRingQuota();
}

BOOL SpecialStage_Unk_ov001_02309894(void) { return gSpecialStageHud->unkBD4 >= 0; }

/* ------------------------------------------------------------ helpers */
u16 SpecialStageHud_Unk_ov001_02314368(void) {
    return ((SpSst *)gSpecialStageData->unk14)->quota2;
}

s32 SpecialStageHud_Unk_ov001_02314214(void) {
    SpData *g = gSpecialStageData;
    SpSst *t = (SpSst *)g->unk14;
    float v;
    s32 n, r;
    v = (float)g->unkC + (float)FX_Div(SpecialStageMap_GetScrollDistance(), 0x12c000) / 4096.0f;
    if (v <= 2.0f)
        return 0;
    if (SpecialStageMap_GetCurrentSegment()->type == 5)
        return 0x800;
    if (SpecialStageMap_GetCurrentSegment()->type == 6)
        return 0x1000;
    n = t->unk14;
    if (v <= (float)(n - 1))
        return (s32)(2048.0f * (v - 2.0f) / (float)(n - 5));
    n = t->unk14;
    r = (s32)(2048.0f * (v - (float)(n + 3)) / (float)(t->unk18 - n - 6)) + 0x800;
    if (r > 0x1000)
        r = 0x1000;
    return r;
}

void SpecialStageHud_AnimateSprites(void) {
    SpHud *g = gSpecialStageHud;
    s32 i;
    for (i = 0; i < 0x17; i++)
        AnimatorSprite_ProcessAnimation(&g->spr[i], 0, 0);
    AnimatorSprite_ProcessAnimation(&g->spr0, 0, 0);
    AnimatorSprite_ProcessAnimation(&g->spr1, 0, 0);
}

void SpecialStageHud_CalcCirclePos(s32 i, float scale, s16 *px, s16 *py) {
    u16 a = (u16)((gSpecialStageHud->unkBD8[i] << 16) / 175 + 0xa000);
    s16 c = COS(a), s = SIN(a);
    if (px != NULL)
        *px = (s16)(0.5f + (128.0f + scale * (c / 4096.0f)));
    if (py != NULL)
        *py = (s16)(0.5f + (120.0f - scale * (s / 4096.0f)));
}

void SpecialStageHud_UpdateEmeraldCircle(s32 i, s32 t) {
    SpHud *g = gSpecialStageHud;
    SpSprite *o;
    float f;
    if (t < i * 0x19)
        return;
    o = &g->spr[i + 0xb];
    f = g->unkBD8[i] / 60.0f;
    g->unkBD8[i]++;
    if (f > 1.0f)
        f = 1.0f;
    f = 1.0f - f;
    f = 120.0f - 70.0f * (1.0f - f * (f * f));
    if (f < 50.0f)
        f = 50.0f;
    SpecialStageHud_CalcCirclePos(i, f, &o->x, &o->y);
}

void SpecialStageHud_SpawnSparkle(s32 i) {
    SpHud *g = gSpecialStageHud;
    u16 r;
    r = Rand();
    g->timers[i] = r % 10 + 15;
    r = Rand();
    g->pos[i].x = (s16)(g->unkBF8 + (r % 11 - 5));
    r = Rand();
    g->pos[i].y = (s16)(g->unkBFA + (r % 9 - 15));
    AnimatorSprite_SetAnimation(&g->spr3[i], 0x40);
}

void SpecialStageHud_UpdateRingCounter(void) {
    SpHud *g = gSpecialStageHud;
    s32 n = SpecialStage_GetPlayer()->unk148;
    AnimatorSprite_SetAnimation(&g->spr[2], (u16)(n / 100 % 10 + 0x1e));
    AnimatorSprite_SetAnimation(&g->spr[3], (u16)(n / 10 % 10 + 0x1e));
    AnimatorSprite_SetAnimation(&g->spr[4], (u16)(n % 10 + 0x1e));
}
void SpecialStageHud_UpdateRingCounterThunk(void) { SpecialStageHud_UpdateRingCounter(); }

void SpecialStageHud_UpdateQuotaCounter(void) {
    SpHud *g = gSpecialStageHud;
    s32 n = g->unkAE4;
    AnimatorSprite_SetAnimation(&g->spr[5], (u16)(n / 100 % 10 + 0x1e));
    AnimatorSprite_SetAnimation(&g->spr[6], (u16)(n / 10 % 10 + 0x1e));
    AnimatorSprite_SetAnimation(&g->spr[7], (u16)(n % 10 + 0x1e));
}

void SpecialStageHud_SetDigitAnims(s32 t) {
    SpHud *g = gSpecialStageHud;
    s32 n = SpecialStageHud_Unk_ov001_02314368();
    switch (t) {
    case 0:
        AnimatorSprite_SetAnimation(&g->spr[5], (u16)(n / 100 % 10 + 0x1e));
        break;
    case 0x14:
        AnimatorSprite_SetAnimation(&g->spr[6], (u16)(n / 10 % 10 + 0x1e));
        break;
    case 0x28:
        AnimatorSprite_SetAnimation(&g->spr[7], (u16)(n % 10 + 0x1e));
        break;
    }
}

void SpecialStageHud_ShowTrickRating(s32 n) {
    SpHud *g = gSpecialStageHud;
    g->unkAF0 = 0x3c;
    switch (n) {
    case 3:
        AnimatorSprite_SetAnimation(&g->spr1, 0xb);
        break;
    case 0:
        AnimatorSprite_SetAnimation(&g->spr1, 4);
        break;
    case 1:
        AnimatorSprite_SetAnimation(&g->spr1, 6);
        break;
    case 2:
        AnimatorSprite_SetAnimation(&g->spr1, 8);
        break;
    default:
        AnimatorSprite_SetAnimation(&g->spr1, 10);
        break;
    }
}

void SpecialStageHud_EndTrickRating(void) { gSpecialStageHud->unkAF0 = 5; }

/* checkpoint / goal triggers (gimmick types 0x15 / 0x14) */
void SpecialStageGimmick_Unk_ov001_02309994(void) {
    gSpecialStageHud->unkBC8 = -1;
    gSpecialStageHud->unkBCC = 0;
    Sound_PlaySeqArc_SS(0x10, 1);
}
void SpecialStageGimmick_Unk_ov001_023099c4(void) {
    gSpecialStageHud->unkBC8 = 0;
    Sound_PlaySeqArc_SS(0x10, 1);
}

/* SpecialStageHud_Unk_ov001_02308b74: palette lerp of OBJ palette slot 8 */
static void SpecialStageHud_Unk_ov001_02308b74(void) {
    SpHud *g = gSpecialStageHud;
    float f;
    if (g->unkBAC < 0x3c)
        f = 0.0f;
    else if (g->unkBAC <= 0x46)
        f = (g->unkBAC - 0x3c) / 10.0f;
    else if (g->unkBAC < 0x82)
        f = 1.0f;
    else if (g->unkBAC <= 0x8c)
        f = (0x8c - g->unkBAC) / 10.0f;
    else {
        g->unkBAC = -1;
        f = 0.0f;
    }
    g_sp_hud_palf = f;
    g->unkBAC++;
}

/* ------------------------------------------------------------ start / checkpoint / goal / fail / clear */
static void SpecialStageHud_Unk_ov001_02309228(void) {
    SpHud *g = gSpecialStageHud;
    s32 i, t, d;
    s16 tgt;
    SpSprite *o;
    g->unkBC4++;
    if (g->unkBC4 < 0x118)
        SpecialStageMap_SetScrollSpeed((s32)(9216.0f * g->unkBC4 / 280.0f));
    if (g->unkBC4 <= 0x10)
        gSpDisplaySub.bright = (s16)(0x10 - g->unkBC4);
    if (g->unkBC4 >= 0x96 && g->unkBC4 < 0x136) {
        t = g->unkBC4 - 0x96;
        for (i = 0; i < 7; i++) {
            tgt = sp_rom_s16(0x02317098 + i * 2);
            o = &g->spr[i + 0xb];
            d = tgt - g->spr[i + 0xb].x;
            if (t > (6 - i) * 10) {
                if (d <= 0)
                    o->x = tgt;
                else if (g->a[i] <= 0.0f) {
                    g->a[i] = 0;
                    g->b[i] = 0;
                    o->x = tgt;
                } else {
                    o->x = (s16)g->c[i];
                    g->c[i] += g->a[i];
                    g->a[i] += g->b[i];
                }
            }
            if (g->unkBC4 == i + 0x10e)
                AnimatorSprite_SetAnimation(o, (u8)(sp_rom_u16(0x02317114 + (i + 0xb) * 2) + 1));
            else if (g->unkBC4 > i + 0x10e && (o->flags & 2))
                AnimatorSprite_SetAnimation(o, (u8)sp_rom_u16(0x02317114 + (i + 0xb) * 2));
        }
    }
    if (g->unkBC4 < 0x96)
        return;
    g->spr0.x = 0x80;
    g->spr0.y = 0x22;
    g->spr1.x = 0x80;
    g->spr1.y = 0x2a;
    switch (g->unkBC4) {
    case 0x96:
        AnimatorSprite_SetAnimation(&g->spr0, (u16)(SpecialStage_GetStageIndex() * 2 + 1));
        g->unk0 = (void *)1;   /* Task_Create(SpecialStageHud_FlushPaletteNow, .., 0xf300) */
        g->unkBB0 = 1;
        break;
    case 0x118:
        AnimatorSprite_SetAnimation(&g->spr1, 2);
        g->unkBC0 = 0;
        SpecialStageCamera_SetMode(7);
        g->unk4 = (void *)1;   /* Task_Create(SpecialStageHud_Unk_ov001_023095e0, .., 0xf200) */
        SpecialStageMap_SetScrollSpeed(0x3000);
        SpecialStage_Unk_ov001_02312dd0(SpecialStage_GetPlayer(), 0x50);
        break;
    case 0x12c:
        SpecialStageCamera_SetMode(8);
        break;
    case 0x168:
        SpecialStageCamera_SetMode(0);
        SpecialStageHud_Unk_ov001_02312dd8(SpecialStage_GetPlayer(), 1);
        g->unkBC4 = -1;
        break;
    }
    if (g->unkBC4 <= 0x118)
        return;
    g->unkBC0 += 0x200;
    if (g->unkBC0 >= 0x1000)
        g->unkBC0 = 0x1000;
}

/* SpecialStageHud_Unk_ov001_023095e0 (task 0xf200): main BG1 (sp_fix_touch) shown blended 4/16
 * from 0x168 to 0x262 */
static void SpecialStageHud_Unk_ov001_023095e0(void) {
    SpHud *g = gSpecialStageHud;
    float f;
    if (g->unkAE0 > 0x262)
        gSpDisplayMain.bg_enable &= ~2u;
    else if (g->unkAE0 > 0x168)
        gSpDisplayMain.bg_enable |= 2;
    gSpDisplayMain.blend_on = 1;
    gSpDisplayMain.eva = 4;
    gSpDisplayMain.evb = 0xf - gSpDisplayMain.eva;
    if (g->unkAE0 >= 0x168 && g->unkAE0 < 0x172) {
        f = (g->unkAE0 - 0x168) / 10.0f;
        if (f > 1.0f)
            f = 1.0f;
        gSpDisplayMain.eva = (u8)((u32)(4.0f * f) & 0xf);
        gSpDisplayMain.evb = (u8)((0xf - gSpDisplayMain.eva) & 0xf);
    } else if (g->unkAE0 >= 0x258) {
        f = 1.0f - (g->unkAE0 - 0x258) / 10.0f;
        if (f < 0.0f)
            f = 0.0f;
        gSpDisplayMain.eva = (u8)((u32)(4.0f * f) & 0xf);
        gSpDisplayMain.evb = (u8)((0xf - gSpDisplayMain.eva) & 0xf);
    }
}

/* SpecialStageHud_ToggleSubBg1Task (0xf400) */
static void SpecialStageHud_ToggleSubBg1Task(void) {
    s32 v = gSpecialStageHud->unkBD4;
    if (v < 0x3e)
        gSpDisplaySub.bg_enable &= ~2u;
    else if (v == 0x3e)
        gSpDisplaySub.bg_enable |= 2;
}

void sp_hud_tasks(void) {
    SpHud *g = gSpecialStageHud;
    if (!g)
        return;
    if (g->unk4)
        SpecialStageHud_Unk_ov001_023095e0();
    /* 0xf300 SpecialStageHud_FlushPaletteNow: palette slot 8 <- pal2 (g_sp_hud_palf) */
    if (g->unkBF4)
        SpecialStageHud_ToggleSubBg1Task();
}

static void SpecialStageHud_UpdateCheckpoint(void) {
    SpHud *g = gSpecialStageHud;
    s32 n;
    g->unkBC8++;
    SpecialStageHud_UpdateRingCounterThunk();
    SpecialStageHud_SetDigitAnims(g->unkBC8 - 0xb4);
    switch (g->unkBC8) {
    case 1:
        g->spr1.x = 0x80;
        g->spr1.y = 0x2a;
        n = SpecialStage_GetRingQuota();
        if (n <= SpecialStage_GetPlayer()->unk148) {
            AnimatorSprite_SetAnimation(&g->spr1, 3);
            SpecialStageCamera_SetMode(4);
            SpecialStagePlayer_Action_Celebrate(SpecialStage_GetPlayer());
            return;
        }
        AnimatorSprite_SetAnimation(&g->spr1, 10);
        g->unkBC8 = -1;
        g->unkBD0 = 0;
        return;
    case 0xf0:
        g->spr0.x = 0x80;
        g->spr0.y = 0x22;
        AnimatorSprite_SetAnimation(&g->spr0, (u16)(SpecialStage_GetStageIndex() * 2 + 2));
        g->unkBB4 = 1;
        g->unkBA8 = 1;
        g->unkBAC = 0;
        return;
    case 0x172:
        g->unkBC8 = -1;
        SpecialStageCamera_SetMode(0);
        break;
    default:
        return;
    }
    g->unkAE4 = SpecialStageHud_Unk_ov001_02314368();
}

static void SpecialStageHud_InitGoalSequence(void) {
    SpHud *g = gSpecialStageHud;
    s32 i;
    g->unkBD4 = 0;
    g_sp_layer_sub[0] = g_sp_layer_sub[1] = g_sp_layer_sub[2] = g_sp_layer_sub[3] = -1; /* clear sub BG VRAM */
    Background_Draw(&g->bg[2]);
    Background_Draw(&g->bg[3]);
    for (i = 0; i < 7; i++) {
        g->unkBD8[i] = 0;
        g->spr[i + 11].x = -100;
        g->spr[i + 11].y = -100;
    }
    g->unkBF8 = 0x80;
    g->unkBFA = 0x78;
    AnimatorSprite_SetAnimation(&g->spr[22], 1);
    g->unkBF4 = (void *)1;   /* Task_Create(SpecialStageHud_ToggleSubBg1Task, .., 0xf400) */
    for (i = 0; i < 3; i++) {
        AnimatorSprite_ProcessAnimation(&g->spr3[i], 0, 0);
        g->spr3[i].flags &= ~0x40u;
        SpecialStageHud_SpawnSparkle(i);
    }
    Sound_PlaySeq_SS(0x1f);
    gSpDisplaySub.bg_enable |= 2;
}

static void SpecialStageHud_UpdateGoal(void) {
    SpHud *g = gSpecialStageHud;
    s16 b;
    g->unkBCC++;
    if (g->unkBCC >= 500) {
        b = (s16)(g->unkBCC - 500);
        if (b > 0x10)
            b = 0x10;
        gSpDisplaySub.bright = b;
    }
    SpecialStageHud_UpdateRingCounterThunk();
    SpecialStageHud_UpdateQuotaCounter();
    switch (g->unkBCC) {
    case 1:
        g->spr1.x = 0x80;
        g->spr1.y = 0x2a;
        if (SpecialStageHud_Unk_ov001_02314368() <= SpecialStage_GetPlayer()->unk148) {
            AnimatorSprite_SetAnimation(&g->spr1, 3);
            SpecialStagePlayer_Action_Celebrate(SpecialStage_GetPlayer());
            return;
        }
        AnimatorSprite_SetAnimation(&g->spr1, 10);
        g->unkBCC = -1;
        g->unkBD0 = 0;
        return;
    case 0x1cc:
        Sound_StopBGM_SS(0x3c);
        return;
    case 0x208:
        g->unkBCC = -1;
        SpecialStageHud_InitGoalSequence();
        return;
    }
}

static void SpecialStageHud_UpdateFail(void) {
    SpHud *g = gSpecialStageHud;
    s16 b;
    g->unkBD0++;
    if (g->unkBD0 >= 0xb4) {
        b = (s16)(g->unkBD0 - 0xb4);
        if (b > 0x10)
            b = 0x10;
        gSpDisplayMain.bright = b;
        gSpDisplaySub.bright = b;
    }
    if (g->unkBD0 <= 0x3c)
        SpecialStageMap_SetScrollSpeed((s32)(12288.0f * (1.0f - g->unkBD0 / 60.0f)));
    switch (g->unkBD0) {
    case 1:
        SpecialStagePlayer_Action_Fail(SpecialStage_GetPlayer());
        g->unkBBC = 0;
        return;
    case 200:
        g->unkBD0 = -1;
        SpecialStage_RequestExit(0);
        return;
    }
}

static void SpecialStageHud_UpdateClear(void) {
    SpHud *w = gSpecialStageHud;
    SpSprite *s21 = &w->spr[21], *s22 = &w->spr[22], *sp;
    s32 k, i, t;
    s16 b;
    float f;
    k = SpecialStage_GetStageIndex() + 0xb;
    w->unkBD4++;
    sp = &w->spr[k];
    if (w->unkBD4 <= 0x32) {
        b = (s16)(0x32 - w->unkBD4);
        if (b > 0x10)
            b = 0x10;
        else if (b < 0)
            b = 0;
        gSpDisplaySub.bright = b;
    }
    if (w->unkBD4 >= 0x2e4) {
        b = (s16)(w->unkBD4 - 0x2e4);
        if (b > 0x10)
            b = 0x10;
        gSpDisplayMain.bright = b;
        gSpDisplaySub.bright = b;
    }
    if (w->unkBD4 >= 0x28 && w->unkBD4 <= 0x32) {
        s21->x = (s16)((w->unkBD4 << 8) / 10 - 0x100);
        s21->y = 0x20;
        AnimatorSprite_ProcessAnimation(s21, 0, 0);
    } else if (w->unkBD4 < 0x3c) {
        AnimatorSprite_ProcessAnimation(s21, 0, 0);
    } else if (w->unkBD4 >= 0x3c) {
        s22->x = 0;
        s22->y = 0x20;
        AnimatorSprite_ProcessAnimation(s22, 0, 0);
        gSpDisplaySub.bg[sp_rom_u8(0x0231708c + 3)].x = (s16)(w->unkBD4 - 0x32);
        gSpDisplaySub.bg[sp_rom_u8(0x0231708c + 3)].y = (s16)0xffe8;
    }
    if (w->unkBD4 >= 0x46) {
        t = w->unkBD4 - 0x46;
        for (i = 0; i < 7; i++)
            SpecialStageHud_UpdateEmeraldCircle(i, t);
        if (w->unkBD4 >= 0x17c) {
            f = (float)(w->unkBD4 - 0x17c) / 60.0f;
            if (f > 1.0f)
                f = 1.0f;
            SpecialStageHud_CalcCirclePos(SpecialStage_GetStageIndex(), 50.0f * f, &w->unkBF8, &w->unkBFA);
        }
    }
    if (w->unkBD4 == 0xfc) {
        AnimatorSprite_SetAnimation(sp, (u16)(sp_rom_u16(0x02317114 + k * 2) + 1));
        sp->unk46 = 3;
        Sound_PlaySeqArc_SS(0xe, 3);   /* data_ov001_0231f5c8 archive, SE_Emerald_Light */
    } else if (w->unkBD4 > 0xfc && (sp->flags & 2))
        AnimatorSprite_SetAnimation(sp, sp_rom_u16(0x02317114 + k * 2));
    AnimatorSprite_ProcessAnimation(sp, 0, 0);
    for (i = 0; i < 3; i++) {
        AnimatorSprite_ProcessAnimation(&w->spr3[i], 0, 0);
        if (w->timers[i] > 0) {
            w->pos[i].y++;
            w->spr3[i].x = w->pos[i].x;
            w->spr3[i].y = w->pos[i].y;
            w->timers[i]--;
        } else
            SpecialStageHud_SpawnSparkle(i);
    }
    if (w->unkBD4 == 0x302)
        SpecialStage_RequestExit(1);
}

void SpecialStageHud_Update(void) {
    SpHud *w = gSpecialStageHud;
    SpObj *p;
    s32 i;
    float f;
    p = SpecialStage_GetPlayer();
    w->unkAE0++;
    if (SpecialStageMap_GetCurrentSegment()->type == 5 || SpecialStageMap_GetCurrentSegment()->type == 6)
        SpecialStageHud_Unk_ov001_02312dd8(p, 0);
    else if (w->unkBC4 < 0)
        SpecialStageHud_Unk_ov001_02312dd8(p, 1);
    if (w->unkBD4 >= 0) {
        SpecialStageHud_UpdateClear();
        return;
    }
    if (w->unkAE0 >= 0x96)
        SpecialStageHud_Unk_ov001_02308b74();
    if (w->unkAE0 >= 0x118) {
        s32 off[2] = {0, 0};
        w->unkBB8++;
        if (p->unk120 != 0) {
            u16 r = Rand();
            off[0] = r % 5 - 2;
            r = Rand();
            off[1] = r % 5 - 2;
            w->unkBB8 = 0x20;
        }
        w->spr[9].x = (s16)(off[0] + 0x18 + ((SpecialStageHud_Unk_ov001_02314214() * 0xd0) >> 12));
        w->spr[9].y = (s16)(off[1] + 0xac);
    }
    if (p->unk128 != 0) {
        SpTrickSeq *info = SpecialStageTrick_GetSequence(p->unk150);
        if (p->unk14C == 0 && p->unk14C != w->unkAEC)
            for (i = 0; i < info->count; i++)
                AnimatorSprite_SetAnimation(&w->spr2[i], sp_rom_u16(0x023170b8 + i * 2));
        if (p->unk14C != w->unkAEC) {
            for (i = 0; i < info->count; i++) {
                if (i == p->unk14C) {
                    AnimatorSprite_SetAnimation(&w->spr2[i], (u16)(sp_rom_u16(0x023170b8 + i * 2) + 1));
                    w->spr2[i].flags |= 4;
                } else if (i == p->unk14C - 1) {
                    AnimatorSprite_SetAnimation(&w->spr2[i], 0x3e);
                    w->spr2[i].flags &= ~4u;
                    Sound_PlaySeqArc_SS(0x10, 0);
                }
            }
            if (p->unk14C == 0x1234) {
                AnimatorSprite_SetAnimation(&w->spr2[info->count - 1], 0x3e);
                w->spr2[info->count - 1].flags &= ~4u;
                Sound_PlaySeqArc_SS(0x10, 0);
            }
        }
        for (i = 0; i < info->count; i++) {
            w->spr2[i].x = (s16)info->pts[i].x;
            w->spr2[i].y = (s16)info->pts[i].y;
            AnimatorSprite_ProcessAnimation(&w->spr2[i], 0, 0);
        }
        AnimatorSprite_ProcessAnimation(&w->spr2[8], 0, 0);
        w->unkAEC = p->unk14C;
    } else
        w->unkAEC = -1;
    if (w->unkAF0 > 0) {
        w->unkAF0--;
        if (w->unkAF0 > 0x35) {
            f = (float)(0x3c - w->unkAF0) / 7.0f;
            if (f > 1.0f)
                f = 1.0f;
            f = 1.0f - f;
            w->spr1.x = (s16)(148.0f + 256.0f * (f * (f * f)));
            w->spr1.y = 0x22;
        } else if (w->unkAF0 < 5) {
            f = (float)w->unkAF0 / 5.0f;
            if (f > 1.0f)
                f = 1.0f;
            f = 1.0f - f;
            w->spr1.x = (s16)(138.0f - 256.0f * (f * (f * f)));
            w->spr1.y = 0x22;
        } else {
            if (w->unkAF0 & 1)
                w->spr1.x--;
            w->spr1.y = 0x22;
            if (w->unkAF0 == 0x33 && w->spr1.anim != 0xb)
                AnimatorSprite_SetAnimation(&w->spr1, (u16)(w->spr1.anim + 1));
        }
    }
    if (w->unkBC4 >= 0)
        SpecialStageHud_Unk_ov001_02309228();
    if (w->unkBC8 >= 0) {
        SpecialStageHud_UpdateCheckpoint();
        SpecialStageHud_AnimateSprites();
        return;
    }
    if (w->unkBCC >= 0) {
        SpecialStageHud_UpdateGoal();
        SpecialStageHud_AnimateSprites();
        return;
    }
    if (w->unkBD0 >= 0)
        SpecialStageHud_UpdateFail();
    SpecialStageHud_UpdateRingCounter();
    SpecialStageHud_UpdateQuotaCounter();
    if (w->unkAE0 >= 0x17c && w->unkAE0 < 0x258) {
        w->spr2[9].x = 0x80;
        w->spr2[9].y = 0x6e;
        AnimatorSprite_ProcessAnimation(&w->spr2[9], 0, 0);
    }
    SpecialStageHud_AnimateSprites();
}

/* ------------------------------------------------------------ draw */
static void Draw(SpSprite *s) { sp_draw_sprite(s, 0x1000, 0x1000, 0); }
static void DrawRS(SpSprite *s, s32 a, s32 b) { sp_draw_sprite(s, a, b, 1); }

static void SpecialStageHud_DrawRingCounter(void) {
    SpHud *g = gSpecialStageHud;
    s32 v = SpecialStage_GetPlayer()->unk148;
    if (v >= 100) {
        Draw(&g->spr[2]);
        Draw(&g->spr[3]);
        Draw(&g->spr[4]);
    } else if (v >= 10) {
        Draw(&g->spr[3]);
        Draw(&g->spr[4]);
    } else
        Draw(&g->spr[4]);
}

static void SpecialStageHud_DrawRingCounterScaled(s32 t) {
    SpHud *g = gSpecialStageHud;
    s32 v = SpecialStage_GetPlayer()->unk148;
    s32 s = 0x1000;
    if (t >= 0 && t <= 0x14)
        s += (s32)(1228.8f * (0x14 - t) / 20.0f);
    else if (t > 0x14 && t <= 0x28)
        s += (s32)(1228.8f * (0x28 - t) / 20.0f);
    else if (t > 0x28 && t <= 0x3c)
        s += (s32)(1228.8f * (0x3c - t) / 20.0f);
    if (v >= 100) {
        DrawRS(&g->spr[2], s, s);
        DrawRS(&g->spr[3], s, s);
        DrawRS(&g->spr[4], s, s);
    } else if (v >= 10) {
        DrawRS(&g->spr[3], s, s);
        DrawRS(&g->spr[4], s, s);
    } else
        DrawRS(&g->spr[4], s, s);
}

static void SpecialStageHud_DrawDigits(void) {
    SpHud *g = gSpecialStageHud;
    Draw(&g->spr[5]);
    Draw(&g->spr[6]);
    Draw(&g->spr[7]);
}

static void SpecialStageHud_DrawPopDigits(s32 t) {
    SpHud *g = gSpecialStageHud;
    s32 sa = 0x1000, sb = 0x1000, sc = 0x1000;
    if (t >= 0 && t <= 0x14)
        sa += (s32)(1228.8f * (0x14 - t) / 20.0f);
    else if (t > 0x14 && t <= 0x28)
        sb += (s32)(1228.8f * (0x28 - t) / 20.0f);
    else if (t > 0x28 && t <= 0x3c)
        sc += (s32)(1228.8f * (0x3c - t) / 20.0f);
    DrawRS(&g->spr[5], sa, sa);
    DrawRS(&g->spr[6], sb, sb);
    DrawRS(&g->spr[7], sc, sc);
}

static void SpecialStageHud_DrawEmeralds(void) {
    SpHud *g = gSpecialStageHud;
    SpSprite *o19, *o;
    s32 i, d, s;
    s16 sx, sy;
    if (g->unkBD4 <= 0x3c)
        Draw(&g->spr[0x15]);
    else
        Draw(&g->spr[0x16]);
    for (i = 0; i < 7; i++) {
        u32 m = SpecialStage_GetEmeraldFlags();
        if ((m & (1u << i)) && i != SpecialStage_GetStageIndex())
            Draw(&g->spr[0xb + i]);
        else {
            g->spr[10].x = g->spr[0xb + i].x;
            g->spr[10].y = g->spr[0xb + i].y;
            Draw(&g->spr[10]);
        }
    }
    if (g->unkBD4 < 0xf0)
        return;
    o19 = &g->spr[19];
    o = &g->spr[SpecialStage_GetStageIndex() + 0xb];
    d = g->unkBD4 - 0xf0;
    if (g->unkBD4 < 0xf5) {
        s = (d << 12) / 7 + 0x800;
        if (s > 0x1800)
            s = 0x1800;
    } else {
        s = 0x1800 - ((g->unkBD4 - 0xf5) * 0x1800) / 7;
        if (s < 0)
            s = 0;
    }
    if (s > 0) {
        o19->x = 0x80;
        o19->y = 0x78;
        DrawRS(o19, s, s);
    }
    if (g->unkBD4 < 0xf5)
        return;
    sx = o->x;
    sy = o->y;
    o->x = g->unkBF8;
    o->y = g->unkBFA;
    Draw(o);
    o->x = sx;
    o->y = sy;
    for (i = 0; i < 3; i++)
        Draw(&g->spr3[i]);
}

/* trick circles + timing bar (main screen) */
static void SpecialStageHud_Unk_ov001_02308884(void) {
    SpHud *g = gSpecialStageHud;
    SpObj *o;
    SpTrickSeq *d;
    s32 i, k;
    float f;
    if (SpecialStage_GetPlayer()->unk128 == 0)
        return;
    o = SpecialStage_GetPlayer();
    d = SpecialStageTrick_GetSequence(o->unk150);
    f = 1.0f - (float)(u32)(u16)o->unk14E / (float)(u32)d->unk20;
    for (i = 0; i < d->count; i++)
        Draw(&g->spr2[i]);
    k = SpecialStageTrick_GetRating(o->unk150, (u16)o->unk14E);
    sp_draw_ratingbar(f, k, k == 0 && (g->unkAE0 & 4));
    Draw(&g->spr2[8]);
}

void SpecialStageHud_Draw(void) {
    SpHud *g = gSpecialStageHud;
    s32 i;
    if (g->unkBD4 >= 0) {
        SpecialStageHud_DrawEmeralds();
        return;
    }
    Draw(&g->spr[0]);
    Draw(&g->spr[1]);
    g->spr[8].flags |= 0x80;
    Draw(&g->spr[8]);
    g->spr[8].flags &= ~0x80u;
    Draw(&g->spr[8]);
    if (g->unkBB8 > 0 && ((g->unkBB8 & 0x20) || g->unkBBC == 0))
        Draw(&g->spr[9]);
    for (i = 0; i < 7; i++) {
        u32 m = SpecialStage_GetEmeraldFlags();
        if (m & (1u << i))
            Draw(&g->spr[0xb + i]);
        else {
            g->spr[10].x = g->spr[0xb + i].x;
            g->spr[10].y = g->spr[0xb + i].y;
            Draw(&g->spr[10]);
        }
    }
    if (g->unkBA8 != 0)
        Draw(&g->spr[18]);
    if (g->unkBB0 != 0) {
        if (g->unkBB4 != 0) {
            g->spr[20].x = 0xe8;
            g->spr[20].y = 0xac;
        } else {
            g->spr[20].x = 0x80;
            g->spr[20].y = 0xac;
        }
        Draw(&g->spr[20]);
    }
    SpecialStageHud_Unk_ov001_02308884();
    if (g->unkAF0 > 0)
        Draw(&g->spr1);
    if (g->unkBC4 >= 0) {
        /* SpecialStageHud_Unk_ov001_023091a0 */
        if (g->unkBC4 >= 0x96 && g->unkBC4 < 0x118)
            Draw(&g->spr0);
        else if (g->unkBC4 >= 0x118 && g->unkBC4 <= 0x168)
            DrawRS(&g->spr1, g->unkBC0, g->unkBC0);
    }
    if (g->unkBC8 >= 0) {
        /* SpecialStageHud_Unk_ov001_02308fc8 */
        if (g->unkBC8 >= 1 && g->unkBC8 < 0x78)
            Draw(&g->spr1);
        else if (g->unkBC8 >= 0xf0 && g->unkBC8 < 0x168)
            Draw(&g->spr0);
        SpecialStageHud_DrawRingCounterScaled(g->unkBC8 - 1);
        SpecialStageHud_DrawPopDigits(g->unkBC8 - 0xb4);
        return;
    }
    if (g->unkBCC >= 0) {
        /* SpecialStageHud_Unk_ov001_02308e44 */
        if (g->unkBCC >= 1 && g->unkBCC < 0x78)
            Draw(&g->spr1);
        SpecialStageHud_DrawRingCounterScaled(g->unkBCC - 1);
        SpecialStageHud_DrawDigits();
        return;
    }
    if (g->unkBD0 >= 0)
        Draw(&g->spr1);   /* SpecialStageHud_Unk_ov001_02308d40 */
    SpecialStageHud_DrawRingCounter();
    SpecialStageHud_DrawDigits();
    /* the stylus demonstration (spr2[9], with the sp_fix_touch arc) is replaced by the controller
     * hint line drawn by sp_draw.c in the same frames (0x17c..0x258) */
}
