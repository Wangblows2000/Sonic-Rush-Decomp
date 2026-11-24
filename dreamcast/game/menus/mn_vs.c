/*
 * Battle Play (local two-controller VS) front end: the menus' side of the mode.
 * Interface for main.c: menus_set_pad2, MENU_REQ_VS_STAGE, menus_vs_exit (menus.h).
 *
 * The DS mode is two units over wireless: each runs its own PlayerSelect (host mode 1 /
 * guest mode 2), TimeSelect (chooser / follower) and ResultsScreen, and they exchange a
 * small record every frame (WirelessSession_GetSendBuffer / GetPartnerRecvBuffer). Here
 * both players are on one Dreamcast: player 1 (the pad of menus_run_frame) is the host,
 * player 2 (menus_set_pad2, controller port B) the guest, and the two records are two
 * arrays in this file, so the original partner logic runs unchanged (the partner's record
 * is the other local player's, seen in the same frame).
 *
 * Vs CPU (native, MENUS.md 15): the same screens with player 2 played by the computer: its pad is
 * ignored, its character select and ready are automatic, its name is "CPU" (MenuRequest.cpu = 1).
 *
 * Flow (MENUS.md 15):
 *   TitleMenu VS page -> TitleMenu_StartVsHost -> PlayerSelectMenu_StartVs (both players)
 *   -> TimeSelectMenu_StartMode0 (player 1 picks the stage) -> Game_StartVsStage
 *   -> MENU_REQ_VS_STAGE -> race (main.c) -> menus_vs_exit -> ResultsScreen
 *   -> rematch (MENU_REQ_VS_STAGE again) / character select / title screen.
 */
#include "mn_internal.h"

static InputState s_pad2;
static u8 s_vs_rec[2][0x40];   /* the two players' wireless records */
static u16 s_p2_name[16];
static int s_p2_len;
static int s_vs_cpu;           /* Vs CPU: player 2 is the computer (menus_vs_set_cpu) */

void menus_set_pad2(const InputState *in2) {
    if (in2)
        s_pad2 = *in2;
    else
        memset(&s_pad2, 0, sizeof(s_pad2));
}
/* Vs CPU: player 2's controller is not read (gPad2 stays released; the CPU's presses are made by the
 * select, PlayerSelectMenu_CpuInput, and TimeSelect's ready, TimeSelectMenu_LocalGuest_Main) */
void mn_vs_pad_update(void) { Pad_Update(&gPad2, s_vs_cpu ? 0 : mn_pad_from_input(&s_pad2)); }

/* Vs CPU (native): the TitleMenu's ONE PLAYER button (TitleMenu_SelectVsMode), main.c's debug start
 * (before menus_vs_prepare); kept by TRY AGAIN and CHANGE CHARACTER/ACT, reset by TWO PLAYERS */
void menus_vs_set_cpu(int on) { s_vs_cpu = on ? 1 : 0; }
int mn_vs_cpu(void) { return s_vs_cpu; }
int mn_vs_cpu_record(const u16 **name, const u8 **mac) {
    static const u16 cpu[3] = { 'C', 'P', 'U' };
    static const u8 nomac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    *name = cpu;
    *mac = nomac;
    return 3;
}

void menus_set_player2_name(const uint16_t *utf16, int len) {
    if (len < 0)
        len = 0;
    if (len > 16)
        len = 16;
    s_p2_len = len;
    if (len)
        memcpy(s_p2_name, utf16, (size_t)len * 2);
}
int mn_player2_name(const u16 **name) {
    *name = s_p2_name;
    return s_p2_len;
}

/* an unnamed player (a fresh VMU has no owner name; player 2 has none unless set) is shown as
 * "PLAYER 1" / "PLAYER 2" in the name-plate font; Vs CPU: player 2 is "CPU" */
int mn_vs_player_name(int p, const u16 **name) {
    static const u16 id[2][8] = { { 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '1' }, { 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '2' } };
    int len;
    if (p && s_vs_cpu) {
        const u8 *mac;
        return -mn_vs_cpu_record(name, &mac);
    }
    len = p ? mn_player2_name(name) : mn_owner_name(name);
    if (len > 0)
        return len;
    *name = id[p ? 1 : 0];
    return -8;
}

/* A race started without the select screens (main.c's debug start): the state the character
 * select (PlayerSelectMenu_ExitToGame: unk4 / unk8) and TimeSelect (Game_StartVsStage: unk10,
 * zone / act) leave, so the results, TRY AGAIN and CHANGE work as after them. */
void menus_vs_prepare(int zone, int act, int character, int character2) {
    character = character ? 1 : 0;
    character2 = character2 ? 1 : 0;
    if (character2 == character)   /* the original select never gives both players one character */
        character2 = character ^ 1;
    gGameState.unk0 = character;   /* the host's own character */
    gGameState.unk4 = character;
    gGameState.unk8 = character2;
    gGameState.unk10 = MENU_MODE_VS;
    gGameState.unk18 |= 0x20u;     /* TitleMenu_StartVsHost */
    gGameState.unk18 &= ~0x10u;
    gGameState.unk1C = (u8)(zone < 0 ? 0 : zone > 6 ? 6 : zone);
    gGameState.unk1D = (u8)(act ? 1 : 0);
    gGameState.vs_winner = 2;
    mn_vs_reset_records();
}

u8 *WirelessSession_GetSendBuffer(s32 player) { return s_vs_rec[player & 1]; }
u8 *WirelessSession_GetPartnerRecvBuffer(s32 player) { return s_vs_rec[(player & 1) ^ 1]; }
void mn_vs_reset_records(void) { memset(s_vs_rec, 0, sizeof(s_vs_rec)); }

/* Game_StartVsStage(zone, act): gGameState.unk10 = 1 (VS), zone / act, Wireless_EnableErrorCallback,
 * SceneChange_ToVsStage */
void Game_StartVsStage(s32 zone, s32 act) {
    gGameState.unk10 = MENU_MODE_VS;
    gGameState.unk1C = (u8)zone;
    gGameState.unk1D = (u8)act;
    mn_request_vs_stage();
}

/* back from a race: data_022c4590.unk40 (1 = the host won, 2 = the guest won, else a draw) and the
 * ResultsScreen (SceneChange_ToVsResults) */
MenuRequest menus_vs_exit(const VsRaceResult *res) {
    memset(&g_mn_request, 0, sizeof(g_mn_request));
    Task_DestroyAll();
    mn_display_reset();
    mn_bgm_forget();   /* the race ran its own music */
    gGameState.vs_winner = res ? res->winner : 2;
    if (gGameState.vs_winner < 0 || gGameState.vs_winner > 2)
        gGameState.vs_winner = 2;
    ResultsScreen_Create();
    return g_mn_request;
}
