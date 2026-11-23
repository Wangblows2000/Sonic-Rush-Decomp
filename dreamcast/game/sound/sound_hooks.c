/*
 * Hooks adapter: routes the sound side effects of the translated player
 * (g_dcp_hooks.sfx / sfx_stop) and objects (g_obj_hooks.sfx) into the native
 * Sound_* layer, the way the original code called it:
 *   player  Sound_PlaySeqArcEx(&w->unk384 / unk38C / unk390 / NULL, 0, id)
 *           (DCP_CH_VOICE = @384, BOOST = @38C, TRICK = @390, GLOBAL = NULL
 *            -> gSESoundHandle), NNS_SndPlayerStopSeq(handle, fade)
 *   objects Sound_PlaySeqArcEx(NULL, 0, id)  (object handles / positional
 *           volume were dropped by the objects module: plays centred)
 *   g_dcp_hooks.add_life  Player_AddLives -> Sound_PlayBGMOnPlayer3(0, 0x18) (bgm.c)
 *   g_dcp_hooks.restart   StageRestart -> Stage_PlayBGM(0) again (bgm.c)
 *   g_obj_hooks.item      OBJ_ITEM_INVINCIBLE -> Player_GiveInvincibility (bgm.c)
 * Previously installed hooks are chained (called first, same ctx).
 */
#include "sound.h"
#include "snd_nns.h"
#include "../player/dcp_player.h"
#include "../objects/objects.h"
#include <string.h>
#include <stdio.h>
#include "snd_internal.h"
#include "bgm.h"
extern uint32_t g_sound_frame;

#define MAX_PLAYERS 2
static struct { DcPlayer *p; SoundHandle h[3]; } s_ph[MAX_PLAYERS];
static void (*s_prev_sfx)(void *, DcPlayer *, s32, s32);
static void (*s_prev_stop)(void *, DcPlayer *, s32);
static void (*s_prev_obj)(void *, s32);
static void (*s_prev_add_life)(void *, DcPlayer *, s32);
static void (*s_prev_restart)(void *, DcPlayer *);
static void (*s_prev_item)(void *, DcPlayer *, s32);
static void (*s_prev_vol)(void *, DcPlayer *, s32, s32);
static int s_installed;

/* Fade used by sfx_stop: the hook carries no fade argument. Most frequent
 * value in src/game/Player per handle: @384 -> 0, @38C -> 0, @390 -> 0x20
 * (9 of 11 NNS_SndPlayerStopSeq(w->unk390, ..) calls). */
static const int s_stop_fade[3] = {0, 0, 0x20};

static SoundHandle *player_handle(DcPlayer *p, s32 ch) {
    int i, free_slot = -1;
    if (ch < DCP_CH_VOICE || ch > DCP_CH_TRICK) return NULL;
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (s_ph[i].p == p) return &s_ph[i].h[ch];
        if (!s_ph[i].p && free_slot < 0) free_slot = i;
    }
    if (free_slot < 0) free_slot = 0; /* more players than slots: reuse the first */
    s_ph[free_slot].p = p;
    for (i = 0; i < 3; i++) NNS_SndHandleInit(&s_ph[free_slot].h[i]);
    return &s_ph[free_slot].h[ch];
}

static void hk_sfx(void *ctx, DcPlayer *p, s32 ch, s32 id) {
    if (s_prev_sfx) s_prev_sfx(ctx, p, ch, id);
    Sound_PlaySeqArcEx(ch == DCP_CH_GLOBAL ? NULL : player_handle(p, ch), 0, id);
}
static void hk_sfx_stop(void *ctx, DcPlayer *p, s32 ch) {
    SoundHandle *h;
    if (s_prev_stop) s_prev_stop(ctx, p, ch);
    if (g_sound_trace) printf("snd f%lu stop ch%ld\n", (unsigned long)g_sound_frame, (long)ch);
    h = ch == DCP_CH_GLOBAL ? &gSESoundHandle : player_handle(p, ch);
    if (h) NNS_SndPlayerStopSeq(h, ch == DCP_CH_GLOBAL ? 0 : s_stop_fade[ch]);
}
/* Player_Action_TrickCombo: NNS_SndPlayerSetVolume(&w->unk384 / unk390, 0x7f >> level) */
static void hk_sfx_volume(void *ctx, DcPlayer *p, s32 ch, s32 vol) {
    SoundHandle *h;
    if (s_prev_vol) s_prev_vol(ctx, p, ch, vol);
    h = ch == DCP_CH_GLOBAL ? &gSESoundHandle : player_handle(p, ch);
    if (h) NNS_SndPlayerSetVolume(h, vol);
}
static void hk_obj_sfx(void *ctx, s32 id) {
    if (s_prev_obj) s_prev_obj(ctx, id);
    Sound_PlaySeqArcEx(NULL, 0, id);
}

/* Sound_PlaySeqArcEx on a player handle (bgm.c: the gravity-flip SE) */
void sound_player_sfx(DcPlayer *p, int ch, int id) {
    Sound_PlaySeqArcEx(ch == DCP_CH_GLOBAL ? NULL : player_handle(p, ch), 0, id);
}
/* Player_AddLives (the ui module counts the life): Sound_PlayBGMOnPlayer3(0, 0x18) */
static void hk_add_life(void *ctx, DcPlayer *p, s32 n) {
    if (s_prev_add_life) s_prev_add_life(ctx, p, n);
    Player_AddLives_Sound(n);
}
/* Player_Respawn -> StageRestart_Create (story / time attack; not VS, not pf2 & 0x400000):
 * the stage is rebuilt and Stage_PlayBGM(0) starts the music again */
static void hk_restart(void *ctx, DcPlayer *p) {
    if (s_prev_restart) s_prev_restart(ctx, p);
    if (g_dcp_world.mode != 1 && !(p->pf2 & 0x400000)) bgm_stage_restart();
}
/* ItemBox_GiveItem 5: Player_GiveInvincibility (audio + timers; the effect is not translated) */
static void hk_item(void *ctx, DcPlayer *p, s32 item) {
    if (s_prev_item) s_prev_item(ctx, p, item);
    if (item == OBJ_ITEM_INVINCIBLE) Player_GiveInvincibility(p);
}

/* Call once at boot BEFORE the first objects_init(): objects_init saves
 * g_dcp_hooks and objects_shutdown restores that copy, so hooks installed
 * before it survive stage changes. Calling it again re-installs whatever was
 * wiped (never chaining to itself). */
void sound_hooks_install(void) {
    if (!s_installed) memset(s_ph, 0, sizeof s_ph);
    if (g_dcp_hooks.sfx != hk_sfx) { s_prev_sfx = g_dcp_hooks.sfx; g_dcp_hooks.sfx = hk_sfx; }
    if (g_dcp_hooks.sfx_stop != hk_sfx_stop) { s_prev_stop = g_dcp_hooks.sfx_stop; g_dcp_hooks.sfx_stop = hk_sfx_stop; }
    if (g_obj_hooks.sfx != hk_obj_sfx) { s_prev_obj = g_obj_hooks.sfx; g_obj_hooks.sfx = hk_obj_sfx; }
    if (g_dcp_hooks.add_life != hk_add_life) { s_prev_add_life = g_dcp_hooks.add_life; g_dcp_hooks.add_life = hk_add_life; }
    if (g_dcp_hooks.restart != hk_restart) { s_prev_restart = g_dcp_hooks.restart; g_dcp_hooks.restart = hk_restart; }
    if (g_dcp_hooks.sfx_volume != hk_sfx_volume) { s_prev_vol = g_dcp_hooks.sfx_volume; g_dcp_hooks.sfx_volume = hk_sfx_volume; }
    if (g_obj_hooks.item != hk_item) { s_prev_item = g_obj_hooks.item; g_obj_hooks.item = hk_item; }
    s_installed = 1;
}

/* forget the per-player handles (call when the players are recreated) */
void sound_hooks_reset_players(void) {
    int i, k;
    for (i = 0; i < MAX_PLAYERS; i++) {
        for (k = 0; k < 3; k++) if (s_ph[i].p) NNS_SndHandleReleaseSeq(&s_ph[i].h[k]);
        s_ph[i].p = NULL;
    }
}

/* ---------------------------------------------------------------- bosses
 * g_boss_hooks (boss/boss.h) -> the original calls of the boss code:
 *   sfx        Sound_PlaySeqArc(NULL, 0, id)        (gSESoundHandle)
 *   voice      Sound_PlayVoice(NULL, 0, id)         (gVoiceSoundHandle: Eggman's lines)
 *   sfx_handle Sound_PlaySeqArc(handle, 0, id)      (a boss's own handles from Sound_AllocHandle;
 *              the boss code keeps small ids, mapped here to SoundHandles)
 *   sfx_stop   NNS_SndPlayerStopSeq(handle, 0)
 * main.c installs these with the other boss hooks. */
#define BOSS_HANDLES 8
static struct { s32 id; SoundHandle h; } s_bh[BOSS_HANDLES];

static SoundHandle *boss_handle(s32 id, int alloc) {
    int i, free_slot = -1;
    for (i = 0; i < BOSS_HANDLES; i++) {
        if (s_bh[i].id == id && id != 0) return &s_bh[i].h;
        if (s_bh[i].id == 0 && free_slot < 0) free_slot = i;
    }
    if (!alloc) return NULL;
    if (free_slot < 0) free_slot = 0;
    s_bh[free_slot].id = id;
    NNS_SndHandleInit(&s_bh[free_slot].h);
    return &s_bh[free_slot].h;
}
void sound_boss_sfx(void *ctx, s32 id) { (void)ctx; Sound_PlaySeqArc(NULL, 0, (uint32_t)id); }
void sound_boss_voice(void *ctx, s32 id) { (void)ctx; Sound_PlayVoice(NULL, 0, (uint32_t)id); }
void sound_boss_sfx_handle(void *ctx, s32 handle, s32 id) {
    (void)ctx;
    Sound_PlaySeqArc(handle ? boss_handle(handle, 1) : NULL, 0, (uint32_t)id);
}
void sound_boss_sfx_stop(void *ctx, s32 handle) {
    SoundHandle *h = boss_handle(handle, 0);
    (void)ctx;
    if (h) NNS_SndPlayerStopSeq(h, 0);
}
/* a new boss fight: drop the handles of the previous one */
void sound_boss_reset(void) {
    int i;
    for (i = 0; i < BOSS_HANDLES; i++) {
        if (s_bh[i].id) NNS_SndHandleReleaseSeq(&s_bh[i].h);
        s_bh[i].id = 0;
    }
}
