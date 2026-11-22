/*
 * Pause menu, translated from src/game/Menus/Pause:
 *   PauseMenu_Create, PauseMenu_Main, PauseMenu_Draw, PauseMenu_HighlightSelection,
 *   PauseMenu_Main_Quit, PauseMenu_QuitToMap, PauseMenu_QuitToTryAgain, _Destructor.
 *
 * DS services replaced:
 *   - System_Freeze / NNS_SndPlayerPauseByPlayerNo -> ui_is_paused() (main.c skips
 *     the world update) and g_ui_hooks.music_pause.
 *   - PauseMenu_HighlightSelection rewrote the OBJ palette of the unselected entry
 *     through the data_0207ba24 nibble LUT -> texture variant UI_VAR_DIM (same LUT,
 *     applied to the converted pixels in ui_draw.c).
 *   - gPad.rep (key repeat) -> this frame's presses.
 *   - Special-stage pause (alt = TRUE: palette 0xf, other sound ids) and the extra
 *     boss quit path are kept as code paths but not reachable from this module.
 */
#include "ui_internal.h"

typedef struct PauseWork {
    s16 sel;          /* 0x000 */
    s16 max;          /* 0x002 */
    u16 flags;        /* 0x004: 1 close, 2 entries shown, 4 frame off, 8 title off,
                         0x10 entries off, 0x20 special stage */
    u16 state;        /* 0x008 */
    s16 val;          /* 0x00A frame opening */
    UiSpr spr[4];     /* 0x020 frame, title, entry 0, entry 1 */
    struct { s32 x, y; } pos[4]; /* 0x140 */
    s16 mark[2];      /* 0x160 / 0x162 selection bump */
    s32 unk164, unk168, unk16C, unk170; /* sound archive / ids */
    u16 trig;         /* 0x176 */
    int quitting;     /* gTaskCurrent->fn == PauseMenu_Main_Quit */
    int active;
} PauseWork;

static PauseWork s_pause;

int PauseMenu_IsActive(void) { return s_pause.active; }

static void sfx(s32 arc, s32 id) {
    if (g_ui_hooks.sfx)
        g_ui_hooks.sfx(g_ui_hooks.ctx, arc, id);
}

/* PauseMenu_Destructor */
void PauseMenu_Destroy(void) {
    if (!s_pause.active)
        return;
    s_pause.active = 0;
    if (g_ui_hooks.music_pause)
        g_ui_hooks.music_pause(g_ui_hooks.ctx, 0);
}

static void PauseMenu_HighlightSelection(PauseWork *w) {
    /* the selected entry keeps its palette, the other one is darkened */
    (void)w;
}

void PauseMenu_Create(int alt) {
    PauseWork *w = &s_pause;
    u16 i, a, b, c;
    s32 lang;
    if (g_ui_bright_main != 0)                 /* gDisplayMain.unk48 */
        return;
    if (W.flags & 0x20)                        /* VS */
        return;
    if (g_ui_bright_sub != 0)
        return;
    if (g_obj_stage.check_time == W.frame)     /* data_022c4590.unk18 == gGameState.unk20 */
        return;
    if (w->active)
        return;
    memset(w, 0, sizeof(*w));
    w->active = 1;
    if (g_ui_hooks.music_pause)                /* System_Freeze + pause players 3..9 */
        g_ui_hooks.music_pause(g_ui_hooks.ctx, 1);
    if (!G.stage_new || W.mode == 2)
        w->flags |= 2;
    if (alt) {
        w->flags &= ~2;
        w->flags |= 0x20;
        w->unk164 = 0x10;
        w->unk168 = 0x12;
        w->unk16C = 0x13;
        w->unk170 = 0x14;
    } else {
        w->unk164 = 0;
        w->unk168 = 0x37;
        w->unk16C = 0x38;
        w->unk170 = 0x39;
    }
    /* ObjAction_Load2dBAC: frame = ac_fix_pause_frame, the rest ac_fix_pause */
    UiSpr_Init(&w->spr[0], UI_RES_PAUSE_FRAME, 0, 0);
    UiSpr_Init(&w->spr[1], UI_RES_PAUSE, 0, 0);
    UiSpr_Init(&w->spr[2], UI_RES_PAUSE, 0, 0);
    UiSpr_Init(&w->spr[3], UI_RES_PAUSE, 0, 0);
    b = 2;
    a = 3;
    if (W.mode == 2 || W.mode == 4)
        a++;
    lang = ui_lang();
    b += (u16)(lang * 3);
    a += (u16)(lang * 3);
    UiSpr_SetAnimation(&w->spr[2], b);
    UiSpr_SetAnimation(&w->spr[3], a);
    c = 0;
    if (lang > 3)
        c++;
    UiSpr_SetAnimation(&w->spr[1], c);
    for (i = 0; i < 4; i++)
        UiSpr_ProcessAnimation(&w->spr[i]);
    w->pos[0].x = 0x6000;
    w->pos[0].y = 0x6000;
    if (w->flags & 2) {
        w->pos[1].x = 0x6000;
        w->pos[1].y = 0x4000;
        w->max = 1;
        PauseMenu_HighlightSelection(w);
    } else {
        w->pos[1].x = 0x6000;
        w->pos[1].y = 0x5800;
        w->max = 0;
    }
    w->flags |= 0x18;
    sfx(w->unk164, w->unk168);
}

static void PauseMenu_Draw(PauseWork *w) {
    u16 i;
    if (w->trig != 0 && (w->flags & 2))
        w->flags &= ~0x18;
    switch (w->state) {
    case 0:
        w->state = 1;
        return;
    case 1:
        w->val = (s16)ObjShift_SetFromRef(w->val, 0xc00, 0, 1, 0x800, 0x200);
        if (w->val == 0xc00) {
            w->state = 2;
            w->flags &= ~0x18;
        }
        break;
    case 2:
        w->val = (s16)ObjShift_Set(w->val, 0x800, 1, 0x400, 0x200);
        break;
    case 3:
        break;
    }
    for (i = 0; i < 4; i++) {
        w->spr[i].x = (s16)(w->pos[i].x >> 8);
        w->spr[i].y = (s16)(w->pos[i].y >> 8);
    }
    if (!(w->flags & 4)) {
        w->spr[0].y -= w->val >> 8;
        ui_emit_spr(&w->spr[0], UI_SP_SCREEN);
        w->spr[0].y += (w->val >> 8) * 2;
        ui_emit(&w->spr[0], UI_SP_SCREEN, 0x1000, 0x1000, UI_FLIP_V, 0xffffffff, 0); /* flags ^= 0x100 */
    }
    if (!(w->flags & 8)) {
        if (w->flags & 2)
            w->spr[1].y -= (w->val - 0x800) >> 8;
        ui_emit_spr(&w->spr[1], UI_SP_SCREEN);
    }
    if (w->flags & 0x10)
        return;
    if (!(w->flags & 2))
        return;
    w->spr[2].y -= w->mark[0];
    w->spr[3].y += w->mark[1];
    if (w->mark[0] > 0)
        w->mark[0]--;
    if (w->mark[1] > 0)
        w->mark[1]--;
    /* PauseMenu_HighlightSelection: palette[cur] on entry cur, LUT-dimmed on the other */
    ui_emit(&w->spr[2], UI_SP_SCREEN, 0x1000, 0x1000, 0, 0xffffffff, w->sel == 0 ? 0 : UI_VAR_DIM);
    ui_emit(&w->spr[3], UI_SP_SCREEN, 0x1000, 0x1000, 0, 0xffffffff, w->sel == 1 ? 0 : UI_VAR_DIM);
}

/* PauseMenu_QuitToMap: SaveData_StoreLives + SceneChange_ToZoneMap / ZoneActSelect */
static void PauseMenu_QuitToMap(void) {
    G.save_lives[G.character == 1 ? 1 : 0] = (u8)W.lives;
    ui_request_exit(UI_EXIT_QUIT);
}

static void PauseMenu_Main_Quit(PauseWork *w) {
    if (BrightnessFade_IsDone()) {
        PauseMenu_Destroy();
        if (W.mode == 2) {
            ui_request_exit(UI_EXIT_TRY_AGAIN); /* StageExit_Create(PauseMenu_QuitToTryAgain, 3) */
            return;
        }
        /* zone 8 act 2 (extra boss) -> player select; special stage -> SpecialStage_RequestExit */
        PauseMenu_QuitToMap();
        return;
    }
    PauseMenu_Draw(w);
}

void PauseMenu_Main(const InputState *in) {
    PauseWork *w = &s_pause;
    u16 trig = ui_pad_bits(in->pressed);
    int moved;
    if (!w->active)
        return;
    w->trig = trig;
    if (w->quitting) {
        PauseMenu_Main_Quit(w);
        return;
    }
    if ((w->flags & 2) && (trig & 0xc0)) {
        moved = 0;
        if (trig & 0x40) {
            if (w->sel != 0)
                moved = 1;
            w->sel--;
            if (w->sel < 0)
                w->sel = 0;
        }
        if (trig & 0x80) {
            if (w->sel == 0)
                moved = 1;
            w->sel++;
            if (w->sel >= w->max)
                w->sel = w->max;
        }
        sfx(w->unk164, w->unk16C);
        w->mark[w->sel] = 2;
        if (moved)
            PauseMenu_HighlightSelection(w);
    }
    PauseMenu_Draw(w);
    if (trig & 1) {
        switch (w->sel) {
        case 0:
            w->flags |= 1;
            break;
        case 1:
            if (W.mode == 2)
                BrightnessFade_Create(4, 0x100);
            else
                BrightnessFade_Create(5, 0x100);
            w->quitting = 1;
            return;
        }
    }
    if (!(w->flags & 1) && !(trig & 0xa))
        return;
    sfx(w->unk164, w->unk168);
    PauseMenu_Destroy();
}
