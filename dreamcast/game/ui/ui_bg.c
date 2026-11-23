/*
 * The title card and act results backgrounds (BBG), which the DS draws on BG layers:
 *
 * StageTitleCard_InitBg / _InitBgBoss + StageTitleCardBg_Draw:
 *   main BG2  stdm_zigzag (512x512 affine; boss: stdm_bs_zigzag) over the stage,
 *             scrolled by fn78 (y +1 px per frame, x 0 -> 128 px on exit);
 *   sub BG3   stdm_bg<zone> (the zone art; boss: stdm_bs_bg<zone>), static;
 *   sub BG2   the same zigzag, same scroll, behind BG3.
 *   The 16 colours at data_02076b48[character] of the zone art's palette are copied
 *   to both (tools/uibg.py bakes them per character).  Boss: windows 0x38..0xc0
 *   show the zigzag only inside that column.
 * ClearDemo_InitBg / _InitBgBoss + ClearDemoBg_*:
 *   main BG1  cldm_far (static), BG2 cldm_bg (affine, y = unk8 >> 12), BG3 cldm_bg
 *   (text, y = unk4 >> 12); window 0 (x 0..unk10, y 33..35) shows BG0/BG1 inside,
 *   BG2/BG3 outside, until ClearDemoBg_State_BeginSlide turns it off.
 *
 * One frame on the Dreamcast (UI.md 4.3): the title card frame is the DS main screen
 * with the sub screen's zone art composed in, in the sub screen's own layer order:
 * stage < zigzag < zone art < title card sprites, all at 2.5x (256x192 -> 640x480).
 * The zone art takes the sub screen's master brightness (UiQuad.screen = 1).
 * The results frame is the main screen: cldm_bg < cldm_far < tally sprites.
 * BG planes are drawn as clipped quads of the converted pieces (the zigzag edge band
 * and its 1-px repeating fill, the cldm_bg 8x8 tile).
 */
#include <stdio.h>
#include "ui_internal.h"
#ifdef _arch_dreamcast
#include "../engine/pak.h"   /* pak_prefetch */
#endif

/* ------------------------------------------------------------ title card */
static int s_tc_file = -1, s_tc_boss;

/* StageTitleCard_Create: act_com file zone + 6 (normal) / zone + 0x10 (boss) */
void ui_tcbg_load(void) {
    char dir[128], id[32];
    int boss = !UI_NORMAL_STAGE(), file = (boss ? 0x10 : 6) + W.zone;
    if (W.zone > 8)
        file = boss ? 0x18 : 0xe;
    s_tc_boss = boss;
    if (file == s_tc_file && g_ui_res[UI_RES_TCBG].blob)
        return;
    snprintf(dir, sizeof(dir), "%s/res", g_ui_dir);
    snprintf(id, sizeof(id), "tcbg_%d", file);
    ui_res_load_dir(UI_RES_TCBG, dir, id);
    s_tc_file = file;
}

void ui_tcbg_free(void) {
    ui_res_free(UI_RES_TCBG);
    s_tc_file = -1;
}

/* the stage's own UI resources (title card and results backgrounds, navigator, the
 * face texture), freed when the stage is torn down: only ui_init's resources stay */
void ui_stage_end(void) {
    StageTitleCard_Destroy();
    ClearDemo_Destroy();
    ui_cdbg_free();
    ui_navi_stage_end();
    ui_face_release();
}

static float fmodp(float v, float m) {
    float r = v - (float)((int)(v / m)) * m;
    return r < 0 ? r + m : r;
}

/* StageTitleCardBg_Draw (scroll from fn78: unk4 = x, unk8 = y, 20.12) */
void ui_tcbg_draw(s32 scroll_x, s32 scroll_y) {
    const UiRes *r = &g_ui_res[UI_RES_TCBG];
    int chara = G.character == 1 ? 1 : 0, zz = 1 + chara;
    const UiImage *pz;
    float sx = (float)(scroll_x >> 12), sy = (float)(scroll_y >> 12), ph, x0, y;
    if (!r->blob || r->n_images < 3)
        return;
    pz = &r->images[zz];
    ph = pz->h;
    /* sub BG3: the zone art, in front of the zigzag */
    g_ui_screen = 1;
    ui_emit_image(UI_RES_TCBG, 0, UI_SP_SCREEN, 0, 0, 0, 0, 256, 192, 0xffffffff);
    g_ui_screen = 0;
    /* BG2: the zigzag plane; screen (0, 0) shows plane (sx, sy); BGxCNT 0x2000: wraps */
    if (s_tc_boss) {
        /* stdm_bs_zigzag: a band at plane x 0..63 of 256, inside window 0 (x 0x38..0xc0) */
        x0 = -fmodp(sx, 256.0f);
        for (y = -fmodp(sy, ph); y < 192.0f; y += ph) {
            float x;
            for (x = x0; x < 256.0f; x += 256.0f)
                ui_emit_image(UI_RES_TCBG, zz, UI_SP_SCREEN, x, y, 0x38, 0, 0xc0, 192.0f, 0xffffffff);
        }
        return;
    }
    /* stdm_zigzag: the edge band (plane x 0..63) and a fill whose columns are all
     * equal (tools/uibg.py checks it): one column stretched to the plane's end (512) */
    x0 = -fmodp(sx, 512.0f);
    for (y = -fmodp(sy, ph); y < 192.0f; y += ph) {
        float x;
        for (x = x0; x < 256.0f; x += 512.0f) {
            ui_emit_image_part(UI_RES_TCBG, zz, UI_SP_SCREEN, x, y, 64, ph, 0, 0, 64, ph, 0, 0, 256, 192, 0xffffffff);
            ui_emit_image_part(UI_RES_TCBG, zz, UI_SP_SCREEN, x + 64, y, 448, ph, (float)pz->w - 1.5f, 0, 1, ph, 0,
                               0, 256, 192, 0xffffffff);
        }
    }
}

/* ---------------------------------------------------------------- results */
void ui_cdbg_load(void) {
    char dir[128], id[32];
    snprintf(dir, sizeof(dir), "%s/res", g_ui_dir);
    snprintf(id, sizeof(id), "cldm_%d", G.character == 1 ? 1 : 0);
    ui_res_load_dir(UI_RES_CLDMBG, dir, id);
}
void ui_cdbg_free(void) { ui_res_free(UI_RES_CLDMBG); }
/* DC: the results BG files read in the stage load (pak_prefetch), so ui_cdbg_load at the goal
 * reads nothing from the disc (it held the goal frame for ~15 ms in Flycast) */
void ui_cdbg_prefetch(void) {
#ifdef _arch_dreamcast
    static const char *const k_files[] = { "frames.bin", "palettes.bin", "atlas0.pt4", "atlas1.pt4" };
    char path[200];
    unsigned i;
    for (i = 0; i < sizeof(k_files) / sizeof(k_files[0]); i++) {
        snprintf(path, sizeof(path), "%s/res/cldm_%d/%s", g_ui_dir, G.character == 1 ? 1 : 0, k_files[i]);
        pak_prefetch(path);
    }
#endif
}

void ui_cdbg_draw(s32 unk4, s32 unk8, int win_on, int win_x2) {
    const UiRes *r = &g_ui_res[UI_RES_CLDMBG];
    const UiImage *pt;
    float y2;
    if (!r->blob || r->n_images < 2)
        return;
    pt = &r->images[1];
    /* BG1 cldm_far: inside window 0 while it is on, everywhere after */
    if (win_on)
        ui_emit_image(UI_RES_CLDMBG, 0, UI_SP_SCREEN, 0, 0, 0, 33, (float)win_x2, 35, 0xffffffff);
    else
        ui_emit_image(UI_RES_CLDMBG, 0, UI_SP_SCREEN, 0, 0, 0, 0, 256, 192, 0xffffffff);
    /* BG2 (affine, no wrap: its 256x256 plane starts at screen y = -(unk8 >> 12)) and
     * BG3 (text, wraps, y = unk4 >> 12): the cldm_bg plane is one tile repeated
     * (tools/uibg.py: period 1x1, a uniform colour), so each is one stretched quad */
    (void)unk4;
    y2 = (float)(-(unk8 >> 12));
    ui_emit_image_part(UI_RES_CLDMBG, 1, UI_SP_SCREEN, 0, 0, 256, 192, pt->w * 0.5f, pt->h * 0.5f, 0, 0, 0, 0, 256,
                       192, 0xffffffff);
    ui_emit_image_part(UI_RES_CLDMBG, 1, UI_SP_SCREEN, 0, y2, 256, 256, pt->w * 0.5f, pt->h * 0.5f, 0, 0, 0, 0, 256,
                       192, 0xffffffff);
}
