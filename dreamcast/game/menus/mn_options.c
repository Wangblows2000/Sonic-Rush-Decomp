/*
 * Options (src/game/Menus/Options/): difficulty, time limit, sound test,
 * delete save data, exit with a save commit. The settings code is
 * translated as is (Options_LoadSettingsFromSave, OptionsMenu_SetDifficultyChoice,
 * Options_SetTimeLimit, Options_EraseSaveData, Options_ExitToTitleMenu,
 * OptionsMenu_State_TimeLimit / _SoundTest input rules, SoundTest_BuildTrackNameText,
 * OptionsMenu_DrawSoundTestTrackName). The page/animation framework of the
 * original (OptionsMenu_* key-frame pages, 6 pages, wireless download play)
 * is replaced by a direct layout of the same sprites (mnu_opti_<lang>,
 * font_opti_sound) on the bottom screen: rewritten, see MENUS.md.
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
    s32 text[0x29];
    MnSpr spr[16], glyph[41];
    s32 anim[16], ganim[41];
    MnRes *res_lang, *res_font, *res_titl;
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

static void opt_spr(OptionsWork *w, int slot, s32 anim, s32 x, s32 y) {
    MnSpr *s = &w->spr[slot];
    if (w->anim[slot] != anim) {
        AnimatorSprite_SetAnimation(s, (u16)anim);
        w->anim[slot] = anim;
    }
    s->x = (s16)x;
    s->y = (s16)y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}

static void Options_Draw(OptionsWork *w) {
    s32 i, y;
    opt_spr(w, 0, 0, 0, 8);           /* OPTIONS */
    opt_spr(w, 15, 52, 24, 0xb0);     /* A:OK / B:BACK */
    if (w->page == PG_LIST) {
        for (i = 0; i < w->nitems; i++) {
            s32 it = w->items[i], sel = i == w->sel;
            y = 0x30 + i * 0x1c;
            switch (it) {
            case OPT_PLAYER_DATA:
                opt_spr(w, 1 + i, sel ? 2 : 1, 0xe8, y);
                break;
            /* the value first: the first sprite drawn is in front (OAM order) */
            case OPT_DIFFICULTY:
                opt_spr(w, 6, (w->settings[0] ? 15 : 13) + (sel ? 1 : 0), 0xb6, y);
                opt_spr(w, 1 + i, sel ? 4 : 3, 0xe8, y);
                break;
            case OPT_TIME_LIMIT:
                opt_spr(w, 7, (w->settings[1] ? 17 : 19) + (sel ? 1 : 0), 0xb6, y);
                opt_spr(w, 1 + i, sel ? 6 : 5, 0xe8, y);
                break;
            case OPT_SOUND_TEST:
                opt_spr(w, 1 + i, sel ? 8 : 7, 0xe8, y);
                break;
            case OPT_ERASE:
                opt_spr(w, 1 + i, sel ? 10 : 9, 0xe8, y);
                break;
            }
        }
        return;
    }
    Background_Show(1, 2, w->bg[w->page == PG_SOUND ? 3 : 2]);
    if (w->page == PG_PLAYER) {
        /* PLAYER DATA: BATTLE PLAY RECORD (28/29) / TIME ATTACK RECORD (31/32) */
        opt_spr(w, 8, 21, 24, 0x30);
        opt_spr(w, 9, 28 + (w->choice == 0 ? 1 : 0), 0x38, 0x50);
        opt_spr(w, 10, 31 + (w->choice == 1 ? 1 : 0), 0x38, 0x70);
        return;
    }
    if (w->page == PG_TA_KIND) {
        /* TIME ATTACK RECORD: ACT (34/35) / BOSS (37/38) */
        opt_spr(w, 8, 31, 0x38, 0x30);
        opt_spr(w, 9, 34 + (w->choice == 0 ? 1 : 0), 0x28, 0x58);
        opt_spr(w, 10, 37 + (w->choice == 1 ? 1 : 0), 0x88, 0x58);
        return;
    }
    if (w->page == PG_SOUND) {
        opt_spr(w, 8, 24, 24, 0x30);
        OptionsMenu_DrawSoundTestTrackName(w, 0x80, 0x70);
        return;
    }
    {
        s32 q = 0, l = 40, r = 42;
        switch (w->page) {
        case PG_DIFFICULTY: q = 22; l = 40; r = 42; break;
        case PG_TIME_LIMIT: q = 23; l = 44; r = 46; break;
        case PG_ERASE1: q = 25; l = 48; r = 50; break;
        case PG_ERASE2: q = 26; l = 48; r = 50; break;
        }
        opt_spr(w, 8, q, 24, 0x30);
        opt_spr(w, 9, l + (w->choice == 0 ? 1 : 0), 0x1f, 0x4e);
        opt_spr(w, 10, r + (w->choice == 1 ? 1 : 0), 0x87, 0x4e);
    }
}

/* -------------------------------------------------------------- states */
static void Options_ExitToTitleMenu(void) { SaveData_CommitOrShowError(TitleMenu_StartFromOptions); }
static void Options_ExitToRecords(void) { SaveData_CommitOrShowError(Records_Start); }
static void Options_ExitToTimeAttackMenuMode2(void) { SaveData_CommitOrShowError(TimeAttackMenu_StartMode2); }
static void Options_ExitToTimeAttackMenuMode3(void) { SaveData_CommitOrShowError(TimeAttackMenu_StartMode3); }
static void (*s_opt_exit)(void) = Options_ExitToTitleMenu;

typedef struct { s32 t; } OptFadeWork;

static void Options_State_Exit(void) {
    OptionsWork *w = TASK_WORK(OptionsWork);
    w->unk14++;
    BRIGHT_MAIN = BRIGHT_SUB = (s16)(-w->unk14);
    Options_Draw(w);
    if (w->unk14 >= 16) {
        void (*fn)(void) = s_opt_exit;
        s_opt_exit = Options_ExitToTitleMenu;
        Task_Destroy(gTaskCurrent);
        fn();
    }
}

static void opt_back(OptionsWork *w) {   /* OptionsMenu_Action_Back */
    w->page = PG_LIST;
    w->chosen = -1;
    w->unk18 = 0;
    w->unk1c = -1;
}

static void Options_State_Main(void) {
    OptionsWork *w = TASK_WORK(OptionsWork);
    w->unk14++;
    w->unk18++;
    if (BRIGHT_MAIN < 0) {
        BRIGHT_MAIN++;
        BRIGHT_SUB = BRIGHT_MAIN;
    }
    switch (w->page) {
    case PG_LIST:
        if (gPad.rep & PAD_UP) {
            w->sel = w->sel > 0 ? w->sel - 1 : w->nitems - 1;
            SysSound_PlaySE(4);
        } else if (gPad.rep & PAD_DOWN) {
            w->sel = w->sel + 1 < w->nitems ? w->sel + 1 : 0;
            SysSound_PlaySE(4);
        }
        if (gPad.trg & PAD_A) {
            SysSound_PlaySE(0);
            w->chosen = -1;
            w->unk18 = 0;
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
                /* OptionsMenu_Action_SoundTest: key repeat interval 4 */
                {
                    u16 buf[12];
                    int i;
                    for (i = 0; i < 12; i++)
                        buf[i] = 4;
                    Pad_Reset(&gPad, NULL, buf);
                }
                NNS_SndStopSoundAll();
                w->page = PG_SOUND;
                w->playing = 0;
                SoundTest_BuildTrackNameText(w, w->cur);
                break;
            }
        } else if (gPad.trg & PAD_B) {
            SysSound_PlaySE(1);
            w->unk14 = 0;
            gTaskCurrent->func = Options_State_Exit;
        }
        break;
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
                SysSound_PlaySE(4);
            }
        } else if (gPad.rep & PAD_RIGHT) {
            if (w->choice != 1) {
                w->choice = 1;
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
        if (gPad.rep & (w->page == PG_PLAYER ? PAD_UP : PAD_LEFT)) {
            if (w->choice != 0) {
                w->choice = 0;
                SysSound_PlaySE(4);
            }
        } else if (gPad.rep & (w->page == PG_PLAYER ? PAD_DOWN : PAD_RIGHT)) {
            if (w->choice != 1) {
                w->choice = 1;
                SysSound_PlaySE(4);
            }
        }
        if (gPad.trg & PAD_A) {
            SysSound_PlaySE(0);
            if (w->page == PG_PLAYER && w->choice == 1) {
                w->page = PG_TA_KIND;
                w->choice = 0;
                break;
            }
            /* Options_ExitToRecords / Options_ExitToTimeAttackMenuMode2 / 3 */
            s_opt_exit = w->page == PG_PLAYER ? Options_ExitToRecords
                         : w->choice == 0     ? Options_ExitToTimeAttackMenuMode2
                                              : Options_ExitToTimeAttackMenuMode3;
            w->unk14 = 0;
            gTaskCurrent->func = Options_State_Exit;
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
        } else if (gPad.trg & PAD_B) {
            NNS_SndStopSoundAll();   /* SoundTest_ReleaseTrack */
            if (w->playing) {
                w->playing = 0;
            } else {
                opt_back(w);
                Pad_Reset(&gPad, NULL, NULL);
                SysSound_PlayBGM(0);
                SysSound_PlaySE(1);
            }
        }
        break;
    }
    }
    Options_Draw(w);
}

static void Options_Destructor(void) {
    OptionsWork *w = TASK_WORK(OptionsWork);
    int i;
    mn_display_reset();
    mn_res_free(w->res_lang);
    mn_res_free(w->res_font);
    mn_res_free(w->res_titl);
    for (i = 0; i < 4; i++)
        mn_bg_free(w->bg[i]);
    free(w->sld);
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
    w->page = page == 0 ? PG_PLAYER : PG_TA_KIND;
    w->choice = child;
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
    w->bg[0] = mn_bg_load("bg/mnu_opti_bg.bbg");
    w->bg[1] = mn_bg_load("bg/mnu_opti_image.bbg");
    w->bg[2] = mn_bg_load("bg/mnu_opti_bg02_b.bbg");
    w->bg[3] = mn_bg_load("bg/mnu_opti_bg02_b_sou.bbg");
    Background_Show(0, 3, w->bg[0]);
    Background_Show(0, 2, w->bg[1]);
    Background_Show(1, 3, w->bg[0]);
    Background_Show(1, 2, w->bg[2]);
    snprintf(path, sizeof(path), "%s/sound_test.sld", g_mn_dir);
    if (mn_load_file(path, &w->sld, &size) == 0 && size >= 16 && memcmp(w->sld, "SLD", 3) == 0) {
        u32 off = (u32)w->sld[8] | (u32)w->sld[9] << 8 | (u32)w->sld[10] << 16 | (u32)w->sld[11] << 24;
        u32 n = (u32)w->sld[12] | (u32)w->sld[13] << 8;
        if (off + n * sizeof(SoundTestEntry) <= size) {
            w->tracks = (const SoundTestEntry *)(void *)(w->sld + off);
            w->ntracks = (s32)n;
        }
    }
    for (i = 0; i < 16; i++) {
        AnimatorSprite_Init(&w->spr[i], w->res_lang, 0, 0, 1);
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
    w->chosen = -1;
    w->unk1c = -1;
    Options_LoadSettingsFromSave(w);
    BRIGHT_MAIN = BRIGHT_SUB = -16;   /* Options_CreateFadeTask(-16, 0, 0x10, 0) */
    gSaveDataBusy = 0;
    return w;
}
