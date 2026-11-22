/*
 * Enemies and their destruction, faithful translations of
 *   src/game/Enemies/EnemyPawnGroup_* (Egg Pawn types 0x00 / 0x14, Egg Pawn M 0x01)
 *   src/game/Stage/ObjectSystem/MapObject_OnHit_Default, _Action_KnockAway,
 *     _State_KnockedAway, _SpawnDestroyBurst, _SpawnFreedAnimal
 *   src/game/Effects/EffZakoBombBurst_Create, EffZakoDebris_*,
 *     Effect_SetMoveUntilAnimEndState / Effect_State_MoveUntilAnimEnd
 */
#include <string.h>
#include "obj_internal.h"

/* ============================================================== effects */
static void eff_state_move_until_anim_end(Obj *w) {
    if (w->wC != 0) {
        if (w->h.disp_flags & 1)
            w->h.vel_x = (s16)(w->h.vel_x - (s16)(w->wC >> 16));
        else
            w->h.vel_x = (s16)(w->h.vel_x + (s16)(w->wC >> 16));
        w->h.vel_y = (s16)(w->h.vel_y + (s16)w->wC);
    }
    if (w->h.disp_flags & 8)
        w->h.obj_flags |= 4; /* + StageObj_ReleaseSpriteVram */
}
void eff_set_move_until_anim_end(Obj *s) { s->state = eff_state_move_until_anim_end; }

static Obj *eff_new(int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}

/* EffZakoBombBurst_Create */
Obj *eff_zako_bomb_burst(Obj *arg, s16 x, s16 y) {
    Obj *w = eff_new(OBJ_BAC_ZAKO_BOMB);
    if (w == NULL)
        return NULL;
    w->spr.order = 9;
    if (arg->h.grav_rot != 0)
        w->spr.flags |= 0x200;
    if (arg->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->h.z = arg->h.z;
    if (W.objmgr & 0x100)
        w->spr.flags = (w->spr.flags & ~0x60000u) | (1u << 17);
    w->h.angle = arg->h.grav_rot;
    dcp_rotate_vec(&x, &y, arg->h.grav_rot);
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x100;
    eff_set_move_until_anim_end(w);
    return w;
}

static void debris_state_main(Obj *w) {
    if (w->wC == 0) {
        if (w->h.move_flags & 1) {
            w->h.vel_y = (s16)-(w->h.move_y >> 1);
            w->h.vel_x >>= 1;
            w->h.move_flags |= 0x10;
            w->wC = 1;
            w->h.hitstop = 0x40;
            w->h.move_flags &= ~1u;
        }
    } else if ((w->h.move_flags & 1) && !(w->h.move_flags & 0x2000)) {
        w->h.hitstop = 0x40;
        w->h.move_flags |= 0x2100;
        w->h.vel_x = 0;
        w->h.vel_y = 0;
    }
    if (!(w->h.move_flags & 0x2000))
        return;
    w->w8++;
    if (w->w8 > 0x20) {
        if (w->w8 & 2)
            w->h.disp_flags |= 0x20;
        else
            w->h.disp_flags &= ~0x20;
    }
    if (w->w8 > 0x30)
        w->h.obj_flags |= 4;
}

/* EffZakoDebris_Create */
Obj *eff_zako_debris(Obj *arg, s16 x, s16 y, s16 vx, s16 vy, u8 type) {
    Obj *w = eff_new(OBJ_BAC_HAHEN);
    dcp_bool flag = DCP_FALSE;
    if (w == NULL)
        return NULL;
    if (type >= 4) {
        if (type != 8)
            flag = DCP_TRUE;
        if (type <= 7)
            type -= 4;
        else
            type = (u8)(RAND() & 3);
    }
    if (type <= 3)
        obj_set_anim(w, type);
    w->spr.order = 9;
    if (arg->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->h.grav_rot = arg->h.grav_rot;
    dcp_rotate_vec(&x, &y, arg->h.grav_rot);
    w->h.vel_x = vx;
    w->h.vel_y = vy;
    if (!(w->h.disp_flags & 1)) {
        w->h.x += x << 8;
    } else {
        w->h.x -= x << 8;
        w->h.vel_x = (s16)-w->h.vel_x;
    }
    w->h.y += y << 8;
    if (flag) {
        w->h.vel_x = (s16)(w->h.vel_x + (s16)(0x400 - (RAND() & 0x7ff)));
        w->h.vel_y = (s16)(w->h.vel_y + (s16)-(RAND() & 0x3ff));
        w->h.x += (s16)(0x1000 - (RAND() & 0x1fff));
        w->h.y += (s16)(0x1000 - (RAND() & 0x1fff));
    }
    w->h.gravity = 0x2a;
    w->h.fall_max = 0xf00;
    if (arg->h.obj_flags & 1)
        w->h.obj_flags |= 1;
    w->h.box_l = -2; /* StageObj_SetMapHitbox(-2, -4, 2, 4) */
    w->h.box_t = -4;
    w->h.box_r = 2;
    w->h.box_b = 4;
    w->h.move_flags |= 0x280;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x100;
    w->h.move_flags |= 0x10; /* EffZakoDebris_Action_Init */
    w->state = debris_state_main;
    return w;
}

/* MapObject_SpawnDestroyBurst */
void mapobject_spawn_destroy_burst(Obj *w) {
    u8 r;
    Obj *o;
    obj_sfx(0x33);
    eff_zako_bomb_burst(w, 0, 0);
    r = (u8)(RAND() & 3);
    o = eff_zako_debris(w, 0, -0x18, 0, -0x200, (u8)(r + 4));
    if (o != NULL && ((w->h.move_flags & 0x1000) || (w->f1F4 & 0x40)))
        o->h.move_flags |= 0x100;
    o = eff_zako_debris(w, 0, -0x18, 0, -0x200, (u8)(((r + 1) & 3) + 4));
    if (o != NULL && ((w->h.move_flags & 0x1000) || (w->f1F4 & 0x40)))
        o->h.move_flags |= 0x100;
}

/* MapObject_SpawnFreedAnimal: the freed animal is a decoration (DecAnimal) */
void mapobject_spawn_freed_animal(Obj *w) {
    s16 dy;
    u8 kind;
    Obj *o;
    if (w->f1F4 & 0x20)
        return;
    if (W.zone < 8)
        kind = g_obj.data.animal[W.zone][RAND() % 3];
    else
        kind = (u8)(RAND() & 0xf);
    dy = 0x800;
    dcp_rotate_vec(NULL, &dy, w->h.grav_rot);
    o = Decoration_Spawn(kind, w->h.x, w->h.y - dy);
    obj_log("freed_animal", kind, w->h.x >> 8);
    if (o == NULL)
        return;
    o->h.grav_rot = w->h.grav_rot;
    if (!(w->h.move_flags & 0x1000) && !(w->f1F4 & 0x40))
        return;
    if (w->f1F4 & 0x40)
        o->w8 = 1;
    else
        o->h.move_flags |= 0x100;
}

/* MapObject_State_KnockedAway */
static void mapobject_state_knocked_away(Obj *p) {
    p->h.angle = (u8)(p->h.angle + 0x10);
    if (p->wC == 0) {
        mapobject_spawn_destroy_burst(p);
        mapobject_spawn_freed_animal(p);
        p->h.obj_flags |= 4;
    }
    p->wC--;
}

/* MapObject_Action_KnockAway (boost hitbox kill; see OBJECTS.md) */
void mapobject_knock_away(Obj *s, DcPlayer *o) {
    s16 sp;
    u16 a;
    if (s->prm->type != 0xb1 && s->prm->type != 0xb2)
        obj_disable_colliders_set_anim(s, 0);
    s->h.disp_flags |= 0x10;
    s->h.disp_flags |= 0x100;
    s->h.move_flags |= 0x10;
    s->h.move_flags |= 0x100;
    s->h.obj_flags |= 0x10;
    s->state = mapobject_state_knocked_away;
    s->h.gnd_vel = 0;
    sp = (s16)(0x37f - (RAND() & 0x1fe));
    s->h.vel_x = (s16)(o->x - o->px);
    s->h.vel_y = (s16)(o->y - o->py);
    a = (u16)(dcp_atan2(o->move_y, o->move_x) - 0x4000);
    if (o->disp_flags & 1)
        a = (u16)(a + 0x8000);
    s->h.vel_x = (s16)(s->h.vel_x + (s16)((sp * dcp_cos(a)) >> 12));
    s->h.vel_y = (s16)(s->h.vel_y + (s16)((sp * dcp_sin(a)) >> 12));
    obj_sfx(0x7a);
    s->wC = 0x20;
    s->w8 = 0;
    s->f1F4 &= ~5u;
}

/* MapObject_OnHit_Default (coll0: a = the object's box, b = the attacker) */
void mapobject_on_hit_default(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *pl = obj_box_player(b);
    if (w == NULL)
        return;
    if (w->vitality == 0) {
        if ((u16)(w->prm->type - 0xb1) <= 1) {
            if (pl == NULL || pl->type != 1) {
                w->h.obj_flags |= 2;
                w->coll[0].flags |= 0x1000;
                w->coll[1].flags |= 0x1000;
                w->coll[2].flags |= 0x1000;
            } else {
                rect_no_hit(a, b);
                return;
            }
        } else {
            if (!(w->h.move_flags & 0x1000))
                w->f1F4 |= 0x10;
            w->h.obj_flags |= 2;
            w->coll[0].flags |= 0x1000;
            w->coll[1].flags |= 0x1000;
            w->coll[2].flags |= 0x1000;
        }
        if (pl != NULL && pl->type == 1) {
            obj_player_add_tension_combo(pl, 400);
        } else if (b->owner_kind == OBJ_OWNER_OBJECT && b->owner != NULL &&
                   ((Obj *)b->owner)->h.type == 5 && ((Obj *)b->owner)->k.atk.par != NULL) {
            /* attacker = AttackHitbox task: pl = pl->unk88 (the player) */
            pl = ((Obj *)b->owner)->k.atk.par;
            if (pl->type == 1) {
                obj_player_add_tension_combo(pl, 400);
                mapobject_knock_away(w, pl);
                if (pl->pf & 0x80) {
                    pl->hitstop = 0x44;
                    w->h.hitstop = 0x44;
                }
                g_obj.stats.enemies_destroyed++;
                g_obj.stats.knock_aways++;
                obj_log("enemy_knocked_away", w->prm->type, w->h.x >> 8);
                return;
            }
        }
        mapobject_spawn_destroy_burst(w);
        mapobject_spawn_freed_animal(w);
        w->h.obj_flags |= 8;
        g_obj.stats.enemies_destroyed++;
        obj_log("enemy_destroyed", w->prm->type, w->h.x >> 8);
    } else {
        /* EffVs_Create x3 at the hit centre (VS effect, not translated); keep the RNG draws */
        (void)rect_hit_center_x(a, b);
        RAND(); RAND();
        RAND(); RAND();
        obj_sfx(0x7a);
        /* EffVitality_Create: HUD effect, not translated */
        w->vitality--;
        w->prm->pB++;
        w->flash = 0x3c;
        w->coll[1].atk &= ~7u;
        if (pl != NULL && pl->type == 1) {
            obj_player_attack_recoil(pl);
            return;
        }
    }
    if (pl == NULL || pl->type != 1)
        return;
    obj_player_attack_bounce(pl);
}

/* ==================================================== EnemyPawnGroup */
static void pawn_action_walk(Obj *p);
static void pawn_action_turn(Obj *p);

static u16 frame_rand(void) { return (u16)((((u32)W.frame << 16) * 0x196225u + 0x3c6ef35fu) >> 16); }

static void pawn_state_turn(Obj *p) {
    s16 spd = 0x100;
    u16 f = p->prm->flags;
    if (f & 3)
        spd = (s16)(spd + (((f & 0xc) >> 2) << 7));
    p->h.gnd_vel = (s16)dcp_spd_down(p->h.gnd_vel, (spd << 6) >> 8);
    if (!(p->h.disp_flags & 8))
        return;
    p->h.disp_flags ^= 1;
    pawn_action_walk(p);
}
static void pawn_action_turn(Obj *p) {
    obj_disable_colliders_set_anim(p, 1);
    p->h.move_flags &= ~0x10u;
    p->state = pawn_state_turn;
    p->h.gnd_vel = 0;
    if ((p->f1F4 & 1) == 0)
        p->wC += 0x18;
}
static void pawn_state_walk(Obj *w) {
    s32 y = w->home_x, d;
    s16 spd = 0x100;
    u16 k;
    u32 flags;
    /* (unk23A != 0: child of a moving parent, not used by these types) */
    k = w->prm->type;
    if (k == 7 || k == 0x15)
        spd -= 0x40;
    if (k == 8)
        spd -= 0x80;
    if (k == 0xf)
        spd += 0x300;
    flags = w->f1F4;
    if (flags & 4) {
        w->wC--;
        if (w->wC == 0) {
            w->f1F4 &= ~4u;
            w->wC = 200;
            pawn_action_turn(w);
            return;
        }
    } else if (flags & 1) {
        w->wC--;
        if (w->wC == 0) {
            w->f1F4 &= ~1u;
            w->wC = (frame_rand() & 0x1f) + 0x40;
            w->f1F4 |= 4;
        }
    } else if (w->wC != 0) {
        w->wC--;
    } else if (!(frame_rand() & 0x1ff) && k != 8) {
        switch (k) {
        case 7: case 0xd: case 0x15: case 0xa: case 0xe: case 0xf:
            break; /* (LongAttack / Attack: types not translated) */
        default:
            w->f1F4 |= 1;
            pawn_action_turn(w);
            return;
        }
    }
    if (w->prm->flags & 3)
        spd = (s16)(spd + (((w->prm->flags & 0xc) >> 2) << 7));
    if (w->f1F4 & 5) {
        w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, (spd << 6) >> 8);
        obj_handle_collider(&w->h, w, OBJ_OWNER_OBJECT, &w->detect, 1);
        return;
    }
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -((spd * 10) >> 8), (spd * 0xc0) >> 8);
    else
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, (spd * 10) >> 8, (spd * 0xc0) >> 8);
    if (w->u23C <= 200 || !(w->h.disp_flags & 1)) {
        d = w->h.move_x - w->h.push_x;
        if (d > 0 && w->h.x > (w->prm->p8 + (y + w->prm->p6)) << 8) {
            pawn_action_turn(w);
            return;
        }
        if (d < 0 && w->h.x < (y + w->prm->p6) << 8) {
            pawn_action_turn(w);
            return;
        }
    }
    if (w->h.move_flags & 4) {
        pawn_action_turn(w);
        return;
    }
    if (w->u23C <= 200) {
        if (w->w8 != 0)
            w->w8--;
        else
            obj_handle_collider(&w->h, w, OBJ_OWNER_OBJECT, &w->detect, 1);
    }
    if (w->prm->type == 0xf && (w->spr.frame_t >> 4) > 0x13)
        w->h.gnd_vel = 0;
    if (w->h.move_flags & 1)
        return;
    if (w->h.move_flags & 0x80)
        w->h.gnd_vel = 0;
}
static void pawn_action_walk(Obj *p) {
    obj_disable_colliders_set_anim(p, 0);
    p->h.disp_flags |= 4;
    p->h.move_flags &= ~0x10u;
    p->h.move_flags &= ~0x100u;
    p->h.disp_flags &= ~0x10;
    p->state = pawn_state_walk;
    if (p->f1F4 & 1)
        p->wC = 0x10;
}

static void pawn_state_chase(Obj *w) {
    u16 f;
    s16 width = 0x180, sp = 0x100;
    s32 base = w->home_x, lim;
    f = w->prm->flags;
    if (f & 3)
        width = (s16)(width + ((f & 3) << 7));
    if (f & 3)
        sp = (s16)(sp + (((f & 0xc) >> 2) << 7));
    if (w->target != NULL) {
        if (((DcPlayer *)w->target)->x > w->h.x)
            w->h.disp_flags &= ~1;
        else
            w->h.disp_flags |= 1;
        if (obj_gravity_flipped(w->h.x, w->h.y))
            w->h.disp_flags ^= 1;
    }
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -((sp * 0x1a) >> 8), (sp << 9) >> 8);
    else
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, (sp * 0x1a) >> 8, (sp << 9) >> 8);
    if (w->h.move_x - w->h.push_x > 0) {
        lim = width * (w->prm->p6 + w->prm->p8) + (base << 8);
        if (w->h.x > lim) {
            w->h.x = lim;
            w->h.gnd_vel = 0;
            obj_disable_colliders_set_anim(w, 3);
        } else if (!(w->h.disp_flags & 4)) {
            obj_disable_colliders_set_anim(w, 2);
            w->h.disp_flags |= 4;
        }
    }
    if (w->h.move_x - w->h.push_x < 0) {
        lim = w->prm->p6 * width + (base << 8);
        if (w->h.x < lim) {
            w->h.x = lim;
            w->h.gnd_vel = 0;
            obj_disable_colliders_set_anim(w, 3);
        } else if (!(w->h.disp_flags & 4)) {
            obj_disable_colliders_set_anim(w, 2);
            w->h.disp_flags |= 4;
        }
    }
    if (--w->wC == 0)
        pawn_action_turn(w);
}
static void pawn_action_chase(Obj *p) {
    obj_disable_colliders_set_anim(p, 2);
    p->h.disp_flags |= 4;
    p->h.move_flags &= ~0x10u;
    p->state = pawn_state_chase;
    p->wC = 0x12c;
    p->w8 = 0x78;
    if ((g_obj_stage.save_flags & 1) == 0 || W.mode == 2 || W.mode == 3)
        return;
    p->wC -= p->wC >> 1;
}
static void pawn_state_chase_windup(Obj *s) {
    s->wC--;
    if (s->wC != 0)
        return;
    pawn_action_chase(s);
}
static void pawn_action_start_chase(Obj *p) {
    obj_disable_colliders_set_anim(p, 3);
    p->state = pawn_state_chase_windup;
    p->h.gnd_vel = 0;
    p->wC = 0x20;
    if ((g_obj_stage.save_flags & 1) == 0 || W.mode == 2 || W.mode == 3)
        return;
    p->wC += p->wC >> 1;
}

/* EnemyPawnGroup_SpawnChild */
static void pawn_spawn_child(Obj *w, s32 type, s16 dx, s16 dy) {
    u16 f = 0;
    Obj *o;
    if (w->h.disp_flags & 1)
        f = w->h.disp_flags & 1;
    f |= w->prm->flags & 0xc;
    o = obj_mapobject_spawn(type, w->h.x, w->h.y, (s16)f, 0, 0, 0, 0);
    if (o == NULL) {
        obj_log("spawn_child_failed", type, w->h.x >> 8); /* EggPawnM_Create not translated */
        return;
    }
    o->h.grav_rot = w->h.grav_rot;
    dcp_rotate_vec(&dx, &dy, w->h.grav_rot);
    if (o->h.disp_flags & 1)
        o->h.x -= dx << 8;
    else
        o->h.x += dx << 8;
    o->h.y += dy << 8;
}
static void pawn_state_attack_spawn_m(Obj *p) {
    if ((p->spr.frame_t >> 4) == 6) {
        if ((p->f1F4 & 4) == 0) {
            obj_sfx(0x78);
            pawn_spawn_child(p, 0xdf, 0x10, -32);
        }
        p->f1F4 |= 4;
    }
    if (p->h.disp_flags & 0x10) {
        if (p->wC != 0) {
            p->wC--;
            if (p->wC == 0) {
                p->h.disp_flags &= ~0x10;
                p->f1F4 |= 1;
                p->wC = 0x20;
            }
        }
    }
    if ((p->h.disp_flags & 8) == 0)
        return;
    p->wC--;
    if (p->wC != 0)
        return;
    p->f1F4 &= ~5u;
    pawn_action_walk(p);
}
static void pawn_action_attack_spawn_m(Obj *p) {
    obj_disable_colliders_set_anim(p, 2);
    p->h.disp_flags |= 0x10;
    p->h.move_flags &= ~0x10u;
    p->state = pawn_state_attack_spawn_m;
    p->h.gnd_vel = 0;
    p->wC = 0x40;
    p->w8 = 0x3c;
    p->f1F4 &= ~5u;
    if ((g_obj_stage.save_flags & 1) == 0 || W.mode == 2 || W.mode == 3)
        return;
    p->wC += p->wC >> 1;
    p->w8 += p->wC >> 1;
}

/* EnemyPawnGroup_OnDetect (detect box @0x20C: a = detect box, b = player body) */
static void pawn_on_detect(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *o = obj_box_player(b);
    if (w == NULL || o == NULL)
        return;
    if (w->h.obj_flags & 2)
        return;
    obj_sfx(0x79);
    EffFound_Create(w, 0, -0x40);
    w->target = o;
    if (w->h.x > o->x)
        w->h.disp_flags |= 1;
    else
        w->h.disp_flags &= ~1;
    if (w->h.grav_rot != 0)
        w->h.disp_flags ^= 1;
    obj_log("pawn_detect", w->prm->type, w->h.x >> 8);
    switch (w->prm->type) {
    case 0:
    case 0x14:
    case 10:
        pawn_action_start_chase(w);
        return;
    case 1:
        pawn_action_attack_spawn_m(w);
        return;
    }
}

Obj *pawn_create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    if (p->x0 != 0xff || p->y0 != 0xff) {
        if (p->flags & 0x80) {
            if (g_obj_stage.save_flags & 1) {
                if (W.mode != 2 && W.mode != 3)
                    return NULL;
            }
        }
    }
    /* translated: Egg Pawn (0x00, 0x14) and Egg Pawn M spawner (0x01), 2D, walking start */
    if (p->type != 0 && p->type != 1 && p->type != 0x14)
        return NULL;
    if (p->p9 != 0 || (p->flags & 0x40))
        return NULL; /* drop-in (3D BAC) / entrance variants not translated */
    w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_PAWN;
    w->h.box_l = -8; /* StageObj_SetMapHitbox(w, -8, -10, 8, -3) */
    w->h.box_t = -10;
    w->h.box_r = 8;
    w->h.box_b = -3;
    if (p->type == 20)
        w->out_t = -0x100;
    obj_load_bac(w, p->type == 1 ? OBJ_BAC_EGGPAWN_M : OBJ_BAC_EGGPAWN);
    w->spr.order = 0x14;
    rect_set_box2d(&w->detect, -0x78, -0x30, -16, -16);
    w->detect.flags |= 0xc0;
    rect_set_atkdef(&w->detect, 0, (u32)-16);
    w->detect.mask = 0x12;
    w->detect.cb = pawn_on_detect;
    w->detect.owner = w;
    w->detect.owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x40;
    if (w->prm->flags & 0x10)
        w->h.disp_flags &= ~1;
    if (w->prm->p6 == 0 && w->prm->p8 == 0) {
        w->prm->p6 = -0x28;
        w->prm->p8 = 0x50;
    }
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.grav_rot = 0x80;
        w->h.disp_flags |= 0x100;
    }
    w->wC = 200;
    w->h.move_flags |= 0x80;
    pawn_action_walk(w);
    w->snd = 1;
    return w;
}
