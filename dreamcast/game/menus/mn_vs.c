/*
 * Battle Play (local two-controller VS) front end: the menus' side of the mode.
 * Interface for main.c: menus_set_pad2, MENU_REQ_VS_STAGE, menus_vs_exit (menus.h).
 */
#include "mn_internal.h"

static InputState s_pad2;
void menus_set_pad2(const InputState *in2) { if (in2) s_pad2 = *in2; }

MenuRequest menus_vs_exit(const VsRaceResult *res) {
    MenuRequest r;
    (void)res;
    memset(&r, 0, sizeof r);
    r.kind = MENU_REQ_NONE;
    return r;
}
