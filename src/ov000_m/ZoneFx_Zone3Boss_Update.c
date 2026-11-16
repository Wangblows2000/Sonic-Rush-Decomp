#pragma opt_propagation off
#include <nitro/types.h>

extern u32 gBossStatus;
extern u16 data_02074990[];
extern u16 data_ov000_02364dbc[];
void NNS_G3dGlbLightColor(s32 a, u16 b);

void ZoneFx_Zone3Boss_Update(u16 *cnt) {
    if (!(gBossStatus & 1))
        return;
    (*cnt)++;
    if ((*cnt & ~0x8000) <= 6)
        return;
    if (*cnt & 0x8000) {
        u16 *p = data_02074990;
        NNS_G3dGlbLightColor(0, p[0]);
        NNS_G3dGlbLightColor(1, p[1]);
        NNS_G3dGlbLightColor(2, p[2]);
    } else {
        NNS_G3dGlbLightColor(0, data_ov000_02364dbc[0]);
        NNS_G3dGlbLightColor(1, data_ov000_02364dbc[1]);
        NNS_G3dGlbLightColor(2, data_ov000_02364dbc[2]);
    }
    *cnt &= 0x8000;
    *cnt ^= 0x8000;
}
