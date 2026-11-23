/*
 * Battle Play effects: translation of src/game/VsMode
 *   EffVs_Create (direct-attack hit spark), EffVsSlowdown_Create / _State_FollowPlayer,
 *   EffVsConfusion_Create / _State_FollowPlayer, EffVsTensionDrain_Create / _State_Main,
 *   EffVsTensionMax_Create / _State_Main, FixVsPushIcon_Create / _State_FollowPlayer / _Destructor.
 * ac_eff_vs.bac anims: 0 slowdown, 1 confusion, 2 tension max, 3 tension drain, 4 hit spark;
 * ac_fix_vs.bac anim 3: the push icon. The DS kept one push icon (gFixVsPushIconTask, the local
 * player); here each player has one.
 */
#include "obj_internal.h"
#include "obj_zone.h"

static Obj *vs_alloc(int bac, ObjFn dtor) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, dtor ? dtor : obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}
/* the common part of the follow-player effects (unk5C |= 1 (| 4), unk58 |= 0x2100, unk54 |= 0x12) */
static Obj *vs_follow(int anim, int disp4, DcPlayer *p, ObjFn state) {
    Obj *w = vs_alloc(OBJ_BAC_EFF_VS, NULL);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, (u16)anim);
    w->spr.order = 9;
    z_set_prio(w, 1);
    w->h.disp_flags |= 1;
    if (disp4)
        w->h.disp_flags |= 4;
    w->target = p;
    w->h.move_flags |= 0x100 | 0x2000;
    w->h.obj_flags |= 2 | 0x10;
    w->h.x = p->x;
    w->h.y = p->y - 0x2000;
    w->state = state;
    return w;
}

/* EffVs_Create */
Obj *EffVs_Create(s32 x, s32 y) {
    Obj *w = vs_alloc(OBJ_BAC_EFF_VS, NULL);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 4);
    w->spr.order = 9;
    z_set_prio(w, 1);
    w->h.disp_flags |= 1;
    w->h.x = x;
    w->h.y = y;
    w->h.move_flags |= 0x100 | 0x2000;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}

/* EffVsSlowdown_State_FollowPlayer / EffVsConfusion_State_FollowPlayer (timer: unk3C2 / unk3C0) */
static void follow_timer(Obj *w, s16 t) {
    DcPlayer *p = (DcPlayer *)w->target;
    w->h.x = p->x;
    w->h.y = p->y - 0x2000;
    if (t < 0x80) {
        if (t & 2)
            w->h.disp_flags |= 0x20;
        else
            w->h.disp_flags &= ~0x20;
    }
    if (t == 0)
        w->h.obj_flags |= 4;
}
static void EffVsSlowdown_State_FollowPlayer(Obj *w) {
    if (w->target != NULL)
        follow_timer(w, ((DcPlayer *)w->target)->slowdown);
}
static void EffVsConfusion_State_FollowPlayer(Obj *w) {
    if (w->target != NULL)
        follow_timer(w, ((DcPlayer *)w->target)->confuse);
}
/* EffVsSlowdown_Create: none while the player is already slowed */
void EffVsSlowdown_Create(DcPlayer *p) {
    if (p->slowdown != 0)
        return;
    vs_follow(0, 1, p, EffVsSlowdown_State_FollowPlayer);
}
/* EffVsConfusion_Create */
void EffVsConfusion_Create(DcPlayer *p) {
    if (p->confuse != 0)
        return;
    vs_follow(1, 1, p, EffVsConfusion_State_FollowPlayer);
}

/* EffVsTensionDrain_State_Main: rises from the player, x scale 0x100 -> 0x200, y 0x100 -> 8, twice */
static void EffVsTensionDrain_State_Main(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    if (p == NULL)
        return;
    if (w->scale_x == 0x200 && w->scale_y == 8) {
        w->scale_x = 0x100;
        w->scale_y = 0x100;
        w->wC = 0;
        w->w8++;
    }
    w->h.x = p->x;
    w->h.y = (p->y - 0x2000) + (w->wC << 7);
    w->wC++;
    w->scale_x = (s16)ObjShift_Set(w->scale_x, 0x200, 4, 0, 8);
    w->scale_y = (s16)ObjShift_Set(w->scale_y, 8, 4, 0, 8);
    if ((u32)w->w8 > 1)
        w->h.obj_flags |= 4;
}
void EffVsTensionDrain_Create(DcPlayer *p) {
    Obj *w;
    if (p->slowdown != 0)   /* (sic: the original tests unk3C2) */
        return;
    w = vs_follow(3, 0, p, EffVsTensionDrain_State_Main);
    if (w == NULL)
        return;
    w->scale_x = 0x100;
    w->scale_y = 0x100;
}

/* EffVsTensionMax_State_Main: rises above the player, x scale 0x100 -> 8, y 0x100 -> 0x180, twice */
static void EffVsTensionMax_State_Main(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    if (p == NULL)
        return;
    if (w->scale_x == 8 && w->scale_y == 0x180) {
        w->scale_x = 0x100;
        w->scale_y = 0x100;
        w->wC = 0;
        w->w8++;
    }
    w->h.x = p->x;
    w->h.y = (p->y - 0x1000) - w->wC * 0x140;
    w->wC++;
    w->scale_x = (s16)ObjShift_Set(w->scale_x, 8, 4, 0, 8);
    w->scale_y = (s16)ObjShift_Set(w->scale_y, 0x180, 4, 0, 8);
    if ((u32)w->w8 > 1)
        w->h.obj_flags |= 4;
}
void EffVsTensionMax_Create(DcPlayer *p) {
    Obj *w;
    if (p->slowdown != 0)   /* (sic: unk3C2) */
        return;
    w = vs_follow(2, 0, p, EffVsTensionMax_State_Main);
    if (w == NULL)
        return;
    w->spr.flags |= 0x200;
    w->scale_x = 0x100;
    w->scale_y = 0x100;
}

/* FixVsPushIcon: shown over the player while the rival pushes it (pf 0x10000), gone 4 frames after */
static Obj *s_push_icon[2];
static DcPlayer *s_push_owner[2];
static void FixVsPushIcon_State_FollowPlayer(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    if (p == NULL)
        return;
    w->h.x = p->x;
    w->h.y = p->y - 0x2000;
    if (!(p->pf & 0x10000)) {
        w->wC++;
        if (w->wC > 4)
            w->h.obj_flags |= 4;
        return;
    }
    w->wC = 0;
}
static void FixVsPushIcon_Destructor(Obj *w) {
    int i;
    for (i = 0; i < 2; i++)
        if (s_push_icon[i] == w)
            s_push_icon[i] = NULL;
    obj_stageobj_dtor(w);
}
void FixVsPushIcon_Create(DcPlayer *p) {
    Obj *w;
    int i, slot = -1;
    for (i = 0; i < 2; i++) {
        if (s_push_icon[i] != NULL && s_push_owner[i] == p)
            return;   /* gFixVsPushIconTask != NULL */
        if (s_push_icon[i] == NULL && slot < 0)
            slot = i;
    }
    if (slot < 0)
        return;
    w = vs_alloc(OBJ_BAC_FIX_VS, FixVsPushIcon_Destructor);
    if (w == NULL)
        return;
    s_push_icon[slot] = w;
    s_push_owner[slot] = p;
    w->spr.order = 3;
    z_set_prio(w, 0);
    obj_set_anim(w, 3);
    w->h.disp_flags |= 4 | 1;
    w->h.x = p->x;
    w->h.y = p->y - 0x2000;
    w->target = p;
    w->h.move_flags |= 0x100 | 0x2000;
    w->h.obj_flags |= 2 | 0x10;
    w->state = FixVsPushIcon_State_FollowPlayer;
}
/* a new stage: the pool was cleared */
void obj_vs_reset(void) {
    s_push_icon[0] = s_push_icon[1] = NULL;
    s_push_owner[0] = s_push_owner[1] = NULL;
}

/* ------------------------------------------------ Battle Play player-box queries */
int objects_player_box_hit(const DcPlayer *p, s32 x, s32 y, s32 ox, s32 oy, u16 w, u16 h) {
    ObjPlayerCtx *pc = obj_player_ctx(p);
    return pc != NULL && rect_box_check(&pc->body, x, y, ox, (s16)oy, w, h);
}
s32 objects_player_box_cx(const DcPlayer *a, const DcPlayer *b) {
    ObjPlayerCtx *pa = obj_player_ctx(a), *pb = obj_player_ctx(b);
    return (pa && pb) ? rect_hit_center_x(&pa->body, &pb->body) : (a->x + b->x) / 2;
}
s32 objects_player_box_cy(const DcPlayer *a, const DcPlayer *b) {
    ObjPlayerCtx *pa = obj_player_ctx(a), *pb = obj_player_ctx(b);
    return (pa && pb) ? rect_hit_center_y(&pa->body, &pb->body) : (a->y + b->y) / 2;
}
