/* KOS link test of the front end (make link). Not part of the game build. */
#include <kos.h>
#include "menus/menus.h"
#include "save/save.h"
#include "engine/render.h"
int main(void) {
    InputState in;
    int i;
    pvr_init_defaults();
    save_vmu_install();
    menus_set_paths("/cd/menus", "/cd/sprites");
    menus_init();
    memset(&in, 0, sizeof(in));
    for (i = 0; i < 600; i++) {
        MenuRequest r;
        input_poll(&in);
        r = menus_run_frame(&in);
        render_begin_frame();
        render_begin_list(PVR_LIST_TR_POLY);
        menus_draw_tr();
        render_end_list();
        render_end_frame();
        if (r.kind == MENU_REQ_STAGE)
            r = menus_stage_exit(MENU_EXIT_CLEARED, 0, 3, 0, 0);
    }
    return 0;
}
