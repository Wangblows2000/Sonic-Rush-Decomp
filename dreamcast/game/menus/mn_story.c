/*
 * Story progression (src/game/GameSystem/StoryProgress_*, Game_StartStoryStage)
 * and the scene requests the front end hands to main.c.
 * gGameState / data_022c4590 fields: MnGameState (mn_internal.h).
 */
#include "mn_internal.h"

MnGameState gGameState;
MenuRequest g_mn_request;

/* data_02074bbc / data_02074bb4: the story step at which zone i is done */
static const u8 *story_tbl(void) { return gGameState.unk0 != 1 ? g_mn_data.story_s : g_mn_data.story_b; }

u8 StoryProgress_CalcMapStep(u16 t) {
    u8 r = 0, i = 0;
    const u8 *tbl = story_tbl();
    for (; i < 7; i++) {
        if (t < tbl[i]) {
            if (t == tbl[i] - 1)
                r += 2;
            return r;
        }
        r += 3;
    }
    if (t < tbl[7])
        return r;
    if (gSaveData.unk8 & 0x1000)
        return (u8)(r + 2);
    return (u8)(r + 1);
}

u8 StoryProgress_GetZoneAt(u32 i) {
    if (i > 8)
        i = 8;
    return g_mn_data.zone_ord[gGameState.unk0 == 1][(u8)i];
}

u8 StoryProgress_GetZoneIndex(u8 v) {
    const u8 *tbl = g_mn_data.zone_ord[gGameState.unk0 == 1];
    u8 i;
    for (i = 0; i < 9; i++)
        if (v == tbl[i])
            return i;
    return 0xff;
}

/* empty in the ROM (bx lr): returns its argument */
u8 StoryProgress_GetActIndex(u8 act) { return act; }
u8 StoryProgress_GetActAt(u8 act) { return act; }

BOOL StoryProgress_IsStageNew(void) {
    u32 z = StoryProgress_GetZoneIndex(gGameState.unk1C);
    u32 a = StoryProgress_GetActIndex(gGameState.unk1D);
    u8 v;
    if (z == 8)
        return (gSaveData.unk8 & 0x1000) == 0;
    if (z > 7)
        return FALSE;   /* 0xff: not a story zone (the DS reads past the table) */
    if (gGameState.unk0 != 1) {
        v = g_mn_data.story_s[z];
        if (a <= 1)
            v = (u8)(v - 1);
        return v > gSaveData.unkC;
    }
    v = g_mn_data.story_b[z];
    if (a <= 1)
        v = (u8)(v - 1);
    return v > gSaveData.unk64;
}

void StoryProgress_RestoreFromSave(void) {
    u8 v, i;
    if (gGameState.unk0 != 1) {
        gGameState.unkE = StoryProgress_CalcMapStep(gSaveData.unkC);
        v = gSaveData.unkE;
        if (v > 99)
            v = 99;
        gGameState.lives = v;
        gGameState.unk28 = gSaveData.unkF;
    } else {
        gGameState.unkE = StoryProgress_CalcMapStep(gSaveData.unk64);
        v = gSaveData.unk66;
        if (v > 99)
            v = 99;
        gGameState.lives = v;
        gGameState.unk28 = 0;
        for (i = 0; i < 7; i++)
            if (gSaveData.unk64 >= g_mn_data.story_b[i])
                gGameState.unk28 |= (u16)(1 << i);
    }
    gGameState.unk14 = (gSaveData.unk8 & 1) ? 0 : 1;
}

void StoryProgress_Update(void) {
    u32 z = StoryProgress_GetZoneIndex(gGameState.unk1C);
    u8 a = StoryProgress_GetActIndex(gGameState.unk1D);
    if (z >= 7)
        return;
    if (a == 0) {
        if (StoryProgress_IsStageNew())
            gGameState.new_flags |= 1;
        return;
    }
    if (StoryProgress_IsStageNew()) {
        gGameState.new_flags |= 1;
        if (gGameState.unk0 != 1)
            SaveData_SetSonicProgress(a != 2 ? g_mn_data.story_s[z] - 1 : g_mn_data.story_s[z]);
        else
            SaveData_SetBlazeProgress(a != 2 ? g_mn_data.story_b[z] - 1 : g_mn_data.story_b[z]);
    }
    if (gGameState.unk0 != 1)
        gGameState.unkE = StoryProgress_CalcMapStep(gSaveData.unkC);
    else
        gGameState.unkE = StoryProgress_CalcMapStep(gSaveData.unk64);
}

/* Game_StartStoryStage(zoneIndex, act) */
void Game_StartStoryStage(s32 p0, s32 p1) {
    gGameState.unk10 = 0;
    gGameState.unk18 &= ~0x20u;
    gGameState.unk18 &= ~0x10u;
    gGameState.unk1C = StoryProgress_GetZoneAt((u8)p0);
    gGameState.unk1D = StoryProgress_GetActAt((u8)p1);
    StoryProgress_RestoreFromSave();
    NaviM_SaveStep();
    /* act < 2: SceneChange_ToStage; else DemoScript_StartScript0 (the boss
     * intro cutscene, then the boss act): the port starts the boss act. */
    mn_request_stage();
}

/* -------------------------------------------------------------- requests */
static void fill_request(MenuRequestKind k) {
    MenuRequest *r = &g_mn_request;
    memset(r, 0, sizeof(*r));
    r->kind = k;
    r->zone = gGameState.unk1C;
    r->act = gGameState.unk1D;
    r->character = gGameState.unk0;
    r->mode = gGameState.unk10;
    r->demo = gGameState.unk1E;
    r->lives = gGameState.lives;
    r->emeralds = gGameState.unk28;
    r->easy = gGameState.unk14;
}
void mn_request_stage(void) { fill_request(MENU_REQ_STAGE); }
void mn_request_demo(void) { fill_request(MENU_REQ_DEMO); }
void mn_request_unavailable(const char *scene) {
    fill_request(MENU_REQ_UNAVAILABLE);
    g_mn_request.scene = scene;
}

/* ------------------------------------------------------------------ sound */
/* SysSound_PlaySE -> SysSound_StartSE: SE index -> (SSAR, sequence) of the
 * sys SDAT. Cases 10..19 use the character's archive (2 Sonic, 3 Blaze). */
void SysSound_PlaySE(s32 n) {
    static const s8 arc_tab[23] = { 1, 1, 1, 1, 1, 1, 1, 0x11, 0x11, 0x11,
                                    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0xa, 0xa, 0xa };
    static const u8 seq_tab[23] = { 0, 1, 2, 3, 5, 0xb, 4, 0, 1, 2,
                                    0, 1, 2, 3, 4, 5, 6, 7, 8, 0x10, 0, 1, 2 };
    s32 a;
    if (n < 0 || n >= 23 || !g_menu_hooks.se)
        return;
    a = arc_tab[n] < 0 ? (gGameState.unk0 == 0 ? 2 : 3) : arc_tab[n];
    g_menu_hooks.se(a, seq_tab[n]);
}
/* SysSound_PlayBGM -> SysSound_StartBGM: BGM index -> sys SDAT sequence
 * (0 SEQ_MENU, 1 SEQ_JI_EXTRA, 2 SEQ_DL_SELECT, 3 SEQ_DL_RESULT,
 *  4 SEQ_ZONE_NOR, 5 SEQ_ZONE_LAST); 6 and up stop the BGM.
 * gSysSound->unk4 (the BGM playing) starts as 7 at SysSound_Init; SysSound_Release
 * frees gSysSound, so the next scene's SysSound_Init + PlayBGM starts the BGM again
 * even when it is the same index (zone map -> stage -> zone map, map talks). */
static const u8 s_bgm_seq[6] = { 1, 2, 0x20, 0x21, 3, 4 };
static s32 s_bgm_cur = 7;
void SysSound_PlayBGM(s32 n) {
    if (n == s_bgm_cur)
        return;
    s_bgm_cur = n;
    if (n >= 0 && n < 6) {
        if (g_menu_hooks.bgm)
            g_menu_hooks.bgm(s_bgm_seq[n]);
    } else if (g_menu_hooks.stop_all)
        g_menu_hooks.stop_all();
}
/* SysSound_Release (+ the SysSound_Init that follows it): all sound stopped, no BGM */
void SysSound_Release(void) {
    if (g_menu_hooks.stop_all)
        g_menu_hooks.stop_all();
    s_bgm_cur = 7;
}
/* a stage or a cutscene played its own music: the next PlayBGM starts the BGM again */
void mn_bgm_forget(void) { s_bgm_cur = 7; }
void menus_restart_bgm(void) {
    s32 n = s_bgm_cur;
    s_bgm_cur = 7;
    if (n >= 0 && n < 6)
        SysSound_PlayBGM(n);
}
void Sound_PlaySeq(s32 seq) {
    if (g_menu_hooks.bgm)
        g_menu_hooks.bgm(seq);
}
void Sound_PlaySeqArc(s32 arc, s32 id) {
    if (g_menu_hooks.se)
        g_menu_hooks.se(arc, id);
}
void Sound_PlayVoice(s32 id) {
    if (g_menu_hooks.voice)
        g_menu_hooks.voice(id);
}
void Sound_PlayVoiceAlt(s32 id) {
    if (g_menu_hooks.voice_alt)
        g_menu_hooks.voice_alt(id);
}
void Sound_PlayVoiceArc(s32 arc, s32 id) {
    if (g_menu_hooks.voice_arc)
        g_menu_hooks.voice_arc(arc, id);
}
void NNS_SndStopSoundAll(void) {
    if (g_menu_hooks.stop_all)
        g_menu_hooks.stop_all();
    s_bgm_cur = 7;   /* the BGM stopped too: the next SysSound_PlayBGM starts it */
}
