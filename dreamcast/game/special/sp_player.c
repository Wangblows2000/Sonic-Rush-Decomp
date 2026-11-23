/*
 * sp_player.c - SpecialStagePlayer_* (src/game/SpecialStage/Player/): Sonic on the
 * half-pipe, the state table gSpecialStagePlayerStateTable (0x02317904, 0x34 per
 * state, read from spdata.bin), steering, dash, tricks, hurt, attack, goal.
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpPlayerRes *gSpecialStagePlayerResources;
int sp_mdl_load(SpMdlRes *r, const char *file);
void sp_mdl_free(SpMdlRes *r);
int sp_anm_load(SpAnmRes *r, const char *file, int kind);
void sp_anm_free(SpAnmRes *r);

#define ST(state) (0x02317904 + (u32)(state) * 0x34)
#define ST_U16(state, o) sp_rom_u16(ST(state) + (o))
#define ST_S32(state, o) sp_rom_s32(ST(state) + (o))
#define MDL(p) ((p)->unkAC)

void SpecialStagePlayer_State_Run(SpObj *p);
void SpecialStagePlayer_State_Trick(SpObj *p);
void SpecialStagePlayer_State_TrickInput(SpObj *p);
void SpecialStagePlayer_State_Fail2(SpObj *p);
void SpecialStagePlayer_State_Fail3(SpObj *p);

void SpecialStage_LoadPlayerArchive(void) {
    SpPlayerRes *G = (SpPlayerRes *)calloc(1, sizeof(SpPlayerRes));
    u32 n, i;
    const char *nodes;
    gSpecialStagePlayerResources = G;
    /* narc/ss_player_lz7.narc "ply": 0 son.nsbmd (son, son_ball), 1 son.nsbca, 2 son.nsbva */
    sp_mdl_load(&G->mdl[0], "narc__ss_player_lz7__son__son.dcm");
    sp_mdl_load(&G->mdl[1], "narc__ss_player_lz7__son__son_ball.dcm");
    sp_anm_load(&G->bca, "narc__ss_player_lz7__son.dca", 0);
    sp_anm_load(&G->bva, "narc__ss_player_lz7__son.dcv", 1);
    Model_Init(&G->model, 0);
    G->model.flags[0] |= 4;   /* unk110 */
    Model_SetResource(&G->model, &G->mdl[0]);
    Model_SetAnimation(&G->model, 0, &G->bca, 5);
    Model_SetAnimation(&G->model, 4, &G->bva, 1);
    G->r_hand = -1;
    nodes = (const char *)sp_blob("son_nodes", &n);
    for (i = 0; nodes && i < n / 16 && nodes[i * 16]; i++)
        if (!strcmp(nodes + i * 16, "r_hand"))   /* data_ov001_023178f4 */
            G->r_hand = (int)i;
}

void SpecialStage_ReleasePlayerArchive(void) {
    SpPlayerRes *G = gSpecialStagePlayerResources;
    if (!G)
        return;
    sp_mdl_free(&G->mdl[0]);
    sp_mdl_free(&G->mdl[1]);
    sp_anm_free(&G->bca);
    sp_anm_free(&G->bva);
    free(G);
    gSpecialStagePlayerResources = NULL;
}

SpObj *SpecialStagePlayer_Create(u16 id) {
    SpPlayerRes *s = gSpecialStagePlayerResources;
    SpObj *p = (SpObj *)calloc(1, sizeof(SpObj));
    p->unk16 = 0;
    p->unk18 = 0;
    p->unk1C = 0;
    p->pos.x = 0;
    p->pos.y = -0x1e000;
    p->pos.z = 0;
    p->vel.x = p->vel.y = p->vel.z = 0;
    p->acc.x = p->acc.y = p->acc.z = 0;
    p->scale.x = p->scale.y = p->scale.z = 0x1000;
    p->unk10 = 0;
    p->unk12 = 0x8000;
    p->unk14 = 0;
    p->unk5C = 1;
    p->unk5E = 0;
    p->unk60 = 0;
    p->unk62 = 0;
    p->unk64 = 0x2000;
    p->unk66 = 0;
    p->unk68id = id;
    p->unk6C = 0;
    p->unk74 = 0;
    p->unk78 = NULL;
    p->unkAC = &s->model;
    p->unkB0 = NULL;
    p->unkB4 = SP_ROM(0x0231eec0, SpCircle);
    p->unkB8 = 1;
    p->unkBC = SP_ROM(0x0231eed0, SpCircle);
    p->unkC0 = 1;
    p->unkD8 = p;
    p->unk12C = SP_ROM(0x0231eee0, SpCircle);
    p->unk130 = 1;
    p->unk134 = 0;
    p->unk13C = 0;
    p->unk148 = 0;
    p->unk14C = 0;
    p->unk14E = 0;
    p->unk150 = -1;
    p->unk152 = -1;
    s->model.scale[0] = p->scale.x;
    s->model.scale[1] = p->scale.y;
    s->model.scale[2] = p->scale.z;
    SpecialStagePlayer_ChangeState(p, 3);
    return p;
}

void SpecialStagePlayer_Destroy(SpObj *p) { free(p); }

void SpecialStagePlayer_UpdateAngleFromX(SpObj *p) { p->unk16 = SpecialStage_PipeXToAngle(p->pos.x); }

void SpecialStagePlayer_EndDash(SpObj *p) {
    SpecialStageMap_SetScrollSpeed(0x3000);
    p->unk144 = 0;
    SpecialStageCamera_SetMode(0);
}

void SpecialStagePlayer_StartDash(SpObj *p, SpObj *e) {
    SpecialStageMap_SetScrollSpeed(0x6000);
    p->unk144 = 0x3c;
    SpecialStageCamera_SetMode(3);
    p->unk16 = e->unk16;
    Sound_PlaySeqArc_SS(0x10, 0x17);
}

void SpecialStagePlayer_ChangeState(SpObj *w, s32 state) {
    SpPlayerRes *R = gSpecialStagePlayerResources;
    s32 neg = -1;
    s32 cur = w->unk228;
    switch (ST_S32(state, 0x14)) {
    case 0:
        w->vel.x = ST_S32(state, 0x18);
        w->vel.y = ST_S32(state, 0x20);
        w->vel.z = ST_S32(state, 0x28);
        w->acc.x = ST_S32(state, 0x1c);
        w->acc.y = ST_S32(state, 0x24);
        w->acc.z = ST_S32(state, 0x2c);
        if (w->unk16 >= 0x8000) {
            w->vel.x *= neg;
            w->acc.x *= neg;
        }
        break;
    case 2:
        w->acc.x = ST_S32(state, 0x1c);
        w->acc.y = ST_S32(state, 0x24);
        w->acc.z = ST_S32(state, 0x2c);
        if (w->unk16 >= 0x8000)
            w->acc.x *= neg;
        break;
    case 3:
        w->vel.y = 0;
        w->vel.x = 0;
        w->vel.z = 0;
        w->acc.x = 0;
        w->acc.y = 0;
        w->acc.z = 0;
        break;
    case 4:
        w->vel.y = w->vel.x * ST_S32(state, 0x20);
        w->vel.x = ST_S32(state, 0x18);
        w->vel.z = ST_S32(state, 0x28);
        w->acc.x = ST_S32(state, 0x1c);
        w->acc.y = ST_S32(state, 0x24);
        w->acc.z = ST_S32(state, 0x2c);
        if (w->unk16 >= 0x8000) {
            w->vel.x *= neg;
            w->acc.x *= neg;
            w->vel.y *= neg;
        }
        break;
    case 5:
        w->vel.x = w->vel.y / ST_S32(state, 0x18);
        w->vel.y = ST_S32(state, 0x20);
        w->vel.z = ST_S32(state, 0x28);
        w->acc.x = ST_S32(state, 0x1c);
        w->acc.y = ST_S32(state, 0x24);
        w->acc.z = ST_S32(state, 0x2c);
        if (w->unk16 >= 0x8000) {
            w->vel.x *= neg;
            w->acc.x *= neg;
        }
        break;
    case 6:
        w->vel.x = FX_Mul(SIN(w->unk16), ST_S32(state, 0x18));
        w->vel.y = ST_S32(state, 0x20);
        w->vel.z = ST_S32(state, 0x28);
        w->acc.x = FX_Mul(SIN(w->unk16), ST_S32(state, 0x1c));
        w->acc.y = ST_S32(state, 0x24);
        w->acc.z = ST_S32(state, 0x2c);
        break;
    case 7:
        w->vel.x = ST_S32(state, 0x18);
        w->vel.y = ST_S32(state, 0x20) + -w->pos.z / 300;
        w->vel.z = ST_S32(state, 0x28);
        w->acc.x = ST_S32(state, 0x1c);
        w->acc.y = ST_S32(state, 0x24);
        w->acc.z = ST_S32(state, 0x2c);
        if (w->unk134 == sp_rom_s32(0x023178d8) || w->unk134 == sp_rom_s32(0x023178d8 + 8))
            w->vel.y += 0x800;
        break;
    }
    if (ST_U16(cur, 0) != ST_U16(state, 0)) {
        Model_SetResource(MDL(w), &R->mdl[ST_U16(state, 0)]);
        Model_SetAnimation(MDL(w), 0, &R->bca, ST_U16(state, 2));
        Model_SetAnimation(&R->model, 4, &R->bva, ST_U16(state, 4));
    } else {
        if (ST_U16(cur, 2) != ST_U16(state, 2)) {
            MDL(w)->flags[0] |= 0x40;
            switch (w->unk228) {
            case 3:
                MDL(w)->blendSpeeds[0] = 0xcc;
                break;
            case 0xb:
            case 0xc:
            case 0xd:
                MDL(w)->blendSpeeds[0] = 0x1eb;
                break;
            case 0x15:
                MDL(w)->blendSpeeds[0] = 0x400;
                break;
            default:
                MDL(w)->blendSpeeds[0] = 0x333;
                break;
            }
            Model_SetAnimation(MDL(w), 0, &R->bca, ST_U16(state, 2));
        }
        if (ST_U16(cur, 4) != ST_U16(state, 4))
            Model_SetAnimation(&R->model, 4, &R->bva, ST_U16(state, 4));
    }
    MDL(w)->speed = ST_S32(state, 8);
    if (ST_S32(state, 0xc) != 0)
        MDL(w)->flags[0] |= 4;
    else
        MDL(w)->flags[0] &= ~4;
    if (ST_U16(state, 6) & 1) {
        w->unkBC = NULL;
        w->unkC0 = 0;
    } else {
        w->unkBC = SP_ROM(0x0231eed0, SpCircle);
        w->unkC0 = 1;
    }
    if (ST_U16(state, 6) & 2) {
        w->unk12C = NULL;
        w->unk130 = 0;
    } else {
        w->unk12C = SP_ROM(0x0231eee0, SpCircle);
        w->unk130 = 1;
    }
    if (ST_U16(state, 6) & 4) {
        w->unkB4 = SP_ROM(0x0231eec0, SpCircle);
        w->unkB8 = 1;
    } else {
        w->unkB4 = NULL;
        w->unkB8 = 0;
    }
    if (ST_U16(state, 6) & 0x10) {
        w->unk16 = 0;
        w->unk18 = 0;
        w->unk1C = 0;
        SpecialStagePlayer_UpdateAngleFromX(w);
    }
    if (!(ST_U16(state, 6) & 0x800) && w->unk144 > 0)
        SpecialStagePlayer_EndDash(w);
    if (ST_U16(state, 6) & 0x1000)
        w->unk1F0 = 1;
    else
        w->unk1F0 = 0;
    if (ST_U16(state, 6) & 0x20)
        w->unk16 = 0;
    w->unk228 = state;
    w->unk22C = (void (*)(SpObj *))sp_func(sp_rom_u32(ST(state) + 0x30));
    if (ST_S32(state, 0x10) >= 0) {
        Sound_PlaySeqArc_SS(0x10, ST_S32(state, 0x10));
    } else {
        switch (state) {
        case 0xb:
        case 0xc:
        case 0xd: {
            s32 se = sp_rom_s32(0x023178e4 + (Rand() % 4) * 4);
            Sound_PlaySeqArc_SS(0x10, se);
            Sound_PlaySeqArc_SS(0x10, sp_rom_s32(0x0231ee9c + w->unk158 * 4));
            break;
        }
        }
    }
    w->unk140 = 0;
    w->unk15C = 0;
}

BOOL SpecialStagePlayer_IsGrounded(SpObj *p) {
    if (ST_U16(p->unk228, 6) & 0x10) {
        VecFx32 v;
        if (p->pos.z < -0x5000)
            return FALSE;
        v.x = p->pos.x;
        v.y = p->pos.y;
        v.z = 0;
        if (VEC_Mag(&v) >= 0x1e000 && p->pos.y <= 0)
            return TRUE;
    } else if (p->unk18 <= 0)
        return TRUE;
    return FALSE;
}

BOOL SpecialStagePlayer_IsAngleWithin90(u32 x) {
    if (x <= 0x4000 || (x >= 0xc000 && (s32)x <= 0x10000))
        return TRUE;
    return FALSE;
}

void SpecialStagePlayer_TurnTowards(SpObj *p, s32 target) {
    u32 a = p->unk16;
    s32 d;
    if (a >= 0x8000)
        a -= 0x10000;
    if ((s32)a == target)
        return;
    d = target - (s32)a;
    if (d > p->unk138)
        d = p->unk138;
    else if (d < -p->unk138)
        d = -p->unk138;
    if (d == 0) {
        p->vel.x = 0;
        p->unk16 = (u16)target;
    } else
        p->vel.x = d;
}

void SpecialStagePlayer_UpdateTurn(SpObj *p) {
    if (SpecialStagePlayer_IsAngleWithin90(p->unk16) == 1)
        SpecialStagePlayer_TurnTowards(p, p->unk134);
    if (p->vel.x > 0x708) {
        p->vel.x = 0x708;
        return;
    }
    if (p->vel.x < -1800)
        p->vel.x = -1800;
}

void SpecialStagePlayer_ClampTilt(SpObj *p) {
    if (p->unk134 > 0x8000)
        p->unk134 -= 0x10000;
    if (p->unk134 < -0x4000) {
        p->unk134 = -0x4000;
        return;
    }
    if (p->unk134 > 0x4000)
        p->unk134 = 0x4000;
}

BOOL SpecialStagePlayer_IsWithinTouchRadius(s32 x, s32 y) {
    VecFx32 c = *SP_ROM(0x0231eea8, VecFx32), v;
    v.x = x << 12;
    v.y = y << 12;
    v.z = 0;
    return VEC_Distance(&c, &v) < 0x4b000;
}

/* controller scheme (sp_input.c): the target comes from the stick / D-pad instead of the
 * touch point (SpecialStage_TouchToPipeAngle(gTouchState.x, gTouchState.y)); the rest is the
 * original, including the wall target that arms the edge jump */
void SpecialStagePlayer_UpdateTouchSteer(SpObj *p) {
    if (!g_sp_pad.steer)
        return;
    p->unk134 = g_sp_pad.steer_angle;
    p->unk13C = 0;
    p->unk140 = 0;
    p->unk138 = 0x800;
    if (p->unk134 == sp_rom_s32(0x023178d8) || p->unk134 == sp_rom_s32(0x023178d8 + 8))
        p->unk140 = 1;
}

BOOL SpecialStagePlayer_CheckTouchTarget(SpObj *p) {
    if (p->unk114 == 0)
        return FALSE;
    /* controller scheme: A instead of a tap on the gimmick (SpecialStageGimmick_FindTouched at
     * gTouchState.startX/Y): the nearest gimmick the tap could hit */
    if (g_sp_pad.attack) {
        p->unk118 = SpecialStageGimmick_FindAttackTarget(p, *SpecialStage_GetGimmickManager());
        if (p->unk118 != NULL) {
            SpecialStagePlayer_ChangeState(p, 0x12);
            return TRUE;
        }
    }
    return FALSE;
}

s32 SpecialStagePlayer_Unk_ov001_02312250(SpObj *p) { return p->unk140; }

void SpecialStagePlayer_MoveToTarget(SpObj *p) {
    VecFx32 v;
    s32 mag;
    VEC_Subtract(&p->unk118->pos, &p->pos, &v);
    mag = VEC_Mag(&v);
    if (mag > 0x4000) {
        v.x = FX_Div(v.x, mag);
        v.y = FX_Div(v.y, mag);
        v.z = FX_Div(v.z, mag);
        v.x = FX_Mul(v.x, 0x4000);
        v.y = FX_Mul(v.y, 0x4000);
        v.z = FX_Mul(v.z, 0x4000);
    }
    p->vel.x = v.x / 4;
    p->vel.y = v.y / 4;
    p->vel.z = v.z / 4;
    p->acc.x = 0;
    p->acc.y = 0;
    p->acc.z = 0;
    p->pos.x += v.x;
    p->pos.y += v.y;
    p->pos.z += v.z;
}

void SpecialStagePlayer_SlowDownMap(void) {
    s32 v = SpecialStageMap_GetScrollSpeed();
    v -= 182;   /* _ffix(0x4336febc): (s32)182.995f */
    if (v < 0)
        v = 0;
    SpecialStageMap_SetScrollSpeed(v);
}

s32 SpecialStagePlayer_GetSpeedAnimRate(s32 a) { return FX_Div(a, 0x3000) * 4 + 0x1000; }

void SpecialStage_Unk_ov001_02312dd0(SpObj *p, s32 v) { p->unk1F4 = v; }
void SpecialStageHud_Unk_ov001_02312dd8(SpObj *p, s32 v) { p->unk114 = v; }

void SpecialStagePlayer_SetRHandCallback(void) {
    /* NNS_G3dRenderObjSetCallBack(.., RHandMtxCallback, 0, NODE, 3): the right-hand node
     * matrix is captured while the model is drawn (SpecialStagePlayer_Draw) */
    gSpecialStagePlayerResources->rhand_cb = 1;
    gSpecialStagePlayerResources->model.cb[0] = 4; /* b->unkA = 4: FlushVP, so the captured matrix is in world space */
}

/* ------------------------------------------------------------ actions */
void SpecialStagePlayer_Action_Start(SpObj *p) { SpecialStagePlayer_ChangeState(p, 0); }

void SpecialStagePlayer_Action_Celebrate(SpObj *p) {
    Sound_PlaySeqArc_SS(0x10, sp_rom_s32(0x023178e4 + ((s32)Rand() % 4) * 4));
    SpecialStagePlayer_ChangeState(p, 0x14);
}

void SpecialStagePlayer_Action_Fail(SpObj *p) {
    SpecialStagePlayer_ChangeState(p, 0x16);
    SpecialStageCamera_SetMode(5);
}

void SpecialStagePlayer_Action_StartTrick(SpObj *p, s32 seq) {
    SpecialStageMap_SetScrollSpeed(0);
    SpecialStagePlayer_ChangeState(p, 0xa);
    p->unk16 = 0;
    p->unk14C = 0;
    p->unk14E = 1;
    p->unk128 = 1;
    p->unk150 = (s16)seq;
}

/* ------------------------------------------------------------ states */
void SpecialStagePlayer_State_Start0(SpObj *p) {
    MDL(p)->speed += 0x40;
    if (MDL(p)->speed < 0x2000)
        return;
    MDL(p)->flags[0] &= ~4;
    if (MDL(p)->flags[0] & 2)
        SpecialStagePlayer_ChangeState(p, 1);
}

void SpecialStagePlayer_State_Start1(SpObj *p) {
    MDL(p)->speed += 0x40;
    if (MDL(p)->speed < 0x3000)
        return;
    MDL(p)->flags[0] &= ~4;
    if (MDL(p)->flags[0] & 2)
        SpecialStagePlayer_ChangeState(p, 2);
}

static inline BOOL IsLeft(u16 a) { return a < 0x8000; }

void SpecialStagePlayer_State_Run(SpObj *p) {
    p->unk12 = 0x8000;
    if (p->vel.x >= 0x708 || p->vel.x <= -0x708 || p->unk144 > 0)
        p->unk1F0 = 1;
    else
        p->unk1F0 = 0;
    if (p->unk114 != 0) {
        /* the DS ignores touches within 75 px of (128, 45) (SpecialStagePlayer_IsWithinTouchRadius):
         * the touch dead zone; the stick has its own (sp_input.c) */
        if (p->unk144 <= 0)
            SpecialStagePlayer_UpdateTouchSteer(p);
    } else {
        p->unk134 = 0;
        p->unk138 = 0x100;
        if (p->unk11C != NULL && p->unk11C->pos.z > -0x6e000) {
            SpecialStagePlayer_ChangeState(p, 0x18);
            SpecialStage_Unk_ov001_02312dd0(p, 0x1e);
            SpecialStagePlayer_SetRHandCallback();
            return;
        }
    }
    SpecialStagePlayer_UpdateTurn(p);
    if (SpecialStagePlayer_CheckTouchTarget(p))
        return;
    if (SpecialStagePlayer_Unk_ov001_02312250(p) && (u16)p->unk134 == p->unk16 &&
        (p->unk16 == (u16)sp_rom_s32(0x023178d8) || p->unk16 == (u16)sp_rom_s32(0x023178d8 + 8))) {
        p->vel.x = 0x4b0;
        p->vel.x = p->vel.x * (IsLeft(p->unk16) > 0 ? 1 : -1);
        p->unk13C = 0;
        SpecialStagePlayer_ChangeState(p, 6);
        return;
    }
    if (p->unk16 == (u16)p->unk134) {
        p->vel.x = 0;
        p->unk13C = 0;
    }
}

void SpecialStagePlayer_State_WaitLand(SpObj *p) {
    if (SpecialStagePlayer_IsGrounded(p)) {
        p->unk18 = 0;
        SpecialStagePlayer_ChangeState(p, 5);
        return;
    }
    SpecialStagePlayer_CheckTouchTarget(p);
}

void SpecialStagePlayer_State_Land(SpObj *p) {
    p->unk64 |= 0x2000;
    SpecialStagePlayer_ChangeState(p, 2);
    SpecialStagePlayer_UpdateAngleFromX(p);
    p->unk134 = p->unk16;
    SpecialStageMap_SetScrollSpeed(0x3000);
}

void SpecialStagePlayer_State_EdgeJump(SpObj *p) {
    if (!SpecialStagePlayer_IsGrounded(p))
        return;
    p->unk18 = 0;
    p->vel.y = 0;
    p->pos.y = 0;
    SpecialStagePlayer_ChangeState(p, 7);
}

void SpecialStagePlayer_State_EdgeLand(SpObj *p) { SpecialStagePlayer_ChangeState(p, 2); }

void SpecialStagePlayer_State_AttackFall(SpObj *p) {
    if (!SpecialStagePlayer_IsGrounded(p))
        return;
    SpecialStagePlayer_ChangeState(p, 5);
}

void SpecialStagePlayer_State_AttackRebound(SpObj *p) {
    s32 d;
    if (SpecialStagePlayer_IsGrounded(p)) {
        p->unk124 = 0;
        SpecialStagePlayer_ChangeState(p, 5);
        return;
    }
    SpecialStagePlayer_UpdateAngleFromX(p);
    d = SpecialStage_PipeAngleToX((u16)p->unk134) - p->pos.x;
    if (d > 0x1400)
        d = 0x1400;
    else if (d < -0x1400)
        d = -0x1400;
    p->pos.x += d;
    SpecialStagePlayer_CheckTouchTarget(p);
}

void SpecialStagePlayer_State_TargetAttack(SpObj *p) {
    SpObj *q = p->unk118;
    BOOL b;
    if (q == NULL)
        b = FALSE;
    else
        b = q->pos.z > 0 ? FALSE : TRUE;
    if (!b) {
        if (SpecialStagePlayer_IsGrounded(p)) {
            p->unk18 = 0;
            SpecialStagePlayer_ChangeState(p, 2);
        } else {
            SpecialStagePlayer_ChangeState(p, 8);
            Sound_PlaySeqArc_SS(0x10, 6);
        }
        p->unk124 = 0;
        return;
    }
    p->unk124 = 1;
}

void SpecialStagePlayer_State_TrickJump(SpObj *p) {
    if (p->unk15C < 3) {
        u16 old = p->unk12;
        p->unk12 += 0xb00;
        if (old > 0x8000 && p->unk12 <= 0x8000) {
            p->unk15C++;
            if (p->unk15C >= 3)
                p->unk12 = 0;
        }
    }
    if (p->vel.y < 0) {
        SpecialStagePlayer_Action_StartTrick(p, p->unk150);
        return;
    }
    if (p->vel.y <= 0x800)
        return;
    if (!(p->unk6C & 1))
        return;
    {
        u16 r1 = Rand(), r2;
        s32 x = p->pos.x + r1 % 0x6000 - 0x3000;
        r2 = Rand();
        SpecialStageActObj_SpawnAt(2, x, p->pos.y + r2 % 0x6000 - 0x3000, p->pos.z);
    }
}

void SpecialStagePlayer_State_TrickInput(SpObj *p) {
    s32 n;
    p->unk14C = (s16)SpecialStageTrick_CheckTouch(p->unk150, p->unk14E, p->unk14C);
    p->unk14E++;
    p->unk12 = 0;
    if (p->unk14C == -1) {
        SpecialStageCamera_SetMode(0);
        SpecialStagePlayer_ChangeState(p, 0xe);
        p->unk128 = 0;
        Sound_PlaySeqArc_SS(0x10, 0xb);
        Sound_PlaySeqArc_SS(0x10, 0x10);
        SpecialStageMap_SetScrollSpeed(0x800);
        SpecialStageHud_ShowTrickRating(3);
        g_sp_trace.tricks_fail++;
        return;
    }
    if (p->unk14C == 0x1234) {
        n = SpecialStageTrick_GetRating(p->unk150, p->unk14E);
        p->unk158 = n;
        p->unk154 = sp_rom_s32(0x023178cc + n * 4);
        p->unk152 = (s16)SpecialStagePlayer_Unk_ov001_02314694(p->unk150);
        SpecialStageHud_ShowTrickRating(n);
        SpecialStagePlayer_ChangeState(p, Rand() % 3 + 0xb);
        g_sp_trace.tricks_ok++;
    }
}

void SpecialStagePlayer_State_Trick(SpObj *p) {
    p->unk12 = 0;
    if (MDL(p)->flags[0] & 2) {
        Sound_StopSeqArc_SS(0x10, 0xf, 0x5a);   /* NNS_SndPlayerStopSeqBySeqArcIdx(0x10, 0xf, 0x5a) */
        if (p->unk152 >= 0) {
            p->unk150 = p->unk152;
            p->unk128 = 0;
            SpecialStagePlayer_ChangeState(p, 0xf);
            SpecialStageHud_EndTrickRating();
        } else {
            SpecialStageCamera_SetMode(0);
            SpecialStagePlayer_ChangeState(p, 0xe);
            p->unk128 = 0;
            SpecialStageMap_SetScrollSpeed(0x800);
        }
        return;
    }
    if (p->unk154 <= 0)
        return;
    {
        /* arguments evaluated right to left (MWCC) */
        u16 rz = Rand(), ry = Rand(), rx = Rand();
        SpecialStageGimmick_SpawnTrickReward(SpecialStage_GetGimmickManager(), p->pos.x + (rx % 100 - 0x32) * 0x1000,
                                             p->pos.y + (ry % 100 - 0x32) * 0x1000, p->pos.z + (rz % 100 - 0x32) * 0x1000);
    }
    p->unk154--;
}

void SpecialStagePlayer_State_TrickChain(SpObj *p) {
    if (p->unk15C < 1) {
        u16 old = p->unk12;
        p->unk12 += 0xc00;
        if (old > 0x8000 && p->unk12 <= 0x8000) {
            p->unk15C += 1;
            if (p->unk15C >= 1)
                p->unk12 = 0;
        }
    }
    if (p->vel.y < -ST_S32(p->unk228, 0x20))   /* data_ov001_02317924[state][0] */
        SpecialStagePlayer_Action_StartTrick(p, p->unk150);
}

void SpecialStagePlayer_State_TrickEnd(SpObj *p) {
    if (p->unk15C < 2) {
        u16 old = p->unk12;
        p->unk12 += 0x780;
        if (old < 0x8000 && p->unk12 >= 0x8000) {
            p->unk15C += 1;
            if (p->unk15C >= 2)
                p->unk12 = 0x8000;
        }
    }
    if (SpecialStagePlayer_IsGrounded(p))
        SpecialStagePlayer_ChangeState(p, 5);
}

void SpecialStagePlayer_State_Hurt(SpObj *p) {
    s32 i;
    if (SpecialStagePlayer_IsGrounded(p)) {
        p->vel.y = 0;
        p->acc.y = 0;
    }
    if (MDL(p)->flags[0] & 2) {
        SpecialStagePlayer_ChangeState(p, 0x11);
        return;
    }
    if (p->unk120 == 0 && p->unk148 > 0) {
        Sound_PlaySeqArc_SS(0x10, 4);
        for (i = 0; i < p->unk148; i++) {
            SpecialStageActObj_SpawnAt(4, p->pos.x, p->pos.y, p->pos.z);
            if (i == 9)
                break;
        }
        p->unk148 -= 10;
        if (p->unk148 < 0)
            p->unk148 = 0;
    }
    p->unk120 = 1;
}

void SpecialStagePlayer_State_HurtRecover(SpObj *p) {
    p->unk74 = 0xb4;
    SpecialStageCamera_SetMode(0);
    SpecialStagePlayer_ChangeState(p, 2);
    p->unk120 = 0;
    p->unk134 = p->unk16;
    SpecialStageMap_SetScrollSpeed(0x3000);
}

void SpecialStagePlayer_State_Celebrate(SpObj *p) {
    if (!(MDL(p)->flags[0] & 2))
        return;
    SpecialStagePlayer_ChangeState(p, 0x15);
}

void SpecialStagePlayer_State_CelebrateSpin(SpObj *p) {
    if (p->unk15C < 2) {
        u16 old = p->unk12;
        p->unk12 = old + 0x6c0;
        if (old < 0x8000 && p->unk12 >= 0x8000) {
            p->unk15C += 1;
            if (p->unk15C >= 2)
                p->unk12 = 0x8000;
        }
    }
    if (MDL(p)->flags[0] & 2)
        SpecialStagePlayer_ChangeState(p, 2);
}

void SpecialStagePlayer_State_Fail(SpObj *p) {
    SpPlayerRes *R = gSpecialStagePlayerResources;
    s32 t0 = SpecialStageMap_GetScrollSpeed();
    if (t0 < 0x2400) {
        MDL(p)->flags[0] &= ~4;
        if (!(MDL(p)->flags[0] & 2))
            return;
        Model_SetResource(MDL(p), &R->mdl[0]);
        Model_SetAnimation(MDL(p), 0, &R->bca, 3);
        Model_SetAnimation(&R->model, 4, &R->bva, 1);
        MDL(p)->flags[0] |= 4;
        p->unk22C = SpecialStagePlayer_State_Fail2;
        return;
    }
    MDL(p)->speed = SpecialStagePlayer_GetSpeedAnimRate(t0);
}

void SpecialStagePlayer_State_Fail2(SpObj *p) {
    SpPlayerRes *R = gSpecialStagePlayerResources;
    s32 t0 = SpecialStageMap_GetScrollSpeed();
    if (t0 < 0x1800) {
        MDL(p)->flags[0] &= ~4;
        if (!(MDL(p)->flags[0] & 2))
            return;
        Model_SetResource(MDL(p), &R->mdl[0]);
        Model_SetAnimation(MDL(p), 0, &R->bca, 2);
        Model_SetAnimation(&R->model, 4, &R->bva, 1);
        MDL(p)->flags[0] |= 4;
        p->unk22C = SpecialStagePlayer_State_Fail3;
        return;
    }
    MDL(p)->speed = SpecialStagePlayer_GetSpeedAnimRate(t0);
}

void SpecialStagePlayer_State_Fail3(SpObj *p) {
    s32 v = SpecialStageMap_GetScrollSpeed();
    FX_Div(v, 0x3000);
    MDL(p)->speed = SpecialStagePlayer_GetSpeedAnimRate(v);
    if (v >= 0xc00)
        return;
    MDL(p)->flags[0] &= ~4;
    Sound_PlaySeqArc_SS(0x10, 0x10);
    SpecialStagePlayer_ChangeState(p, 0x17);
}

void SpecialStagePlayer_State_GoalSlowDown(SpObj *p) {
    SpecialStagePlayer_SlowDownMap();
    if (!(MDL(p)->flags[0] & 2))
        return;
    SpecialStagePlayer_ChangeState(p, 0x19);
}

void SpecialStagePlayer_State_Brake(SpObj *p) {
    SpecialStagePlayer_SlowDownMap();
    if (SpecialStageMap_GetScrollSpeed() != 0)
        return;
    SpecialStagePlayer_ChangeState(p, 0x1a);
}

void SpecialStagePlayer_State_Stopped(SpObj *p) { (void)p; }

/* ------------------------------------------------------------ collision callbacks */
void SpecialStagePlayer_OnTouchGimmick(SpObj *p, SpObj *e) {
    if (p->unk22C == SpecialStagePlayer_State_Trick || p->unk22C == SpecialStagePlayer_State_TrickInput) {
        switch (e->unk68) {
        case 0:
        case 1:
            p->unk148 += e->unk5E;
            Sound_PlaySeqArc_SS(0x10, 3);
            break;
        }
        return;
    }
    switch (e->unk68) {
    case 0:
    case 1:
        p->unk148 += e->unk5E;
        Sound_PlaySeqArc_SS(0x10, 3);
        break;
    case 2:
        SpecialStagePlayer_Action_StartTrick(p, e->unk6A);
        break;
    case 3:
        p->unk150 = e->unk6A;
        SpecialStagePlayer_ChangeState(p, 9);
        SpecialStageMap_SetScrollSpeed(0x800);
        SpecialStageCamera_SetMode(1);
        p->pos = e->pos;
        break;
    case 4:
        SpecialStagePlayer_StartDash(p, e);
        p->unk134 = e->unk16;
        SpecialStagePlayer_ClampTilt(p);
        p->unk138 = 0x800;
        break;
    case 5:
        SpecialStagePlayer_StartDash(p, e);
        p->unk134 = e->unk16 - 0x4000;
        SpecialStagePlayer_ClampTilt(p);
        p->unk138 = 0x111;
        break;
    case 6:
        SpecialStagePlayer_StartDash(p, e);
        p->unk134 = e->unk16 + 0x4000;
        SpecialStagePlayer_ClampTilt(p);
        p->unk138 = 0x111;
        break;
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
        Sound_PlaySeqArc_SS(0x10, 0x15);
        break;
    case 0x17:
        SpecialStageCamera_SetMode(6);
        break;
    }
}

void SpecialStagePlayer_OnHurt(SpObj *p, SpObj *o) {
    SpObj *q;
    if (p->unk74 > 0)
        return;
    switch (o->unk68) {
    case 0xd:
    case 0xe:
        q = SpecialStageActObj_SpawnAt(1, o->pos.x, o->pos.y, o->pos.z);
        if (q != NULL) {
            q->unkAC->speed = 0x2000;
            SpecialStage_UpdateObject(NULL, q);
        }
        SpecialStageGimmick_Destroy(o);
        Sound_PlaySeqArc_SS(0x10, 0x11);
        break;
    }
    SpecialStagePlayer_ChangeState(p, 0x10);
    SpecialStageCamera_SetMode(2);
    g_sp_trace.hurt++;
}

void SpecialStagePlayer_OnAttackHit(SpObj *p) {
    SpecialStagePlayer_UpdateAngleFromX(p);
    SpecialStagePlayer_ChangeState(p, 0x13);
}

/* ------------------------------------------------------------ update / draw */
void SpecialStagePlayer_Update(SpObj *w) {
    SpObj *p;
    MtxFx33 mx, my, mz;
    MtxFx43 m1, m2;
    u16 fl = ST_U16(w->unk228, 6);
    w->unk6C++;
    if (w->unk144 > 0) {
        w->unk144--;
        if (w->unk144 == 0)
            SpecialStagePlayer_EndDash(w);
    }
    p = w->unk118;
    if (p != NULL) {
        if (p->unk60 & 1)
            w->unk118 = NULL;
        else if (!SpecialStageObj_IsInReach(w, p))
            w->unk118 = NULL;
    }
    if (w->unk74 > 0)
        w->unk74--;
    w->unk178 = w->unk16C;
    w->unk16C = w->unk160;
    w->unk160 = w->pos;
    w->unk1CC = w->unk1A8;
    w->unk1A8 = w->unk184;
    w->unk184 = w->mtx;
    w->unk1F4--;
    if (SpecialStageMap_GetScrollSpeed() > 0x3000) {
        w->unk178.z += SpecialStageMap_GetScrollSpeed();
        w->unk16C.z += SpecialStageMap_GetScrollSpeed();
        w->unk160.z += SpecialStageMap_GetScrollSpeed();
    } else if (w->unk1F4 > 0) {
        w->unk178.z += w->unk1F4 * 0x230;
        w->unk16C.z += w->unk1F4 * 0x230;
        w->unk160.z += w->unk1F4 * 0x230;
    }
    if (fl & 0x400) {
        s32 v0 = w->vel.x;
        s32 n = v0 + w->acc.x;
        if (v0 == 0)
            w->acc.x = 0;
        else if ((v0 >= 0 && n <= 0) || (v0 <= 0 && n >= 0)) {
            w->vel.x = 0;
            w->acc.x = 0;
        }
    }
    if (!(fl & 0x10)) {
        w->vel.x += w->acc.x;
        w->vel.y += w->acc.y;
        w->vel.z += w->acc.z;
        w->unk16 += w->vel.x;
        w->unk18 += w->vel.y;
        w->pos.z += w->vel.z;
        if (w->unk18 < 0)
            w->unk18 = 0;
    } else {
        if (ST_S32(w->unk228, 0x14) == 3 && w->unk118 != NULL)
            SpecialStagePlayer_MoveToTarget(w);
        else {
            w->vel.x += w->acc.x;
            w->vel.y += w->acc.y;
            w->vel.z += w->acc.z;
            w->pos.x += w->vel.x;
            w->pos.y += w->vel.y;
            w->pos.z += w->vel.z;
        }
        if (w->pos.x > 0x1e000)
            w->pos.x = 0x1e000;
        else if (w->pos.x < -0x1e000)
            w->pos.x = -0x1e000;
        if (w->pos.y < -0x1e000)
            w->pos.y = -0x1e000;
    }
    if (fl & 0x100) {
        if (w->pos.z < 0) {
            w->vel.z = 0x2800;
            w->acc.z = 0;
        } else if (w->pos.z > 0) {
            w->pos.z = 0;
            w->vel.z = 0;
            w->acc.z = 0;
        }
    }
    if (fl & 0x200) {
        if (w->unk16 < 0x8000) {
            if (w->vel.x > 0) {
                w->vel.x = 0;
                w->acc.x = 0;
            }
        } else if (w->vel.x < 0) {
            w->vel.x = 0;
            w->acc.x = 0;
        }
    }
    if (w->unk22C != NULL)
        w->unk22C(w);
    fl = ST_U16(w->unk228, 6);
    if (!(fl & 0x10)) {
        if (w->unk228 == 6) {
            w->pos.x = (w->unk16 < 0x8000) ? 0x1e000 : -0x1e000;
            w->pos.y = w->unk18;
        } else {
            u16 a = (u16)(w->unk16 - 0x4000);
            w->pos.x = FX_Mul(COS(a), 0x1e000);
            w->pos.y = FX_Mul(SIN(a), 0x1e000);
            if (!(w->unk64 & 0x2000)) {
                w->pos.y += w->unk18;
            } else {
                w->pos.x -= FX_Mul(w->unk18, SIN(w->unk16));
                w->pos.y += FX_Mul(w->unk18, COS(w->unk16));
            }
        }
    }
    MTX_RotY33_(&my, SIN(w->unk12), COS(w->unk12));
    MTX_RotX33_(&mx, SIN(w->unk10), COS(w->unk10));
    MTX_RotZ33_(&mz, SIN(w->unk16), COS(w->unk16));
    MTX_Concat33(&my, &mx, &w->mtx);
    MTX_Concat33(&w->mtx, &mz, &w->mtx);
    {
        VecFx32 v = {0, 0, 0};
        MTX_Copy33To43_(&w->mtx, &m1);
        MTX_Identity43_(&m2);
        MTX_TransApply43(&m2, &m2, w->pos.x, w->pos.y, w->pos.z);
        MTX_Concat43(&m1, &m2, &m2);
        MTX_TransApply43(&m2, &m2, 0, 0x4000, 0);
        MTX_MultVec43(&v, &m2, &w->unkE4);
    }
    Model_Animate(&gSpecialStagePlayerResources->model);
}

/* SpecialStagePlayer_RHandMtxCallback: world matrix of the "r_hand" node (fx32) */
static void rhand_capture(SpObj *p) {
    SpPlayerRes *R = gSpecialStagePlayerResources;
    const SpModel *m = &R->model;
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_mtx_t world[DCM_MAX_JOINTS];
    dcm_mtx_t mdl, hand;
    int r, c;
    if (!m->res || !m->res->loaded || R->r_hand < 0 || R->r_hand >= m->res->mdl.hdr->num_joints)
        return;
    if (m->anm[0] && m->anm[0]->res && m->anm[0]->res->loaded)
        dca_sample(&m->anm[0]->res->set, m->anm[0]->idx, (float)(m->anm[0]->frame >> 12), &m->res->mdl, loc);
    else
        dcm_bind_locals(&m->res->mdl, loc);
    dcm_world(&m->res->mdl, loc, world);
    memset(&mdl, 0, sizeof(mdl));
    for (r = 0; r < 3; r++)
        for (c = 0; c < 3; c++)
            mdl.m[r][c] = m->rot.m[r * 3 + c] / 4096.0f * (m->scale[r] / 4096.0f);
    for (c = 0; c < 3; c++)
        mdl.m[3][c] = m->trans[c] / 4096.0f;
    dcm_mtx_mul(&hand, &world[R->r_hand], &mdl);
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++)
            p->unk1F8.m[r * 3 + c] = (fx32)(hand.m[r][c] * 4096.0f);
}

void SpecialStagePlayer_DrawModels(SpObj *p) {
    SpModel *m = p->unkAC;
    s32 i, k;
    g_sp_light_mode = 1;   /* NNS_G3dGlbLightColor(0..2, 0x6182) */
    if (p->unk228 == 0x12 || p->unk228 == 0x13) {
        if (p->unk6C & 1) {
            const VecFx32 *pos[3] = {&p->unk160, &p->unk16C, &p->unk178};
            const MtxFx33 *rot[3] = {&p->unk184, &p->unk1A8, &p->unk1CC};
            for (i = 0; i < 3; i++) {
                m->rot = *rot[i];
                m->trans[0] = pos[i]->x;
                m->trans[1] = pos[i]->y;
                m->trans[2] = pos[i]->z;
                sp_draw_model(m, 1);
            }
        }
    } else {
        const VecFx32 *pos[3] = {&p->unk160, &p->unk16C, &p->unk178};
        const MtxFx33 *rot[3] = {&p->unk184, &p->unk1A8, &p->unk1CC};
        if (p->unk6C & 1)
            k = 0;
        else if (p->unk6C & 2)
            k = 1;
        else
            k = 2;
        m->rot = *rot[k];
        m->trans[0] = pos[k]->x;
        m->trans[1] = pos[k]->y;
        m->trans[2] = pos[k]->z;
        sp_draw_model(m, 1);
    }
    g_sp_light_mode = 0;   /* 0x7fff, 0x4631, 0x2108 */
}

void SpecialStagePlayer_Draw(SpObj *p) {
    SpModel *m;
    if (p->unk74 & 1)
        return;
    m = p->unkAC;
    m->rot = p->mtx;
    m->trans[0] = p->pos.x;
    m->trans[1] = p->pos.y;
    m->trans[2] = p->pos.z;
    sp_draw_model(&gSpecialStagePlayerResources->model, 0);
    if (gSpecialStagePlayerResources->rhand_cb)
        rhand_capture(p);
    if (p->unk1F0 == 0 && p->unk1F4 <= 0)
        return;
    SpecialStagePlayer_DrawModels(p);
}

/* table of the native functions behind the code pointers of the ROM tables */
typedef struct {
    const char *name;
    SpFn fn;
} SpFuncEnt;
void SpecialStageGimmick_OnDestroyGiveRings(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e634(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e6c0(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e710(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e78c(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e8fc(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230e94c(SpObj *p);
void SpecialStageGimmick_SwayRotation(SpObj *p);
void SpecialStageGimmick_SwayX(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230ed48(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230ee44(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230ef64(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230ef9c(SpObj *p);
void SpecialStageGimmick_Unk_ov001_0230f0f4(SpObj *p);
#define F(n) {#n, (SpFn)(n)}
const SpFuncEnt g_sp_func_table[] = {
    F(SpecialStagePlayer_State_Start0), F(SpecialStagePlayer_State_Start1), F(SpecialStagePlayer_State_Run),
    F(SpecialStagePlayer_State_WaitLand), F(SpecialStagePlayer_State_Land), F(SpecialStagePlayer_State_EdgeJump),
    F(SpecialStagePlayer_State_EdgeLand), F(SpecialStagePlayer_State_AttackFall), F(SpecialStagePlayer_State_TrickJump),
    F(SpecialStagePlayer_State_TrickInput), F(SpecialStagePlayer_State_Trick), F(SpecialStagePlayer_State_TrickEnd),
    F(SpecialStagePlayer_State_TrickChain), F(SpecialStagePlayer_State_Hurt), F(SpecialStagePlayer_State_HurtRecover),
    F(SpecialStagePlayer_State_TargetAttack), F(SpecialStagePlayer_State_AttackRebound),
    F(SpecialStagePlayer_State_Celebrate), F(SpecialStagePlayer_State_CelebrateSpin), F(SpecialStagePlayer_State_Fail),
    F(SpecialStagePlayer_State_GoalSlowDown), F(SpecialStagePlayer_State_Brake), F(SpecialStagePlayer_State_Stopped),
    F(SpecialStageGimmick_OnDestroyGiveRings), F(SpecialStageGimmick_Unk_ov001_0230e634),
    F(SpecialStageGimmick_Unk_ov001_0230e6c0), F(SpecialStageGimmick_Unk_ov001_0230e710),
    F(SpecialStageGimmick_Unk_ov001_0230e78c), F(SpecialStageGimmick_Unk_ov001_0230e8fc),
    F(SpecialStageGimmick_Unk_ov001_0230e94c), F(SpecialStageGimmick_SwayRotation), F(SpecialStageGimmick_SwayX),
    F(SpecialStageGimmick_Unk_ov001_0230ed48), F(SpecialStageGimmick_Unk_ov001_0230ee44),
    F(SpecialStageGimmick_Unk_ov001_0230ef64), F(SpecialStageGimmick_Unk_ov001_0230ef9c),
    F(SpecialStageGimmick_Unk_ov001_0230f0f4), F(SpecialStage_ActObjBounceMove), F(SpecialStage_ActObjFollowParent),
    F(SpecialStage_ActObjSpawnGimmick), F(SpecialStage_ActObjDestroyWhenDone), F(SpecialStage_ActObjReleaseSlots),
    F(SpecialStageTrickReward_Update), F(SpecialStage_Unk_ov001_0230ea6c),
    {NULL, NULL}};
