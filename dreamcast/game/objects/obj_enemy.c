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
static void pawn_action_long_attack(Obj *s);
static void pawn_action_attack(Obj *s);
static void pawn_state_walk(Obj *w) {
    s32 y = w->home_x, d;
    s16 spd = 0x100;
    u16 k;
    u32 flags;
    if (w->u23A != 0) {
        if (g_obj_camera[0].water != 0xffff) {
            w->h.move_flags &= ~0x100u;
            w->h.move_flags &= ~0x80u;
            w->h.vel_y = 0;
        }
        if (w->h.ride != NULL)
            y = (s16)((w->h.ride->x >> 8) + w->u23A);
        w->u23C++;
    }
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
        case 7:
        case 0xd:
        case 0x15:
            if (w->u23C <= 200 && w->w8 == 0) {
                pawn_action_long_attack(w);
                return;
            }
            break;
        case 0xa:
            pawn_action_attack(w);
            return;
        case 0xe:
        case 0xf:
            break;
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
    if (w->prm->p9 == 0)
        return;
    o->h.ride = w->h.ride;
    o->h.z = w->h.z;
    if (o->h.z <= -0x1400)
        return;
    o->spr.flags = (o->spr.flags & ~0x60000u) | (1u << 17); /* StageObj_SetOAMPriority(o, 1) */
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

/* --- objdata v3 variants: Egg Lancer (3), Egg Bishop (7, 0x15), Egg Hammer (8), drop-in (p9) */
static dcp_bool easy_slow(void) { /* gSaveData.unk8 & 1 (easy) and not time attack / VS modes */
    return (g_obj_stage.save_flags & 1) && W.mode != 2 && W.mode != 3;
}
static s32 pawn_base_x(Obj *w) { /* the patrol origin; relative to the ridden object (@0x23A) */
    s32 base = w->home_x;
    if (w->u23A != 0 && w->h.ride != NULL)
        base = (s16)(w->u23A + (w->h.ride->x >> 8));
    return base;
}
/* EnemyPawnGroup_State_Charge / _Action_StartCharge (Egg Lancer) */
static void pawn_state_charge(Obj *w) {
    u16 f;
    s16 width = 0x180, sp = 0x100;
    s32 base = pawn_base_x(w);
    if (w->spr.anim == 2) {
        if (w->wC != 0 && --w->wC == 0) {
            if (w->f1F4 & 8) {
                w->h.disp_flags &= ~0x10;
                pawn_action_turn(w);
                return;
            }
            w->h.disp_flags &= ~0x10;
        }
        if (!(w->h.disp_flags & 8))
            return;
        w->wC = 0x30;
        w->f1F4 |= 8;
        obj_disable_colliders_set_anim(w, 3);
        w->h.disp_flags |= 4;
        return;
    }
    f = w->prm->flags;
    if (f & 3)
        width = (s16)(width + ((f & 3) << 7));
    if (f & 3)
        sp = (s16)(sp + (((f & 0xc) >> 2) << 7));
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -((sp * 0x140) >> 8), (sp << 10) >> 8);
    else
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, (sp * 0x140) >> 8, (sp << 10) >> 8);
    if (w->h.move_x - w->h.push_x > 0 && w->h.x > width * (w->prm->p6 + w->prm->p8) + (base << 8))
        w->h.gnd_vel = 0;
    if (w->h.move_x - w->h.push_x < 0 && w->h.x < w->prm->p6 * width + (base << 8))
        w->h.gnd_vel = 0;
    if (w->h.gnd_vel == 0 || (w->h.move_flags & 4)) {
        obj_disable_colliders_set_anim(w, 2);
        w->h.disp_flags |= 0x10;
    }
}
static void pawn_action_start_charge(Obj *p) {
    obj_disable_colliders_set_anim(p, 2);
    p->h.disp_flags |= 0x10;
    p->h.move_flags &= ~0x10u;
    p->f1F4 &= ~8u;
    p->state = pawn_state_charge;
    p->h.gnd_vel = 0;
    p->wC = 0x20;
    p->w8 = 0x78;
    if (easy_slow())
        p->wC += p->wC >> 1;
}
/* EnemyPawnGroup_State_Stomp / _Action_StartStomp (Egg Hammer) */
static void pawn_state_stomp(Obj *p) {
    u32 t0 = p->f1F4;
    if (!(p->f1F4 & 1)) {
        if (p->wC == 0)
            return;
        p->wC--;
        if (p->wC != 0)
            return;
        p->h.disp_flags &= ~0x10;
        p->f1F4 |= 1;
        p->wC = 0x6d;
        if (easy_slow())
            p->wC += p->wC >> 2;
        return;
    }
    if (!(t0 & 4)) {
        if ((p->spr.frame_t >> 4) == 0x2c) {
            obj_screen_shake(8);
            obj_sfx(0x75);
        }
        p->wC--;
        if (p->wC != 0)
            return;
        obj_disable_colliders_set_anim(p, 3);
        p->f1F4 |= 4;
        return;
    }
    if (!(p->h.disp_flags & 8))
        return;
    p->f1F4 = t0 & ~5u;
    pawn_action_walk(p);
}
static void pawn_action_start_stomp(Obj *p) {
    obj_disable_colliders_set_anim(p, 2);
    p->h.disp_flags |= 0x10;
    p->state = pawn_state_stomp;
    p->h.gnd_vel = 0;
    p->wC = 0x10;
    p->w8 = 0xb4;
    p->f1F4 &= ~5u;
    if (easy_slow())
        p->wC += p->wC;
}
/* EnemyPawnGroup_State_LongAttackSpin / _Action_LongAttack (Egg Bishop spin) */
static void pawn_state_long_attack_spin(Obj *w) {
    s16 v = 0x100;
    if (w->prm->flags & 3)
        v = (s16)(v + ((w->prm->flags & 0xc) >> 2) * 0x80);
    w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, (v << 6) >> 8);
    if ((w->spr.frame_t >> 4) == 0xf) {
        w->h.disp_flags ^= 1;
        w->h.obj_flags |= 0x1000;
    }
    if ((w->spr.frame_t >> 4) == 7 && (w->h.obj_flags & 0x1000)) {
        obj_disable_colliders_set_anim(w, 1);
        w->h.obj_flags &= ~0x1000u;
    }
    if (w->h.disp_flags & 8) {
        w->u238--;
        if (w->u238 == 0) {
            rect_set_box2d(&w->detect, -0x78, -0x30, -0x10, -0x10);
            pawn_action_walk(w);
            return;
        }
    }
    obj_handle_collider(&w->h, w, OBJ_OWNER_OBJECT, &w->detect, 1);
}
/* EnemyPawnGroup_State_LongAttackAlt / _Action_LongAttackAlt (Gun Hunter, type 0xd) */
static void pawn_state_long_attack_alt(Obj *p0) {
    if (p0->h.disp_flags & 8) {
        p0->u238--;
        if (p0->u238 == 0) {
            rect_set_box2d(&p0->detect, -120, -48, -16, -16);
            pawn_action_walk(p0);
            return;
        }
    }
    obj_handle_collider(&p0->h, p0, OBJ_OWNER_OBJECT, &p0->detect, 1);
}
static void pawn_action_long_attack_alt(Obj *p0) {
    obj_disable_colliders_set_anim(p0, 2);
    p0->h.move_flags &= ~0x10u;
    p0->h.disp_flags |= 4;
    p0->h.gnd_vel = 0;
    p0->state = pawn_state_long_attack_alt;
    p0->u238 = 2;
    rect_set_box2d(&p0->detect, -120, -48, 0x50, -16);
}
static void pawn_action_long_attack(Obj *s) {
    if (s->prm->type == 0xd) {
        pawn_action_long_attack_alt(s);
        return;
    }
    obj_disable_colliders_set_anim(s, 1);
    s->h.move_flags &= ~0x10u;
    s->h.disp_flags |= 4;
    s->h.obj_flags &= ~0x1000u;
    s->state = pawn_state_long_attack_spin;
    s->u238 = 3;
    rect_set_box2d(&s->detect, -0x78, -0x30, 0x50, -0x10);
}
/* EnemyPawnGroup_State_Attack / _Action_Attack (type 0xa) */
static void pawn_state_attack(Obj *s) {
    if (s->h.disp_flags & 8) {
        pawn_action_walk(s);
        return;
    }
    obj_handle_collider(&s->h, s, OBJ_OWNER_OBJECT, &s->detect, 1);
}
static void pawn_action_attack(Obj *p) {
    obj_disable_colliders_set_anim(p, 3);
    p->h.move_flags &= ~0x10u;
    p->state = pawn_state_attack;
    p->h.gnd_vel = 0;
}
/* EnemyPawnGroup_State_AttackSpawnEggBishop / _Action_ (throws an Egg Bishop, type 0xe4) */
static void pawn_state_attack_spawn_bishop(Obj *w) {
    if ((w->spr.frame_t >> 4) == 0x15) {
        if (!(w->f1F4 & 4)) {
            obj_sfx(0x74);
            pawn_spawn_child(w, 0xe4, 0x30, -0x23);
            g_obj.stats.bishops_spawned++;
        }
        w->f1F4 |= 4;
    }
    if (!(w->f1F4 & 1)) {
        if (w->wC == 0)
            return;
        w->wC--;
        if (w->wC != 0)
            return;
        obj_disable_colliders_set_anim(w, 2);
        w->h.disp_flags |= 4;
        w->f1F4 |= 1;
        w->wC = 0x3a;
        if (easy_slow())
            w->wC += w->wC >> 1;
        return;
    }
    w->wC--;
    if (w->wC == 0) {
        w->f1F4 &= ~5u;
        pawn_action_walk(w);
    }
}
static void pawn_action_attack_spawn_bishop(Obj *p) {
    obj_disable_colliders_set_anim(p, 3);
    p->h.move_flags &= ~0x10u;
    p->state = pawn_state_attack_spawn_bishop;
    p->h.gnd_vel = 0;
    p->wC = 0x10;
    p->w8 = 0x12c;
    p->f1F4 &= ~5u;
    if (easy_slow())
        p->wC += p->wC;
}
/* StageObj_State_EntranceDrop / EnemyPawnGroup_Action_StartEntrance (enemy-room spawn) */
static void StageObj_State_EntranceDrop(Obj *w) {
    switch (w->w8) {
    default:
        w->wC++;
        if (w->wC & 2)
            w->h.disp_flags &= ~0x20;
        else
            w->h.disp_flags |= 0x20;
        if (w->h.move_flags & 1)
            w->w8++;
        break;
    case 0:
        if (w->wC != 0) {
            w->wC--;
        } else {
            w->scale_x = (s16)ObjShift_SetFromRef(w->scale_x, 0x100, 8, 2, 0, 8);
            w->scale_y = (s16)ObjShift_SetFromRef(w->scale_y, 0x100, 0x180, 4, 0, 8);
            w->h.disp_flags &= ~0x20;
        }
        if (w->scale_y == 0x100 && w->scale_x == 0x100) {
            w->w8++;
            w->wC = 0x14;
            w->h.hitstop |= 0x80;
        }
        break;
    case 1:
        w->wC--;
        if (w->wC == 0) {
            w->w8++;
            w->h.move_flags |= 0x80;
            w->h.move_flags &= ~0x2000u;
            w->h.move_flags &= ~0x100u;
        }
        break;
    }
    if (w->w8 == 3) {
        w->h.disp_flags &= ~0x20;
        w->spr.flags &= ~0x200u;
        w->h.obj_flags &= ~2u;
        w->wC = 200;
        w->w8 = 0;
        pawn_action_walk(w);
    }
}
int obj_pawn_in_entrance(const Obj *o) { return o->state == StageObj_State_EntranceDrop; }
static void pawn_action_start_entrance(Obj *p) {
    obj_disable_colliders_set_anim(p, 0);
    p->spr.flags |= 0x200;
    p->h.disp_flags |= 0x10;
    p->h.move_flags |= 0x2000;
    p->h.move_flags &= ~0x80u;
    p->h.move_flags |= 0x100;
    p->h.obj_flags |= 2;
    p->h.disp_flags |= 0x20;
    p->scale_x = 8;
    p->scale_y = 0x180;
    p->wC = 0x18;
    p->w8 = 0;
    p->state = StageObj_State_EntranceDrop;
    EffEntrance_Create(p, 0, -16);
}

/* EnemyPawnGroup_State_DropIn / _Action_StartDropIn (drop onto the GmkTable, p9 = delay) */
static void pawn_state_dropin(Obj *w) {
    if (w->wC != 0) {
        w->wC--;
        if (w->wC & 0x10)
            w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -0x10, 0x40);
        else
            w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, 0x10, 0x40);
        if (w->wC <= 0xc)
            w->h.vel_y = 0;
        if (w->wC == 8)
            w->h.hitstop |= 0x80;
        return;
    }
    w->h.move_flags &= ~0x2000u;
    w->h.move_flags |= 0x80;
    w->h.move_flags &= ~0x100u;
    if (!(w->h.move_flags & 1))
        return;
    if (w->w8 == 0xc) {
        w->h.hitstop |= 0x40;
        if (w->h.ride != NULL)
            w->u23A = (s16)((w->h.x - w->h.ride->x) >> 8);
        if (w->u23A == 0)
            w->u23A = 1;
    }
    if (w->w8 != 0) {
        w->w8--;
        return;
    }
    w->wC = 200;
    /* (!(W.flags & 0x20): spr1 |= 0x80008, the second DS screen sprite: not kept) */
    pawn_action_walk(w);
}
static void pawn_action_start_dropin(Obj *p) {
    obj_disable_colliders_set_anim(p, 0);
    p->h.obj_flags &= ~1u;
    p->h.disp_flags |= 0x10;
    p->h.move_flags &= ~0x80u;
    p->h.move_flags &= ~0x200u;
    p->h.move_flags |= 0x1500;
    if (p->prm->p7 < 0x28)
        p->h.z = -(p->prm->p7 << 8);
    p->spr.flags = (p->spr.flags & ~0x60000u) | 0x20000u;
    p->wC = p->prm->p9;
    p->w8 = 0xc;
    p->state = pawn_state_dropin;
    EffShadow_Create(&p->h);
}

/* ------------------------------------- objdata v4 variants (zones 4..7) */
/* EnemyPawnGroup_State/_Action_AttackSpawnEggSf (type 9: throws an Egg Sf, 0xe6) */
static void pawn_state_attack_spawn_eggsf(Obj *w) {
    if ((w->spr.frame_t >> 4) == 6) {
        if (!(w->f1F4 & 4)) {
            obj_sfx(0x77);
            pawn_spawn_child(w, 0xe6, 0x1c, -0x20);
        }
        w->f1F4 |= 4;
    }
    if ((w->h.disp_flags & 0x10) && w->wC != 0) {
        w->wC--;
        if (w->wC == 0) {
            w->h.disp_flags &= ~0x10;
            w->f1F4 |= 1;
            w->wC = 0x20;
            if (easy_slow())
                w->wC += w->wC >> 1;
        }
    }
    if (!(w->h.disp_flags & 8))
        return;
    w->wC--;
    if (w->wC == 0) {
        w->f1F4 &= ~5u;
        pawn_action_walk(w);
    }
}
static void pawn_action_attack_spawn_eggsf(Obj *p0) {
    obj_disable_colliders_set_anim(p0, 2);
    p0->h.disp_flags |= 0x10;
    p0->h.move_flags &= ~0x10u;
    p0->state = pawn_state_attack_spawn_eggsf;
    p0->h.gnd_vel = 0;
    p0->wC = 0x40;
    p0->w8 = 0x3c;
    p0->f1F4 &= ~5u;
    if (easy_slow())
        p0->wC += p0->wC >> 1;
}
/* EnemyPawnGroup_State/_Action_AttackSpawnGunHunter (type 0xd: three shots, 0xec) */
static void pawn_state_attack_spawn_gun_hunter(Obj *w) {
    switch (w->spr.frame_t >> 4) {
    case 0x2e:
    case 0x30:
    case 0x32:
        obj_sfx(0x79);
        pawn_spawn_child(w, 0xec, 0x28, (s16)((frame_rand() & 3) - 0x20));
        break;
    }
    if (!(w->h.disp_flags & 8))
        return;
    w->f1F4 &= ~5u;
    pawn_action_walk(w);
}
static void pawn_action_attack_spawn_gun_hunter(Obj *p0) {
    obj_disable_colliders_set_anim(p0, 3);
    p0->h.move_flags &= ~0x10u;
    p0->state = pawn_state_attack_spawn_gun_hunter;
    p0->h.gnd_vel = 0;
    p0->wC = 0x18;
    p0->w8 = 0x3c;
    p0->f1F4 &= ~5u;
    if (easy_slow())
        p0->wC += p0->wC >> 1;
}
/* EnemyPawnGroup_State/_Action_Dash, _Brake (type 0xe: Rhino) */
static void pawn_state_dash(Obj *w) {
    u16 f;
    s16 width = 0x180, sp = 0x100;
    s32 base = pawn_base_x(w);
    if (w->spr.anim == 0) {
        if (w->wC == 0)
            return;
        if (--w->wC != 0)
            return;
        w->h.disp_flags &= ~0x10;
        pawn_action_turn(w);
        return;
    }
    f = w->prm->flags;
    if (f & 3)
        width = (s16)(width + ((f & 3) << 7));
    if (f & 3)
        sp = (s16)(sp + (((f & 0xc) >> 2) << 7));
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -((sp << 9) >> 8), (sp << 11) >> 8);
    else
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, (sp << 9) >> 8, (sp << 11) >> 8);
    if (w->h.move_x - w->h.push_x > 0 && w->h.x > width * (w->prm->p6 + w->prm->p8) + (base << 8))
        w->h.gnd_vel = 0;
    if (w->h.move_x - w->h.push_x < 0 && w->h.x < w->prm->p6 * width + (base << 8))
        w->h.gnd_vel = 0;
    if (w->h.gnd_vel == 0) {
        obj_disable_colliders_set_anim(w, 0);
        w->h.disp_flags |= 4;
    }
}
static void pawn_action_dash(Obj *p0) {
    obj_disable_colliders_set_anim(p0, 3);
    p0->h.disp_flags |= 4;
    p0->h.move_flags &= ~0x10u;
    p0->state = pawn_state_dash;
    p0->h.gnd_vel = 0;
    p0->wC = 0x30;
    p0->w8 = 0x78;
    if (easy_slow())
        p0->wC += p0->wC >> 1;
}
static void pawn_state_brake(Obj *w) {
    if ((W.frame & 7) == 0 && (W.flags & 1))
        EffBrakeSmoke_Create(w, (s16)(-0xd - (RAND() & 0xe)), -6);
    if (w->wC != 0) {
        w->wC--;
        return;
    }
    pawn_action_dash(w);
}
static void pawn_action_brake(Obj *p0) {
    obj_disable_colliders_set_anim(p0, 2);
    p0->h.move_flags &= ~0x10u;
    p0->h.disp_flags |= 4;
    p0->state = pawn_state_brake;
    p0->h.gnd_vel = 0;
    p0->w8 = 0x3c;
    p0->wC = 0x20;
    p0->f1F4 &= ~5u;
    if (easy_slow())
        p0->wC += p0->wC >> 1;
}
/* EnemyPawnGroup_State/_Action_Bounced + EnemyPawnGroup_OnHit_Box2_Bounce (Rhino back) */
static void pawn_state_bounced(Obj *s) {
    if (!(s->h.disp_flags & 8))
        return;
    pawn_action_walk(s);
}
static void pawn_action_bounced(Obj *p0) {
    obj_disable_colliders_set_anim(p0, 4);
    p0->h.move_flags &= ~0x10u;
    p0->state = pawn_state_bounced;
    p0->h.gnd_vel = 0;
    p0->h.hitstop = 0x80;
}
static void pawn_on_hit_box2_bounce(ObjBox *a, ObjBox *b) {
    Obj *w = (a && a->owner_kind == OBJ_OWNER_OBJECT) ? (Obj *)a->owner : NULL;
    DcPlayer *p = obj_box_player(b);
    s32 y;
    if (w == NULL || p == NULL)
        return;
    pawn_action_bounced(w);
    y = -0x780;
    y -= w->prm->p7 * 0x180;
    dcp_act_spring(p, 0, (s16)y);
    obj_player_set_source(p, w);
    obj_log("pawn_bounce", w->prm->type, w->h.x >> 8);
}
/* EnemyPawnGroup_OnHit_Box2_PushPlayer (type 0xa: Egg Solid shield) */
static void pawn_on_hit_box2_push_player(ObjBox *a, ObjBox *b) {
    Obj *w = (a && a->owner_kind == OBJ_OWNER_OBJECT) ? (Obj *)a->owner : NULL;
    DcPlayer *p = obj_box_player(b);
    if (w == NULL || p == NULL)
        return;
    if (p->type != 1)
        return;
    if (p->invinc != 0) {
        rect_no_hit(a, b);
        return;
    }
    p->gnd_vel = (s16)(p->gnd_vel < 0 ? -p->gnd_vel : p->gnd_vel);
    if (p->gnd_vel < 0x500)
        p->gnd_vel = 0x500;
    if (p->vel_x != 0) {
        p->vel_x = (s16)(p->vel_x < 0 ? -p->vel_x : p->vel_x);
        if (p->vel_x < 0x500)
            p->vel_x = 0x500;
    }
    if (w->h.x > p->x) {
        p->gnd_vel = (s16)-p->gnd_vel;
        p->vel_x = (s16)-p->vel_x;
    }
    if (w->h.grav_rot != 0) {
        p->gnd_vel = (s16)-p->gnd_vel;
        p->vel_x = (s16)-p->vel_x;
    }
    if (p->vel_y > 0)
        p->vel_y = 0;
    p->dash_timer = 0x10;
    p->hitstop = 0x44;
    w->h.hitstop = 0x44;
    obj_log("pawn_push", w->prm->type, w->h.x >> 8);
}
/* EnemyPawnGroup_State/_Action_JumpAttack (type 0xf: Egg Bunny) */
static void pawn_state_jump_attack(Obj *p0) {
    u32 t0;
    if ((p0->h.move_flags & 1) == 0)
        return;
    t0 = p0->f1F4;
    if (p0->f1F4 & 1) {
        if ((p0->h.disp_flags & 8) == 0)
            return;
        p0->f1F4 = t0 & ~1u;
        pawn_action_turn(p0);
        return;
    }
    p0->h.gnd_vel = 0;
    p0->f1F4 |= 1;
    obj_disable_colliders_set_anim(p0, 2);
}
static void pawn_action_jump_attack(Obj *w) {
    s32 lo, hi, x, ox;
    s16 sc;
    obj_disable_colliders_set_anim(w, 3);
    w->h.move_flags |= 0x10;
    w->h.disp_flags |= 4;
    w->h.move_flags &= ~1u;
    w->state = pawn_state_jump_attack;
    w->h.vel_y = -0x2c0;
    x = w->w8;
    ox = w->home_x;
    sc = 0x180;
    if (w->prm->flags & 3)
        sc = (s16)(sc + ((w->prm->flags & 3) << 7));
    lo = w->prm->p6 * sc + (ox << 8);
    hi = sc * (w->prm->p6 + w->prm->p8) + (ox << 8);
    if (x > hi)
        x = hi;
    if (x < lo)
        x = lo;
    w->h.gnd_vel = (s16)((x - w->h.x) >> 5);
    w->w8 = 0x3c;
    w->wC = 0x20;
    w->f1F4 &= ~1u;
    obj_sfx(0x91);
    obj_log("pawn_jump_attack", w->prm->type, w->h.x >> 8);
}
static void pawn_state_jump_attack_windup(Obj *s) {
    if (s->wC != 0) {
        s->wC--;
        return;
    }
    pawn_action_jump_attack(s);
}
static void pawn_action_start_jump_attack(Obj *p0) {
    DcPlayer *t;
    obj_disable_colliders_set_anim(p0, 2);
    p0->h.move_flags &= ~0x10u;
    p0->state = pawn_state_jump_attack_windup;
    p0->h.gnd_vel = 0;
    t = (DcPlayer *)p0->target;
    if (t != NULL) {
        p0->w8 = t->x;
        p0->target = NULL;
    }
    p0->wC = 0x20;
    if (easy_slow())
        p0->wC += p0->wC >> 1;
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
    case 3:
        pawn_action_start_charge(w);
        return;
    case 7:
    case 0x15:
        pawn_action_attack_spawn_bishop(w);
        return;
    case 8:
        pawn_action_start_stomp(w);
        return;
    /* objdata v4 */
    case 9:
        pawn_action_attack_spawn_eggsf(w);
        return;
    case 0xd:
        pawn_action_attack_spawn_gun_hunter(w);
        return;
    case 0xe:
        pawn_action_brake(w);
        return;
    case 0xf:
        w->target = o;
        pawn_action_start_jump_attack(w);
        break;
    }
}

Obj *pawn_create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    int bac;
    if (p->x0 != 0xff || p->y0 != 0xff) {
        if (p->flags & 0x80) {
            if (g_obj_stage.save_flags & 1) {
                if (W.mode != 2 && W.mode != 3)
                    return NULL;
            }
        }
    }
    /* translated types: Egg Pawn (0x00, 0x14), Egg Pawn M spawner (0x01), Egg Lancer (3),
     * Egg Bishop (7, 0x15), Egg Hammer (8); the zone 5..7 types return NULL */
    switch (p->type) {
    case 0x00: case 0x14: bac = OBJ_BAC_EGGPAWN; break;
    case 0x01: bac = OBJ_BAC_EGGPAWN_M; break;
    case 0x03: bac = OBJ_BAC_EGG_LANCER; break;
    case 0x07: case 0x15: bac = OBJ_BAC_EGG_BISHOP; break;
    case 0x08: bac = OBJ_BAC_EGG_HAMMER; break;
    /* objdata v4 (zones 4..7) */
    case 0x09: bac = OBJ_BAC_EGG_SF; break;
    case 0x0a: bac = OBJ_BAC_EGG_SOLID; break;
    case 0x0d: bac = OBJ_BAC_GUN_HUNTER; break;
    case 0x0e: bac = OBJ_BAC_RHINO; break;
    case 0x0f: bac = OBJ_BAC_EGG_BUNNY; break;
    default: return NULL;
    }
    if (p->p9 != 0 && (p->type == 0 || (u16)(p->type - 0x14) <= 1)) {
        if ((b >> 8) < W.map_left + 0xc4)
            return NULL;
    }
    w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_PAWN;
    w->h.box_l = -8; /* StageObj_SetMapHitbox(w, -8, -10, 8, -3) */
    w->h.box_t = -10;
    w->h.box_r = 8;
    w->h.box_b = -3;
    if (p->type == 20 || p->type == 21)
        w->out_t = -0x100;
    /* drop-in (p9 != 0): the original draws the 3D BAC (eggpawn3d / egg_bishop3d)
     * and hides the 2D sprite (0x80000); the DC draws the 2D sprite (OBJECTS.md, deviations) */
    obj_load_bac(w, bac);
    if (p->type == 8) {
        w->vitality = 2;
        w->vitality -= w->prm->pB;
    }
    switch (p->type) { /* objdata v4 */
    case 10:
        rect_set_atkdef(&w->coll[2], 0x13, 8);
        w->coll[2].flags |= 0x800;
        w->coll[2].cb = pawn_on_hit_box2_push_player;
        break;
    case 13:
        w->f1F4 |= 0x20;
        break;
    case 14:
        w->f1F4 |= 0x20;
        w->vitality = 2;
        w->vitality -= w->prm->pB;
        rect_set_atkdef(&w->coll[2], 0, (u32)-16);
        w->coll[2].cb = pawn_on_hit_box2_bounce;
        w->coll[2].flags |= 0x800;
        break;
    case 15:
        w->h.box_l = -8; /* StageObj_SetMapHitbox(w, -8, -10, 8, 0x20) */
        w->h.box_t = -10;
        w->h.box_r = 8;
        w->h.box_b = 0x20;
        break;
    }
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
    if (w->prm->p9 != 0) {
        pawn_action_start_dropin(w);
    } else if (p->flags & 0x40) {
        pawn_action_start_entrance(w); /* spawned by an enemy room (MapObj8D) */
        w->f1F4 |= 0x40;
    } else {
        w->h.move_flags |= 0x80;
        if (w->prm->type == 7 && (w->prm->flags & 0x20))
            w->h.move_flags &= ~0x80u;
        pawn_action_walk(w);
    }
    w->snd = 1;
    return w;
}
