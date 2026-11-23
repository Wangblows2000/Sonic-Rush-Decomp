/*
 * MsgBox_* (src/game/UI/MessageBox, 44 functions): the message box of the menus.
 * MsgBox_Create(msg, icon, buttons, engine, bg, a5, palette, vram) starts the task: two frames of
 * file loading (MsgBox_Main_LoadFiles), the frame BG (MsgBox_Main_InitBg: one of the five
 * bg2/caution/caution_base_*.bbg by data_02074c3c[msg]) on BG `bg` of `engine`, the icon
 * (icon 0 / 1: caution_<lang> anim 8 / 9), the buttons (0: OK anim 0, 1: YES anim 0 + NO anim 3
 * with the cursor of caution.bac, 2: anim 6, 3: none), the message (data_02074ca8[msg] file,
 * data_02074c04[msg] anim); then 8 frames of opening (MsgBox_SetSplitScroll: the lines above
 * data_02074bf0[kind] scroll in from -data_02074bdc[kind] px, the others from +that, linear
 * MsgBox_EaseLerp), the input (A / B / LEFT / RIGHT, the cursor blinks for 6 frames after a
 * move), 0x20 frames of the pressed button (MsgBox_Main_Confirm), 8 frames of closing, and the
 * result (MsgBox_Destructor: gMsgBoxResult 0 OK / YES, 1 NO, 2, 4).
 *
 * DS service                          here
 * BG `bg` + its line scroll           two mn_emit_bg_part pieces of the frame image in the sprite
 *   (MsgBox_StartLineScroll, HBlank)  order of the BG's priority, emitted after the box's sprites
 *                                     (sprites of a priority are in front of its BGs)
 * MsgBox_GetBgPriority (BGxCNT & 3)   g_mn_layer[engine][bg].prio
 * MsgBox_SetBgVisible                 the BG pieces are emitted only while the box is shown
 * ObjVram_Alloc / OBJ palette rows    not needed (converted atlas colours)
 * touch buttons (MsgBox_Is*Touched)   not used (no touch screen)
 */
#include <stdlib.h>
#include "mn_internal.h"

/* data_02074ca8: message -> file; data_02074c3c: message -> frame kind; data_02074c04: message -> anim */
static const u8 s_mb_file[27] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 5, 6, 6, 7, 7, 7, 7, 7};
static const u8 s_mb_kind[27] = {1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 2, 4, 0, 2, 2, 4, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2};
static const u8 s_mb_anim[27] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 1, 2, 3, 0, 1, 0, 0, 1, 0, 1, 2, 3, 4};
static const s16 s_mb_split[5] = {96, 96, 80, 96, 88};   /* data_02074bf0 */
static const s16 s_mb_dist[5] = {64, 64, 48, 64, 66};    /* data_02074bdc */
/* data_0207a80c[lang]: files 0..7 (sprites), 8..12 (frames by kind) */
static const char *const s_mb_spr[8] = {"caution", "caution_%s", "caution_a_%s", "caution_ad_%s",
                                        "caution_b_%s", "caution_c_%s", "caution_d_%s", "caution_e_%s"};
static const char *const s_mb_bg[5] = {"ann", "cau", "zsel", "search", "error"};
static const char *const s_mb_lang[6] = {"jpn", "eng", "fra", "deu", "ita", "spa"};

typedef struct {
    s32 t;              /* unk0: frames in the phase */
    s32 sel, nbtn;      /* unk4 cursor, unk8 button count */
    s32 btn[2];         /* unkC / unk10: button animations (-1 none) */
    u32 flags;          /* unk14 (MsgBox_SetFlags: 1 = YES plays SE 0x12) */
    s32 phase;          /* unk18: 1 files, 2 open, 3 input, 4 confirm, 5 close */
    s32 msg, icon, btns, engine, layer, prio, kind;
    s32 close_req;      /* unk48 (MsgBox_RequestClose) */
    MnRes *res[3];      /* caution.bac (cursor), caution_<lang>.bac (icon, buttons), the message's file */
    MnBgImg *bg;
    MnSpr spr[5];       /* 0 icon, 1 message, 2 / 3 buttons, 4 cursor */
    s32 anim[5];
    s32 have[5];
} MsgBoxWork;

static MnTask *s_mb_task;
static s32 s_mb_result;   /* gMsgBoxResult */
/* the files of a box destroyed during a frame: that frame's quad list may still draw from them, so
 * they are freed when the next frame starts (mn_quads_reset -> mn_msgbox_gc) */
static MnRes *s_mb_trash_res[3];
static MnBgImg *s_mb_trash_bg;
void mn_msgbox_gc(void) {
    int i;
    for (i = 0; i < 3; i++) {
        mn_res_free(s_mb_trash_res[i]);
        s_mb_trash_res[i] = NULL;
    }
    mn_bg_free(s_mb_trash_bg);
    s_mb_trash_bg = NULL;
}

BOOL MsgBox_IsClosed(void) { return s_mb_task == NULL; }
s32 MsgBox_GetResult(void) { return s_mb_result; }
void MsgBox_RequestClose(void) {
    if (s_mb_task)
        ((MsgBoxWork *)s_mb_task->work)->close_req = 1;
}
void MsgBox_SetFlags(u32 v) {
    if (s_mb_task)
        ((MsgBoxWork *)s_mb_task->work)->flags |= v;
}
void MsgBox_Destroy(void) {
    if (s_mb_task) {
        Task_Destroy(s_mb_task);
        s_mb_task = NULL;
    }
}

static void MsgBox_Destructor(void) {
    MsgBoxWork *w = TASK_WORK(MsgBoxWork);
    int i;
    mn_msgbox_gc();
    for (i = 0; i < 3; i++)
        s_mb_trash_res[i] = w->res[i];
    s_mb_trash_bg = w->bg;
    s_mb_task = NULL;   /* gMsgBoxTask: one box at a time */
    switch (w->btns) {
    case 0:
        s_mb_result = 0;
        break;
    case 1:
        s_mb_result = w->sel == 0 ? 0 : 1;
        break;
    case 2:
        s_mb_result = 2;
        break;
    default:
        s_mb_result = 4;
        break;
    }
}

/* MsgBox_InitSprite: anim `idx` of `res`, at the BG's priority (positions are the animation's own) */
static void mb_sprite(MsgBoxWork *w, int k, const MnRes *res, s32 idx) {
    if (!res)
        return;
    AnimatorSprite_Init(&w->spr[k], res, (u16)idx, 0, w->engine);
    w->spr[k].flags = (w->spr[k].flags & ~SPR_PRIO_MASK) | (u32)w->prio << 17;
    w->anim[k] = idx;
    w->have[k] = 1;
    AnimatorSprite_ProcessAnimation(&w->spr[k], 0, 0);
}
static void mb_draw(MsgBoxWork *w, int k) {
    if (!w->have[k])
        return;
    AnimatorSprite_ProcessAnimation(&w->spr[k], 0, 0);
    AnimatorSprite_Draw(&w->spr[k]);
}
static void mb_set_anim(MsgBoxWork *w, int k, s32 a) {
    if (w->have[k] && a != w->anim[k]) {
        AnimatorSprite_SetAnimation(&w->spr[k], (u16)a);
        w->anim[k] = a;
    }
}

/* MsgBox_SetSplitScroll(cur, 8, 0x1000): line y shows frame row y + vofs, vofs -d..0 above the
 * split and +d..0 below it */
static void mb_bg(MsgBoxWork *w, s32 cur) {
    s32 split = s_mb_split[w->kind], d = s_mb_dist[w->kind];
    s32 a = -d + d * cur / 8, b = d - d * cur / 8;
    s32 ys = a < 0 ? -a : 0, ye = 192 - b;
    if (!w->bg)
        return;
    mn_emit_bg_part(w->engine, w->prio, w->bg, 0, (float)ys, 0, (float)(ys + a), 256, (float)(split - ys));
    mn_emit_bg_part(w->engine, w->prio, w->bg, 0, (float)split, 0, (float)(split + b), 256, (float)(ye - split));
}

static void mb_cursor_pos(MsgBoxWork *w) {
    MnSpr *s = &w->spr[4];
    if (w->nbtn == 1) {
        s->x = 0x60;
        s->y = 0x6c;
    } else if (w->sel == 0) {
        s->x = 0x3c;
        s->y = 0x68;
    } else {
        s->x = 0x7c;
        s->y = 0x68;
    }
}

static void MsgBox_Main_Close(void) {
    MsgBoxWork *w = TASK_WORK(MsgBoxWork);
    if (w->t + 1 >= 8) {   /* the destructor hides the BG in the same frame (MsgBox_SetBgVisible(0)) */
        Task_Destroy(gTaskCurrent);
        return;
    }
    mb_bg(w, 7 - w->t);
    w->t++;
}
static void MsgBox_BeginClose(MsgBoxWork *w) {
    w->t = 0;
    w->phase = 5;
    gTaskCurrent->func = MsgBox_Main_Close;
}

static void MsgBox_Main_Confirm(void) {
    MsgBoxWork *w = TASK_WORK(MsgBoxWork);
    int i;
    if (w->icon != 3)
        mb_draw(w, 0);
    mb_draw(w, 1);
    for (i = 0; i < w->nbtn; i++) {
        s32 a = w->btn[i];
        if (i == w->sel) {
            a += w->nbtn == 1 ? 1 : 2;   /* the pressed button */
            w->spr[2 + i].flags |= SPR_LOOP;
        } else {
            w->spr[2 + i].flags &= ~SPR_LOOP;
        }
        mb_set_anim(w, 2 + i, a);
        mb_draw(w, 2 + i);
    }
    if (w->btns == 1) {
        mb_set_anim(w, 4, 0);
        mb_cursor_pos(w);
        w->spr[4].flags &= ~SPR_LOOP;
        if (w->nbtn > 1)
            mb_draw(w, 4);
    }
    mb_bg(w, 8);
    if (++w->t >= 0x20)
        MsgBox_BeginClose(w);
}

static void MsgBox_Main_Input(void) {
    MsgBoxWork *w = TASK_WORK(MsgBoxWork);
    BOOL done = FALSE, a, b;
    int i;
    if (mn_automap_enabled())   /* test autopilot (debug/automap.txt): LEFT, then A: YES / OK */
        gPad.cur = gPad.trg = (w->t & 1) ? PAD_A : PAD_LEFT;
    a = (gPad.trg & PAD_A) != 0;
    b = (gPad.trg & PAD_B) != 0;
    if (w->icon != 3)
        mb_draw(w, 0);
    mb_draw(w, 1);
    for (i = 0; i < w->nbtn; i++) {
        s32 an = w->btn[i];
        if (i == w->sel && w->nbtn == 2)
            an++;   /* the selected button */
        mb_set_anim(w, 2 + i, an);
        w->spr[2 + i].flags &= ~SPR_LOOP;
        mb_draw(w, 2 + i);
    }
    if (w->btns == 1) {
        s32 an = 0;
        if (w->t < 6) {   /* just moved: the cursor's other animation */
            an = 1;
            w->spr[4].flags |= SPR_LOOP;
        } else {
            w->spr[4].flags &= ~SPR_LOOP;
        }
        mb_set_anim(w, 4, an);
        mb_cursor_pos(w);
        if (w->nbtn > 1)
            mb_draw(w, 4);
    }
    mb_bg(w, 8);
    switch (w->btns) {
    case 0:
    case 2:
        if (a) {
            done = TRUE;
            SysSound_PlaySE(0);   /* SysSound_PlayDecideSE */
        }
        break;
    case 1:
        if (a) {
            done = TRUE;
            SysSound_PlaySE(w->sel == 0 && (w->flags & 1) ? 0x12 : 0);
        } else if (b) {
            done = TRUE;
            w->sel = 1;
            SysSound_PlaySE(1);   /* SysSound_PlayCancelSE */
        }
        break;
    case 3:
        if (a) {
            done = TRUE;
            SysSound_PlaySE(0);
        }
        break;
    default:
        break;
    }
    if (!done) {
        if (gPad.trg & PAD_LEFT) {
            if (w->sel > 0) {
                w->sel--;
                w->t = 0;
                SysSound_PlaySE(4);   /* SysSound_PlayCursorSE */
            }
        } else if (gPad.trg & PAD_RIGHT) {
            if (w->sel < w->nbtn - 1) {
                w->sel++;
                w->t = 0;
                SysSound_PlaySE(4);
            }
        }
    }
    w->t++;
    if (done) {
        if (b || w->btns == 3) {
            MsgBox_BeginClose(w);
        } else {
            w->t = 0;
            w->phase = 4;
            gTaskCurrent->func = MsgBox_Main_Confirm;
        }
    } else if (w->close_req) {
        MsgBox_BeginClose(w);
    }
}

static void MsgBox_Main_Open(void) {
    MsgBoxWork *w = TASK_WORK(MsgBoxWork);
    if (w->t >= 1)   /* MsgBox_SetBgVisible(1) at unk0 == 1 */
        mb_bg(w, w->t);
    if (++w->t >= 8) {
        w->t = 6;    /* MsgBox_BeginInput */
        w->phase = 3;
        gTaskCurrent->func = MsgBox_Main_Input;
    }
}

/* MsgBox_Main_LoadFiles (frame 0 waits, frame 1 loads the files), MsgBox_BeginSetup, then one
 * frame each for InitBg / InitIcon / InitButton / InitButton2 / InitCursor / InitMessage (frame 7
 * calls MsgBox_BeginOpen): the files are loaded and the sprites set up at frame 1 here, the
 * opening starts at frame 8 as on the DS */
static void MsgBox_Main_LoadFiles(void) {
    MsgBoxWork *w = TASK_WORK(MsgBoxWork);
    char p[64], n[32];
    const char *lang = s_mb_lang[mn_lang()];
    if (w->t != 1) {
        if (++w->t >= 8) {
            w->t = 0;
            w->phase = 2;
            gTaskCurrent->func = MsgBox_Main_Open;   /* MsgBox_BeginOpen */
        }
        return;
    }
    w->t++;
    snprintf(n, sizeof(n), s_mb_spr[0], lang);
    snprintf(p, sizeof(p), "act2/caution/%s.bac", n);
    if (w->btns == 1)
        w->res[0] = mn_res_load(p);
    snprintf(n, sizeof(n), s_mb_spr[1], lang);
    snprintf(p, sizeof(p), "act2/caution/%s.bac", n);
    if (w->icon <= 1 || w->btns <= 2)
        w->res[1] = mn_res_load(p);
    if (w->msg < 0x1b) {
        snprintf(n, sizeof(n), s_mb_spr[s_mb_file[w->msg]], lang);
        snprintf(p, sizeof(p), "act2/caution/%s.bac", n);
        w->res[2] = mn_res_load(p);
    }
    snprintf(p, sizeof(p), "bg2/caution/caution_base_%s.bbg", s_mb_bg[w->kind]);
    w->bg = mn_bg_load(p);
    /* InitIcon: caution_<lang> anim 8 / 9 */
    if (w->icon == 0 || w->icon == 1)
        mb_sprite(w, 0, w->res[1], w->icon == 0 ? 8 : 9);
    /* InitButton / InitButton2 */
    w->btn[0] = w->btns == 0 || w->btns == 1 ? 0 : w->btns == 2 ? 6 : -1;
    w->btn[1] = w->btns == 1 ? 3 : -1;
    if (w->btn[0] >= 0)
        mb_sprite(w, 2, w->res[1], w->btn[0]);
    if (w->btn[1] >= 0)
        mb_sprite(w, 3, w->res[1], w->btn[1]);
    /* InitCursor: caution.bac anim 0 */
    if (w->btns == 1)
        mb_sprite(w, 4, w->res[0], 0);
    /* InitMessage */
    if (w->msg < 0x1b)
        mb_sprite(w, 1, w->res[2], s_mb_anim[w->msg]);
}

/* MsgBox_Create(msg, icon, buttons, engine, bg, a5, palette, vram mode) */
void MsgBox_Create(s32 msg, s32 icon, s32 btns, s32 engine, s32 bg, s32 a5, s32 pal, u32 vram) {
    MsgBoxWork *w;
    (void)a5;
    (void)pal;
    (void)vram;
    MsgBox_Destroy();
    if (msg >= 0x1b)
        msg = 0x1c;
    if (icon >= 2)
        icon = 3;
    if (btns >= 4)
        btns = 5;
    s_mb_task = Task_Create(MsgBox_Main_LoadFiles, MsgBox_Destructor, 0, 0x1000, 0, sizeof(MsgBoxWork));
    w = (MsgBoxWork *)s_mb_task->work;
    memset(w, 0, sizeof(*w));
    w->msg = msg;
    w->icon = icon;
    w->btns = btns;
    w->engine = engine & 1;
    w->layer = bg & 3;
    w->prio = g_mn_layer[w->engine][w->layer].prio;   /* MsgBox_GetBgPriority */
    w->kind = msg < 0x1b ? s_mb_kind[msg] : 0;
    w->phase = 1;
    switch (btns) {
    case 1:
        w->sel = 1;   /* YES / NO: the cursor starts on NO */
        w->nbtn = 2;
        break;
    case 0:
    case 2:
        w->nbtn = 1;
        break;
    default:
        w->nbtn = 0;
        break;
    }
}
