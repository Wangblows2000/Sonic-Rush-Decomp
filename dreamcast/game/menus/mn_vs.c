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

void menus_set_pad2(const InputState *in2) {
    if (in2)
        s_pad2 = *in2;
    else
        memset(&s_pad2, 0, sizeof(s_pad2));
}
void mn_vs_pad_update(void) { Pad_Update(&gPad2, mn_pad_from_input(&s_pad2)); }

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
