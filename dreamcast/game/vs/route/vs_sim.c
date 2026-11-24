/* Host Battle Play simulation, see vs_sim.h. The frame order follows main.c run_vs_stage. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vs_sim.h"
#include "../../camera/camera.h"

VsSim g_sim;
float g_cam_x, g_cam_y;   /* engine/render.c (not linked) */
static u8 *s_level;
static unsigned char s_cam[2][4096];
static u16 s_prev[2];

static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END); *size = ftell(f); fseek(f, 0, SEEK_SET);
    b = malloc((size_t)*size);
    if (fread(b, 1, (size_t)*size, f) != (size_t)*size) { free(b); b = NULL; }
    fclose(f);
    return b;
}

/* g_dcp_hooks.gravity_flipped as main.c installs it for every stage (Stage_IsGravityFlipped): Dead Line
 * acts flip gravity below (act 1) / above (act 2) the split line */
s32 Stage_GetSplitLineY(s32 x);
static dcp_bool sim_gravity_flipped(void *ctx, s32 x, s32 y) {
    (void)ctx;
    if (g_dcp_world.zone == 6 && g_dcp_world.act < 2) {
        if (g_dcp_world.act == 1) { if (y < Stage_GetSplitLineY(x)) return DCP_TRUE; }
        else if (y > Stage_GetSplitLineY(x)) return DCP_TRUE;
    }
    return DCP_FALSE;
}

int vs_sim_init(const char *stages, const char *player, const char *objdata, const char *sprites, const char *stage,
                int chara1, int chara2) {
    char path[512];
    long size;
    s16 *trig;
    u8 *phys;
    const rlvl_object_t *o;
    u32 i;
    VsSim *S = &g_sim;
    if (camera_ctx_size() > sizeof s_cam[0]) { printf("camera ctx too big\n"); return -1; }
    memset(S, 0, sizeof *S);
    snprintf(path, sizeof path, "%s/trig.bin", player); trig = (s16 *)load(path, &size);
    snprintf(path, sizeof path, "%s/physics.bin", player); phys = (u8 *)load(path, &size);
    if (!trig || !phys) return -1;
    dcp_set_trig(trig, trig + 8192);
    if (dcp_set_physics(phys, (u32)size) != 2) return -1;
    snprintf(path, sizeof path, "%s/%s/level.bin", stages, stage);
    s_level = (u8 *)load(path, &size);
    if (!s_level || rlvl_bind(&S->map, s_level) != 0) return -1;
    S->hdr = (const rlvl_header_t *)s_level;
    dcp_bind_map(&S->map);
    g_dcp_hooks.gravity_flipped = sim_gravity_flipped;
    memset(&g_dcp_world, 0, sizeof g_dcp_world);
    g_dcp_world.zone = S->hdr->zone; g_dcp_world.act = S->hdr->act;
    g_dcp_world.mode = 1; g_dcp_world.flags = 1 | 0x20; g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_right = S->map.right; g_dcp_world.death_y = S->map.bottom; g_dcp_world.timescale = 0x100;
    o = (const rlvl_object_t *)rlvl_section(s_level, RLVL_SEC_OBJECTS, NULL);
    S->start_x = 128; S->start_y = 768;
    for (i = 0; i < S->hdr->num_objects; i++) {
        if (o[i].type == 0x72) { S->start_x = o[i].x; S->start_y = o[i].y; }
        if (o[i].type == 0x71) { S->goal_x = o[i].x; S->goal_y = o[i].y; }
    }
    g_dcp_world.check_x = S->start_x; g_dcp_world.check_y = S->start_y;
    dcp_player_create(&S->p[0], (u8)chara1, 0, S->start_x, S->start_y);
    dcp_player_create(&S->p[1], (u8)chara2, 0, S->start_x, S->start_y);
    S->p[0].rings = S->p[1].rings = 1;   /* Player_Create, VS */
    objects_set_paths(objdata, sprites);
    objects_add_player(&S->p[0]); objects_add_player(&S->p[1]);
    if (objects_init(&S->map, s_level) != 0) { printf("objects_init failed\n"); return -1; }
    objects_add_player(&S->p[0]); objects_add_player(&S->p[1]);
    objects_set_view_count(8);
    objects_respawn_view();
    camera_create(s_level, &S->p[0], S->hdr->zone, S->hdr->act);
    camera_set_rival(&S->p[1]);
    camera_ctx_save(s_cam[0]);
    camera_create(s_level, &S->p[1], S->hdr->zone, S->hdr->act);
    camera_set_rival(&S->p[0]);
    camera_ctx_save(s_cam[1]);
    vs_begin(&S->p[0], &S->p[1]);
    return 0;
}

int vs_sim_finished(int i) { return vs_info()->finished[i & 1]; }

void vs_sim_frame(u16 held1, u16 held2) {
    VsSim *S = &g_sim;
    u16 hh[2], pp[2], rr[2];
    u32 c;
    int k;
    hh[0] = held1; hh[1] = held2;
    for (k = 0; k < 2; k++) {
        pp[k] = hh[k] & ~s_prev[k]; rr[k] = s_prev[k] & ~hh[k]; s_prev[k] = hh[k];
    }
    for (k = 0; k < 2; k++) {
        s32 vx, vy, px2, py2;
        camera_ctx_load(s_cam[k]);
        camera_update();
        camera_get_view(&vx, &vy);
        camera_get_partner_view(&px2, &py2);
        objects_set_view(k * 4, camera_layer(0)->x, camera_layer(0)->y);
        objects_set_view(k * 4 + 1, px2, py2);
        objects_set_view(k * 4 + 2, vx - 0x9200, vy);
        objects_set_view(k * 4 + 3, vx + 0x9200, vy);
        camera_ctx_save(s_cam[k]);
    }
    camera_ctx_load(s_cam[0]);
    objects_frame_begin();
    c = g_dcp_world.frame;
    for (k = 0; k < 2; k++) {
        DcPlayer *p = &S->p[k];
        camera_ctx_load(s_cam[k]);
        vs_player_begin(k);
        vs_filter_input(k, &hh[k], &pp[k], &rr[k]);
        dcp_player_frame(p, hh[k], pp[k], rr[k]);
        camera_player_last(p);
        vs_player_end(k);
        camera_ctx_save(s_cam[k]);
        g_dcp_world.frame = c;
        if ((p->pf & DCP_PF_DEAD) && !S->dead[k]) S->deaths[k]++;
        S->dead[k] = (p->pf & DCP_PF_DEAD) != 0;
    }
    camera_ctx_load(s_cam[0]);
    objects_update(&S->p[0]);
    vs_after_players();
    camera_ctx_save(s_cam[0]);
    {   /* player 2's own water level into its camera (main.c vs_sync_water) */
        u16 wl; u32 wf;
        if (objects_player_water(1, &wl, &wf)) {
            camera_ctx_load(s_cam[1]);
            for (k = 0; k < 2; k++) {
                g_obj_camera[k].water = wl;
                g_obj_camera[k].flags = (g_obj_camera[k].flags & ~0x3000000u) | wf;
            }
            camera_ctx_save(s_cam[1]);
        }
    }
    if (g_dcp_world.flags & 1) g_dcp_world.frame++;
    for (k = 0; k < 2; k++) { camera_ctx_load(s_cam[k]); camera_frame_end(); camera_ctx_save(s_cam[k]); }
    vs_frame_end();
    S->frame++;
}
