/*
 * Music selection and in-stage music events (see bgm.h). Original functions
 * keep their names and logic; NNS_Snd* calls on the BGM handles are mapped to
 * the stream engine (engine/audio.c):
 *   NNS_SndArcPlayerStartSeq(gStageBGMHandleB / A, seq)   audio_music_play_layers (both layers, one file)
 *   NNS_SndPlayerMoveVolume(gStageBGMHandleB / A, v, n)   audio_music_layer_volume(layer, v, n)
 *   NNS_SndPlayerSetChannelPriority(stage handles)        no effect (no channel competition with streams)
 *   Sound_PlaySeq / Sound_PlayBGMEx / _OnPlayer3          disc stream or a RAM jingle slot
 *   NNS_SndPlayerStopSeq(handle, fade)                    audio_music_fade_stop / audio_jingle_stop
 *   NNS_SndPlayerPause(StageJingle handle, f)             audio_jingle_pause
 */
#include "bgm.h"
#include "sound.h"
#include "snd_nns.h"
#include "../engine/audio.h"
#ifdef _arch_dreamcast
#include "../engine/pak.h"   /* pak_prefetch */
#endif
#include "../player/dcp_player.h"
#include "../objects/objects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int g_sound_trace;
extern uint32_t g_sound_frame;
void sound_player_sfx(DcPlayer *p, int ch, int id);   /* sound_hooks.c */
const char *sound_bank_name(void);                    /* sound.c */

/* ------------------------------------------------------------ stream table */
typedef struct {
    char sdat[10];
    uint8_t seq, layer, player, ram;
    int16_t file;                /* index into s_files, -1 silent */
    uint32_t ls, le;
} BgmEntry;
static BgmEntry *s_tab;
static int s_n, s_loaded;
static char (*s_files)[40];
static int s_nfiles;
static uint8_t s_bgmtab[9][3][2];   /* data_02074850 / data_02074852 */
static char s_dir[48] = "/cd/music";

void bgm_set_dir(const char *dir) { snprintf(s_dir, sizeof s_dir, "%s", dir); }

static void *read_all(const char *path, long *size)
{
    FILE *f = fopen(path, "rb");
    char *b;
    long n;
    if(!f) return NULL;
    fseek(f, 0, SEEK_END); n = ftell(f); fseek(f, 0, SEEK_SET);
    b = malloc((size_t)n + 1);
    if(b && fread(b, 1, (size_t)n, f) != (size_t)n) { free(b); b = NULL; }
    fclose(f);
    if(b) b[n] = 0;
    *size = n;
    return b;
}

int bgm_init(void)
{
    char path[64], *txt, *p;
    long sz;
    int cap = 0, fcap = 0;
    if(s_loaded) return s_n ? 0 : -1;
    s_loaded = 1;
    snprintf(path, sizeof path, "%s/streams.txt", s_dir);
    txt = read_all(path, &sz);
    if(!txt) { printf("bgm: %s missing\n", path); return -1; }
    for(p = txt; *p; p++) if(*p == '\n') cap++;
    s_tab = calloc((size_t)cap + 1, sizeof *s_tab);
    s_files = calloc((size_t)cap + 1, sizeof *s_files);
    fcap = cap + 1;
    for(p = txt; p && *p; ) {
        char *nl = strchr(p, '\n');
        char sd[10], fn[40];
        unsigned seq, ls, le, layer, player, ram, z, a, s0, s1;
        if(nl) *nl = 0;
        if(sscanf(p, "bgmtab %u %u %u %u", &z, &a, &s0, &s1) == 4) {
            if(z < 9 && a < 3) { s_bgmtab[z][a][0] = (uint8_t)s0; s_bgmtab[z][a][1] = (uint8_t)s1; }
        } else if(p[0] != '#' && sscanf(p, "%9s %u %39s %u %u %u %u %u", sd, &seq, fn, &ls, &le, &layer, &player, &ram) == 8 && s_n < cap) {
            BgmEntry *e = &s_tab[s_n++];
            snprintf(e->sdat, sizeof e->sdat, "%s", sd);
            e->seq = (uint8_t)seq; e->ls = ls; e->le = le; e->layer = (uint8_t)layer; e->player = (uint8_t)player; e->ram = (uint8_t)ram;
            e->file = -1;
            if(strcmp(fn, "-")) {
                int i;
                for(i = 0; i < s_nfiles; i++) if(!strcmp(s_files[i], fn)) break;
                if(i == s_nfiles && s_nfiles < fcap) { snprintf(s_files[s_nfiles], sizeof s_files[0], "%s", fn); s_nfiles++; }
                e->file = (int16_t)i;
            }
        }
        p = nl ? nl + 1 : NULL;
    }
    free(txt);
    printf("bgm: %d streams, %d files\n", s_n, s_nfiles);
    return s_n ? 0 : -1;
}

static const BgmEntry *lookup(const char *sdat, int seq)
{
    int i;
    if(bgm_init() || !sdat) return NULL;
    for(i = 0; i < s_n; i++) if(s_tab[i].seq == seq && !strcmp(s_tab[i].sdat, sdat)) return &s_tab[i];
    return NULL;
}
static void entry_path(const BgmEntry *e, char *out, size_t n) { snprintf(out, n, "%s/%s", s_dir, s_files[e->file]); }

/* ------------------------------------------------------------ state */
typedef struct {
    DcPlayer *pl;
    int state;          /* w->state: 0x16 / 0x17 (0: no task) */
    uint32_t gen;       /* audio slot generation at the start (stopped elsewhere -> task ends) */
    int player;         /* NNS player number of the jingle (pause by player number) */
} StageJingle;

static BgmState S;
static StageJingle s_jg;
static int s_zone = -1, s_act, s_chara;
static int s_layer_b = 0, s_layer_a = -1;   /* stream layer of gStageBGMHandleB / A (-1: silent) */
static uint32_t s_stage_gen;                /* audio slot 0 generation when the stage BGM started */
static int s_bgm_slot = -1;                 /* what plays on gBGMSoundHandle: -1 nothing, 0 disc, 1 jingle slot 1 */
static uint32_t s_bgm_gen;
static int s_bgm_player;
static int s_bgm_layer;                     /* stream layer of the disc track on gBGMSoundHandle */
static int s_grav_prev, s_grav_zone;
static DcPlayer *s_pl;
static uint8_t s_gBGMVolume = 0x7f;         /* gBGMVolume */
static uint32_t s_frames;                   /* frames since bgm_stage_start (test triggers) */

const BgmState *bgm_state(void) { return &S; }
void bgm_set_context(const char *sdat) { snprintf(S.ctx_sdat, sizeof S.ctx_sdat, "%s", sdat ? sdat : ""); }
const char *bgm_context(void) { return S.ctx_sdat; }

static int stage_handles_live(void) { return S.stage_active && audio_slot_gen(0) == s_stage_gen; }

/* NNS_SndPlayerMoveVolume on gStageBGMHandleB (which = 0) / gStageBGMHandleA (1) */
static void move_volume(int which, int vol, int frames)
{
    int layer = which == 0 ? s_layer_b : s_layer_a;
    if(!stage_handles_live() || layer < 0) return;
    audio_music_layer_volume(layer, vol, frames);
}

/* stop what plays on gBGMSoundHandle (NNS_SndPlayerStopSeq(&gBGMSoundHandle, fade)) */
static void bgm_handle_stop(int fade)
{
    if(s_bgm_slot == 0 && audio_slot_gen(0) == s_bgm_gen) audio_music_fade_stop(fade);
    else if(s_bgm_slot == 1 && audio_slot_gen(2) == s_bgm_gen) audio_jingle_stop(AUDIO_JINGLE_1, fade);
    s_bgm_slot = -1;
    S.bgm_seq = -1;
}

/* start sequence `seq` of archive `sdat` on gBGMSoundHandle (playerNo -1: the
 * sequence's player, else forced, as NNS_SndArcPlayerStartSeqEx) */
static int bgm_handle_start(const char *sdat, int seq, int playerNo)
{
    const BgmEntry *e = lookup(sdat, seq);
    char path[96];
    S.plays++;
    if(g_sound_trace) printf("bgm f%lu %s seq %d: %s\n", (unsigned long)g_sound_frame, sdat ? sdat : "-", seq,
                             !e ? "NO ENTRY" : e->file < 0 ? "silent in this SDAT" : s_files[e->file]);
    if(!e || e->file < 0) { S.misses++; return -1; }
    entry_path(e, path, sizeof path);
    S.bgm_seq = seq;
    s_bgm_player = playerNo >= 0 ? playerNo : e->player;
    s_bgm_layer = e->layer;
    if(e->ram) {
        if(audio_jingle_play(AUDIO_JINGLE_1, path, e->ls, e->le, s_gBGMVolume) != 0) return -1;
        s_bgm_slot = 1; s_bgm_gen = audio_slot_gen(2);
        if(S.frozen && s_bgm_player >= 3 && s_bgm_player <= 9) audio_jingle_pause(AUDIO_JINGLE_1, 1);
    } else {
        /* one disc stream: a track on gBGMSoundHandle replaces the stage BGM handles */
        if(audio_music_play_layers(path, e->ls, e->le, e->layer == 0 ? s_gBGMVolume : 0, e->layer == 1 ? s_gBGMVolume : 0) != 0) return -1;
        S.stage_active = 0;
        s_bgm_slot = 0; s_bgm_gen = audio_slot_gen(0);
    }
    return 0;
}

/* Sound_PlaySeq(NULL, seq): gBGMSoundHandle, stop (fade 0) + start */
int bgm_play(const char *sdat, int seq)
{
    bgm_handle_stop(0);
    return bgm_handle_start(sdat ? sdat : S.ctx_sdat, seq, -1);
}
void bgm_stop(int fade) { bgm_handle_stop(fade); }

/* NNS_SndPlayerMoveVolume(&gBGMSoundHandle, vol, frames) */
void bgm_move_volume(int vol, int frames)
{
    if(s_bgm_slot == 0 && audio_slot_gen(0) == s_bgm_gen) audio_music_layer_volume(s_bgm_layer, vol, frames);
    else if(s_bgm_slot == 1 && audio_slot_gen(2) == s_bgm_gen) audio_jingle_volume(AUDIO_JINGLE_1, vol, frames);
}

/* Sound_PlayBGMOnPlayer3(0, seq): same as Sound_PlaySeq, player 3 */
void Sound_PlayBGMOnPlayer3(int seq)
{
    bgm_handle_stop(0);
    bgm_handle_start(S.ctx_sdat, seq, 3);
}

/* ------------------------------------------------------------ Stage_* */
static const char *stage_sdat(int zone, int act, int chara)
{
    if(zone == 8 && act == 2) return "ze";   /* Extra boss: ExtraBoss loads /snd/ze */
    return sound_stage_sdat(zone, act, chara);
}

/* Stage_SelectBGMLayer */
void Stage_SelectBGMLayer(int a)
{
    if(s_zone < 0 || (unsigned)s_zone >= 9 || (unsigned)s_act >= 2) return;
    S.layer_sel = a ? 1 : 0;
    if(g_sound_trace) printf("bgm f%lu layer %d\n", (unsigned long)g_sound_frame, S.layer_sel);
    if(a != 0) {
        move_volume(0, 0, 0);                   /* gStageBGMHandleB, channel priority 0 */
        move_volume(1, s_gBGMVolume, 0);        /* gStageBGMHandleA, 0x40 */
        return;
    }
    move_volume(0, s_gBGMVolume, 0);
    move_volume(1, 0, 0);
}

/* Stage_FadeBGMLayer(p0 volume, p1 frames, p2 layer flag (player @0x32C & 0x4000000)) */
void Stage_FadeBGMLayer(int p0, int p1, int p2)
{
    int t0 = (p0 * s_gBGMVolume) >> 7;
    if(p0 != 0) {
        if(p2 != 0) move_volume(1, t0, p1);      /* + channel priorities B 0, A 0x40 */
        else move_volume(0, t0, p1);             /* + B 0x40, A 0 */
        return;
    }
    move_volume(0, t0, p1);
    move_volume(1, t0, p1);
}

/* Stage_PlayBGM(a) */
void Stage_PlayBGM(int a)
{
    const BgmEntry *eb, *ea;
    const char *sdat = S.stage_sdat;
    char path[96];
    S.stage_active = 0;
    S.layered = 0;
    if((unsigned)s_zone >= 9 || (unsigned)s_act >= 2) {
        /* NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0); Sound_PlaySeq(&gBGMSoundHandle, table[zone][act][0]) */
        bgm_handle_stop(0);
        if((unsigned)s_zone < 9) bgm_handle_start(sdat, s_bgmtab[s_zone][s_act][0], -1);
        return;
    }
    /* B = table[0] (Sonic) / [1] (Blaze), A = the other */
    S.seq_b = s_bgmtab[s_zone][s_act][s_chara == 0 ? 0 : 1];
    S.seq_a = s_bgmtab[s_zone][s_act][s_chara == 0 ? 1 : 0];
    eb = lookup(sdat, S.seq_b);
    ea = lookup(sdat, S.seq_a);
    S.layered = 0;
    s_layer_b = 0; s_layer_a = -1;
    if(g_sound_trace) printf("bgm f%lu Stage_PlayBGM %s B %d (%s) A %d (%s)\n", (unsigned long)g_sound_frame, sdat, S.seq_b,
                             eb && eb->file >= 0 ? s_files[eb->file] : "silent", S.seq_a, ea && ea->file >= 0 ? s_files[ea->file] : "silent");
    if(!eb || eb->file < 0) {
        audio_music_fade_stop(0);   /* NNS_SndPlayerStopSeq(B / A, 0): the previous music ends */
        S.misses++;
        return;
    }
    if(ea && ea->file == eb->file && ea->layer != eb->layer) { S.layered = 1; s_layer_b = eb->layer; s_layer_a = ea->layer; }
    else if(eb->layer != 0) s_layer_b = eb->layer;   /* a single layer of a .lyr (the other one silent) */
    entry_path(eb, path, sizeof path);
    {
        int vb = s_gBGMVolume, va = S.layered ? s_gBGMVolume : 0;   /* NNS_SndPlayerSetVolume(B / A, gBGMVolume) */
        int v0 = s_layer_b == 0 ? vb : va, v1 = s_layer_b == 1 ? vb : va;
        if(audio_music_play_layers(path, eb->ls, eb->le, v0, v1) != 0) { S.misses++; return; }
    }
    S.plays++;
    if(s_bgm_slot == 0) s_bgm_slot = -1;   /* the disc stream now belongs to the stage handles */
    S.stage_active = 1;
    s_stage_gen = audio_slot_gen(0);
    Stage_SelectBGMLayer(a);
}

/* ------------------------------------------------------------ StageJingle */
static void jingle_destroy(void)   /* StageJingle_Destructor: NNS_SndHandleReleaseSeq + Sound_FreeHandle */
{
    s_jg.state = 0;
    S.jingle_state = 0;
}

/* StageJingle_Create(pl, state) */
void StageJingle_Create(DcPlayer *pl, int state)
{
    const BgmEntry *e;
    char path[96];
    if(s_jg.state != 0) audio_jingle_stop(AUDIO_JINGLE_0, 0);   /* NNS_SndPlayerStopSeq(w->spr, 0) */
    s_jg.pl = pl;
    s_jg.state = state;
    S.jingle_state = state;
    Stage_FadeBGMLayer(0, 0, pl ? (pl->pf & 0x4000000) != 0 : 0);
    /* Sound_PlayBGMEx(w->spr, state, 100, -1): player from the sequence info */
    e = lookup(S.stage_sdat, state);
    if(g_sound_trace) printf("bgm f%lu StageJingle %#x: %s\n", (unsigned long)g_sound_frame, state, e && e->file >= 0 ? s_files[e->file] : "silent");
    s_jg.player = e ? e->player : 0;
    if(e && e->file >= 0) {
        entry_path(e, path, sizeof path);
        audio_jingle_play(AUDIO_JINGLE_0, path, e->ls, e->le, s_gBGMVolume);
        if(S.frozen && s_jg.player >= 3 && s_jg.player <= 9) audio_jingle_pause(AUDIO_JINGLE_0, 1);
    } else {
        audio_jingle_stop(AUDIO_JINGLE_0, 0);
        S.misses++;
    }
    s_jg.gen = audio_slot_gen(1);
}

/* StageJingle_Main */
static void StageJingle_Main(void)
{
    DcPlayer *pl = s_jg.pl;
    int uw;
    if(!s_jg.state) return;
    if(audio_slot_gen(1) != s_jg.gen) { jingle_destroy(); return; }   /* stopped by StopSoundAll / a stage change */
    if(!pl) return;
    uw = (pl->pf & 0x4000000) != 0;
    switch(s_jg.state) {
    case 0x16:
        if(pl->invinc == 0x5A) {
            audio_jingle_stop(AUDIO_JINGLE_0, 0x5A);
            Stage_FadeBGMLayer(s_gBGMVolume, 0x5A, uw);
            jingle_destroy();
        } else if(pl->invinc == 0) {
            audio_jingle_stop(AUDIO_JINGLE_0, 0);
            Stage_FadeBGMLayer(s_gBGMVolume, 0, uw);
            jingle_destroy();
        }
        break;
    case 0x17:
        audio_jingle_pause(AUDIO_JINGLE_0, S.frozen);   /* gSystemFlags & 8 */
        if(pl->unk3CA == 0) {
            audio_jingle_stop(AUDIO_JINGLE_0, 0);
            Stage_FadeBGMLayer(s_gBGMVolume, 0x3C, uw);
            jingle_destroy();
        }
        break;
    }
}

/* Player_GiveInvincibility: Sound_PlaySeqArcEx(0, 0, 0x3d); StageJingle_Create(p, 0x16);
 * p->unk3CA = 0; p->invinc = 0x3e7; EffInvincibility_Create (graphics: not here) */
void Player_GiveInvincibility(DcPlayer *p)
{
    Sound_PlaySeqArcEx(NULL, 0, 0x3d);
    StageJingle_Create(p, 0x16);
    if(p) { p->unk3CA = 0; p->invinc = 0x3e7; }
}

/* Player_AddLives: no 1-up in VS (1) and time attack (2); Sound_PlayBGMOnPlayer3(0, 0x18) */
void Player_AddLives_Sound(int n)
{
    (void)n;
    if((uint32_t)(g_dcp_world.mode - 1) <= 1u) return;
    Sound_PlayBGMOnPlayer3(0x18);
}

/* ------------------------------------------------------------ stage lifetime */
static void test_triggers_load(void);
void bgm_bind_player(DcPlayer *p)
{
    s_pl = p;
    s_grav_prev = p ? p->grav_rot : 0;
}

/* DC: ClearDemo's sound bank and music (bgm_ui_music_hook, ClearDemo_Create's choice for
 * the story: act 0xf / boss 0x10 / zone 7 0x11, banks sys_a06 / sys_a07; time attack 0xb /
 * 0xd, sys_a05) read in the stage load, so the goal frame reads nothing from the disc (the
 * bank, the music file's directory walk and header held it for ~30 ms in Flycast) */
static void bgm_prefetch_clear(int zone, int act)
{
#ifdef _arch_dreamcast
    int ta = g_dcp_world.mode == 2;
    int seq = ta ? (act < 2 ? 0xb : 0xd) : zone == 7 ? 0x11 : act >= 2 ? 0x10 : 0xf;
    int arc = ta ? 5 : zone == 7 ? 7 : 6;
    const BgmEntry *e = lookup("sys", seq);
    char path[96];
    snprintf(path, sizeof path, "/cd/sound/sys_a%02d.srb", arc);
    pak_prefetch(path);
    if(e && e->file >= 0 && !e->ram) { entry_path(e, path, sizeof path); audio_music_prefetch(path); }
#else
    (void)zone; (void)act;
#endif
}

void bgm_stage_start(int zone, int act, int chara, DcPlayer *p)
{
    const char *sdat = stage_sdat(zone, act, chara);
    static const int pre[3] = {0x16, 0x17, 0x18};
    int i;
    jingle_destroy();
    audio_jingle_stop(AUDIO_JINGLE_0, 0);
    if(s_bgm_slot == 1) bgm_handle_stop(0);
    if(strcmp(S.stage_sdat, sdat ? sdat : "")) audio_jingle_flush();   /* another zone: other jingle renders */
    s_zone = zone; s_act = act; s_chara = chara;
    snprintf(S.stage_sdat, sizeof S.stage_sdat, "%s", sdat ? sdat : "");
    bgm_set_context(S.stage_sdat);
    s_grav_zone = zone == 6 && act < 2;   /* Dead Line acts: Player_UpdateGravityFlip */
    bgm_bind_player(p);
    s_frames = 0;
    /* RAM jingles of this SDAT (read now, inside the load bracket) */
    for(i = 0; i < 3; i++) {
        const BgmEntry *e = lookup(S.stage_sdat, pre[i]);
        char path[96];
        if(e && e->file >= 0 && e->ram) { entry_path(e, path, sizeof path); audio_jingle_preload(path); }
    }
    test_triggers_load();
    Stage_PlayBGM(0);
    bgm_prefetch_clear(zone, act);
}

void bgm_stage_restart(void)
{
    if(s_zone < 0) return;
    /* StageRestart_State_Release: Stage_DestroyTasks, zone sound archive released (all sound stops) */
    bgm_stop_sound_all();
    bgm_stage_start(s_zone, s_act, s_chara, s_pl);
}

void bgm_stage_end(void)
{
    jingle_destroy();
    s_pl = NULL;
    s_grav_zone = 0;
}

/* NNS_SndStopSoundAll: every sequence (music, jingles, effects) stops */
void bgm_stop_sound_all(void)
{
    audio_music_stop();
    sound_stop_all();
    S.stage_active = 0;
    s_bgm_slot = -1; S.bgm_seq = -1;
    jingle_destroy();
}

/* PauseMenu_Create / _Destructor: System_Freeze + NNS_SndPlayerPauseByPlayerNo(3..9, on) */
void bgm_pause_players(int on)
{
    int k;
    S.frozen = on ? 1 : 0;
    for(k = 3; k < 10; k++) NNS_SndPlayerPauseByPlayerNo(k, on);
    if(s_jg.state && s_jg.player >= 3 && s_jg.player <= 9) audio_jingle_pause(AUDIO_JINGLE_0, on);
    if(s_bgm_slot == 1 && s_bgm_player >= 3 && s_bgm_player <= 9) audio_jingle_pause(AUDIO_JINGLE_1, on);
    if(g_sound_trace) printf("bgm f%lu pause players 3..9: %d\n", (unsigned long)g_sound_frame, on);
}

/* ------------------------------------------------------------ hooks */
/* ClearDemo_Create / ClearDemoTA / GameOverSeq: the sys archive is loaded first
 * (Sound_LoadSoundArchive + NNS_SndArcLoadGroup); ClearDemo's SE / voices are in
 * SSAR w->unk1630 = 5 (time attack), 6 (act / boss), 7 (final zone) -> sys_aNN bank */
void bgm_ui_music_hook(void *ctx, int32_t id)
{
    int arc = -1;
    (void)ctx;
    if(id < 0) { bgm_stop_sound_all(); return; }
    switch(id) {
    case 0xb: case 0xd: arc = 5; break;        /* SEQ_TA_ACT_CLEAR / SEQ_TA_BOSS_CLEAR */
    case 0xf: case 0x10: arc = 6; break;       /* SEQ_ACT_CLEAR / SEQ_BOSS_CLEAR */
    case 0x11: arc = 7; break;                 /* SEQ_FINAL_CLEAR */
    default: break;                            /* 0xc / 0xe new record, 0x14 game over */
    }
    if(arc >= 0) {
        char name[12];
        snprintf(name, sizeof name, "sys_a%02d", arc);
        if(strcmp(sound_bank_name(), name) && sound_load_bank(name) != 0) printf("bgm: sound bank %s missing\n", name);
    }
    bgm_set_context("sys");
    bgm_play("sys", id);
}
void bgm_ui_music_pause_hook(void *ctx, int on) { (void)ctx; bgm_pause_players(on); }
void bgm_ui_sfx_hook(void *ctx, int32_t arc, int32_t id) { (void)ctx; Sound_PlaySeqArcEx(NULL, arc, id); }
void bgm_boss_music_hook(void *ctx, int32_t seq) { (void)ctx; bgm_play(NULL, seq); }
void bgm_boss_music_stop_hook(void *ctx)
{
    (void)ctx;
    /* NNS_SndPlayerStopSeqAll(0): every sequence player, music and effects */
    bgm_handle_stop(0);
    if(stage_handles_live()) audio_music_fade_stop(0);
    S.stage_active = 0;
    audio_jingle_stop(AUDIO_JINGLE_0, 0);
    NNS_SndPlayerStopSeqAll(0);
}
void bgm_boss_music_fade_hook(void *ctx, int32_t frames) { (void)ctx; bgm_stop(frames); }
void bgm_boss_music_volume_hook(void *ctx, int32_t vol, int32_t frames) { (void)ctx; bgm_move_volume(vol, frames); }
void bgm_special_music(int seq) { bgm_set_context("sys"); bgm_play("sys", seq); }
void bgm_special_music_stop(int fade) { bgm_stop(fade); }

int bgm_obj_effect(DcPlayer *p, int32_t kind, int32_t a)
{
    if(kind == OBJ_FX_BGM_LAYER) { Stage_SelectBGMLayer(a); return 1; }
    if(kind == OBJ_FX_JINGLE) { StageJingle_Create(p, a); return 1; }
    return 0;
}

/* ------------------------------------------------------------ test triggers
 * /cd/debug/bgmtest.txt (test discs only): "frame command arg" lines, frame =
 * frames since bgm_stage_start; commands: layer N, jingle 0x16|0x17, oneup,
 * invinc, seq N (Sound_PlaySeq in the stage SDAT), ui N, pause 0|1, stopall,
 * air N (player->unk3CA = N), inv N (player->invinc = N) */
static struct { uint32_t frame; char cmd[12]; int arg, arg2; } s_trig[32];
static int s_pin = 0, s_pin_x, s_pin_y, s_push;     /* 1: absolute position, 2: water line + y, 3: split line + y */
s32 Stage_GetSplitLineY(s32 x) __attribute__((weak));   /* camera module */
static int s_ntrig, s_trig_next, s_trig_loaded;
static void test_triggers_load(void)
{
    long sz;
    char *t, *p;
    s_trig_next = 0;
    s_pin = 0; s_push = 0;
    if(s_trig_loaded) return;
    s_trig_loaded = 1;
    t = read_all("/cd/debug/bgmtest.txt", &sz);
    if(!t) return;
    for(p = t; p && *p && s_ntrig < 32; ) {
        char *nl = strchr(p, '\n');
        unsigned f; char c[12]; int a = 0, b = 0;
        if(nl) *nl = 0;
        if(sscanf(p, "%u %11s %i %i", &f, c, &a, &b) >= 2) { s_trig[s_ntrig].frame = f; strcpy(s_trig[s_ntrig].cmd, c); s_trig[s_ntrig].arg = a; s_trig[s_ntrig].arg2 = b; s_ntrig++; }
        p = nl ? nl + 1 : NULL;
    }
    free(t);
    printf("bgm: %d test triggers\n", s_ntrig);
}
static void test_triggers_run(void)
{
    while(s_trig_next < s_ntrig && s_trig[s_trig_next].frame <= s_frames) {
        const char *c = s_trig[s_trig_next].cmd;
        int a = s_trig[s_trig_next].arg;
        printf("bgm test f%lu: %s %d\n", (unsigned long)s_frames, c, a);
        if(!strcmp(c, "layer")) Stage_SelectBGMLayer(a);
        else if(!strcmp(c, "jingle")) StageJingle_Create(s_pl, a);
        else if(!strcmp(c, "oneup")) Player_AddLives_Sound(1);
        else if(!strcmp(c, "invinc")) Player_GiveInvincibility(s_pl);
        else if(!strcmp(c, "seq")) bgm_play(S.stage_sdat, a);
        else if(!strcmp(c, "ui")) bgm_ui_music_hook(NULL, a);
        else if(!strcmp(c, "pause")) bgm_pause_players(a);
        else if(!strcmp(c, "stopall")) bgm_stop_sound_all();
        else if(!strcmp(c, "air") && s_pl) s_pl->unk3CA = (s16)a;
        else if(!strcmp(c, "inv") && s_pl) s_pl->invinc = (s16)a;
        else if(!strcmp(c, "pin")) { s_pin = 1; s_pin_x = a; s_pin_y = s_trig[s_trig_next].arg2; }   /* pin X Y (px) */
        else if(!strcmp(c, "pinw")) { s_pin = 2; s_pin_y = a; }    /* water line + a px, x kept */
        else if(!strcmp(c, "pins")) { s_pin = 3; s_pin_y = a; }    /* split line + a px (Dead Line gravity) */
        else if(!strcmp(c, "release")) s_pin = 0;
        else if(!strcmp(c, "push")) s_push = a;                   /* ground speed every frame (0 = off) */
        else if(!strcmp(c, "warp") && s_pl) { s_pl->x = a << 8; s_pl->y = s_trig[s_trig_next].arg2 << 8; s_pl->vel_x = s_pl->vel_y = s_pl->gnd_vel = 0; }
        if(s_pl) printf("bgm test: player %ld,%ld water %u split %ld air %d/%d invinc %d grav %d\n", (long)(s_pl->x >> 8), (long)(s_pl->y >> 8),
                        g_obj_camera[0].water, Stage_GetSplitLineY ? (long)(Stage_GetSplitLineY(s_pl->x) >> 8) : -1L, s_pl->unk3CA, s_pl->p374,
                        s_pl->invinc, s_pl->grav_rot);
        s_trig_next++;
    }
    if(s_push && s_pl && !s_pin) { s_pl->gnd_vel = (s16)s_push; if(s_pl->move_flags & DCP_MF_AIR) s_pl->vel_x = (s16)s_push; }
    if(s_pin && s_pl) {   /* test position hold: after the world update of this frame */
        s32 y = s_pin == 1 ? s_pin_y : s_pin == 2 ? (s32)g_obj_camera[0].water + s_pin_y
              : (Stage_GetSplitLineY ? (Stage_GetSplitLineY(s_pl->x) >> 8) : 0) + s_pin_y;
        if(s_pin == 1) s_pl->x = s_pin_x << 8;
        s_pl->y = y << 8; s_pl->vel_y = 0;
        if(s_pin == 1) { s_pl->vel_x = 0; s_pl->gnd_vel = 0; }
    }
}

/* ------------------------------------------------------------ per frame */
void bgm_update(void)
{
    s_frames++;
    if(s_ntrig) test_triggers_run();
    StageJingle_Main();
    /* Player_UpdateGravityFlip (Dead Line): the player module flips gravity; its
     * music / SE half: Stage_SelectBGMLayer(1 / 0) unless invincible, SE 0x66 on
     * the voice handle (not on the first frame after a checkpoint, not with pf & 0x400) */
    if(s_grav_zone && s_pl) {
        int g = s_pl->grav_rot;
        if(g != s_grav_prev && (g == 0x80 || g == 0)) {
            if(s_pl->invinc == 0) Stage_SelectBGMLayer(g == 0x80);
            if(g_obj_stage.check_time + 1 < g_dcp_world.frame && !(s_pl->pf & 0x400)) sound_player_sfx(s_pl, DCP_CH_VOICE, 0x66);
        }
        s_grav_prev = g;
    }
}
