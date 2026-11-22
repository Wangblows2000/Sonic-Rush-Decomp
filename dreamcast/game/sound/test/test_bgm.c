/*
 * Host tests of the music selection / in-stage music events (sound/bgm.c)
 * against the real stream table (musictab.py output) and the real banks:
 *   test_bgm <bank dir> <disc music dir>
 * 1. every stage (zone, act, character): Stage_PlayBGM streams the right file,
 *    the layered stages open the .lyr with the right layer audible; prints the table
 * 2. Stage_SelectBGMLayer: instant switch of the layer volumes
 * 3. StageJingle 0x16 (invincibility): BGM to 0, jingle, end at invinc 0x5A:
 *    jingle fade 90, BGM back to 126 over 90 frames (position kept)
 * 4. StageJingle 0x17 (drowning): pause with the game frozen, resume, surfacing:
 *    stop + the underwater layer back over 60 frames
 * 5. 1-up: RAM jingle on gBGMSoundHandle, stage BGM untouched; boss act: the boss
 *    music stops, SEQ_1up is silent in the boss SDAT; VS / time attack: none
 * 6. boss pinch / talk / Extra; 7. results / game over / new record (sys);
 * 8. restart after a death; 9. gravity flip; 10. pause menu pauses players 3..9
 */
#include "snd_internal.h"
#include "snd_nns.h"
#include "sound.h"
#include "bgm.h"
#include "audio_host.h"
#include "../../player/dcp_player.h"
#include "../../objects/objects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DcpHooks g_dcp_hooks;
ObjHooks g_obj_hooks;
static int fails;
#define CHECK(c, ...) do { if (!(c)) { fails++; printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n"); } } while (0)
static char s_mdir[256];

static int ends(const char *s, const char *t) { size_t a = strlen(s), b = strlen(t); return a >= b && !strcmp(s + a - b, t); }
static void frames(int n) { while (n--) sound_update(); }

static void test_stage_table(void) {
    static const char *zname[9] = {"Leaf Storm", "Water Palace", "Mirage Road", "Night Carnival", "Huge Crisis",
                                   "Altitude Limit", "Dead Line", "zone 7 (z81/z82, final boss)", "zone 8 (z91/z92, Extra)"};
    DcPlayer p;
    printf("\nstage music (Stage_PlayBGM / boss Sound_PlaySeq):\n");
    for (int z = 0; z < 9; z++)
        for (int a = 0; a < 3; a++)
            for (int c = 0; c < 2; c++) {
                memset(&p, 0, sizeof p);
                g_ah.music[0] = 0; g_ah.music_on = 0;
                bgm_stage_start(z, a, c, &p);
                const BgmState *s = bgm_state();
                printf("  %-28s act %d %-5s %-5s -> %-26s vol %3d/%3d%s\n", zname[z], a + 1, c ? "Blaze" : "Sonic", s->stage_sdat,
                       g_ah.music_on ? strrchr(g_ah.music, '/') + 1 : "(none)", g_ah.mvol[0], g_ah.mvol[1],
                       s->layered ? "  layered" : "");
                if (z == 8 && a == 2) { CHECK(g_ah.music_on && ends(g_ah.music, "ze_20.wav"), "extra boss music %s", g_ah.music); continue; }
                CHECK(g_ah.music_on, "zone %d act %d chara %d: no music", z, a, c);
            }
    /* fixed expectations (sequence numbers of data_02074850, SDAT per gStageZoneFileListTable) */
    memset(&p, 0, sizeof p);
    bgm_stage_start(0, 0, 0, &p); CHECK(ends(g_ah.music, "z1s_00.wav") && g_ah.mvol[0] == 127, "z11 sonic %s", g_ah.music);
    bgm_stage_start(0, 1, 1, &p); CHECK(ends(g_ah.music, "z1b_01.wav"), "z12 blaze %s", g_ah.music);
    bgm_stage_start(0, 2, 0, &p); CHECK(ends(g_ah.music, "z1es_14.wav"), "z13 boss %s", g_ah.music);
    bgm_stage_start(0, 2, 1, &p); CHECK(ends(g_ah.music, "z1eb_14.wav"), "z13 boss blaze %s", g_ah.music);
    bgm_stage_start(6, 2, 1, &p); CHECK(ends(g_ah.music, "z7eb_16.wav"), "z73 boss7 %s", g_ah.music);
    bgm_stage_start(7, 2, 0, &p); CHECK(ends(g_ah.music, "zfes_18.wav"), "final boss %s", g_ah.music);
    bgm_stage_start(1, 0, 0, &p);
    CHECK(ends(g_ah.music, "z2s_02_03.lyr") && g_ah.mvol[0] == 127 && g_ah.mvol[1] == 0 && bgm_state()->layered, "z21 sonic layered %s %d/%d", g_ah.music, g_ah.mvol[0], g_ah.mvol[1]);
    bgm_stage_start(1, 1, 1, &p);
    CHECK(ends(g_ah.music, "z2b_02_03.lyr") && g_ah.mvol[0] == 0 && g_ah.mvol[1] == 127, "z22 blaze: B = SEQ_2blaze = layer 1 (%s %d/%d)", g_ah.music, g_ah.mvol[0], g_ah.mvol[1]);
    bgm_stage_start(6, 0, 0, &p); CHECK(ends(g_ah.music, "z7s_12_13.lyr"), "z71 %s", g_ah.music);
}

static void test_layers(void) {
    DcPlayer p;
    memset(&p, 0, sizeof p);
    bgm_stage_start(1, 0, 0, &p);
    Stage_SelectBGMLayer(1);
    CHECK(g_ah.mvol[0] == 0 && g_ah.mvol[1] == 127 && g_ah.mframes[0] == 0 && g_ah.mframes[1] == 0, "underwater: %d/%d", g_ah.mvol[0], g_ah.mvol[1]);
    CHECK(bgm_obj_effect(&p, OBJ_FX_BGM_LAYER, 0) == 1 && g_ah.mvol[0] == 127 && g_ah.mvol[1] == 0, "surface %d/%d", g_ah.mvol[0], g_ah.mvol[1]);
    /* boss acts ignore it (gGameState.unk1D >= 2) */
    bgm_stage_start(1, 2, 0, &p);
    audio_host_reset_log();
    Stage_SelectBGMLayer(1);
    CHECK(!strstr(g_ah.log, "layer"), "boss layer select moved volumes: %s", g_ah.log);
}

static void test_invincibility(void) {
    DcPlayer p;
    memset(&p, 0, sizeof p);
    bgm_stage_start(0, 0, 0, &p);
    frames(10);
    Player_GiveInvincibility(&p);
    CHECK(p.invinc == 0x3e7 && g_ah.j[0].on && strstr(g_ah.j[0].path, "_22.wav"), "muteki: invinc %d jingle %s", p.invinc, g_ah.j[0].path);
    CHECK(g_ah.mvol[0] == 0 && g_ah.mvol[1] == 0 && g_ah.music_on, "BGM muted under the jingle, still playing: %d/%d", g_ah.mvol[0], g_ah.mvol[1]);
    CHECK(bgm_state()->jingle_state == 0x16, "state %#x", bgm_state()->jingle_state);
    p.invinc = 0x200; frames(3);
    CHECK(g_ah.j[0].on, "still invincible");
    p.invinc = 0x5A; frames(1);
    CHECK(!g_ah.j[0].on && g_ah.j[0].stop_frames == 0x5A && g_ah.mvol[0] == 126 && g_ah.mframes[0] == 0x5A && g_ah.mvol[1] == 0,
          "end: jingle fade %d, BGM %d over %d", g_ah.j[0].stop_frames, g_ah.mvol[0], g_ah.mframes[0]);
    CHECK(bgm_state()->jingle_state == 0, "task destroyed");
    /* invincibility cut short (invinc jumps to 0) */
    Player_GiveInvincibility(&p);
    p.invinc = 0; frames(1);
    CHECK(!g_ah.j[0].on && g_ah.j[0].stop_frames == 0 && g_ah.mvol[0] == 126 && g_ah.mframes[0] == 0, "cut: %d %d", g_ah.j[0].stop_frames, g_ah.mframes[0]);
    /* the item hook (ItemBox 5) */
    sound_hooks_install();
    g_obj_hooks.item(NULL, &p, OBJ_ITEM_INVINCIBLE);
    CHECK(p.invinc == 0x3e7 && g_ah.j[0].on, "item hook");
    p.invinc = 0; frames(1);
}

static void test_drowning(void) {
    DcPlayer p;
    memset(&p, 0, sizeof p);
    bgm_stage_start(1, 0, 0, &p);
    p.pf |= 0x4000000; Stage_SelectBGMLayer(1); p.unk3CA = 100;
    bgm_obj_effect(&p, OBJ_FX_JINGLE, 0x17);
    CHECK(g_ah.j[0].on && strstr(g_ah.j[0].path, "_23.wav") && g_ah.mvol[0] == 0 && g_ah.mvol[1] == 0, "brethless %s %d/%d", g_ah.j[0].path, g_ah.mvol[0], g_ah.mvol[1]);
    frames(2);
    bgm_pause_players(1);
    CHECK(g_ah.j[0].paused, "paused (player 9 / frozen)");
    frames(5);
    CHECK(g_ah.j[0].paused && g_ah.j[0].on, "stays paused");
    bgm_pause_players(0); frames(1);
    CHECK(!g_ah.j[0].paused, "resumed");
    p.unk3CA = 0; p.pf &= ~0x4000000u;   /* surfaced: Player_UpdateUnderwater cleared unk3CA */
    Stage_SelectBGMLayer(0);             /* (invinc 0) */
    frames(1);
    CHECK(!g_ah.j[0].on && g_ah.mvol[0] == 126 && g_ah.mframes[0] == 0x3C, "surface: BGM %d over %d", g_ah.mvol[0], g_ah.mframes[0]);
    /* surfacing while the uw flag is still set restores layer A */
    p.pf |= 0x4000000; p.unk3CA = 50; Stage_SelectBGMLayer(1);
    StageJingle_Create(&p, 0x17);
    p.unk3CA = 0; frames(1);
    CHECK(g_ah.mvol[1] == 126 && g_ah.mframes[1] == 0x3C && g_ah.mvol[0] == 0, "uw layer back %d/%d", g_ah.mvol[0], g_ah.mvol[1]);
}

static void test_oneup(void) {
    DcPlayer p;
    memset(&p, 0, sizeof p);
    g_dcp_world.mode = 0;
    bgm_stage_start(0, 0, 0, &p);
    sound_hooks_install();
    g_dcp_hooks.add_life(NULL, &p, 1);
    CHECK(g_ah.j[1].on && strstr(g_ah.j[1].path, "_24.wav") && g_ah.music_on && g_ah.mvol[0] == 127, "1up over the BGM: %s", g_ah.j[1].path);
    bgm_pause_players(1);
    CHECK(g_ah.j[1].paused, "1up on player 3: paused by the pause menu");
    bgm_pause_players(0);
    /* boss act: Sound_PlayBGMOnPlayer3 stops the boss music; zone boss SDATs have no 1up */
    bgm_stage_start(0, 2, 0, &p);
    CHECK(g_ah.music_on, "boss music");
    g_dcp_hooks.add_life(NULL, &p, 1);
    CHECK(!g_ah.music_on && !g_ah.j[1].on, "boss act 1up: music stopped (DS), no jingle");
    /* time attack: no 1-up */
    g_dcp_world.mode = 2;
    bgm_stage_start(0, 0, 0, &p);
    g_ah.j[1].on = 0;
    g_dcp_hooks.add_life(NULL, &p, 1);
    CHECK(!g_ah.j[1].on, "time attack: no 1up jingle");
    g_dcp_world.mode = 0;
}

static void test_boss_and_ui(void) {
    DcPlayer p;
    memset(&p, 0, sizeof p);
    bgm_stage_start(0, 2, 0, &p);
    bgm_boss_music_hook(NULL, 0xf);
    CHECK(ends(g_ah.music, "z1es_15.wav"), "pinch %s", g_ah.music);
    bgm_stage_start(6, 2, 0, &p);
    bgm_boss_music_hook(NULL, 0x11);
    CHECK(ends(g_ah.music, "z7eb_17.wav") || ends(g_ah.music, "z7es_17.wav"), "boss7 clash %s", g_ah.music);
    bgm_stage_start(7, 2, 0, &p);
    bgm_boss_music_hook(NULL, 0x13);
    CHECK(ends(g_ah.music, "zfeb_19.wav") || ends(g_ah.music, "zfes_19.wav"), "final talk %s", g_ah.music);
    bgm_set_context("ze");
    bgm_boss_music_hook(NULL, 0x14); CHECK(ends(g_ah.music, "ze_20.wav"), "extra %s", g_ah.music);
    bgm_boss_music_hook(NULL, 0x15); CHECK(ends(g_ah.music, "ze_21.wav"), "extra pinch %s", g_ah.music);
    bgm_boss_music_volume_hook(NULL, 0x20, 0xb4);
    CHECK(g_ah.music_on && g_ah.mvol[0] == 0x20 && g_ah.mframes[0] == 0xb4, "Boss7 defeat: BGM to 0x20 over 0xb4 (%d / %d)", g_ah.mvol[0], g_ah.mframes[0]);
    bgm_boss_music_fade_hook(NULL, 0x3c);
    CHECK(!g_ah.music_on && g_ah.music_stop_frames == 0x3c, "ClashFadeOutBGM: fade-stop over 0x3c (%d)", g_ah.music_stop_frames);
    bgm_boss_music_hook(NULL, 0x15);
    bgm_boss_music_stop_hook(NULL);
    CHECK(!g_ah.music_on, "boss music stop");
    /* results: StopSoundAll, then the clear music + the ClearDemo bank */
    bgm_stage_start(0, 0, 0, &p);
    bgm_ui_music_hook(NULL, -1);
    CHECK(!g_ah.music_on, "stop all");
    bgm_ui_music_hook(NULL, 0xf);
    CHECK(ends(g_ah.music, "sys_11.wav") && !strcmp(bgm_context(), "sys"), "SEQ_ACT_CLEAR = SEQ_TA_ACT_CLEAR render %s", g_ah.music);
    CHECK(!strcmp(sound_bank_name(), "sys_a06"), "ClearDemo bank %s", sound_bank_name());
    bgm_ui_sfx_hook(NULL, 6, 3);
    CHECK(gSESoundHandle.player != NULL, "ClearDemo SE 6:3 starts");
    bgm_ui_music_hook(NULL, 0xc); CHECK(ends(g_ah.music, "sys_12.wav"), "new record %s", g_ah.music);
    bgm_ui_music_hook(NULL, 0x10); CHECK(ends(g_ah.music, "sys_13.wav"), "boss clear %s", g_ah.music);
    bgm_ui_music_hook(NULL, 0x11); CHECK(ends(g_ah.music, "sys_17.wav") && !strcmp(sound_bank_name(), "sys_a07"), "final clear %s", g_ah.music);
    bgm_ui_music_hook(NULL, 0xb); CHECK(ends(g_ah.music, "sys_11.wav") && !strcmp(sound_bank_name(), "sys_a05"), "TA clear %s", g_ah.music);
    bgm_ui_music_hook(NULL, 0x14); CHECK(ends(g_ah.music, "sys_20.wav"), "game over %s", g_ah.music);
    bgm_special_music(0x1e); CHECK(ends(g_ah.music, "sys_30.wav"), "special %s", g_ah.music);
    bgm_special_music_stop(0x20); CHECK(!g_ah.music_on && g_ah.music_stop_frames == 0x20, "special stop fade %d", g_ah.music_stop_frames);
    sound_load_bank("z1s");
}

static void test_restart_and_gravity(void) {
    DcPlayer p;
    memset(&p, 0, sizeof p);
    g_dcp_world.mode = 0;
    bgm_stage_start(6, 0, 0, &p);
    sound_load_bank("z7s");
    sound_hooks_install();
    Player_GiveInvincibility(&p);
    uint32_t g = g_ah.gen[0];
    g_dcp_hooks.restart(NULL, &p);
    CHECK(g_ah.gen[0] != g && g_ah.music_on && g_ah.mvol[0] == 127 && !g_ah.j[0].on && bgm_state()->jingle_state == 0, "restart: music from the top, jingle gone");
    /* gravity flip (Dead Line act): layer 1 + SE 0x66 on the voice handle */
    g_obj_stage.check_time = 0; g_dcp_world.frame = 100; p.invinc = 0;
    p.grav_rot = 0x80; frames(1);
    CHECK(g_ah.mvol[0] == 0 && g_ah.mvol[1] == 127 && bgm_state()->layer_sel == 1, "flip -> layer 1 (%d/%d)", g_ah.mvol[0], g_ah.mvol[1]);
    p.grav_rot = 0; frames(1);
    CHECK(g_ah.mvol[0] == 127 && g_ah.mvol[1] == 0, "flip back -> layer 0");
    p.invinc = 10; p.grav_rot = 0x80; frames(1);
    CHECK(g_ah.mvol[0] == 127, "invincible: no layer change");
    p.invinc = 0; p.grav_rot = 0; frames(1);
    /* restart in VS: no music restart */
    g_dcp_world.mode = 1; g = g_ah.gen[0];
    g_dcp_hooks.restart(NULL, &p);
    CHECK(g_ah.gen[0] == g, "VS respawn: no restart");
    g_dcp_world.mode = 0;
}

/* trace build: requests that produce no sound are counted (snd MISS lines) */
static void test_miss_trace(void) {
    sound_load_bank("z1s");
    sound_trace_enable(1);
    uint32_t m0 = g_sound_miss_count;
    Sound_PlaySeqArcEx(NULL, 0, 0x2c);    /* SE_Nukari: a Blaze voice, silent in the Sonic SDAT z1s */
    frames(240);
    CHECK(g_sound_miss_count == m0 + 1, "silent id counted (%u -> %u)", m0, g_sound_miss_count);
    m0 = g_sound_miss_count;
    Sound_PlaySeqArcEx(NULL, 0, 0x22);    /* SE_Go: plays */
    frames(240);
    CHECK(g_sound_miss_count == m0, "playing id not counted");
    Sound_PlaySeqArcEx(NULL, 5, 0);       /* no SSAR 5 in a zone SDAT: start fails */
    CHECK(g_sound_miss_count == m0 + 1, "start failure counted");
    sound_trace_enable(0);
}

static void test_pause_se(void) {
    sound_load_bank("z1s");
    Sound_PlaySeqArcEx(NULL, 0, 0x12);   /* an effect on players 3..7 */
    frames(2);
    int pl = gSESoundHandle.player ? gSESoundHandle.player->playerNo : -1;
    bgm_pause_players(1);
    frames(3);
    CHECK(pl < 0 || (gSESoundHandle.player && gSESoundHandle.player->unk2e == 1), "SE paused");
    bgm_pause_players(0);
    CHECK(pl < 0 || !gSESoundHandle.player || gSESoundHandle.player->unk2e == 0, "SE resumed");
}

int main(int argc, char **argv) {
    const char *bank = argc > 1 ? argv[1] : "build/dreamcast/audio/sfxbank";
    snprintf(s_mdir, sizeof s_mdir, "%s", argc > 2 ? argv[2] : "build/dreamcast/audio/disc_music");
    sound_set_dir(bank);
    sound_init();
    sound_load_bank("z1s");
    bgm_set_dir(s_mdir);
    CHECK(bgm_init() == 0, "streams.txt in %s", s_mdir);
    test_stage_table();
    test_layers();
    test_invincibility();
    test_drowning();
    test_oneup();
    test_boss_and_ui();
    test_restart_and_gravity();
    test_pause_se();
    test_miss_trace();
    printf("bgm: plays %u, sequences without a stream %u\n", bgm_state()->plays, bgm_state()->misses);
    printf("%s (%d failures)\n", fails ? "FAILED" : "OK", fails);
    return fails != 0;
}
