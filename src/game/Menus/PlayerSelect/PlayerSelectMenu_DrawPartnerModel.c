#include <game/Menus/mdl.h>

void PlayerSelectMenu_SetupModelLighting(void *p, s32 a, s16 *v);

#pragma thumb on

void PlayerSelectMenu_DrawPartnerModel(UnkStruct_Mdl *w) {
    s16 v[3];
    u32 one;
    UnkMtx33_Mdl a;
    UnkMtx33_Mdl b;
    UnkStruct_MdlSlot *m = &w->slot[1];
    UnkStruct_MdlRender *r = &m->r;
    s32 s;
    s32 idx;

    r->trans.x = w->x;
    r->trans.y = w->y;
    r->trans.z = w->z;
    s = (s32)(((s64)w->scale * 0xF00 + 0x800) >> 12);
    r->scale.x = -s;
    r->scale.y = s;
    r->scale.z = s;
    idx = (s32)((u16)(w->ry + m->angOfs) & 0xffff) >> 4;
    MTX_RotY33_(&a, MDL_SIN_IDX(idx), MDL_COS_IDX(idx));
    idx = (s32)((u16)w->rx & 0xffff) >> 4;
    MTX_RotX33_(&b, MDL_SIN_IDX(idx), MDL_COS_IDX(idx));
    MTX_Concat33(&a, &b, &a);
    idx = (s32)((u16)w->rz & 0xffff) >> 4;
    MTX_RotZ33_(&b, MDL_SIN_IDX(idx), MDL_COS_IDX(idx));
    MTX_Concat33(&a, &b, &r->rot);
    if (m->cur != m->prev) {
        if (m->flag) {
            Model_SetAnimation(&m->r, 0, m->anm158, m->cur, NULL);
            Model_SetAnimation(&m->r, 4, m->anm15C, m->frame, NULL);
        } else {
            Model_SetAnimation(&m->r, 0, m->anm150, m->cur, NULL);
            Model_SetAnimation(&m->r, 4, m->anm154, m->frame, NULL);
        }
        m->prev = m->cur;
    }
    if (m->flag) {
        v[0] = -0x1000;
        v[1] = -0x1000;
        v[2] = -0x2000;
        PlayerSelectMenu_SetupModelLighting(m->r.unk94, 1, v);
    } else {
        v[0] = -0x1000;
        v[1] = -0x1000;
        v[2] = 0x2000;
        PlayerSelectMenu_SetupModelLighting(m->r.unk94, 1, v);
    }
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    Model_Draw(r);
    one = 1;
    NNS_G3dGeBufferOP_N(0x12, &one, 1);
}

#pragma thumb reset
