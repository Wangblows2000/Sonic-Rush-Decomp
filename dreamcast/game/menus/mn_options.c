/*
 * Options (src/game/Menus/Options/): difficulty, time limit, sound test,
 * delete save data, exit with a save commit. The settings code is
 * translated as is (Options_LoadSettingsFromSave, OptionsMenu_SetDifficultyChoice,
 * Options_SetTimeLimit, Options_EraseSaveData, Options_ExitToTitleMenu,
 * OptionsMenu_State_TimeLimit / _SoundTest input rules, SoundTest_BuildTrackNameText,
 * OptionsMenu_DrawSoundTestTrackName). The key-frame slides of the original (mnu_opti.ama:
 * Options_InitOpenAnims / SelectAnims / CursorMoveAnims / EnterItemAnims / ExitAnims,
 * OptionsMenu_InitBackAnims / InitEnterItemAnims, OptionsMenu_UpdateAnims, OptionsMenu_DrawItems,
 * the exit fades of Options_Action_Exit / OptionsMenu_Action_ExitOptions) are translated; the page
 * framework (OptionsMenu_OpenPage* / ClosePage* / State_*) is rewritten as one state with a phase
 * (Options_State_Main) and the pages drawn at the page origin (Options_DrawPage: DrawPage0..4).
 * PLAYER DATA (added with the time-attack / records scenes, rewritten like the rest):
 * BATTLE PLAY RECORD -> Options_ExitToRecords (Records_Start), TIME ATTACK RECORD ->
 * ACT / BOSS -> Options_ExitToTimeAttackMenuMode2 / 3; the way back is
 * Options_StartFromRecords (page 0 child 0) and Options_StartFromZoneMapPauseChoice2 / 3
 * (page 1 child 0 / 1), as OptionsMenu_Create(.., page, child) in the original.
 * Not available: DOWNLOAD DEMO (DS wireless).
 * Touch -> pad: list = UP/DOWN + A, back button = B, choice buttons =
 * LEFT/RIGHT + A, sound test arrows = LEFT/RIGHT, play = A, stop = B.
 */
#include <stdlib.h>
#include "mn_internal.h"

enum { OPT_PLAYER_DATA, OPT_DIFFICULTY, OPT_TIME_LIMIT, OPT_SOUND_TEST, OPT_ERASE, OPT_COUNT };
enum { PG_LIST, PG_DIFFICULTY, PG_TIME_LIMIT, PG_ERASE1, PG_ERASE2, PG_SOUND, PG_PLAYER, PG_TA_KIND };
#define OPT_NSPR 40
/* the task states of the original: Options_State_WaitOpenAnims, Options_State_Select, OptionsMenu_State_EnterItem,
 * the page states, OptionsMenu_State_WaitOpenAnims after OptionsMenu_Action_Back, Options_State_Exit,
 * OptionsMenu_State_Unk_ov001_022ccbe0 (after OptionsMenu_Action_ExitOptions) */
enum { PH_OPEN, PH_SELECT, PH_ENTER, PH_PAGE, PH_BACK, PH_EXIT, PH_LEAVE };
/* mnu_opti.ama sequences (the table of Options_Unk_ov001_022ce960: 0, 1, 7, 13 .. 27) */
enum {
    AS_UNSEL = 0,        /* item at rest (248) */
    AS_OPEN_SEL = 1,     /* + k: item k slides in from 456 to 240, then down to y 24 (k + 1) */
    AS_OPEN_UNSEL = 7,   /* + k: the same to 248 */
    AS_SEL = 13,         /* 248 -> 240 in 5 frames (cursor move; reversed for the old item) */
    AS_ENTER_SEL = 14,   /* 240 -> 0 (bounce) in 33 frames */
    AS_ENTER_UNSEL = 15, /* 248 -> -16 */
    AS_BACK_SEL = 17,    /* 0 -> 240 (bounce) */
    AS_BACK_UNSEL = 18,  /* -16 -> 248 in 12 frames */
    AS_PG_SELECT = 19,   /* page origin at (240, 0) */
    AS_PG_OPEN = 20,     /* 448 -> 240 */
    AS_PG_MOVE = 21,     /* 240 -> 244 -> 240 in 9 frames */
    AS_PG_ENTER = 22,    /* 240 -> 0 (bounce) */
    AS_PG_BACK = 23,     /* 0 -> 240 (bounce) */
    AS_BN_SELECT = 24,   /* banner at (0, 144) */
    AS_BN_OPEN = 25,     /* -160 -> 0 */
    AS_BK_SELECT = 26,   /* back button at (16, 80) */
    AS_BK_OPEN = 27      /* -32 -> 16 */
};

typedef struct {
    char name[0x29];
    u8 unk29;   /* sound archive (0 = the main SDAT, else gSoundTestArchiveNameTable) */
    u8 type;    /* 0 BGM (group 14 of archive 0 = stream), 1 SE, 2 voice */
    u8 unk2b;   /* group */
    u8 unk2c;   /* sequence archive / voice bank */
    u8 unk2d;   /* 1 = only after the extra story */
    u16 id;
} SoundTestEntry;

typedef struct {
    s32 page;
    s32 sel;             /* list cursor */
    s32 choice;          /* choice page cursor */
    s32 chosen;          /* -1 none */
    s32 unk14, unk18, unk1c;   /* timers as in OptionsMenu_State_* */
    BOOL settings[2];    /* Options_LoadSettingsFromSave: [0] easy, [1] time limit on */
    s32 diff_choice, tl_choice;   /* unk3ac + 8 / + 0xc */
    s32 items[OPT_COUNT], nitems;
    /* sound test */
    s32 cur, playing, scroll;
    s32 played, play_frames;   /* sub->sel (the loaded track), sub->unk20 (frames since it started) */
    s32 text[0x29];
    s32 moved;           /* frames since the last cursor move (OptionsMenu page unk14: cursor blink) */
    s32 phase;           /* PH_*: the task state of the original */
    u32 sprio;           /* the OBJ priority (flags 0x60000) of the sprites being drawn */
    s32 ptimer;          /* the work's +0 counter (Options_State_Exit, OptionsMenu_State_Unk_022ccbe0) */
    void *ama;           /* mnu_opti.ama */
    KeyframeAnim apage, abanner, aback;   /* the page origin (+0x208), the OPTIONS banner, the back button */
    KeyframeAnim ait[OPT_COUNT];          /* the list items */
    MnSpr spr[OPT_NSPR], glyph[41];
    s32 anim[OPT_NSPR], ganim[41];
    const MnRes *sres[OPT_NSPR];
    MnRes *res_lang, *res_font, *res_titl;
    MnRes *res_opti, *res_sou, *res_sou_lang;   /* mnu_opti.bac (cursor frames), mnu_opti_sou(_<lang>).bac */
    MnBgImg *bg[4];
    u8 *sld;
    const SoundTestEntry *tracks;
    s32 ntracks;
} OptionsWork;

static void OptionsMenu_SetDifficultyChoice(OptionsWork *p, s32 v) {
    if (p->diff_choice != v) {
        p->diff_choice = v;
        switch (v) {
        case 0:
            SaveData_SetDifficulty(1);
            break;
        case 1:
            SaveData_SetDifficulty(0);
            break;
        }
    }
}

static void Options_SetTimeLimit(OptionsWork *p, s32 v) {
    if (p->tl_choice != v) {
        p->tl_choice = v;
        SaveData_SetTimeLimit(v);
    }
}

static void Options_LoadSettingsFromSave(OptionsWork *p) {
    p->settings[0] = (gSaveData.unk8 & 1) ? TRUE : FALSE;
    p->settings[1] = (gSaveData.unk8 & 2) ? FALSE : TRUE;
    p->diff_choice = p->settings[0] ? 1 : 0;
    p->tl_choice = p->settings[1] ? 1 : 0;
}

static void Options_EraseSaveData(OptionsWork *p) {
    SaveData_FormatOrShowError();
    Options_LoadSettingsFromSave(p);
}

/* SoundTest_BuildTrackNameText (one line) */
static void SoundTest_BuildTrackNameText(OptionsWork *p, s32 a) {
    s32 i = 0;
    char c;
    if (a >= 0 && a < p->ntracks) {
        const char *s = p->tracks[a].name;
        for (; i < 0x29; i++) {
            if (i == 0) {
                p->text[i] = 0x28;
                continue;
            }
            c = s[i - 1];
            if (c == 0)
                break;
            if (c >= 'A' && c <= 'Z')
                p->text[i] = c - 0x40;
            else if (c >= '0' && c <= '9')
                p->text[i] = c - 0x15;
            else if (c == '(')
                p->text[i] = 0x25;
            else if (c == ')')
                p->text[i] = 0x26;
            else if (c == '-')
                p->text[i] = 0x27;
            else if (c == ',')
                p->text[i] = 0x29;
            else if (c == '\'')
                p->text[i] = 0x2a;
            else if (c == '#')
                p->text[i] = 0x2b;
            else if (c == '!')
                p->text[i] = 0x2c;
            else
                p->text[i] = 0;
        }
    }
    for (; i < 0x29; i++)
        p->text[i] = 0x2e;
    p->scroll = 0;
}

/* OptionsMenu_DrawSoundTestTrackName: scrolling ticker of glyph sprites */
static void OptionsMenu_DrawSoundTestTrackName(OptionsWork *w, s32 x, s32 y) {
    s32 left = x - 0x27, i, n = 0, width = 0, s, pos, cx;
    for (i = 0; i < 41; i++) {
        if (w->text[i] == 0x2e)
            break;
        width += g_mn_data.st_widths[w->text[i] & 0x3f];
        n++;
    }
    width += 0x10;
    if (width < 0x5e)
        width = 0x5e;
    s = w->scroll - 0x10;
    if (s < 0)
        s = 0;
    s <<= 0xb;
    if (s > 0 && s % (width << 12) == 0)
        w->scroll = 0x10;
    pos = left - (s >> 12) % width;
    cx = pos;
    while (pos < left + 0x4e) {
        for (i = 0; i < n; i++) {
            MnSpr *spr = &w->glyph[i];
            if (cx >= left - 8 && cx < left + 0x4e) {
                if (w->text[i] != w->ganim[i]) {
                    AnimatorSprite_SetAnimation(spr, (u16)w->text[i]);
                    w->ganim[i] = w->text[i];
                }
                spr->x = (s16)cx;
                spr->y = (s16)y;
                AnimatorSprite_ProcessAnimation(spr, 0, 0);
                AnimatorSprite_Draw(spr);
            }
            cx += g_mn_data.st_widths[w->text[i] & 0x3f];
            if (cx >= left + 0x4e)
                break;
        }
        pos += width;
        cx = pos;
    }
    w->scroll++;
}

/* one sprite of `res` in slot `slot` (re-initialised when the slot changes resource); flags: SPR_HFLIP /
 * SPR_VFLIP (set or cleared), SPR_LOOP (set) as AnimatorSprite flags; the OBJ priority is w->sprio */
static void opt_spr_r(OptionsWork *w, int slot, const MnRes *res, s32 anim, s32 x, s32 y, u32 flags) {
    MnSpr *s = &w->spr[slot];
    if (!res)
        return;
    if (w->sres[slot] != res) {
        AnimatorSprite_Init(s, res, (u16)anim, 0, 1);
        w->sres[slot] = res;
        w->anim[slot] = anim;
    }
    if (w->anim[slot] != anim) {
        AnimatorSprite_SetAnimation(s, (u16)anim);
        w->anim[slot] = anim;
    }
    s->flags = (s->flags & ~(u32)(SPR_HFLIP | SPR_VFLIP | SPR_PRIO_MASK)) | flags | w->sprio;
    /* oamPrio (unk46): the choice buttons 4, the cursor frames 2 (022ca248 / 022ca1cc) / 1 (022ca150) */
    s->oam = (slot >= 9 && slot <= 12) ? 4 : slot == 20 ? 2 : slot == 21 ? 1 : 0;
    s->x = (s16)x;
    s->y = (s16)y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
static void opt_spr(OptionsWork *w, int slot, s32 anim, s32 x, s32 y) { opt_spr_r(w, slot, w->res_lang, anim, x, y, 0); }
/* DrawPage0's entries: the chosen one (m 2) loops its flash, the hovered one (m 1) does not */
static void opt_spr_loop(OptionsWork *w, int slot, s32 m) {
    if (m == 2)
        w->spr[slot].flags |= SPR_LOOP;
    else if (m == 1)
        w->spr[slot].flags &= ~(u32)SPR_LOOP;
}

/* the cursor frame of a choice (OptionsMenu_Unk_ov001_022ca248 / 022ca1cc / 022ca150: mnu_opti.bac anims
 * base + m): m 1 for 7 frames after a move, 2 (3: the first erase question's YES) for 8 frames after
 * the choice, else 0 */
static s32 opt_cursor_mode(const OptionsWork *w, s32 confirmed_mode) {
    if (w->chosen == w->choice)
        return w->unk18 < 8 ? confirmed_mode : 0;
    return w->moved < 7 ? 1 : 0;
}

/* ------------------------------------------------------------ key frames */
static void opt_anim(OptionsWork *w, KeyframeAnim *a, u32 seq, int mode) {   /* mode 1 last frame, 2 reversed */
    KeyframeAnim_Init(a, w->ama, seq);
    if (mode == 1)
        KeyframeAnim_SetToLastFrame(a);
    else if (mode == 2) {
        KeyframeAnim_PlayReverse(a);
        a->flags |= 0x10;
    }
}

/* Options_InitOpenAnims (reverse = 0) / Options_InitExitAnims (reverse = 1) */
static void opt_anims_open(OptionsWork *w, int reverse) {
    int i, m = reverse ? 2 : 0;
    opt_anim(w, &w->abanner, AS_BN_OPEN, m);
    opt_anim(w, &w->apage, AS_PG_OPEN, m);
    opt_anim(w, &w->aback, AS_BK_OPEN, m);
    for (i = 0; i < w->nitems; i++)
        opt_anim(w, &w->ait[i], (i == w->sel ? AS_OPEN_SEL : AS_OPEN_UNSEL) + i, m);
}

/* Options_InitSelectAnims */
static void opt_anims_select(OptionsWork *w) {
    int i;
    opt_anim(w, &w->abanner, AS_BN_SELECT, 0);
    opt_anim(w, &w->apage, AS_PG_SELECT, 0);
    opt_anim(w, &w->aback, AS_BK_SELECT, 0);
    for (i = 0; i < w->nitems; i++)
        opt_anim(w, &w->ait[i], i == w->sel ? AS_SEL : AS_UNSEL, i == w->sel ? 1 : 0);
}

/* Options_InitEnterItemAnims (pad: the touched item is the selected one) / OptionsMenu_InitBackAnims /
 * OptionsMenu_InitEnterItemAnims (a page at rest: OptionsMenu_Create at a page) */
static void opt_anims_page(OptionsWork *w, u32 pg, u32 sel, u32 unsel, int mode) {
    int i;
    opt_anim(w, &w->abanner, AS_BN_SELECT, 0);
    opt_anim(w, &w->apage, pg, mode);
    opt_anim(w, &w->aback, AS_BK_SELECT, 0);
    for (i = 0; i < w->nitems; i++)
        opt_anim(w, &w->ait[i], i == w->sel ? sel : unsel, mode);
}

/* OptionsMenu_UpdateAnims */
static BOOL opt_update_anims(OptionsWork *w) {
    BOOL done = TRUE;
    int i;
    for (i = 0; i < w->nitems; i++) {
        KeyframeAnim_Update(&w->ait[i]);
        if (!KeyframeAnim_IsFinished(&w->ait[i]))
            done = FALSE;
    }
    KeyframeAnim_Update(&w->abanner);
    if (!KeyframeAnim_IsFinished(&w->abanner))
        done = FALSE;
    KeyframeAnim_Update(&w->apage);
    if (!KeyframeAnim_IsFinished(&w->apage))
        done = FALSE;
    KeyframeAnim_Update(&w->aback);
    if (!KeyframeAnim_IsFinished(&w->aback))
        done = FALSE;
    return done;
}

/* Options_ResetItemPageState: the page of the selected item, with the choice at the current setting
 * (the page is drawn at the page origin also on the list, where it is off screen but for its panel) */
static void opt_reset_page(OptionsWork *w) {
    w->chosen = -1;
    w->unk1c = -1;
    switch (w->items[w->sel]) {
    case OPT_PLAYER_DATA:
        w->page = PG_PLAYER;
        w->choice = 0;
        break;
    case OPT_DIFFICULTY:
        w->page = PG_DIFFICULTY;
        w->choice = w->diff_choice;
        break;
    case OPT_TIME_LIMIT:
        w->page = PG_TIME_LIMIT;
        w->choice = w->tl_choice ? 0 : 1;
        break;
    case OPT_ERASE:
        w->page = PG_ERASE1;
        w->choice = 1;
        break;
    case OPT_SOUND_TEST:
        w->page = PG_SOUND;
        w->playing = 0;
        SoundTest_BuildTrackNameText(w, w->cur);
        break;
    }
}

/* OptionsMenu_DrawItems: the list items at their key frames (y from the key frames only while the list
 * opens / closes, else 0x18 + 0x18 i), the page of the selected item at the page origin, the sub BG0 / BG1
 * panels scrolled by -origin (OptionsMenu_SetSubScroll, x clamped to +-256: on the list only their left 16
 * columns show at x 240; sub BG1 = the sound-test panel when the sound test is selected, as the DISPCNT
 * of Options_ResetItemPageState), the OPTIONS banner (022cbe50) and the back button
 * (DrawListBackButton, mnu_titl anim 0) at theirs. Draw order = OAM order. */
static void Options_DrawPage(OptionsWork *w, s32 x, s32 y);
static void Options_Draw(OptionsWork *w) {
    s32 i, y, ox = w->apage.x, oy = w->apage.y, sx = ox < -0x100 ? -0x100 : ox > 0x100 ? 0x100 : ox;
    BOOL ky = w->phase == PH_OPEN || w->phase == PH_EXIT;
    /* OBJ priorities (Options_Unk_ov001_022ce6f8, DrawListBackButton, OpenPage0..2 / 4): the list's sprites 3,
     * behind the page panel (sub BG0, priority 1; here layer 2) that slides over them; the pages' 1 */
    w->sprio = 0x60000;
    Background_Show(1, 2, w->bg[w->page == PG_SOUND ? 3 : 2]);
    gDisplaySub[2].x = (u16)-sx;
    gDisplaySub[2].y = (u16)-oy;
    for (i = 0; i < w->nitems; i++) {
        s32 it = w->items[i], sel = i == w->sel, x = w->ait[i].x;
        y = ky ? w->ait[i].y : 0x18 + i * 0x18;
        switch (it) {
        case OPT_PLAYER_DATA:
            opt_spr(w, 1 + i, sel ? 2 : 1, x, y);
            break;
        /* the value first: the first sprite drawn is in front (OAM order) */
        case OPT_DIFFICULTY:
            opt_spr(w, 6, (w->settings[0] ? 15 : 13) + (sel ? 1 : 0), x - 0x44, y);
            opt_spr(w, 1 + i, sel ? 4 : 3, x, y);
            break;
        case OPT_TIME_LIMIT:
            opt_spr(w, 7, (w->settings[1] ? 17 : 19) + (sel ? 1 : 0), x - 0x44, y);
            opt_spr(w, 1 + i, sel ? 6 : 5, x, y);
            break;
        case OPT_SOUND_TEST:
            opt_spr(w, 1 + i, sel ? 8 : 7, x, y);
            break;
        case OPT_ERASE:
            opt_spr(w, 1 + i, sel ? 10 : 9, x, y);
            break;
        }
    }
    if (ox < 0x100) {
        w->sprio = w->page == PG_SOUND ? 0 : 0x20000;
        Options_DrawPage(w, ox, oy);
        w->sprio = 0x60000;
    }
    opt_spr(w, 0, 0, w->abanner.x, w->abanner.y);                          /* OPTIONS */
    opt_spr_r(w, 15, w->res_titl, 0, w->aback.x, w->aback.y, 0);         /* back button */
    w->sprio = 0;
}

/* OptionsMenu_DrawPage0..4 at the page origin (x, y): the question at (0x18, 0x18) in the panel's dark
 * header, the choices, the cursor frame of mnu_opti.bac on the hovered one, "A: OK / B: BACK" (anim 0x34)
 * in the footer at (0x18, 0x98); the DS's touch back button (DrawPageBackButton) is not drawn. */
static void Options_DrawPage(OptionsWork *w, s32 x, s32 y) {
    if (w->page == PG_PLAYER || w->page == PG_TA_KIND) {
        /* OptionsMenu_DrawPage0: PLAYER DATA, BATTLE PLAY RECORD (0x1c) / TIME ATTACK RECORD (0x1f), then
         * ACT (0x22) / BOSS (0x25) under it; the chosen entry flashes (anim + 2, looped) until the exit */
        BOOL ta = w->page == PG_TA_KIND, ch = w->chosen != -1 && w->chosen == w->choice;
        s32 a;
        opt_spr(w, 8, 0x15, x + 0x18, y + 0x18);
        /* the cursor frame 022ca1cc: mnu_opti anim 0 / 1 (4 frames after a move) on the hovered entry,
         * anim 2 on TIME ATTACK RECORD while its ACT / BOSS choice is open */
        if (ta)
            opt_spr_r(w, 20, w->res_opti, 2, x + 0x38, y + 0x58, 0);
        else if (!ch)
            opt_spr_r(w, 20, w->res_opti, w->moved < 4 ? 1 : 0, x + 0x38, y + (w->choice == 0 ? 0x40 : 0x58), 0);
        a = 0x1c + (!ta && w->choice == 0 ? (ch ? 2 : 1) : 0);
        opt_spr_loop(w, 9, a - 0x1c);
        opt_spr(w, 9, a, x + 0x38, y + 0x40);
        a = 0x1f + (!ta && w->choice == 1 ? 1 : 0);
        opt_spr(w, 10, a, x + 0x38, y + 0x58);
        if (ta) {
            if (!ch)
                opt_spr_r(w, 21, w->res_opti, 3 + (w->moved < 4 ? 1 : 0), x + (w->choice == 0 ? 0x38 : 0x80),
                          y + 0x70, 0);
            a = 0x22 + (w->choice == 0 ? (ch ? 2 : 1) : 0);
            opt_spr_loop(w, 11, a - 0x22);
            opt_spr(w, 11, a, x + 0x38, y + 0x70);
            a = 0x25 + (w->choice == 1 ? (ch ? 2 : 1) : 0);
            opt_spr_loop(w, 12, a - 0x25);
            opt_spr(w, 12, a, x + 0x80, y + 0x70);
        }
        opt_spr(w, 14, 0x34, x + 0x18, y + 0x98);
        return;
    }
    if (w->page == PG_SOUND) {
        /* OptionsMenu_DrawPage3 */
        s32 n = w->cur + 1, h = n / 100 % 10, t = n / 10 % 10, u = n % 10, k;
        static const s8 gx[4] = {1, 1, -1, -1}, gy[4] = {1, -1, -1, 1};
        static const u32 gf[4] = {0, SPR_VFLIP, SPR_HFLIP | SPR_VFLIP, SPR_HFLIP};
        opt_spr(w, 8, 0x18, x + 0x18, y + 0x18);
        /* the speakers (022c9f54 / 022c9d58: anim 7 / 8 and the glow anim 6 in four mirrored quarters;
         * their scale follows the level meter on the DS, drawn at 1 here) */
        for (k = 0; k < 2; k++) {
            s32 sx = x + (k ? 0xca : 0x36), i2;
            opt_spr_r(w, 22 + k, w->res_sou, 7 + k, sx, y + 0x5c, 0);
            for (i2 = 0; i2 < 4; i2++)
                opt_spr_r(w, 24 + k * 4 + i2, w->res_sou, 6, sx + gx[i2], y + 0x5c + gy[i2], gf[i2]);
        }
        opt_spr_r(w, 32, w->res_sou, 0x14, x + 0x4c, y + 0x3c, 0);              /* 022c9b78: the arrows */
        opt_spr_r(w, 33, w->res_sou, 0x14, x + 0xb4, y + 0x3c, SPR_HFLIP);
        OptionsMenu_DrawSoundTestTrackName(w, x + 0x80, y + 0x49);
        /* OptionsMenu_DrawSoundTestTrackNumber: anims 10 + digit, 9 = blank */
        opt_spr_r(w, 34, w->res_sou, h > 0 ? h + 10 : 9, x + 0x68, y + 0x4c, 0);
        opt_spr_r(w, 35, w->res_sou, (h > 0 || t > 0) ? t + 10 : 9, x + 0x78, y + 0x4c, 0);
        opt_spr_r(w, 36, w->res_sou, (h > 0 || t > 0 || u > 0) ? u + 10 : 9, x + 0x88, y + 0x4c, 0);
        opt_spr_r(w, 37, w->res_sou, 4, x, y, SPR_LOOP);
        opt_spr_r(w, 38, w->res_sou, 5, x, y, SPR_LOOP);
        opt_spr_r(w, 16, w->res_sou, w->playing ? 1 : 0, x, y, 0);     /* PLAY */
        opt_spr_r(w, 17, w->res_sou, w->playing ? 2 : 3, x, y, 0);     /* STOP */
        opt_spr_r(w, 39, w->res_sou_lang, 0, x, y, 0);
        opt_spr(w, 14, 0x34, x + 0x18, y + 0x98);
        return;
    }
    {
        /* OptionsMenu_DrawPage1 / 2 / 4: the question, two choices lit by the current setting (the
         * erase pages: by the answer), the cursor frame on the hovered one */
        s32 q = 0, l = 0x28, r = 0x2a, litl = 0, litr = 0, cm = 2;
        BOOL show = TRUE;
        switch (w->page) {
        case PG_DIFFICULTY: q = 0x16; l = 0x28; r = 0x2a; litl = w->diff_choice == 0; litr = w->diff_choice == 1; break;
        case PG_TIME_LIMIT: q = 0x17; l = 0x2c; r = 0x2e; litl = w->tl_choice != 0; litr = w->tl_choice == 0; break;
        case PG_ERASE1:
        case PG_ERASE2:
            q = w->page == PG_ERASE1 ? 0x19 : 0x1a;
            l = 0x30;
            r = 0x32;
            litl = w->chosen == 0;
            litr = w->chosen != 0;
            if (w->page == PG_ERASE1)
                cm = 3;
            if (w->chosen != -1 && w->page == PG_ERASE1 && w->chosen == 0 && w->unk18 >= 0x10)
                show = FALSE;
            break;
        }
        opt_spr(w, 8, q, x + 0x18, y + 0x18);
        if (show) {
            opt_spr_r(w, 20, w->res_opti, 5 + opt_cursor_mode(w, cm), x + (w->choice == 0 ? 0x20 : 0x88), y + 0x50, 0);
            opt_spr(w, 9, l + (litl ? 1 : 0), x + 0x20, y + 0x50);
            opt_spr(w, 10, r + (litr ? 1 : 0), x + 0x88, y + 0x50);
        }
        opt_spr(w, 14, 0x34, x + 0x18, y + 0x98);
    }
}

/* -------------------------------------------------------------- states */
static void Options_ExitToTitleMenu(void) { SaveData_CommitOrShowError(TitleMenu_StartFromOptions); }
static void Options_ExitToRecords(void) { SaveData_CommitOrShowError(Records_Start); }
static void Options_ExitToTimeAttackMenuMode2(void) { SaveData_CommitOrShowError(TimeAttackMenu_StartMode2); }
static void Options_ExitToTimeAttackMenuMode3(void) { SaveData_CommitOrShowError(TimeAttackMenu_StartMode3); }
static void (*s_opt_exit)(void) = Options_ExitToTitleMenu;

/* OptionsFadeTask_Main: the brightness of frame t of a fade from -> to in dur frames after delay */
static s16 opt_fade(s32 t, s32 from, s32 to, s32 dur, s32 delay) {
    s32 a = from << 12, b = to << 12, v;
    if (t < delay)
        return (s16)from;
    if (t - delay >= dur)
        return (s16)to;
    a += a >= 0 ? 0x800 : -0x800;
    b += b >= 0 ? 0x800 : -0x800;
    v = (a + (t - delay) * (b - a) / dur) >> 12;
    return (s16)(v < -16 ? -16 : v > 16 ? 16 : v);
}

static void Options_Finish(void) {
    void (*fn)(void) = s_opt_exit;
    s_opt_exit = Options_ExitToTitleMenu;
    Task_Destroy(gTaskCurrent);
    fn();
}

/* Options_Action_Exit (the list slides out, white after 0x1e frames, Options_State_Exit) /
 * OptionsMenu_Action_ExitOptions (black after 8 frames, SE 2 at 8, OptionsMenu_State_Unk_ov001_022ccbe0) */
static void opt_exit(OptionsWork *w, void (*fn)(void), BOOL from_page) {
    s_opt_exit = fn;
    w->ptimer = 0;
    if (from_page) {
        w->phase = PH_LEAVE;
    } else {
        opt_anims_open(w, 1);
        w->phase = PH_EXIT;
    }
}

/* Options_InitCursorMoveAnims + Options_ResetItemPageState */
static void opt_move(OptionsWork *w, s32 old) {
    opt_anim(w, &w->ait[w->sel], AS_SEL, 0);
    KeyframeAnim_Init(&w->ait[old], w->ama, AS_SEL);
    KeyframeAnim_PlayReverse(&w->ait[old]);
    opt_anim(w, &w->apage, AS_PG_MOVE, 0);
    opt_reset_page(w);
    w->unk14 = 0;
    SysSound_PlaySE(4);
}

/* Options_Action_EnterItem: the list slides out to the left, the page in */
static void opt_enter(OptionsWork *w) {
    opt_reset_page(w);
    opt_anims_page(w, AS_PG_ENTER, AS_ENTER_SEL, AS_ENTER_UNSEL, 0);
    SysSound_PlaySE(0);
    w->phase = PH_ENTER;
}

/* OptionsMenu_State_EnterItem, the slide done: OptionsMenu_Action_* */
static void opt_entered(OptionsWork *w) {
    w->phase = PH_PAGE;
    w->unk18 = 0;
    if (w->page == PG_SOUND) {   /* OptionsMenu_Action_SoundTest: key repeat interval 4 */
        u16 buf[12];
        int i;
        for (i = 0; i < 12; i++)
            buf[i] = 4;
        Pad_Reset(&gPad, NULL, buf);
        NNS_SndStopSoundAll();
    }
}

static void opt_back(OptionsWork *w) {   /* OptionsMenu_Action_Back: the page slides out, the list in */
    w->chosen = -1;
    w->unk18 = 0;
    w->unk1c = -1;
    opt_anims_page(w, AS_PG_BACK, AS_BACK_SEL, AS_BACK_UNSEL, 0);
    Pad_Reset(&gPad, NULL, NULL);
    w->phase = PH_BACK;
}

/* One frame: OptionsMenu_UpdateAnims, OptionsMenu_DrawItems, then the input of the state (the DS order) */
static void Options_State_Main(void) {
    OptionsWork *w = TASK_WORK(OptionsWork);
    BOOL done = opt_update_anims(w);
    Options_Draw(w);
    w->unk14++;
    w->unk18++;
    w->moved++;
    if (w->phase < PH_EXIT && BRIGHT_MAIN < 0) {   /* Options_CreateFadeTask(-16, 0, 0x10, 0) */
        BRIGHT_MAIN++;
        BRIGHT_SUB = BRIGHT_MAIN;
    }
    switch (w->phase) {
    case PH_OPEN:   /* Options_State_WaitOpenAnims */
    case PH_BACK:   /* OptionsMenu_State_WaitOpenAnims */
        if (done) {
            opt_anims_select(w);
            w->phase = PH_SELECT;
        }
        return;
    case PH_ENTER:
        if (done)
            opt_entered(w);
        return;
    case PH_EXIT:   /* Options_State_Exit + the fade task (0, 0x10, 0x10, 0x1e) */
        w->ptimer++;
        BRIGHT_MAIN = BRIGHT_SUB = opt_fade(w->ptimer - 1, 0, 16, 16, 0x1e);
        if (done && w->ptimer >= 0x2e)
            Options_Finish();
        return;
    case PH_LEAVE:   /* OptionsMenu_State_Unk_ov001_022ccbe0 + the fade task (0, -16, 8, 8) */
        w->ptimer++;
        BRIGHT_MAIN = BRIGHT_SUB = opt_fade(w->ptimer - 1, 0, -16, 8, 8);
        if (w->ptimer == 8)
            SysSound_PlaySE(2);   /* SysSound_PlaySEDelayed(2, 8, ..) */
        if (w->ptimer >= 30)
            Options_Finish();
        return;
    case PH_SELECT: {   /* Options_State_Select */
        s32 old = w->sel;
        if (gPad.rep & PAD_UP) {
            w->sel = old > 0 ? old - 1 : w->nitems - 1;
            if (w->sel != old)
                opt_move(w, old);
        } else if (gPad.rep & PAD_DOWN) {
            w->sel = old + 1 < w->nitems ? old + 1 : 0;
            if (w->sel != old)
                opt_move(w, old);
        }
        if (gPad.trg & PAD_A) {
            opt_enter(w);
        } else if (gPad.trg & PAD_B) {
            SysSound_PlaySE(1);
            opt_exit(w, Options_ExitToTitleMenu, FALSE);
        }
        return;
    }
    }
    /* PH_PAGE */
    switch (w->page) {
    case PG_DIFFICULTY:
    case PG_TIME_LIMIT:
    case PG_ERASE1:
    case PG_ERASE2:
        if (w->chosen != -1) {
            if (w->unk18 < 0x10)
                break;
            if (w->page == PG_ERASE1 && w->chosen == 0) {
                w->page = PG_ERASE2;
                w->choice = 1;
                w->chosen = -1;
                break;
            }
            if (w->page == PG_ERASE2 && w->chosen == 0)
                Options_EraseSaveData(w);
            opt_back(w);
            break;
        }
        if (gPad.rep & PAD_LEFT) {
            if (w->choice != 0) {
                w->choice = 0;
                w->moved = 0;
                SysSound_PlaySE(4);
            }
        } else if (gPad.rep & PAD_RIGHT) {
            if (w->choice != 1) {
                w->choice = 1;
                w->moved = 0;
                SysSound_PlaySE(4);
            }
        }
        if (gPad.trg & PAD_A) {
            w->chosen = w->choice;
            w->unk18 = 0;
            if (w->page == PG_DIFFICULTY)
                OptionsMenu_SetDifficultyChoice(w, w->choice);
            else if (w->page == PG_TIME_LIMIT)
                Options_SetTimeLimit(w, w->choice == 0 ? 1 : 0);
            Options_LoadSettingsFromSave(w);
            SysSound_PlaySE(0);
        } else if (gPad.trg & PAD_B) {
            SysSound_PlaySE(1);
            opt_back(w);
        }
        break;
    case PG_PLAYER:
    case PG_TA_KIND:
        /* OptionsMenu_State_Unk_ov001_022cd6e8: the chosen entry flashes for 30 frames, then
         * OptionsMenu_Action_ExitOptions (Options_ExitToRecords / Options_ExitToTimeAttackMenuMode2 / 3);
         * TIME ATTACK RECORD opens ACT / BOSS at once */
        if (w->chosen != -1) {
            if (w->page == PG_PLAYER && w->chosen == 1) {
                w->page = PG_TA_KIND;
                w->choice = 0;
                w->chosen = -1;
            } else if (w->unk18 >= 30) {
                opt_exit(w,
                         w->page == PG_PLAYER ? Options_ExitToRecords
                         : w->choice == 0     ? Options_ExitToTimeAttackMenuMode2
                                              : Options_ExitToTimeAttackMenuMode3,
                         TRUE);
                w->chosen = -1;
            }
            break;
        }
        if (gPad.rep & (w->page == PG_PLAYER ? PAD_UP : PAD_LEFT)) {
            if (w->choice != 0) {
                w->choice = 0;
                w->moved = 0;
                SysSound_PlaySE(4);
            }
        } else if (gPad.rep & (w->page == PG_PLAYER ? PAD_DOWN : PAD_RIGHT)) {
            if (w->choice != 1) {
                w->choice = 1;
                w->moved = 0;
                SysSound_PlaySE(4);
            }
        }
        if (gPad.trg & PAD_A) {
            w->chosen = w->choice;
            w->unk18 = 0;
            SysSound_PlaySE(0);
        } else if (gPad.trg & PAD_B) {
            SysSound_PlaySE(1);
            if (w->page == PG_TA_KIND) {
                w->page = PG_PLAYER;
                w->choice = 1;
            } else {
                opt_back(w);
            }
        }
        break;
    case PG_SOUND: {
        BOOL busy = (gSaveData.unk8 & 0x1000) != 0;   /* Options_IsExtraStoryCleared */
        /* track 100 (AUDIENCE 3) loops: NNS_SndPlayerStopSeqBySeqArcIdx(arc, id, 0x1e) 0x4b frames in */
        w->play_frames++;
        if (w->playing && w->played == 100 && w->play_frames == 0x4b && w->played < w->ntracks &&
            g_menu_hooks.sound_test_stop)
            g_menu_hooks.sound_test_stop(w->tracks[100].unk2c, w->tracks[100].id, 0x1e);
        if (w->ntracks <= 0) {
            if (gPad.trg & PAD_B)
                opt_back(w);
            break;
        }
        if (gPad.rep & PAD_LEFT) {
            do {
                w->cur--;
                if (w->cur < 0)
                    w->cur = w->ntracks - 1;
            } while (!busy && w->tracks[w->cur].unk2d == 1);
            SoundTest_BuildTrackNameText(w, w->cur);
        } else if (gPad.rep & PAD_RIGHT) {
            do {
                w->cur++;
                if (w->cur >= w->ntracks)
                    w->cur = 0;
            } while (!busy && w->tracks[w->cur].unk2d == 1);
            SoundTest_BuildTrackNameText(w, w->cur);
        }
        if (gPad.trg & PAD_A) {   /* SoundTest_LoadTrack / _PlayTrack */
            const SoundTestEntry *e = &w->tracks[w->cur];
            NNS_SndStopSoundAll();
            if (g_menu_hooks.sound_test)
                g_menu_hooks.sound_test(w->cur, e->unk29, e->type, e->unk2b, e->unk2c, e->id);
            w->playing = 1;
            w->played = w->cur;
            w->play_frames = 0;
        } else if (gPad.trg & PAD_B) {
            NNS_SndStopSoundAll();   /* SoundTest_ReleaseTrack */
            if (w->playing) {
                w->playing = 0;
            } else {
                opt_back(w);
                mn_bgm_forget();   /* SysSound_Init: the stopped menu BGM starts again */
                SysSound_PlayBGM(0);
                SysSound_PlaySE(1);
            }
        }
        break;
    }
    }
}

static void Options_Destructor(void) {
    OptionsWork *w = TASK_WORK(OptionsWork);
    int i;
    mn_display_reset();
    mn_res_free(w->res_lang);
    mn_res_free(w->res_font);
    mn_res_free(w->res_titl);
    mn_res_free(w->res_opti);
    mn_res_free(w->res_sou);
    mn_res_free(w->res_sou_lang);
    for (i = 0; i < 4; i++)
        mn_bg_free(w->bg[i]);
    free(w->sld);
    free(w->ama);
}

static const char *const s_opt_lang[6] = { "act/mnu_opti_jpn.bac", "act/mnu_opti_eng.bac", "act/mnu_opti_fra.bac",
                                           "act/mnu_opti_deu.bac", "act/mnu_opti_ita.bac", "act/mnu_opti_spa.bac" };

/* One page: the zigzag BG (main BG3) fills the frame at 2.5x; Tails (main
 * BG2) stands on the right at 2x; the options panel with its sprites (the
 * other screen, without its zigzag BG) is on the left at 1.5x. */
static void Options_Compose(void) {
    mn_comp_reset(1);
    mn_comp_add(0, 0, 0, 256, 192, 0, 0, 2.5f, 1 << 2, 1);
    mn_comp_add(0, 130, 4, 248, 188, 392, 56, 2.0f, 1 << 3, 1);
    mn_comp_add(1, 0, 0, 256, 192, 4, 96, 1.5f, 1 << 3, 0);
}

static OptionsWork *Options_Create(void);
void Options_StartFromTitleMenu(void) { Options_Create(); }

/* OptionsMenu_Create(t0, page, child): back from Records (0, 0) and from the record
 * lists of TimeAttackMenu modes 2 / 3 (1, 0) / (1, 1) */
static void Options_StartAtPlayerData(s32 page, s32 child) {
    OptionsWork *w = Options_Create();
    w->sel = 0;   /* PLAYER DATA */
    opt_reset_page(w);
    w->page = page == 0 ? PG_PLAYER : PG_TA_KIND;
    w->choice = child;
    opt_anims_page(w, AS_PG_ENTER, AS_ENTER_SEL, AS_ENTER_UNSEL, 1);   /* OptionsMenu_InitEnterItemAnims */
    w->phase = PH_PAGE;
}
void Options_StartFromRecords(void) { Options_StartAtPlayerData(0, 0); }
void Options_StartFromZoneMapPauseChoice2(void) { Options_StartAtPlayerData(1, 0); }
void Options_StartFromZoneMapPauseChoice3(void) { Options_StartAtPlayerData(1, 1); }

static OptionsWork *Options_Create(void) {
    OptionsWork *w = (OptionsWork *)Task_Create(Options_State_Main, Options_Destructor, 0, 0x1000, 0x1000,
                                                sizeof(OptionsWork))->work;
    char path[160];
    u32 size;
    int i;
    mn_display_reset();
    Options_Compose();
    g_mn_scene = "Options";
    /* gOptionsFileTable[lang] */
    w->res_lang = mn_res_load(s_opt_lang[mn_lang()]);
    w->res_font = mn_res_load("act/font_opti_sound.bac");
    {   /* gOptionsFileTable[lang] files 0, 1, 2, 4 */
        static const char *const lang[6] = {"jpn", "eng", "fra", "deu", "ita", "spa"};
        char p2[48];
        w->res_opti = mn_res_load("act/mnu_opti.bac");
        w->res_titl = mn_res_load("act/mnu_titl.bac");
        w->res_sou = mn_res_load("act/mnu_opti_sou.bac");
        snprintf(p2, sizeof(p2), "act/mnu_opti_sou_%s.bac", lang[mn_lang()]);
        w->res_sou_lang = mn_res_load(p2);
    }
    w->bg[0] = mn_bg_load("bg/mnu_opti_bg.bbg");
    w->bg[1] = mn_bg_load("bg/mnu_opti_image.bbg");
    w->bg[2] = mn_bg_load("bg/mnu_opti_bg02_b.bbg");
    w->bg[3] = mn_bg_load("bg/mnu_opti_bg02_b_sou.bbg");
    Background_Show(0, 3, w->bg[0]);
    Background_Show(0, 2, w->bg[1]);
    Background_Show(1, 3, w->bg[0]);
    Background_Show(1, 2, w->bg[2]);
    Background_SetSize(1, 2, 512, 256);   /* sub BG0 / BG1: 512 x 256 (BGxCNT 0x4088 / 0x428c) */
    snprintf(path, sizeof(path), "%s/sound_test.sld", g_mn_dir);
    if (mn_load_file(path, &w->sld, &size) == 0 && size >= 16 && memcmp(w->sld, "SLD", 3) == 0) {
        u32 off = (u32)w->sld[8] | (u32)w->sld[9] << 8 | (u32)w->sld[10] << 16 | (u32)w->sld[11] << 24;
        u32 n = (u32)w->sld[12] | (u32)w->sld[13] << 8;
        if (off + n * sizeof(SoundTestEntry) <= size) {
            w->tracks = (const SoundTestEntry *)(void *)(w->sld + off);
            w->ntracks = (s32)n;
        }
    }
    for (i = 0; i < OPT_NSPR; i++) {
        AnimatorSprite_Init(&w->spr[i], w->res_lang, 0, 0, 1);
        w->sres[i] = w->res_lang;
        w->anim[i] = -1;
    }
    for (i = 0; i < 41; i++) {
        AnimatorSprite_Init(&w->glyph[i], w->res_font, 0, 0, 1);
        w->ganim[i] = -1;
    }
    w->nitems = 0;
    w->items[w->nitems++] = OPT_PLAYER_DATA;
    w->items[w->nitems++] = OPT_DIFFICULTY;
    w->items[w->nitems++] = OPT_TIME_LIMIT;
    if (gSaveData.unk8 & 0x200)   /* Options_IsSoundTestUnlocked */
        w->items[w->nitems++] = OPT_SOUND_TEST;
    w->items[w->nitems++] = OPT_ERASE;
    Options_LoadSettingsFromSave(w);
    w->ama = mn_ama_load("ama/mnu_opti.ama");
    opt_reset_page(w);
    opt_anims_open(w, 0);   /* Options_InitOpenAnims */
    w->phase = PH_OPEN;
    BRIGHT_MAIN = BRIGHT_SUB = -16;   /* Options_CreateFadeTask(-16, 0, 0x10, 0) */
    gSaveDataBusy = 0;
    SysSound_PlayBGM(0);   /* SysSound_Init + SysSound_PlayBGM(0): nothing new while the menu BGM runs */
    return w;
}
