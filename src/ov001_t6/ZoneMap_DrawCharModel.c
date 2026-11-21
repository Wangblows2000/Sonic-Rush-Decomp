#include <nitro/types.h>

typedef struct {
    s32 m[9];
} UnkMtx33_022e397c;

typedef struct {
    s32 mode;
    s32 draw;
    s32 x;
    s32 y;
    s32 rotY;
    s32 rotX;
    s32 unk18;
    s32 cur;
    s32 prev;
    u8 model[0x3c - 0x24];
    s32 scale[3];
    UnkMtx33_022e397c mtx;
    s32 pos[3];
    u8 pad78[0xb8 - 0x78];
    s32 unkb8;
    u8 padbc[0x164 - 0xbc];
    void *res164;
    void *res168;
    void *res16c;
    void *res170;
} UnkStruct_022e397c;

extern s16 data_0206e470[];
extern void *data_ov001_023162ac[];
extern void *data_ov001_023162c4[];
extern void *data_ov001_023162b8[];
extern void *data_ov001_023162a0[];

void ZoneMap_SetupLighting(s32 a, s32 b);
void ZoneMap_SetOrthoProjection(s32 *p);
void ZoneMap_ApplyCameraLookAt(s32 *p);
void MTX_RotY33_(UnkMtx33_022e397c *m, s32 s, s32 c);
void MTX_RotX33_(UnkMtx33_022e397c *m, s32 s, s32 c);
void MTX_Concat33(UnkMtx33_022e397c *a, UnkMtx33_022e397c *b, UnkMtx33_022e397c *ab);
void Model_SetAnimation(void *model, s32 idx, void *res, void *anim, s32 f);
void Model_Draw(void *model);
void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);

#pragma thumb on
void ZoneMap_DrawCharModel(UnkStruct_022e397c *p) {
    u32 one;
    s32 cam[6];
    s32 lt[9];
    UnkMtx33_022e397c my;
    UnkMtx33_022e397c mx;
    s32 idx;
    void *r0;
    void *r1;

    ZoneMap_SetupLighting(p->unkb8, p->unk18);
    cam[0] = 0x5f000;
    cam[1] = -0x60000;
    cam[2] = -0x80000;
    cam[3] = 0x7f000;
    cam[4] = 0x1000;
    cam[5] = 0xc8000;
    ZoneMap_SetOrthoProjection(cam);
    lt[0] = 0;
    lt[1] = 0;
    lt[2] = 0x64000;
    lt[3] = 0;
    lt[4] = 0;
    lt[5] = 0;
    lt[6] = 0;
    lt[7] = 0x1000;
    lt[8] = 0;
    ZoneMap_ApplyCameraLookAt(lt);
    p->scale[0] = 0x4000;
    p->scale[1] = 0x4000;
    p->scale[2] = 0x4000;
    p->pos[0] = cam[2] + p->x;
    p->pos[1] = cam[0] - p->y;
    p->pos[2] = 0;
    idx = ((u16)p->rotY & 0xffff) >> 4;
    MTX_RotY33_(&my, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    idx = ((u16)p->rotX & 0xffff) >> 4;
    MTX_RotX33_(&mx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    MTX_Concat33(&my, &mx, &p->mtx);
    if (p->cur != p->prev) {
        if (p->cur != 2) {
            r0 = p->res164;
            r1 = p->res168;
        } else {
            r0 = p->res16c;
            r1 = p->res170;
        }
        if (p->mode == 0) {
            Model_SetAnimation(p->model, 0, r0, data_ov001_023162ac[p->cur], 0);
            Model_SetAnimation(p->model, 4, r1, data_ov001_023162c4[p->cur], 0);
        } else {
            Model_SetAnimation(p->model, 0, r0, data_ov001_023162b8[p->cur], 0);
            Model_SetAnimation(p->model, 4, r1, data_ov001_023162a0[p->cur], 0);
        }
        p->prev = p->cur;
    }
    if (p->draw != 0) {
        NNS_G3dGeBufferOP_N(0x11, NULL, 0);
        Model_Draw(p->model);
        one = 1;
        NNS_G3dGeBufferOP_N(0x12, &one, 1);
    }
}
#pragma thumb reset
