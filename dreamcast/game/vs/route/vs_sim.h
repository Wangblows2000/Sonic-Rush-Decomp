/* Host Battle Play simulation (route tool, CPU tests): one stage, two players, the real player,
 * object, camera and race code; the frame order of main.c run_vs_stage. */
#ifndef SRDC_VS_SIM_H
#define SRDC_VS_SIM_H
#include "../vs.h"
#include "../../objects/objects.h"

typedef struct VsSim {
    DcPlayer p[2];
    s32 start_x, start_y, goal_x, goal_y;
    u32 frame;               /* frames simulated */
    int deaths[2], dead[2];
    rlvl_map_t map;
    const rlvl_header_t *hdr;
} VsSim;
extern VsSim g_sim;

/* dirs: stages, player, objdata, sprites (build/dreamcast/...); stage "z11"; characters of P1 / P2 */
int vs_sim_init(const char *stages, const char *player, const char *objdata, const char *sprites, const char *stage,
                int chara1, int chara2);
/* one frame with the two pads (DS bits, held only: pressed / released from the previous frame) */
void vs_sim_frame(u16 held1, u16 held2);
int vs_sim_finished(int i);
#endif
