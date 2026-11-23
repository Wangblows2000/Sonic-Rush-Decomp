/* Native front end (Dreamcast port): SegaLogo, TitleScreen, TitleMenu,
 * PlayerSelect, ZoneMap (+ act select, map pause), Options, story progression.
 * Translated from src/game/Menus/ and src/game/GameSystem/StoryProgress_ (files).
 * See docs/dreamcast/translation/MENUS.md. */
#ifndef SRDC_MENUS_H
#define SRDC_MENUS_H
#include <stdint.h>
#include "../engine/input.h"

/* What the front end asks main.c to do (the SceneChange_* calls that leave
 * the menus). */
typedef enum {
    MENU_REQ_NONE = 0,     /* stay in the menus */
    MENU_REQ_STAGE,        /* start zone/act with character (story / time attack) */
    MENU_REQ_DEMO,         /* attract demo: SceneChange_ToDemoPlay, demo index in .demo */
    MENU_REQ_UNAVAILABLE,  /* a scene the port does not have (ending...): .scene names it */
    MENU_REQ_VS_STAGE      /* Battle Play race (two local controllers): zone/act, character = P1, character2 = P2 */
} MenuRequestKind;

/* gGameState.unk10 */
enum { MENU_MODE_STORY = 0, MENU_MODE_VS = 1, MENU_MODE_TIME_ATTACK = 2 };

typedef struct {
    MenuRequestKind kind;
    int zone;        /* gGameState.unk1C (0..9; DS zone numbering, 7 = extra/8 final ...) */
    int act;         /* gGameState.unk1D (0, 1, 2 = boss act / demo script) */
    int character;   /* gGameState.unk0: 0 Sonic, 1 Blaze */
    int mode;        /* gGameState.unk10 */
    int demo;        /* gGameState.unk1E (attract demo 0..3) */
    int lives;       /* data_022c4590.unk14 (StoryProgress_RestoreFromSave) */
    int emeralds;    /* gGameState.unk28 */
    int easy;        /* gGameState.unk14 (difficulty: 1 = normal ... see MENUS.md) */
    const char *scene;
    int character2;  /* Battle Play: player 2's character (0 Sonic, 1 Blaze; never == character) */
} MenuRequest;

/* Sound hooks: original SDAT ids (SysSound_* / Sound_* call sites). NULL = silent. */
typedef struct {
    void (*bgm)(int seq);                   /* Sound_PlaySeq / SysSound_PlayBGM */
    void (*se)(int arc, int id);            /* Sound_PlaySeqArc / SysSound_PlaySE (arc -1) */
    void (*voice)(int id);                  /* Sound_PlayVoice / SysSound_PlayMenuVoice */
    void (*voice_alt)(int id);              /* SysSound_PlayMenuVoiceAlt (second voice handle) */
    void (*voice_arc)(int arc, int id);     /* Sound_PlayVoice(NULL, arc, id) (title voice: SSAR 0 seq 5) */
    void (*stop_all)(void);                 /* NNS_SndStopSoundAll */
    /* sound test: entry index of sound_test.sld and its fields (archive,
     * type 0 BGM/1 SE/2 voice, group, sequence archive or bank, id) */
    void (*sound_test)(int index, int archive, int type, int group, int arc, int id);
    /* ZoneMap_State_PlayTalkDemo (cutscene module, CUTSCENES.md): TalkDemo_CreateForZoneMap
     * for the story step (gGameState.unk0, map step unk4 / unk8; the talk id is
     * cutscene_zonemap_talk_id(character, step)); 0 = started. talk_demo_active =
     * TalkDemo_IsActive. NULL = the scene is skipped (32-frame fade only). */
    int (*talk_demo_start)(int character, int step);
    int (*talk_demo_active)(void);
    /* SysSound_StopBGM(frames) (appended): NNS_SndPlayerStopSeq(&gBGMSoundHandle, frames);
     * PlayerSelectMenu_State_Confirmed 0x10 at the fade-out. NULL = the music runs on. */
    void (*bgm_stop)(int frames);
    /* NNS_SndPlayerStopSeqBySeqArcIdx(arc, id, fade) (appended): OptionsMenu_State_SoundTest
     * ends the looping track 100 0x4b frames after it started. */
    void (*sound_test_stop)(int arc, int id, int fade);
} MenuHooks;
extern MenuHooks g_menu_hooks;

void menus_set_paths(const char *menus_dir, const char *sprite_dir);
/* loads menudata.bin, installs the save signature and VMU icon, reads the save
 * (save_load) and starts SegaLogo. Returns 0 when menudata.bin was found. */
int menus_init(void);
/* one frame: input, tasks, then draw commands for menus_draw_tr. */
MenuRequest menus_run_frame(const InputState *in);
/* TR list: the scene as one 640x480 composition (MENUS.md section 3) + brightness. */
void menus_draw_tr(void);
/* the menus' 3D layer (DS BG0: the zone map's character model) into one PVR list (OP, PT or TR),
 * called inside each list after the list's other draws; nothing when the scene has no model */
void menus_draw_list(int list);
/* Back from a stage. The front end does what the original does at that exit
 * (ClearDemo_GoToNextScene, PauseMenu_QuitToMap, Stage_ExitAfterFail, demo
 * end) and returns the next request: MENU_REQ_STAGE when the original goes
 * straight to another stage (next act of a new stage, boss act), else
 * MENU_REQ_NONE and the menus run (zone map, act select, title...). */
typedef enum {
    MENU_EXIT_CLEARED = 0,  /* act results done (story: progress + best score saved; time attack: record) */
    MENU_EXIT_QUIT,         /* pause menu "go back" (PauseMenu_QuitToMap) */
    MENU_EXIT_GAME_OVER,    /* game over / time over (SceneChange_ToSegaLogo) */
    MENU_EXIT_DEMO_END,     /* attract demo skipped with a button (DemoPlay_Main: SceneChange_ToTitleScreen) */
    MENU_EXIT_DEMO_TIMEOUT, /* attract demo ran to its frame limit (SceneChange_ToSegaLogo) */
    MENU_EXIT_TRY_AGAIN,    /* time attack left without a clear (pause quit, Stage_ExitAfterFail):
                               SceneChange_ToTryAgain, no time record */
    MENU_EXIT_EXTRA_QUIT    /* Extra boss pause GO BACK (PauseMenu_Main_Quit zone 8 act 2):
                               SceneChange_ToPlayerSelectMenuMode0 */
} MenuExit;
/* score = data_022c4590.unk4, lives = data_022c4590.unk14, emeralds = gGameState.unk28,
 * time = gGameState.unk20 (frames, time attack records) */
MenuRequest menus_stage_exit(MenuExit how, uint32_t score, int lives, int emeralds, uint32_t time);

/* ------------------------------------------------------------ Battle Play (menus/mn_vs.c)
 * Local two-controller VS (BATTLE PLAY page of the title menu; the DS wireless lobby, VsConnect
 * and download play have no DC counterpart). Player 2's pad for the menu frame (player select:
 * each player picks with their own controller); call before menus_run_frame. */
void menus_set_pad2(const InputState *in2);
/* back from a race: ResultsScreen (win / lose / draw, rematch or quit). winner 0 = P1, 1 = P2,
 * 2 = draw (time over); times in frames. Returns MENU_REQ_VS_STAGE for a rematch, else
 * MENU_REQ_NONE (the menus run). */
typedef struct {
    int winner;
    uint32_t time[2];
    int rings[2];
    int finished[2];
} VsRaceResult;
MenuRequest menus_vs_exit(const VsRaceResult *res);
/* (mn_results.c: the ResultsScreen is interactive, so menus_vs_exit itself returns MENU_REQ_NONE
 * and the menus run it; the rematch's MENU_REQ_VS_STAGE (same zone / act / characters) comes from
 * menus_run_frame when player 1 picks TRY AGAIN. VsRaceResult.winner decides WIN / LOSE / DRAW; the
 * original screen shows no times or rings.) */
/* player 2's name on the VS screens and in the VS record (UTF-16, up to 10 characters; default
 * empty). Player 1's is menus_set_owner_name. */
void menus_set_player2_name(const uint16_t *utf16, int len);
/* a race started without the VS select screens (debug start): sets what the character select and
 * the stage select would have (characters, zone / act, VS mode, the records) so menus_vs_exit's
 * results, TRY AGAIN and CHANGE work; character2 is made the other character if equal */
void menus_vs_prepare(int zone, int act, int character, int character2);
void menus_shutdown(void);
/* The staff roll ended with CS_NEXT_UNLOCK_NOTICES (PostStaffRoll): show the unlock
 * notices the save calls for (player-select page, sound test), then the SEGA logo.
 * Call instead of resuming the scene the menus had before the ending. */
void menus_post_staff_roll(void);
/* main.c ran a full-screen cutscene over the front end (DemoScript 4 before the zone
 * map) or stopped the stream: start the BGM of the current menu scene again. */
void menus_restart_bgm(void);
int menus_language(void);          /* gGameLanguage, default 1 (English) */
/* the owner's nickname shown by Records (the DS firmware's OS owner info; the DC has
 * none): UTF-16, up to 10 characters, default empty */
void menus_set_owner_name(const uint16_t *utf16, int len);
void menus_set_language(int lang);
#endif
