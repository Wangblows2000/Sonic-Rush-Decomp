/*
 * sp_camera.c - SpecialStageCamera_* (src/game/SpecialStage/Camera/): one
 * registered camera ("main") chasing the goal position / target set by the
 * camera modes (SpecialStageCamera_SetupMode), perspective 0x1555 (30 deg half
 * fov), aspect 0x1555, near 3, far 1500 (fx32 world units).
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpCamera gSpecialStageCamera;
SpCamEnt *gSpecialStageCameraList;
s32 data_ov001_0231f6a8;
MtxFx43 g_sp_view;
fx32 g_sp_proj[5];
static VecFx32 s_cam_pos, s_cam_target, s_cam_up;   /* data_02084f78 / f84 / f90 */

static VecFx32 vrom(u32 a) { return *SP_ROM(a, VecFx32); }

void SpecialStageCamera_CreateList(s32 n) {
    data_ov001_0231f6a8 = n;
    gSpecialStageCameraList = (SpCamEnt *)calloc((size_t)n, sizeof(SpCamEnt));
}

void SpecialStageCamera_FreeList(void) {
    free(gSpecialStageCameraList);
    gSpecialStageCameraList = NULL;
}

void SpecialStageCamera_UpdateOrbitAngles(SpCamEnt *e) {
    s32 dx = e->pos.x - e->target.x;
    s32 dy = e->pos.y - e->target.y;
    s32 dz = e->pos.z - e->target.z;
    if (dy == 0)
        e->lat = 0x4000;
    else if (dz > 0)
        e->lat = dy == 0 ? 0x4000 : FX_Atan2(dz, dy);
    else
        e->lat = dy == 0 ? 0x4000 : FX_Atan2(-dz, dy);
    if (dz == 0)
        e->lon = dx > 0 ? 0x4000 : 0xc000;
    else
        e->lon = FX_Atan2(dx, dz);
}

void SpecialStageCamera_SetPosition(s32 idx, VecFx32 v) {
    SpCamEnt *e = &gSpecialStageCameraList[idx];
    e->pos = v;
    SpecialStageCamera_UpdateOrbitAngles(e);
}

void SpecialStageCamera_SetTarget(s32 idx, VecFx32 v) {
    SpCamEnt *e = &gSpecialStageCameraList[idx];
    e->target = v;
    SpecialStageCamera_UpdateOrbitAngles(e);
}

void SpecialStageCamera_SetPerspective(s32 idx, u16 rot, s32 x, s32 y, s32 z) {
    gSpecialStageCameraList[idx].rot = rot;
    gSpecialStageCameraList[idx].x = x;
    gSpecialStageCameraList[idx].y = y;
    gSpecialStageCameraList[idx].z = z;
}

s32 SpecialStageCamera_Register(const char *name) {
    VecFx32 a = vrom(0x0231efb0);
    VecFx32 b = {0, 0, 0};
    VecFx32 c = vrom(0x0231f004);
    s32 n = data_ov001_0231f6a8, i;
    for (i = 0; i < n; i++)
        if (gSpecialStageCameraList[i].name[0] == 0)
            break;
    if (i >= n)
        return -1;
    memset(&gSpecialStageCameraList[i], 0, 0x20);
    strncpy(gSpecialStageCameraList[i].name, name, 0x1f);
    gSpecialStageCameraList[i].pos = a;
    gSpecialStageCameraList[i].target = b;
    gSpecialStageCameraList[i].up = c;
    SpecialStageCamera_SetPerspective(i, 0x1555, 0x1555, 0x3000, 0x5dc000);
    gSpecialStageCameraList[i].lat = 0x4000;
    gSpecialStageCameraList[i].lon = 0;
    return i;
}

void SpecialStageCamera_Unregister(s32 i) { gSpecialStageCameraList[i].name[0] = 0; }

void SpecialStageCamera_Init(void) {
    VecFx32 a = vrom(0x0231f010), b = vrom(0x0231efec);
    memset(&gSpecialStageCamera, 0, sizeof(gSpecialStageCamera));
    SpecialStageCamera_CreateList(1);
    gSpecialStageCamera.state = 9;
    gSpecialStageCamera.unk4 = 1;
    gSpecialStageCamera.unk8 = 1;
    gSpecialStageCamera.unkC = 0.1f;
    gSpecialStageCamera.pos = a;
    gSpecialStageCamera.tgt = b;
    gSpecialStageCamera.unk28 = 0x1000;
    gSpecialStageCamera.unk34 = SpecialStageCamera_Register((const char *)sp_rom(0x0231f01c, 5)); /* "main" */
    SpecialStageCamera_SetPosition(gSpecialStageCamera.unk34, a);
}

void SpecialStageCamera_Shutdown(void) {
    SpecialStageCamera_Unregister(gSpecialStageCamera.unk34);
    SpecialStageCamera_FreeList();
}

void SpecialStageCamera_SetMode(s32 v) {
    gSpecialStageCamera.state = v;
    gSpecialStageCamera.unk4 = 1;
}

void SpecialStageCamera_GetVectors(VecFx32 *pos, VecFx32 *target, VecFx32 *up) {
    SpCamera *c = &gSpecialStageCamera;
    if (pos != NULL)
        *pos = gSpecialStageCameraList[c->unk34].pos;
    if (target != NULL)
        *target = gSpecialStageCameraList[c->unk34].target;
    if (up != NULL)
        *up = gSpecialStageCameraList[c->unk34].up;
}

void SpecialStageCamera_Orbit(s32 idx, s32 dlat, s16 dlon) {
    SpCamEnt *e = &gSpecialStageCameraList[idx];
    s32 v, d;
    VecFx32 dir;
    v = e->lat + dlat;
    e->lon += dlon;
    if (v > 0x7fc0)
        v = 0x7fc0;
    else if (v < 0x40)
        v = 0x40;
    e->lat = (u16)v;
    d = VEC_Distance(&e->pos, &e->target);
    dir.x = FX_Mul(SIN(e->lat), SIN(e->lon));
    dir.y = COS(e->lat);
    dir.z = FX_Mul(SIN(e->lat), COS(e->lon));
    VEC_MultAdd(d, &dir, &e->target, &e->pos);
}

void SpecialStageCamera_SetupMode(SpCamera *c) {
    SpObj *o = SpecialStage_GetPlayer();
    switch (c->state) {
    case 0:
        if (o->pos.y > 0) {
            VecFx32 a = vrom(0x0231efe0), b = {0, 0, 0};
            b.y = o->pos.y;
            c->pos = a;
            c->tgt = b;
            c->unkC = 0.06f;
        } else {
            VecFx32 b = vrom(0x0231ef8c);
            c->pos = vrom(0x0231efd4);
            c->tgt = b;
            c->unkC = 0.06f;
        }
        c->unk4 = 1;
        c->unk8 = 1;
        break;
    case 1:
        c->pos = o->pos;
        c->pos.x += (o->unk16 < 0x8000) ? 0xa000 : -0xa000;
        c->pos.y += 0x2000;
        c->pos.z += 0xf000;
        c->tgt = o->unkE4;
        c->tgt.y += 0x2000;
        c->unk8 = 1;
        c->unkC = 0.175f;
        c->unk4 = 1;
        break;
    case 2: {
        VecFx32 a = vrom(0x0231efbc), b = vrom(0x0231efa4);
        u16 r;
        c->unk28 = 0x1000;
        c->unk2C = 0;
        c->unk30 = 0;
        c->pos = a;
        c->tgt = b;
        r = Rand();
        c->pos.x += r % 0x1000 - 0x800;
        r = Rand();
        c->pos.y += r % 0x1000 + 0x7800;
        c->pos.z -= 0x8000;
        r = Rand();
        c->tgt.x += r % 0x1000 - 0x800;
        r = Rand();
        c->tgt.y += r % 0x1000 - 0x800;
        c->unk4 = 1;
        c->unk8 = 0;
        break;
    }
    case 3: {
        VecFx32 a = vrom(0x0231eff8);
        c->pos = vrom(0x0231efc8);
        c->pos.y += 0x14000;
        c->pos.z += 0x14000;
        c->tgt = a;
        c->unk8 = 1;
        c->unkC = 0.1f;
        break;
    }
    case 4:
    case 5:
        c->pos = o->unkE4;
        c->pos.y += 0x5000;
        c->pos.z += 0x19000;
        c->tgt = o->unkE4;
        c->unk8 = 1;
        c->unkC = 0.03f;
        break;
    case 6:
        c->pos = o->unkE4;
        c->pos.y += 0x1000;
        c->pos.z += 0x12000;
        c->tgt = o->unkE4;
        c->unk8 = 1;
        c->unkC = 0.015f;
        break;
    case 7: {
        VecFx32 t;
        c->pos.x = o->unkE4.x;
        c->pos.y = -0xf000;
        c->pos.z = o->unkE4.z + 0x64000;
        c->tgt = o->unkE4;
        c->tgt.z += 0x32000;
        c->unk8 = 1;
        c->unkC = 0.025f;
        t.x = o->unkE4.x;
        t.y = o->unkE4.y + 0x8000;
        t.z = o->unkE4.z + 0x6000;
        SpecialStageCamera_SetPosition(c->unk34, t);
        break;
    }
    case 8: {
        VecFx32 a = vrom(0x0231ef98);
        c->pos.x = o->unkE4.x;
        c->pos.y = -0xa000;
        c->pos.z = o->unkE4.z + 0x26000;
        c->tgt = a;
        c->unkC = 0.01f;
        c->unk8 = 1;
        break;
    }
    case 9:
        c->unk28 = 0;
        c->unk2C = 0;
        c->unk30 = 0;
        c->pos.x = 0xf000;
        c->pos.y = o->unkE4.y;
        c->pos.z = o->unkE4.z - 0xa000;
        c->tgt = o->unkE4;
        c->tgt.x += 0x12000;
        SpecialStageCamera_SetPosition(c->unk34, c->pos);
        SpecialStageCamera_SetTarget(c->unk34, c->tgt);
        c->unk8 = 0;
        break;
    case 10:
        c->unk28 = 0x1000;
        c->unk2C = 0;
        c->unk30 = 0;
        c->pos.x = 0x7000;
        c->pos.y = o->unkE4.y;
        c->pos.z = o->unkE4.z - 0xa000;
        c->tgt = o->unkE4;
        c->tgt.x += 0x3000;
        c->tgt.y += 0x1000;
        c->tgt.z += 0x1000;
        c->unk8 = 0;
        break;
    case 11:
        c->unk28 = 0x200;
        c->unk2C = 0;
        c->unk30 = 0;
        c->pos.x = o->unkE4.x;
        c->pos.y = o->unkE4.y + 0x1000;
        c->pos.z = o->unkE4.z + 0xb000;
        c->tgt = o->unkE4;
        c->unk8 = 0;
        break;
    }
}

void SpecialStageCamera_Update(void) {
    SpCamera *c = &gSpecialStageCamera;
    VecFx32 dp, dt;
    s32 lp, lt;
    SpecialStage_GetPlayer();
    if (c->unk4 != 0) {
        c->unk4 = 0;
        SpecialStageCamera_SetupMode(c);
    }
    VEC_Subtract(&c->pos, &gSpecialStageCameraList[c->unk34].pos, &dp);
    VEC_Subtract(&c->tgt, &gSpecialStageCameraList[c->unk34].target, &dt);
    lp = VEC_Mag(&dp);
    lt = VEC_Mag(&dt);
    if (c->state == 0xb) {
        c->unk38++;
        if (c->unk38 >= sp_rom_s32(0x0231ef88))
            c->unk28 = 0x400;
        else {
            SpecialStageCamera_Orbit(c->unk34, 0, (s16)(sp_rom_u16(0x0231ef84) / sp_rom_s32(0x0231ef88)));
            lp = 0;
        }
    } else
        c->unk38 = 0;
    if (c->unk8 == 0) {
        if (c->unk30 < lp && lp > c->unk2C) {
            if (lp > c->unk28) {
                dp.x = FX_Div(dp.x, lp);
                dp.y = FX_Div(dp.y, lp);
                dp.z = FX_Div(dp.z, lp);
                dp.x = FX_Mul(dp.x, c->unk28);
                dp.y = FX_Mul(dp.y, c->unk28);
                dp.z = FX_Mul(dp.z, c->unk28);
            }
            VEC_Add(&dp, &gSpecialStageCameraList[c->unk34].pos, &dp);
            SpecialStageCamera_SetPosition(c->unk34, dp);
        }
        if (c->unk30 < lt && lt > c->unk2C) {
            if (lt > c->unk28) {
                dt.x = FX_Div(dt.x, lt);
                dt.y = FX_Div(dt.y, lt);
                dt.z = FX_Div(dt.z, lt);
                dt.x = FX_Mul(dt.x, c->unk28);
                dt.y = FX_Mul(dt.y, c->unk28);
                dt.z = FX_Mul(dt.z, c->unk28);
            }
            VEC_Add(&dt, &gSpecialStageCameraList[c->unk34].target, &dt);
            SpecialStageCamera_SetTarget(c->unk34, dt);
        }
    } else if (c->unk8 == 1) {
        dp.x = (s32)((float)dp.x * c->unkC);
        dp.y = (s32)((float)dp.y * c->unkC);
        dp.z = (s32)((float)dp.z * c->unkC);
        VEC_Add(&dp, &gSpecialStageCameraList[c->unk34].pos, &dp);
        SpecialStageCamera_SetPosition(c->unk34, dp);
        dt.x = (s32)((float)dt.x * c->unkC);
        dt.y = (s32)((float)dt.y * c->unkC);
        dt.z = (s32)((float)dt.z * c->unkC);
        VEC_Add(&dt, &gSpecialStageCameraList[c->unk34].target, &dt);
        SpecialStageCamera_SetTarget(c->unk34, dt);
    }
}

/* SpecialStageCamera_Apply: G3i_PerspectiveW_ + G3i_LookAt_ (camera matrix data_02084d84) */
void SpecialStageCamera_Apply(void) {
    SpCamEnt *e = &gSpecialStageCameraList[gSpecialStageCamera.unk34];
    g_sp_proj[0] = SIN(e->rot);
    g_sp_proj[1] = COS(e->rot);
    g_sp_proj[2] = e->x;
    g_sp_proj[3] = e->y;
    g_sp_proj[4] = e->z;
    s_cam_pos = e->pos;
    s_cam_target = e->target;
    s_cam_up = e->up;
    G3i_LookAt_(&e->pos, &e->up, &e->target, &g_sp_view);
}

/* the same camera as a dcm_camera_t (row vectors; 640x480 viewport = the DS 256x192 x2.5) */
void sp_camera_dcm(dcm_camera_t *c) {
    float cot, n, f, aspect;
    int r, k;
    memset(c, 0, sizeof(*c));
    for (r = 0; r < 4; r++)
        for (k = 0; k < 3; k++)
            c->view.m[r][k] = g_sp_view.m[r * 3 + k] / 4096.0f;
    cot = (float)g_sp_proj[1] / (float)(g_sp_proj[0] ? g_sp_proj[0] : 1);
    aspect = g_sp_proj[2] / 4096.0f;
    n = g_sp_proj[3] / 4096.0f;
    f = g_sp_proj[4] / 4096.0f;
    c->proj[0][0] = cot / aspect;
    c->proj[1][1] = cot;
    c->proj[2][2] = (f + n) / (n - f);
    c->proj[2][3] = -1.0f;
    c->proj[3][2] = 2 * f * n / (n - f);
    c->vp_x = 0;
    c->vp_y = 0;
    c->vp_w = 640.0f;
    c->vp_h = 480.0f;
    c->near_w = n * 0.5f;
}

/* NNS_G3dWorldPosToScrPos: 0 inside the 256x192 screen, -1 outside / behind */
BOOL NNS_G3dWorldPosToScrPos(const VecFx32 *v, s32 *x, s32 *y) {
    VecFx32 e;
    float cot, aspect, w, sx, sy;
    MTX_MultVec43(v, &g_sp_view, &e);
    w = -e.z / 4096.0f;
    cot = (float)g_sp_proj[1] / (float)(g_sp_proj[0] ? g_sp_proj[0] : 1);
    aspect = g_sp_proj[2] / 4096.0f;
    if (w <= 0.0f) {
        *x = *y = -1;
        return -1;
    }
    sx = (e.x / 4096.0f) * (cot / aspect) / w;
    sy = (e.y / 4096.0f) * cot / w;
    *x = (s32)((sx + 1.0f) * 128.0f);
    *y = (s32)((1.0f - sy) * 96.0f);
    if (*x < 0 || *x >= 256 || *y < 0 || *y >= 192)
        return -1;
    return 0;
}
