/*
 * Attract demos: DemoInput (recorded pad playback) and DemoPlay (the demo stage
 * controller), translated from src/game/Cutscenes/DemoScript/DemoInput_*.c and
 * DemoPlay_*.c. See demo.h for the frame order and MENUS.md section 10.
 *
 * DS service                          here
 * Task_Create(DemoInput_Main, prio 1) demo_input_frame, called first in the frame
 * Task_Create(DemoPlay_Main, 0x4000)  demo_play_frame, called after the world
 * gPad / Pad_Update                   s_pad (the same cur/trg/rel/rep rules as GameSystem/Pad_Update)
 * gSystemFlags & 0x40                 s_sysflag40: main()'s hardware Pad_Update is skipped
 * REG_KEYINPUT (DemoPlay skip test)   the real controller (InputState), DS key bits
 * BrightnessFade_Create / IsDone      ui_fade / ui_fade_done (UI module)
 * AnimatorSpriteDS "DEMO PLAY"        ui_demo_label (UI module)
 * StageExit_Create(SceneChange_*)     DEMO_EXIT_TITLE / DEMO_EXIT_SEGA_LOGO
 * File_Load(key file)                 /cd/demo/key/keyNN.bin (demodata.py)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo.h"
#include "../player/dcp_player.h"

/* UI module (ui.h / ui_demo.c) */
void ui_fade(s32 kind, s32 speed);
int ui_fade_done(void);
void ui_demo_label(int visible);

typedef struct {
    u16 id;
    u8 dur;
    u8 pad3;
} DemoRecord;   /* UnkSeq_02067ddc */

/* DemoInput work (UnkWork_02067ddc, 0x20 bytes) */
typedef struct {
    u32 flags;          /* 1 recording restart, 2 playing, 4 file loaded */
    s32 unk4;           /* frames since create */
    u32 unk8;           /* record index */
    u16 unkc;           /* frames into the record */
    u16 unke;           /* gDemoInputMode seen last frame */
    u16 unk10;          /* countdown (8), not read */
    s32 unk14;          /* the pad (gPad) */
    u8 *unk18;          /* records */
    u32 *unk1c;         /* the file */
} DemoInputWork;

/* DemoPlay work (UnkWork_02048ba4 / 02048898) */
typedef struct {
    u32 timer;
    u16 id;
    u16 flags;          /* 1 fading out, 2 skipped by a key, 4 fade in when the timer runs */
} DemoPlayWork;

/* gPad (MnPad layout of GameSystem/Pad_Update) */
typedef struct {
    u16 cur, prev, trg, rel, rep;
    u16 cnt[12], delay[12], interval[12];
} DemoPadState;

static char s_dir[96] = "/cd/demo";
static int s_lead = DEMO_LEAD_FRAMES;
static DemoInputWork s_in;
static DemoPlayWork s_play;
static DemoPadState s_pad;
static u16 gDemoInputMode;
static int s_input_task, s_play_task;   /* the two DS tasks exist */
static int s_sysflag40;                 /* gSystemFlags & 0x40 */
static int s_limit;
static u32 s_seed;
static int s_character;

void demo_set_path(const char *dir) {
    if (dir)
        snprintf(s_dir, sizeof(s_dir), "%s", dir);
}
void demo_set_lead_frames(int n) { s_lead = n < 0 ? 0 : n; }
int demo_lead_frames(void) { return s_lead; }
int demo_active(void) { return s_play_task; }
int demo_input_mode(void) { return gDemoInputMode; }
uint32_t demo_timer(void) { return s_play.timer; }

static int load(const char *path, u8 **out, u32 *size) {
    FILE *f = fopen(path, "rb");
    long n;
    *out = NULL;
    *size = 0;
    if (!f)
        return -1;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n <= 0 || !(*out = (u8 *)malloc((size_t)n))) {
        fclose(f);
        return -1;
    }
    if (fread(*out, 1, (size_t)n, f) != (size_t)n) {
        free(*out);
        *out = NULL;
        fclose(f);
        return -1;
    }
    fclose(f);
    *size = (u32)n;
    return 0;
}
static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 rd32(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }

/* ---------------------------------------------------------------- the pad */
/* Pad_Reset(&gPad, NULL, NULL) defaults: delay 0x14, interval 2 */
static void Pad_Reset(DemoPadState *p) {
    int i;
    memset(p, 0, sizeof(*p));
    for (i = 0; i < 12; i++) {
        p->delay[i] = 0x14;
        p->interval[i] = 2;
    }
}
/* GameSystem/Pad_Update */
static void Pad_Update(DemoPadState *p, u16 keys) {
    int i;
    p->prev = p->cur;
    p->cur = keys;
    p->trg = p->cur & (p->cur ^ p->prev);
    p->rel = p->prev & (p->cur ^ p->prev);
    p->rep = p->trg;
    for (i = 0; i < 12; i++) {
        u32 bit = 1u << i;
        if (!(p->cur & bit)) {
            p->cnt[i] = p->delay[i];
        } else if (p->cnt[i] != 0) {
            p->cnt[i]--;
        } else {
            p->rep |= bit;
            p->cnt[i] = p->interval[i];
        }
    }
}

uint16_t demo_input_to_keys(uint32_t b) {
    u16 o = 0;
    if (b & IN_JUMP)
        o |= 0x001;
    if (b & IN_TRICK)
        o |= 0x002;
    if (b & IN_START)
        o |= 0x008;
    if (b & IN_RIGHT)
        o |= 0x010;
    if (b & IN_LEFT)
        o |= 0x020;
    if (b & IN_UP)
        o |= 0x040;
    if (b & IN_DOWN)
        o |= 0x080;
    if (b & IN_BOOST)
        o |= 0x400;
    if (b & IN_ALT)
        o |= 0x800;
    return o;
}
uint32_t demo_keys_to_input(uint16_t k) {
    u32 o = 0;
    if (k & 0x001)
        o |= IN_JUMP;
    if (k & 0x002)
        o |= IN_TRICK;
    if (k & 0x008)
        o |= IN_START;
    if (k & 0x010)
        o |= IN_RIGHT;
    if (k & 0x020)
        o |= IN_LEFT;
    if (k & 0x040)
        o |= IN_UP;
    if (k & 0x080)
        o |= IN_DOWN;
    if (k & 0x400)
        o |= IN_BOOST;
    if (k & 0x800)
        o |= IN_ALT;
    return o;
}

/* ------------------------------------------------------------ DemoInput */
static void DemoInput_Destructor(void) {
    DemoInputWork *w = &s_in;
    if (w->unk1c != NULL)
        free(w->unk1c);   /* the DS frees unk1c via buf (+0x1c) */
    w->unk1c = NULL;
    w->unk18 = NULL;
    gDemoInputMode = 0;
    s_sysflag40 = 0;
    s_input_task = 0;
}

/* DemoInput_Create(gPad, path): the work is cleared, the file loaded, gRandSeed set */
static int DemoInput_Create(const char *path) {
    DemoInputWork *w = &s_in;
    u8 *buf;
    u32 size;
    memset(w, 0, sizeof(*w));
    s_input_task = 1;
    w->unk14 = 0;   /* a = gPad */
    if (path == NULL)
        return 0;
    w->flags |= 2;
    gDemoInputMode = 2;
    if (load(path, &buf, &size) != 0 || size < 8) {
        fprintf(stderr, "demo: missing %s\n", path);
        DemoInput_Destructor();
        return -1;
    }
    w->unk1c = (u32 *)buf;
    w->flags |= 4;
    s_seed = rd32(buf);
    g_dcp_world.rand_seed = s_seed;   /* gRandSeed = *w->unk1c */
    w->unk18 = buf + 4;
    w->unk10 = 8;
    return 0;
}

static const DemoRecord *rec_at(const DemoInputWork *w, u32 i, DemoRecord *tmp) {
    const u8 *p = w->unk18 + i * 4;
    tmp->id = rd16(p);
    tmp->dur = p[2];
    tmp->pad3 = p[3];
    return tmp;
}

/* DemoInput_Main (task priority 1: before every stage task) */
static void DemoInput_Main(void) {
    DemoInputWork *w = &s_in;
    const DemoRecord *e;
    DemoRecord tmp;
    if (w->unke != gDemoInputMode) {
        /* mode 3 (recording) ends with an empty loop over the records: nothing to do */
        w->unkc = 0;
        w->unk8 = 0;
        switch (gDemoInputMode) {
        default:
        case 0:
        case 1:
            s_sysflag40 = 0;
            w->flags &= ~3u;
            break;
        case 2:
        case 4:
            w->unk10 = 8;
            s_sysflag40 = 1;
            w->flags &= ~3u;
            w->flags |= 2;
            break;
        case 3:
            s_sysflag40 = 0;
            w->flags &= ~3u;
            w->flags |= 1;
            break;
        case 5:
        case 6:
            DemoInput_Destructor();   /* Task_Destroy(gTaskCurrent) */
            return;
        }
    }
    w->unke = gDemoInputMode;
    if (w->flags & 2) {
        if (w->unk10 != 0)
            w->unk10--;
        if (w->flags & 1) {
            w->flags &= ~1u;
            w->unk8 = 0;
            w->unk4 = 0;
        }
        e = rec_at(w, w->unk8, &tmp);
        Pad_Update(&s_pad, e->id);
        w->unkc++;
        if (e->dur <= w->unkc) {
            w->unk8++;
            w->unkc = 0;
        }
        if (e->dur == 0) {
            w->flags &= ~2u;
            gDemoInputMode = 1;
            w->unkc = 0;
        }
    }
    w->unk4++;
}

/* main(): `if (!(gSystemFlags & 0x40)) Pad_Update(&gPad, keys)`, then the task list */
static void frame_input(const InputState *real) {
    if (!s_sysflag40)
        Pad_Update(&s_pad, demo_input_to_keys(real ? real->held : 0));
    if (s_input_task)
        DemoInput_Main();
}

/* ------------------------------------------------------------ DemoPlay */
static u8 *s_tab;
static u32 s_tab_size;

/* DemoPlay_Start -> DemoPlay_Create(gGameState.unk1E) */
int demo_start(int id, DemoInfo *out) {
    char path[160];
    const u8 *e;
    DemoPlayWork *w = &s_play;
    if (s_play_task || s_input_task)
        demo_stop();
    if (!s_tab) {
        snprintf(path, sizeof(path), "%s/demotab.bin", s_dir);
        if (load(path, &s_tab, &s_tab_size) != 0 || s_tab_size < 8 || memcmp(s_tab, "RDMO", 4) != 0) {
            fprintf(stderr, "demo: %s missing or invalid\n", path);
            free(s_tab);
            s_tab = NULL;
            return -1;
        }
    }
    if (id < 0 || id >= rd16(s_tab + 6) || 8u + (u32)(id + 1) * 0x22 > s_tab_size)
        return -1;
    memset(w, 0, sizeof(*w));
    s_play_task = 1;
    w->id = (u16)id;
    e = s_tab + 8 + w->id * 0x22;
    /* Stage_ResetProgress: the UI's ui_stage_start runs it with the stage */
    g_dcp_world.zone = e[0];        /* ENT(data_02073bd0) */
    g_dcp_world.act = e[1];         /* ENT(data_02073bd1) */
    s_character = rd16(e + 2);      /* gGameState.unk0 = ENT(data_02073bd2) */
    g_dcp_world.flags |= 4;
    if (rd16(e + 4) != 0 || rd16(e + 6) != 0) {
        g_dcp_world.flags |= 8;
        g_dcp_world.check_x = rd16(e + 4);   /* data_022c4590.unk1c */
        g_dcp_world.check_y = rd16(e + 6);   /* data_022c4590.unk20 */
        w->flags |= 4;
    }
    /* StageStart_Create(3): main.c loads the stage */
    g_dcp_world.mode = 3;
    s_limit = rd16(e + 8);
    Pad_Reset(&s_pad);
    s_sysflag40 = 0;
    {
        char key[25];
        memcpy(key, e + 10, 24);
        key[24] = 0;
        /* "/dat/key/keyNN.bin" -> <dir>/key/keyNN.bin */
        snprintf(path, sizeof(path), "%s/key/%s", s_dir, strrchr(key, '/') ? strrchr(key, '/') + 1 : key);
    }
    if (DemoInput_Create(path) != 0) {
        demo_stop();
        return -1;
    }
    /* File_Load(data_02073b38[lang]) + AnimatorSpriteDS_Init x2: the UI's ui_demo_label */
    if (out) {
        u32 i;
        DemoRecord tmp;
        memset(out, 0, sizeof(*out));
        out->zone = e[0];
        out->act = e[1];
        out->character = s_character;
        out->start_x = rd16(e + 4);
        out->start_y = rd16(e + 6);
        out->limit = s_limit;
        out->seed = s_seed;
        for (i = 0; rec_at(&s_in, i, &tmp)->dur != 0; i++) {
        }
        out->records = (int)i;
    }
    return 0;
}

/* DemoPlay_Destructor (+ DemoInput_Destructor when its task still runs) */
void demo_stop(void) {
    if (s_input_task || s_in.unk1c)
        DemoInput_Destructor();
    if (s_play_task) {
        g_dcp_world.flags &= ~4u;
        g_dcp_world.flags &= ~8u;
    }
    s_play_task = 0;
    gDemoInputMode = 0;
    s_sysflag40 = 0;
    ui_demo_label(-1);
}

/* DemoPlay_Main (task priority 0x4000: after the stage, the player, the objects and
 * BrightnessFade). Returns DEMO_RUNNING or where the original goes next. */
static int DemoPlay_Main(const InputState *real) {
    DemoPlayWork *w = &s_play;
    if ((w->flags & 4) && g_dcp_world.frame != 0) {
        ui_fade(3, 0x40);   /* BrightnessFade_Create(3, 0x40): from white */
        w->flags &= ~4u;
    }
    if (w->flags & 1) {
        if (ui_fade_done()) {
            gDemoInputMode = 5;
            /* zone >= 9 || act >= 2: NaviMsg_SetRestoreSubDisplayFlag(1) (DS sub-display
             * restore after the boss panel; nothing to restore in the port) */
            ui_demo_label(0);
            /* StageExit_Create(SceneChange_ToTitleScreen / SceneChange_ToSegaLogo, 3) */
            return (w->flags & 2) ? DEMO_EXIT_TITLE : DEMO_EXIT_SEGA_LOGO;
        }
    } else {
        /* (KEYINPUT | button XY) & (A | B | START | X | Y), or a touch */
        if (demo_input_to_keys(real ? real->held : 0) & 0xc0b) {
            if (g_dcp_world.frame != 0) {
                ui_fade(5, 0x400);
                w->flags |= 1;
                w->flags |= 2;
            }
        }
        if (w->timer > (u32)s_limit) {
            ui_fade(5, 0x100);
            w->flags |= 1;
        }
    }
    /* AnimatorSpriteDS_Draw x2 while timer & 0x40 (blinking label, both screens) */
    ui_demo_label((w->timer & 0x40) != 0);
    w->timer++;
    return DEMO_RUNNING;
}

static void fill_pad(DemoPad *o) {
    memset(o, 0, sizeof(*o));
    o->held = s_pad.cur;
    o->pressed = s_pad.trg;
    o->released = s_pad.rel;
    o->rep = s_pad.rep;
    o->recorded = s_sysflag40;
    o->in.held = demo_keys_to_input(s_pad.cur);
    o->in.pressed = demo_keys_to_input(s_pad.trg);
    o->in.released = demo_keys_to_input(s_pad.rel);
}

void demo_input_frame(const InputState *real, DemoPad *out) {
    frame_input(real);
    if (out)
        fill_pad(out);
}

int demo_play_frame(const InputState *real) {
    if (!s_play_task)
        return DEMO_EXIT_TITLE;
    return DemoPlay_Main(real);
}

/* The frames the DS spends in StageStart_State_* (loading) after DemoPlay_Create: the
 * DemoInput and DemoPlay tasks run, the stage does not exist yet (no timer, no input
 * consumer). gRandSeed: DemoInput_Create set it before the stage was created. */
void demo_after_stage_load(void) {
    int i;
    g_dcp_world.rand_seed = s_seed;
    for (i = 0; i < s_lead; i++) {
        frame_input(NULL);
        DemoPlay_Main(NULL);
    }
}
