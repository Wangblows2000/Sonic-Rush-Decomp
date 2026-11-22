/*
 * Hooks adapter: routes the sound side effects of the translated player
 * (g_dcp_hooks.sfx / sfx_stop) and objects (g_obj_hooks.sfx) into the native
 * Sound_* layer, the way the original code called it:
 *   player  Sound_PlaySeqArcEx(&w->unk384 / unk38C / unk390 / NULL, 0, id)
 *           (DCP_CH_VOICE = @384, BOOST = @38C, TRICK = @390, GLOBAL = NULL
 *            -> gSESoundHandle), NNS_SndPlayerStopSeq(handle, fade)
 *   objects Sound_PlaySeqArcEx(NULL, 0, id)  (object handles / positional
 *           volume were dropped by the objects module: plays centred)
 * Previously installed hooks are chained (called first, same ctx).
 */
#include "sound.h"
#include "snd_nns.h"
#include "../player/dcp_player.h"
#include "../objects/objects.h"
#include <string.h>

#define MAX_PLAYERS 2
static struct { DcPlayer *p; SoundHandle h[3]; } s_ph[MAX_PLAYERS];
static void (*s_prev_sfx)(void *, DcPlayer *, s32, s32);
static void (*s_prev_stop)(void *, DcPlayer *, s32);
static void (*s_prev_obj)(void *, s32);
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
    h = ch == DCP_CH_GLOBAL ? &gSESoundHandle : player_handle(p, ch);
    if (h) NNS_SndPlayerStopSeq(h, ch == DCP_CH_GLOBAL ? 0 : s_stop_fade[ch]);
}
static void hk_obj_sfx(void *ctx, s32 id) {
    if (s_prev_obj) s_prev_obj(ctx, id);
    Sound_PlaySeqArcEx(NULL, 0, id);
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
