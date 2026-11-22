/*
 * Cutscene entry points (integrator API, cutscene.h): starts the translated
 * cutscene "scene", runs the active parts in the DS task priority order
 * (DemoScript 0x100, PlayerDemo 0x3efe, TransformDemo 0x4000, Ending /
 * StaffRoll 0x5000, TalkDemo 0xeffd..0xf00a) once per frame, and builds the
 * frame's draw list (2D quads + 3D instances).
 */
#include <stdio.h>
#include "cs_internal.h"

static CsKind s_kind;
static int s_running;
static int s_frame;

void cutscene_set_paths(const char *data_dir, const char *sprite_dir, const char *model_dir, const char *music_dir) {
    if (data_dir)
        snprintf(g_cs_dir, sizeof(g_cs_dir), "%s", data_dir);
    if (sprite_dir)
        snprintf(g_cs_sprite_dir, sizeof(g_cs_sprite_dir), "%s", sprite_dir);
    if (model_dir)
        snprintf(g_cs_model_dir, sizeof(g_cs_model_dir), "%s", model_dir);
    if (music_dir)
        snprintf(g_cs_music_dir, sizeof(g_cs_music_dir), "%s", music_dir);
}

void cutscene_set_story(const CsStory *s) {
    if (s)
        g_cs_story = *s;
}

const CsResult *cutscene_result(void) { return &g_cs_result; }

/* SceneChange_* reached by a cutscene. A change that continues inside the
 * cutscene module (Ending -> StaffRoll -> PostStaffRoll) starts the next part
 * here, as the DS scene manager does; the others end the cutscene. */
void cs_scene_change(CsNext next) {
    switch (next) {
    case CS_NEXT_STAFFROLL:
        cs_log("scene StaffRoll");
        StaffRoll_Create(g_cs_story.character == 0 ? 0 : 1);
        return;
    case CS_NEXT_EXTRA_STAFFROLL:
        cs_log("scene ExtraStaffRoll");
        StaffRoll_Create(2);
        return;
    case CS_NEXT_POST_STAFFROLL:
        cs_log("scene PostStaffRoll");
        PostStaffRoll_Start();
        return;
    default:
        g_cs_result.next = next;
        return;
    }
}

int cutscene_zonemap_talk_id(int character, int step) {
    if (character == 0) {
        /* ZoneMap_Unk_ov001_022e1aac (asm): Sonic story steps */
        switch (step) {
        case 0: return 0x15;
        case 6: return 0x16;
        case 9: return 0x17;
        case 12: return 0x18;
        case 15: return 0x19;
        case 19: return 0x1a;
        case 24: return 0x1b;
        case 28: return 0x1c;
        case 31: return 0x1d;
        }
        return 0;
    }
    /* ZoneMap_SelectTalkDemo: Blaze */
    switch (step) {
    case 1: return 0x28;
    case 4: return 0x29;
    case 7: return 0x2a;
    case 15: return 0x2b;
    case 0x13: return 0x2c;
    case 0x17: return 0x2d;
    case 0x1b: return 0x2e;
    case 0x1e: return 0x2f;
    }
    return 0;
}

static void kill_all(void) {
    DemoScript_Kill();
    TalkDemo_Kill();
    PlayerDemo_Kill();
    TransformDemo_Kill();
    Ending_Kill();
    cs3d_reset();
    cs_quads_reset();
    BrightnessFade_Destroy();
}

int cutscene_start(CsKind kind, int id, int character) {
    if (cs_data_load() != 0)
        return -1;
    kill_all();
    memset(&g_cs_result, 0, sizeof(g_cs_result));
    g_cs_result.progress = -1;
    g_cs_result.set_sonic_progress = -1;
    g_cs_result.set_blaze_progress = -1;
    g_cs_story.character = character;
    g_cs_bright = 0;
    g_cs_backdrop = 0;
    s_kind = kind;
    s_frame = 0;
    cs_log("start kind %d id %d character %d", kind, id, character);
    switch (kind) {
    case CS_TALK:
        gTalkDemoId = (s16)id;
        TalkDemo_Create();
        break;
    case CS_TALK_MAP:
        gTalkDemoId = (s16)id;
        TalkDemo_CreateForZoneMap();
        break;
    case CS_PLAYER:
        gPlayerDemoId = (s16)id;
        PlayerDemo_Start();
        break;
    case CS_TRANSFORM:
        gTransformDemoId = (s16)id;
        TransformDemo_Start();
        break;
    case CS_SCRIPT:
        if (id == 1)
            DemoScript_StartAfterBoss();
        else
            DemoScript_Create(id);
        break;
    case CS_ENDING:
        Ending_Create();
        break;
    case CS_EXTRA_ENDING:
        ExtraEnding_Create();
        break;
    case CS_STAFFROLL:
        StaffRoll_Create(id);
        break;
    case CS_POST_STAFFROLL:
        PostStaffRoll_Start();
        break;
    default:
        return -1;
    }
    s_running = 1;
    return 0;
}

int cutscene_active(void) {
    return DemoScript_IsActive() || TalkDemo_IsActive() || PlayerDemo_IsActive() || TransformDemo_IsActive() ||
           Ending_IsActive();
}

int cutscene_is_fullscreen(void) {
    return PlayerDemo_IsActive() || TransformDemo_IsActive() || Ending_IsActive() ||
           (DemoScript_IsActive() && !TalkDemo_IsActive());
}

static u16 pad_bits(u32 b) {
    u16 o = 0;
    if (b & IN_JUMP) o |= PAD_A;
    if (b & IN_TRICK) o |= PAD_B | PAD_R;   /* B also fast-forwards the dialogue (DS R) */
    if (b & IN_START) o |= PAD_START;
    if (b & IN_BOOST) o |= PAD_X;
    if (b & IN_ALT) o |= PAD_Y;
    if (b & IN_LEFT) o |= PAD_LEFT;
    if (b & IN_RIGHT) o |= PAD_RIGHT;
    if (b & IN_UP) o |= PAD_UP;
    if (b & IN_DOWN) o |= PAD_DOWN;
    return o;
}

int cutscene_run_frame(const InputState *in) {
    if (!s_running)
        return 1;
    gPad.held = in ? pad_bits(in->held) : 0;
    gPad.trg = in ? pad_bits(in->pressed) : 0;
    cs_quads_reset();
    cs3d_reset();
    /* task priority order */
    DemoScript_RunFrame();
    BrightnessFade_Main();
    PlayerDemo_RunFrame();
    TransformDemo_RunFrame();
    Ending_RunFrame();
    TalkDemo_RunFrame();
    cs_bgm_update();
    /* draw list of this frame, back to front */
    PlayerDemo_Draw();
    TransformDemo_Draw();
    Ending_Draw();
    TalkDemo_Draw();
    s_frame++;
    if (!cutscene_active()) {
        s_running = 0;
        cs_log("end kind %d frames %d next %d", s_kind, s_frame, g_cs_result.next);
        return 1;
    }
    return 0;
}

void cutscene_shutdown(void) {
    kill_all();
    cs_snd_shutdown();
    s_running = 0;
}
