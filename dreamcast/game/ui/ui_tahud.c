/*
 * Time-attack HUD parts, translated from src/game/UI/Hud:
 *   CheckpointTimePopup_Create, _State_Main, _Draw, _Destructor, _SetTimeDigits
 *   MarkerTimeList_Create, _State_Idle, _Draw, _DrawRow, _Destructor
 * and the attract-demo label of DemoPlay_Create / DemoPlay_Main (ui_demo_label).
 *
 * CheckpointTimePopup: the split time a marker shows in time attack (Marker_OnHit,
 * gGameState.unk10 == 2) or at every lap of a pass-check marker
 * (Marker_State_WaitPlayerPass). MarkerTimeList: the lap times of that marker
 * (Marker_Action_InitPassCheck). Both are StageObj tasks drawing the HUD digit
 * glyphs of ac_fix_moji (StageObj_Load2dBAC "/ac_fix_moji.bac" anim 0 with the OBJ
 * VRAM pointer moved to the glyph: Hud_GetDigitGlyphVram(0) + d * 0x80 = digit d,
 * Hud_GetDigitGlyphVramB = the orange set (anim d + 10 here, as Hud_DrawTime),
 * VramB + 0xba0 = the time separator (anim 0x15, as Hud_DrawTime's d2ec / d368)).
 *
 * Layout: DS main-screen coordinates in the HUD spaces (UI.md 4.3): the popup at
 * its DS place under the top-right anchor (DS x 256 = safe right), the list under
 * the bottom-left anchor (DS y 192 = safe bottom), at the HUD scale.
 */
#include <stdio.h>
#include "ui_internal.h"

/* ------------------------------------------------- CheckpointTimePopup */
typedef struct {
    int active;
    u32 unk8;        /* Hud_FramesToTimeDigits nibbles */
    s32 unkC;        /* frames shown */
    s32 unk10, unk14;/* position (DS px << 8) */
    UiSpr spr;
} CtpWork;
static CtpWork s_ctp;   /* data_ov000_0236a978: only the newest popup lives */

void CheckpointTimePopup_Destructor(void) { s_ctp.active = 0; }

/* CheckpointTimePopup_Create(player, n): a new popup replaces the old one
 * (CheckpointTimePopup_State_Main of the old one sees data_ov000_0236a978 != itself) */
void CheckpointTimePopup_Create(s32 unused, s32 n) {
    CtpWork *w = &s_ctp;
    (void)unused;
    memset(w, 0, sizeof(*w));
    w->active = 1;
    UiSpr_Init(&w->spr, UI_RES_MOJI, 0, 0);   /* StageObj_Load2dBAC(.., anim 0) */
    UiSpr_ProcessAnimation(&w->spr);
    w->unk10 = 0xf800;
    w->unk14 = 0x1400;
    w->unkC = 0;
    Hud_FramesToTimeDigits(&w->unk8, (u32)n);
}

/* StageObj_Main -> CheckpointTimePopup_State_Main (unk54 |= 4: destroyed) */
static void CheckpointTimePopup_State_Main(CtpWork *p0) {
    if (EnemyRoomHud_IsActive())   /* gEnemyRoomHudTask != 0 */
        p0->active = 0;
    p0->unkC++;
    if (p0->unkC > 0x12c)
        p0->active = 0;
}

static void put(UiSpr *s, u16 anim, s32 x, s32 y, int space) {
    if (s->anim != anim)
        UiSpr_SetAnimation(s, anim);
    s->x = (s16)(x >> 8);
    s->y = (s16)(y >> 8);
    ui_emit_spr(s, space);
}

/* CheckpointTimePopup_Draw: 5 digits right to left, separators at 0x14 / 0x2e */
static void CheckpointTimePopup_Draw(CtpWork *w) {
    s16 i;
    s32 ofs = 0;
    u32 sh = 0, d;
    int b = (G.frame24 & 8) != 0;   /* gGameState.unk24 & 8: the B (orange) glyphs */
    for (i = 0; i < 5; i++) {
        d = (w->unk8 & (0xfu << sh)) >> sh;
        put(&w->spr, (u16)(d + (b ? 10 : 0)), w->unk10 - ofs, w->unk14, UI_SP_HUD_TR);
        ofs += 0xa00;
        if (i == 1)
            ofs += 0x600;
        if (i == 3)
            ofs += 0x600;
        sh += 4;
    }
    for (ofs = 0x1400, i = 0; i < 2; i++) {
        put(&w->spr, 0x15, w->unk10 - ofs, w->unk14, UI_SP_HUD_TR);
        ofs += 0x1a00;
    }
}

/* ------------------------------------------------------ MarkerTimeList */
typedef struct {
    int active;
    s32 unk134[5];   /* lap digits */
    UiSpr spr;
} MtlWork;
static MtlWork s_mtl;   /* gMarkerTimeListTask */

void MarkerTimeList_Destructor(void) { s_mtl.active = 0; }
void MarkerTimeList_Create(void) {
    MtlWork *w = &s_mtl;
    memset(w, 0, sizeof(*w));
    w->active = 1;
    UiSpr_Init(&w->spr, UI_RES_MOJI, 0, 0);
    UiSpr_ProcessAnimation(&w->spr);
}
static void MarkerTimeList_State_Idle(void) {}

/* CheckpointTimePopup_SetTimeDigits(n, t): row n of the list */
void CheckpointTimePopup_SetTimeDigits(s32 p0, s32 p1) {
    if (!s_mtl.active || p0 < 0 || p0 >= 5)
        return;
    Hud_FramesToTimeDigits((u32 *)&s_mtl.unk134[p0], (u32)p1);
}

/* MarkerTimeList_DrawRow: lap number k + 1, 5 digits, 2 separators */
static void MarkerTimeList_DrawRow(MtlWork *w, s32 x, s32 y, u32 digits, u16 k) {
    s16 i;
    s32 off = 0;
    u32 sh, d;
    put(&w->spr, (u16)(k + 1), x, y, UI_SP_HUD_BL);   /* unk34 += (k + 1) << 7 */
    off -= 0x4600;
    for (i = 0, sh = 0; i < 5; i++, sh += 4) {
        d = (digits & (0xfu << sh)) >> sh;
        put(&w->spr, (u16)d, x - off, y, UI_SP_HUD_BL);
        off += 0xa00;
        if (i == 1)
            off += 0x600;
        if (i == 3)
            off += 0x600;
    }
    for (i = 0, off = -0x3200; i < 2; i++, off += 0x1a00)
        put(&w->spr, 0x15, x - off, y, UI_SP_HUD_BL);
}
static void MarkerTimeList_Draw(MtlWork *w) {
    u16 i;
    for (i = 0; i < 5; i++)
        MarkerTimeList_DrawRow(w, 0x800, i * 0x1000 + 0x7c00, (u32)w->unk134[i], i);
}

int CheckpointTimePopup_IsActive(void) { return s_ctp.active; }
int MarkerTimeList_IsActive(void) { return s_mtl.active; }

/* the StageObj tasks (priority 0xff): state while the world runs, drawn every frame */
void ui_tahud_frame(int run) {
    if (s_ctp.active) {
        if (run)
            CheckpointTimePopup_State_Main(&s_ctp);
        if (s_ctp.active)
            CheckpointTimePopup_Draw(&s_ctp);
    }
    if (s_mtl.active) {
        if (run)
            MarkerTimeList_State_Idle();
        MarkerTimeList_Draw(&s_mtl);
    }
}
void ui_tahud_reset(void) {
    CheckpointTimePopup_Destructor();
    MarkerTimeList_Destructor();
}

/* public entries (the objects module's marker code) */
void ui_checkpoint_popup(u32 time) { CheckpointTimePopup_Create(0, (s32)time); }
void ui_marker_time_list(int create) {
    if (create)
        MarkerTimeList_Create();
    else
        MarkerTimeList_Destructor();
}
void ui_marker_time_set(int lap, u32 time) { CheckpointTimePopup_SetTimeDigits(lap, (s32)time); }

/* ------------------------------------------------- attract-demo label */
/* DemoPlay_Create: File_Load(data_02073b38[lang]) = act/dm_demoplay(_<lang>).bac,
 * AnimatorSpriteDS sprA = anim 2 ("DEMO PLAY"), sprB = anim 0 ("PRESS START"), both
 * at (0, 0) of each DS screen (the images carry their place), one per screen
 * (Display_IsMainScreenOnBottom picks which). DemoPlay_Main draws them while
 * timer & 0x40. Single screen: both labels in the one frame at the frame scale
 * (x2.5): "DEMO PLAY" at its own place, "PRESS START" composed under it
 * (DS +24 px), never stretched. */
static int s_demo_label = -1;   /* -1 not loaded, 0 hidden, 1 shown */
static UiSpr s_demo_spr[2];
static const char *const k_demoplay_id[6] = {
    "act__dm_demoplay", "act__dm_demoplay_eng", "act__dm_demoplay_fra",
    "act__dm_demoplay_deu", "act__dm_demoplay_ita", "act__dm_demoplay_spa",
};   /* data_02073b38 */

void ui_demo_label(int visible) {
    if (visible < 0) {   /* DemoPlay_Destructor */
        if (s_demo_label >= 0)
            ui_res_free(UI_RES_DEMOPLAY);
        s_demo_label = -1;
        return;
    }
    if (s_demo_label < 0) {
        if (ui_res_load(UI_RES_DEMOPLAY, k_demoplay_id[ui_lang()]) != 0 &&
            ui_res_load(UI_RES_DEMOPLAY, k_demoplay_id[0]) != 0)
            fprintf(stderr, "ui: demo label sprites missing\n");
        UiSpr_Init(&s_demo_spr[0], UI_RES_DEMOPLAY, 2, 0);
        UiSpr_Init(&s_demo_spr[1], UI_RES_DEMOPLAY, 0, 0);
        UiSpr_ProcessAnimation(&s_demo_spr[0]);
        UiSpr_ProcessAnimation(&s_demo_spr[1]);
    }
    s_demo_label = visible ? 1 : 0;
}
int ui_demo_label_shown(void) { return s_demo_label == 1; }

void ui_demo_label_draw(void) {
    if (s_demo_label != 1)
        return;
    s_demo_spr[0].x = 0;
    s_demo_spr[0].y = 0;
    ui_emit_spr(&s_demo_spr[0], UI_SP_SCREEN);
    s_demo_spr[1].x = 0;
    s_demo_spr[1].y = 24;
    ui_emit_spr(&s_demo_spr[1], UI_SP_SCREEN);
}
