/*
 * Player model presentation (Dreamcast port).
 *
 * Translates the model half of the player's draw path:
 *  - Player_SetAnimation (asm_fallback/Player_SetAnimation.c), model part:
 *    the four ov000 tables at 0x02368bdc (joint animation), 0x02368c44
 *    (model resource: 0 = body, 1 = ball), 0x02368cac (visibility animation,
 *    0 = keep) and the Blaze offset (+0x1b for ids >= 0x30);
 *  - Model_SetResource / Model_SetAnimation (slot 0 joint, slot 4 visibility,
 *    the 0x40 cross-fade request) and Model_AnimateMdl (fx32 frame += speed,
 *    loop / clamp, the "ended" bit, blend ratio);
 *  - StageObj_Draw3DEx for the player (flags 0x5C: facing bit 0, loop bit 4,
 *    ANIM_DONE bit 8 from the joint animation, 0x10 frozen, 0x20 hidden), the
 *    rotation RotY(facing) * RotX * RotY * RotZ(-(angle + rot) << 8), the
 *    shake offset, and the model's own matrix ops set by Player_Create
 *    (cb = FlushVP, Identity, RotateTranslate2Scale: scale 0x34cc and the
 *    model-space translation (0, -16, 0) at @0x18C);
 *  - Player_Draw: the high-speed / homing after-image.
 * See docs/dreamcast/translation/PLAYER.md section 8.
 */
#ifndef DCP_MODEL_H
#define DCP_MODEL_H

#include "dcp_player.h"
#include "../model/model.h"

#define DCP_ANIMTAB_N 0x68

typedef struct DcpModel {
    int ok;
    int chara;
    dcm_model_t res[2];         /* Model_SetResource index: 0 body (son/blz), 1 ball (son_ball/blz_spin) */
    void *res_data[2];
    dca_set_t jnt, vis;         /* son.nsbca / son.nsbva (shared by both characters) */
    void *jnt_data, *vis_data;
    u8 tab_jnt[DCP_ANIMTAB_N];  /* ov000 0x02368bdc */
    u8 tab_res[DCP_ANIMTAB_N];  /* ov000 0x02368c44 */
    u8 tab_vis[DCP_ANIMTAB_N];  /* ov000 0x02368cac */

    /* model @0x124 animation state */
    const DcPlayer *p;
    int cur_res;
    int jnt_anim;               /* anm[0] */
    s32 jnt_frame;              /* fx32 */
    int vis_anim;               /* anm[4], -1 = none attached */
    s32 vis_frame;
    int blend_anim;             /* anm2[0] (frozen frame), -1 = none */
    s32 blend_frame;
    s32 ratio;                  /* anm[0]->ratio while blending (fx32) */
    u8 parts0, parts4;          /* flags[0], flags[4] */
    s32 speed;                  /* @0x114 */
    s32 blend_speed;            /* @0x12C */
    s32 slot_speed;             /* @0x118[] per-slot speed (StageTitleCardPlayer: unk24C) */
    u32 set_count;              /* Player_SetAnimation calls seen (tests) */
    u32 done_count;             /* frames that raised ANIM_DONE (tests) */

    /* pose of this frame (dcp_model_step) */
    int posed;
    dcm_local_t loc[DCM_MAX_JOINTS];
    dcm_local_t loc2[DCM_MAX_JOINTS];
    dcm_mtx_t pal[DCM_MAX_PALETTE];
    u8 visbits[DCM_MAX_JOINTS];
} DcpModel;

extern DcpModel g_dcp_model;

/* Load the character's body + ball models, son.dca / son.dcv and the animation
 * tables from `dir` (stdio paths, e.g. "/cd/player").  upload = 1 puts the
 * textures in PVR memory.  Returns 0 when the body model and the joint set loaded. */
int dcp_model_load(DcpModel *m, int chara, const char *dir, int upload);
void dcp_model_free(DcpModel *m);

/* Follow this player: chains the anim / anim_speed hooks (call once before
 * objects_init, which chains its own hooks after these) and starts from p->anim. */
void dcp_model_attach(DcpModel *m, const DcPlayer *p);

/* Player_SetAnimation model part (called from the hook; exposed for tests). */
void dcp_model_set_anim(DcpModel *m, DcPlayer *p, s32 anim);
/* Player_SetAnimSpeedFromVelocity model part: speed 0x10..0x80 -> @0x114 = s << 8 */
void dcp_model_set_speed(DcpModel *m, s32 s);

/* Model speeds[] of every slot (fx32, 0x1000 = 1.0): the title card's slow-motion close-up. */
void dcp_model_set_slot_speed(DcpModel *m, s32 fx);

/* StageTitleCardPlayer_Draw: camera (LookAt eye/at, perspective or, when zoom, the off-axis
 * frustum of StageObj_Draw3DEx flag 0x200 around the model translation mx/my (20.12)) and the
 * model matrix (facing, rotX/rotY/rotZ without the ground angle). The projection is scaled so
 * every vertex's PVR depth (1/w) falls in (z0, z1): the close-up sorts between two 2D layers. */
void dcp_model_titlecard_view(const DcPlayer *p, const s32 eye[3], const s32 at[3], s32 mx, s32 my, int zoom,
                              float z0, float z1, dcm_camera_t *cam, dcm_mtx_t *mdl);

/* Once per game frame after the player update (StageObj_Main draw step):
 * advances the animation (unless hit-stop / paused), writes DCP_DF_ANIM_DONE
 * and the blend bit into p->disp_flags like StageObj_Draw3DEx, and poses. */
void dcp_model_step(DcpModel *m, DcPlayer *p, int paused);

/* Model matrix of StageObj_Draw3DEx + Model_Draw for a translation already in
 * model/camera space (stage: tx, ty after dcm_camera_stage_offaxis; arenas: world). */
void dcp_model_matrix(const DcPlayer *p, float tx, float ty, float tz, dcm_mtx_t *out);
/* The model's own scale and (0,-16,0) translation in front of an external
 * rotation/translation (boss arenas that build the player transform themselves). */
void dcp_model_matrix_ext(const dcm_mtx_t *ext, dcm_mtx_t *out);
/* Stage lights for the player (flag 0x800: x mirrored by facing), tinted when
 * ghost != 0 (Player_Draw after-image colour) or underwater (pf 0x4000000). */
/* the stage light colours data_02074960[zone][act][0..2] (BGR555; lights.bin, else the common
 * 0x7fff / 0x4631 / 0x2108) */
const u16 *dcp_model_stage_lights(int zone, int act);
void dcp_model_lights(const DcPlayer *p, int ghost, dcm_lights_t *out);

/* Player_Draw: 1 and the DS position (1/256 px) of the after-image when one is drawn this frame. */
int dcp_model_afterimage(const DcPlayer *p, s32 *x, s32 *y, s32 *z);

/* Submit the posed model (current resource). Returns triangles sent; 0 when hidden (flag 0x20). */
/* the ModelPalette_ShiftColor of Player_In for this frame (tension max glow, invincibility flash),
 * 5-bit units; dcp_model_draw applies it with dcm_set_color_shift */
void dcp_model_palette_shift(const DcPlayer *p, int out[3]);
int dcp_model_draw(const DcpModel *m, const DcPlayer *p, int list, const dcm_mtx_t *mdl, const dcm_camera_t *cam,
                   const dcm_lights_t *lights);

/* Current joint animation index into son.dca and the model resource (tests / logs). */
int dcp_model_cur_anim(const DcpModel *m);

#endif
