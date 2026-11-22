/* Native cutscenes (Dreamcast port of Sonic Rush): TalkDemo, PlayerDemo (+
 * PolyDemoUnder2), TransformDemo, DemoScript, Ending, ExtraEnding, StaffRoll,
 * PostStaffRoll. Translated from src/game/Cutscenes/. One 640x480 view: the
 * DS two-screen compositions are rebuilt as one scene (CUTSCENES.md section 3).
 * See docs/dreamcast/translation/CUTSCENES.md. */
#ifndef SRDC_CUTSCENE_H
#define SRDC_CUTSCENE_H
#include <stdint.h>
#include "../engine/input.h"

typedef enum {
    CS_NONE = 0,
    CS_TALK,          /* TalkDemo_Create:            id = gTalkDemoId (1..57)              */
    CS_TALK_MAP,      /* TalkDemo_CreateForZoneMap:  id = gTalkDemoId, over the zone map   */
    CS_PLAYER,        /* PlayerDemo_Start:           id = gPlayerDemoId (1..43)            */
    CS_TRANSFORM,     /* TransformDemo_Start:        id = gTransformDemoId (0..4)          */
    CS_SCRIPT,        /* DemoScript_Create(mode):    id = mode 0..4 (StartScript0, AfterBoss=1/2, 3, 4) */
    CS_ENDING,        /* Ending_Create:              character 0 Sonic / 1 Blaze           */
    CS_EXTRA_ENDING,  /* ExtraEnding_Create (PlayerDemo 0x2a, then the extra staff roll)   */
    CS_STAFFROLL,     /* StaffRoll_Create(mode):     id = 0 Sonic, 1 Blaze, 2 extra        */
    CS_POST_STAFFROLL /* PostStaffRoll_Start: the final TalkDemo 0x37 (Sonic, extra not cleared) */
} CsKind;

/* What the original does when the cutscene ends (the SceneChange_* call). */
typedef enum {
    CS_NEXT_NONE = 0,     /* back to the caller (talk over the map / boss, PlayerDemo inside a script) */
    CS_NEXT_STAGE,        /* DemoScript fn SceneChange_ToStage: start the act (boss act after script 0) */
    CS_NEXT_CLEARDEMO,    /* ClearDemo_Create: the act results, then ClearDemo_GoToNextScene */
    CS_NEXT_ENDING,       /* SceneChange_ToEnding                                       */
    CS_NEXT_EXTRA_BOSS,   /* SceneChange_ToExtraBoss (zone 8 act 2 after script 3)       */
    CS_NEXT_ZONEMAP,      /* SceneChange_ToZoneMap_1_0 (after script 4, progress saved)  */
    CS_NEXT_EXTRA_ACT,    /* SceneChange_ToExtraAct (after script 2)                     */
    CS_NEXT_STAFFROLL,    /* Ending -> SceneChange_ToStaffRoll                           */
    CS_NEXT_EXTRA_STAFFROLL, /* ExtraEnding -> SceneChange_ToExtraStaffRoll              */
    CS_NEXT_POST_STAFFROLL,  /* StaffRoll_State_Finish -> SceneChange_ToPostStaffRoll    */
    CS_NEXT_UNLOCK_NOTICES   /* PostStaffRoll: PostStaffRoll_ShowUnlockNotices (menus)   */
} CsNext;

/* Story state the cutscenes read (gGameState / gSaveData fields). */
typedef struct {
    int character;      /* gGameState.unk0: 0 Sonic, 1 Blaze */
    int zone;           /* gGameState.unk1C (DS zone id, 7 = extra/Dead Line, 8 = final) */
    int stage_new;      /* StoryProgress_IsStageNew() */
    uint32_t save_flags;/* gSaveData.unk8 (0x1000 extra cleared, 0x20000.. talk seen flags) */
    int sonic_progress; /* gSaveData.unkC  */
    int blaze_progress; /* gSaveData.unk64 */
    int language;       /* gGameLanguage 0..5, default 1 (English) */
    int sonic_emeralds; /* gSaveData.unkF: Chaos Emerald bits (0x7f = all 7), PostStaffRoll_Begin */
} CsStory;

typedef struct {
    CsNext next;
    int progress;       /* DemoScript flags & 1: SaveData_Set{Sonic,Blaze}Progress(progress) + StoreLives, else -1 */
    uint32_t set_save_flags;   /* TalkDemo_MarkSeen / StaffRoll_State_Finish: bits to OR into gSaveData.unk8 */
    int set_sonic_progress;    /* StaffRoll_State_Finish (0x1a) or -1 */
    int set_blaze_progress;    /* StaffRoll_State_Finish (0x1c) or -1 */
    int game_cleared;          /* StaffRoll mode 2: SaveData_SetGameCleared */
    int skipped;
} CsResult;

/* Paths (defaults "/cd/cutscene", "/cd/sprites", "/cd/models", "/cd/music"). */
void cutscene_set_paths(const char *data_dir, const char *sprite_dir, const char *model_dir, const char *music_dir);
/* Sound bank (sound_load_bank name) to reload when a cutscene that loaded its own
 * sys bank ends (e.g. the stage SDAT for a boss talk); NULL = leave as is. */
void cutscene_set_return_bank(const char *sdat);
void cutscene_set_story(const CsStory *s);

/* Starts a cutscene; returns 0, or -1 if its data is missing (the caller then
 * continues as if it had ended at once). */
int cutscene_start(CsKind kind, int id, int character);
/* One 60 Hz frame. Returns 0 while running, 1 when done. */
int cutscene_run_frame(const InputState *in);
/* Full-screen cutscenes (PlayerDemo, TransformDemo, Ending, StaffRoll, scripts
 * between their parts): opens and closes the PVR lists OP, PT, TR itself.
 * Call between render_begin_frame() and render_end_frame(). */
void cutscene_draw(void);
/* Overlay cutscenes (TalkDemo over the zone map or a stage): draw into the list
 * the caller has open (PVR_LIST_OP_POLY / PT / TR); 3D goes to OP/PT/TR, 2D to TR
 * at z 100..120 so it lands above the scene. */
void cutscene_draw_list(int pvr_list);
/* 1 when the cutscene covers the whole screen (draw with cutscene_draw), 0 for
 * overlays (TalkDemo): the caller keeps drawing its scene under it. */
int cutscene_is_fullscreen(void);
int cutscene_active(void);
const CsResult *cutscene_result(void);
void cutscene_shutdown(void);

/* original ids of the story (for the integrator's tables) */
int cutscene_zonemap_talk_id(int character, int story_step); /* ZoneMap_Unk_ov001_022e1aac / ZoneMap_SelectTalkDemo */

/* host tests: one line per executed script command / page / sound cue */
int cutscene_log_count(void);
const char *cutscene_log_line(int i);
void cutscene_log_clear(void);
#endif
