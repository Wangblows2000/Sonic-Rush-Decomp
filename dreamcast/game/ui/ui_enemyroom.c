/*
 * Enemy-room counter (EnemyRoomHud_*, src/game/UI/Hud/), fed by the objects
 * module's OBJ_FX_ENEMY_ROOM effect (MapObj8C_Action_StartRoom: EnemyRoomHud_Create;
 * EnemyRoom_GetActive(1): unk212 - unk213 enemies left).
 *
 * A main-screen StageObj in screen space (unk58 & 0x100): the label (anim 0) appears
 * at (128, 40), the two digits pop in, the group slides to (212, 12) and counts
 * down; at 0 it vanishes and the finish sprites (anims 0xb / 0xc) show for 0x80
 * frames.  On the Dreamcast it is HUD art: UI_SP_HUD_TC (2x, DS x 128 = screen
 * centre, DS y 0 = under the HUD's top row), so the top-right score stays clear.
 *
 * DS services replaced: StageObj_Draw2D(Ex) -> ui_emit (scale unk44/unk46 / 0x100,
 * 0 = unscaled, as ScorePopup_Draw); ObjDraw_LoadPaletteRow of the finish -> the
 * converted colours of anims 0xb / 0xc; Sound_PlaySeqArcEx(0, 0, id) -> g_ui_hooks.sfx.
 * Not applied: the s8 unk7 OAM offset of the label during appear/vanish.
 */
#include "ui_internal.h"

typedef struct ErWork ErWork;
struct ErWork {
    s32 unk8, unkC;          /* sub state, counter / t */
    s32 unk10, unk14;        /* position, px << 8 */
    s16 unk44, unk46;        /* scale x / y (0x100 = 1, 0 = unscaled) */
    u32 unk54;               /* 4 destroy, 0x1000 tens on, 0x2000 ones on, 0x4000 finish swapped */
    u16 unk5C;               /* 1 flip, 0x20 label hidden */
    s16 unk62;
    UiSpr spr0;              /* 0x094 label */
    UiSpr s0;                /* 0x134 tens (0x17c: the second engine's copy) */
    UiSpr s2;                /* 0x1c4 ones (0x20c) */
    s16 unk254[2], unk258[2], unk25C[2], unk260[2], unk264[2];
    s32 unk268, unk26C;
    u32 unk270;
    s16 unk274, unk276;
    void (*func)(ErWork *w); /* 0x068 */
    void (*draw)(ErWork *w); /* 0x070 */
    int active;
};
static ErWork s_er;

/* EnemyRoom_GetActive(1) as the objects module reports it */
static int s_room_on;
static s32 s_room_need, s_room_done;
static u32 s_room_f20c;

static int EnemyRoom_Left(s32 *left) {
    if (!s_room_on)
        return 0;
    *left = s_room_need - s_room_done;
    if (*left < 0)
        *left = 0;
    return 1;
}

static void er_sfx(s32 id) {
    if (g_ui_hooks.sfx)
        g_ui_hooks.sfx(g_ui_hooks.ctx, 0, id);
}

void EnemyRoomHud_Destroy(void) { s_er.active = 0; }
int EnemyRoomHud_IsActive(void) { return s_er.active; }

static void EnemyRoomHud_UpdateDigitScale(ErWork *w, s32 i) {
    w->unk254[i] = (s16)ObjShift_SetFromRef(w->unk254[i], 0x100, 0x180, 0, 0x10, 0);
    w->unk258[i] = w->unk254[i];
    if (w->unk254[i] != 0x100)
        return;
    if (w->unk264[i] == 0) {
        w->unk25C[i] = 0;
        w->unk260[i] = 0;
        return;
    }
    w->unk25C[i] = g_ui_navi.er_shake[w->unk264[i] - 1][0];
    w->unk260[i] = g_ui_navi.er_shake[w->unk264[i] - 1][1];
    w->unk264[i]--;
}

static void EnemyRoomHud_State_Finish(ErWork *w) {
    w->unk54 |= 0x3000;
    w->unk276++;
    if (w->unk276 > 0x80)
        w->unk54 |= 4;
}

static void EnemyRoomHud_DrawFinish(ErWork *w);
static void EnemyRoomHud_Action_Finish(ErWork *w) {
    w->unk276 = 0;
    w->unkC = 0;
    w->unk8 = 0;
    w->func = EnemyRoomHud_State_Finish;
    w->draw = EnemyRoomHud_DrawFinish;
    /* ObjDraw_LoadPaletteRow(.., 0xb, ..) x2: the finish colours (converted with the anims) */
    UiSpr_SetAnimation(&w->s0, 0xb);
    UiSpr_SetAnimation(&w->s2, 0xc);
    w->s2.flags |= UI_SPR_LOOP;
    w->unk5C |= 4;
    w->unk5C &= ~0x20;
    w->unk54 |= 0x3000;
}

static void EnemyRoomHud_State_Vanish(ErWork *w) {
    w->unk46 = (s16)ObjShift_SetFromRef(w->unk46, 0, 0x100, 0, 0x10, 0);
    w->unk258[0] = (s16)ObjShift_SetFromRef(w->unk258[0], 0, 0x100, 0, 0x10, 0);
    w->unk260[0] = (s8)(((0x100 - w->unk258[0]) * 4) >> 8);
    w->unk258[1] = (s16)ObjShift_SetFromRef(w->unk258[1], 0, 0x100, 0, 0x10, 0);
    w->unk260[1] = (s8)(((0x100 - w->unk258[1]) * 4) >> 8);
    if (w->unk46 != 0)
        return;
    w->unk5C |= 0x20;
    w->unk54 &= ~0x3000u;
    w->unk276++;
    if (w->unk276 > 0x20)
        EnemyRoomHud_Action_Finish(w);
}

static void EnemyRoomHud_Action_Vanish(ErWork *s) {
    s->unk276 = 0;
    s->unkC = 0;
    s->unk8 = 0;
    s->unk46 = 0x100;
    s->func = EnemyRoomHud_State_Vanish;
}

static void EnemyRoomHud_State_Count(ErWork *w) {
    s32 left;
    w->unk26C = w->unk268;
    w->unk268 = EnemyRoom_Left(&left) ? left : 0;
    if (w->unk26C != w->unk268) {
        Hud_ToDecimalDigits(&w->unk270, (u32)w->unk268, 100);
        UiSpr_SetAnimation(&w->s0, (u16)(((w->unk270 & 0xf0) >> 4) + 1));
        w->unk254[0] = w->unk258[0] = 0x180;
        w->unk264[0] = 4;
        w->unk274 = 8;
    }
    if (w->unk274 != 0) {
        w->unk274--;
        if (w->unk274 == 0) {
            UiSpr_SetAnimation(&w->s2, (u16)((w->unk270 & 0xf) + 1));
            w->unk254[1] = w->unk258[1] = 0x180;
            w->unk264[1] = 4;
        }
    }
    if (w->unk264[0] != 0 || w->unk25C[0] != 0)
        EnemyRoomHud_UpdateDigitScale(w, 0);
    if (w->unk264[1] != 0 || w->unk25C[1] != 0)
        EnemyRoomHud_UpdateDigitScale(w, 1);
    if (w->unk268 == 0) {
        if (w->unk276 == 0)
            er_sfx(0xe8);
        w->unk276++;
    }
    if (w->unk276 > 0x50)
        EnemyRoomHud_Action_Vanish(w);
}

static void EnemyRoomHud_Action_Count(ErWork *s) {
    s->unkC = 0;
    s->unk8 = 0;
    s->unk46 = 0;
    s->func = EnemyRoomHud_State_Count;
    s->unk276 = 0;
}

static void EnemyRoomHud_State_Appear(ErWork *w) {
    switch (w->unk8) {
    case 0:
        w->unk46 = (s16)ObjShift_SetFromRef(w->unk46, 0x120, 0, 0, 0x10, 0);
        if (w->unk46 == 0x120) {
            w->unkC = 0x20;
            w->unk8 = 1;
        }
        break;
    case 1:
        w->unk46 = 0x100;
        w->unkC--;
        if (w->unkC == 0) {
            w->unk8 = 2;
            w->unk54 |= 0x1000;
            w->unk254[0] = w->unk258[0] = 0x180;
            w->unk264[0] = 4;
            w->unk254[1] = w->unk258[1] = 0x180;
            w->unk264[1] = 4;
            w->unk276 = 0;
            er_sfx(0xe7);
        }
        break;
    case 2:
        EnemyRoomHud_UpdateDigitScale(w, 0);
        if (w->unk254[0] <= 0x100) {
            if (!(w->unk54 & 0x2000))
                er_sfx(0xe7);
            w->unk54 |= 0x2000;
            EnemyRoomHud_UpdateDigitScale(w, 1);
        }
        if (w->unk264[0] != 0)
            return;
        if (w->unk264[1] != 0)
            return;
        w->unk276++;
        if (w->unk276 > 0x30) {
            w->unk8 = 3;
            w->unkC = 0;
        }
        break;
    case 3:
        w->unkC = ObjShift_SetFromRef(w->unkC, 0x1000, 0, 1, 0, 0);
        w->unk10 = Math_LerpFx32(0xd400, 0x8000, w->unkC);
        w->unk14 = Math_LerpFx32(0xc00, 0x2800, w->unkC);
        if (w->unk10 != 0xd400)
            return;
        if (w->unk14 != 0xc00)
            return;
        w->unk62 = 0x40;
        EnemyRoomHud_Action_Count(w);
        break;
    }
}

static void EnemyRoomHud_Action_Appear(ErWork *s) {
    s->unkC = 0;
    s->unk8 = 0;
    s->unk46 = 0;
    s->func = EnemyRoomHud_State_Appear;
    er_sfx(0xe7);
}

/* StageObj_Draw2DEx(x, y, ..): screen space, scale unk44/unk46 */
static void er_put(ErWork *w, UiSpr *s, s32 x, s32 y, s16 scx, s16 scy, int flip) {
    s32 sx = scx ? (0x1000 * scx) >> 8 : 0x1000, sy = scy ? (0x1000 * scy) >> 8 : 0x1000;
    (void)w;
    s->x = (s16)(x >> 8);
    s->y = (s16)(y >> 8);
    ui_emit(s, UI_SP_HUD_TC, sx, sy, flip ? UI_FLIP_H : 0, 0xffffffff, 0);
}

static void EnemyRoomHud_Draw(ErWork *w) {
    if (!(w->unk5C & 0x20))
        er_put(w, &w->spr0, w->unk10, w->unk14, w->unk44, w->unk46, w->unk5C & 1);
    if (w->unk54 & 0x1000)
        er_put(w, &w->s0, (w->unk10 - 0xb00) + (w->unk25C[0] << 8), (w->unk14 + 0xe00) + (w->unk260[0] << 8),
               w->unk254[0], w->unk258[0], 0);
    if (w->unk54 & 0x2000)
        er_put(w, &w->s2, (w->unk10 + 0xb00) + (w->unk25C[1] << 8), (w->unk14 + 0xe00) + (w->unk260[1] << 8),
               w->unk254[1], w->unk258[1], 0);
}

static void EnemyRoomHud_DrawFinish(ErWork *w) {
    if (w->unk54 & 0x1000)
        er_put(w, &w->s0, 0x8000, 0x3000, 0, 0, 0);
    if (w->unk54 & 0x2000) {
        if (w->unk54 & 0x4000)
            er_put(w, &w->s2, 0x4800, 0x3000, 0, 0, !(w->unk5C & 1));
        else
            er_put(w, &w->s2, 0xb800, 0x3000, 0, 0, w->unk5C & 1);
    }
}

static void EnemyRoomHud_Create(void) {
    ErWork *w = &s_er;
    s32 left;
    memset(w, 0, sizeof(*w));   /* Task_Destroy of a running one + MIi_CpuClear16 */
    w->active = 1;
    UiSpr_Init(&w->spr0, UI_RES_ENEMYROOM, 0, 0);
    UiSpr_Init(&w->s0, UI_RES_ENEMYROOM, 0, 0);
    UiSpr_Init(&w->s2, UI_RES_ENEMYROOM, 0, 0);
    w->draw = EnemyRoomHud_Draw;
    w->unk10 = 0x8000;
    w->unk14 = 0x2800;
    if (EnemyRoom_Left(&left)) {
        w->unk26C = w->unk268 = left;
        if (s_room_f20c & 1)
            w->unk54 |= 0x4000;
    } else {
        w->unk26C = w->unk268 = 0;
    }
    Hud_ToDecimalDigits(&w->unk270, (u32)w->unk268, 100);
    UiSpr_SetAnimation(&w->s0, (u16)(((w->unk270 & 0xf0) >> 4) + 1));
    UiSpr_SetAnimation(&w->s2, (u16)((w->unk270 & 0xf) + 1));
    EnemyRoomHud_Action_Appear(w);
}

void ui_enemy_room_flags(u32 f20c) { s_room_f20c = f20c; }

void ui_enemy_room(s32 a, s32 b) {
    switch (a) {
    case 0:   /* MapObj8C_Action_StartRoom */
        s_room_on = 1;
        s_room_need = b;
        s_room_done = 0;
        EnemyRoomHud_Create();
        break;
    case 1:   /* an enemy of the room defeated */
        s_room_done = b;
        break;
    default:  /* room cleared: unk213 reached unk212 */
        s_room_done = s_room_need;
        break;
    }
}

/* StageObj_Main: state, then draw (run = 0 while paused: draw only) */
void EnemyRoomHud_Frame(int run, const DcPlayer *p) {
    ErWork *w = &s_er;
    if (!w->active)
        return;
    if (run) {
        if (w->unk54 & 4) {
            w->active = 0;
            s_room_on = 0;
            return;
        }
        w->func(w);
        UiSpr_ProcessAnimation(&w->s2);
    }
    g_ui_screen = 0;
    w->draw(w);
    if (run && p && (p->pf & 0x400))   /* gPlayers[0]->unk32c & 0x400 */
        w->unk54 |= 4;
}
