/*
 * GAME OVER / TIME'S UP sequence, translated from src/game/UI/GameOver:
 *   GameOverSeq_StartGameOver, GameOverSeq_StartTimeOver, GameOverSeq_Main,
 *   GameOverSeq_Destructor, GameOver_Create, GameOver_State_Animate,
 * and src/game/Graphics/Screen ScreenShake_Create/_Start/_Stop/_Main/_GetOffset*
 * (local copy for the sequence; the in-stage shake belongs to the camera code).
 *
 * The letters are StageObj tasks (ac_fix_gameover anims) in screen space; their
 * position is the StageObj x/y (24.8), drawn in UI_SP_SCREEN.
 * Raw KEYINPUT reads ("any of A, B, START, X, Y held") -> InputState.held.
 * DISPCNT/palette writes that blank the stage -> a black backdrop quad.
 */
#include "ui_internal.h"

#define GO_MAX 20

typedef struct GoLetter {
    s32 state;       /* 0x08 */
    s32 unkc;        /* 0x0C delay, then lerp t */
    s32 unk10, unk14; /* x, y (24.8) */
    s32 unk1c, unk20; /* target x (24.8), centre x (px) */
    s16 unk2e, unk30; /* index, y velocity */
    s16 unk62;
    UiSpr spr;
    int used;
} GoLetter;

typedef struct ShakeWork { u16 flags; s16 t; u8 mode; s8 x, y; } ShakeWork;

static struct {
    u16 timer;
    GoLetter l[GO_MAX];
    ShakeWork shake;
    int active;
} s_go;

int GameOverSeq_IsActive(void) { return s_go.active; }
void GameOverSeq_Destroy(void) {
    /* GameOverSeq_Destructor: NNS_SndStopSoundAll + Sound_ResetHeap */
    if (s_go.active && g_ui_hooks.music)
        g_ui_hooks.music(g_ui_hooks.ctx, -1);
    s_go.active = 0;
}

/* ---------------------------------------------------------- ScreenShake */
static void ScreenShake_Stop(void) {
    ShakeWork *w = &s_go.shake;
    w->x = 0;
    w->y = 0;
    w->mode = 0;
    w->flags = 0;
    w->t = 0;
}
static void ScreenShake_Start(u8 v) {
    ShakeWork *w = &s_go.shake;
    if (v == 0) {
        ScreenShake_Stop();
        return;
    }
    if (w->mode <= v) {
        w->mode = v;
        w->flags = 1;
        w->t = 0;
    }
}
static void ScreenShake_Main(void) {
    ShakeWork *w = &s_go.shake;
    const s8 *t3c = g_ui_data.shake, *t40 = g_ui_data.shake + 4, *t4c = g_ui_data.shake + 16;
    w->x = 0;
    w->y = 0;
    if (!(w->flags & 1))
        return;
    switch (w->mode) {
    case 1:
        if (w->t > 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 5:
        if (w->t >= 12) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 9:
        if (w->t >= 12)
            w->t = 0;
        w->x = (s8)((t40[w->t] & 0xf) >> 1);
        w->y = (s8)(t40[w->t] >> 5);
        break;
    case 2:
        if (w->t > 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 6:
        if (w->t >= 12) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 10:
        if (w->t >= 12)
            w->t = 0;
        w->x = (s8)(t40[w->t] & 0xf);
        w->y = (s8)(t40[w->t] >> 4);
        break;
    case 3:
        if (w->t >= 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 7:
        if (w->t >= 16) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 11:
        if (w->t >= 16)
            w->t = 0;
        w->x = (s8)(t3c[w->t & 3] & 0xf);
        w->y = (s8)(t3c[w->t & 3] >> 4);
        break;
    case 4:
        if (w->t >= 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 8:
        if (w->t >= 0x16) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 12:
        if (w->t >= 0x16)
            w->t = 0;
        w->x = (s8)(t4c[w->t] & 0xf);
        w->y = (s8)(t4c[w->t] >> 4);
        break;
    }
    if (w->x & 8)
        w->x = (s8)-(0x10 - w->x);
    w->t++;
}

/* ------------------------------------------------------------- letters */
static void GameOver_Create(s32 a, s32 b, s32 x, s32 y, u16 d, s16 e) {
    GoLetter *w = NULL;
    int i;
    for (i = 0; i < GO_MAX; i++)
        if (!s_go.l[i].used) {
            w = &s_go.l[i];
            break;
        }
    if (w == NULL) /* task == OS_GetArenaLo(0) */
        return;
    memset(w, 0, sizeof(*w));
    w->used = 1;
    UiSpr_Init(&w->spr, UI_RES_GAMEOVER, (u16)a, 0);
    w->unk10 = x << 8;
    w->unk14 = y << 8;
    w->unk20 = b;
    w->unk1c = (w->unk20 << 8) + (e << 8);
    w->unk2e = (s16)d;
}

static void GameOver_State_Animate(GoLetter *w, u16 held) {
    if (held & 0xc0b)
        w->state = 3;
    switch (w->state) {
    case 0:
        w->unk10 = ObjShift_Set(w->unk10, w->unk20 << 8, 2, 0xf00, 0x400);
        if (w->unk10 == w->unk20 << 8) {
            w->state = 1;
            w->unk30 = -0x200;
            w->unk62 = 0x88;
            w->unkc = w->unk2e * 2 + 4;
        }
        break;
    case 1:
        if (w->unkc != 0)
            w->unkc--;
        if (w->unkc == 0)
            w->state = 2;
        break;
    case 2:
        w->unk30 += 0x2a;
        w->unk14 += w->unk30;
        if (w->unk14 > 0x6800) {
            w->unk30 = 0;
            w->unk14 = 0x6800;
        }
        w->unkc = ObjShift_Set(w->unkc, 0x1000, 1, 0x180, 0x180);
        w->unk10 = Math_LerpFx32(w->unk1c, w->unk10, w->unkc);
        if (w->unk10 != w->unk1c)
            break;
        if (w->unk14 == 0x6800) {
            w->state = 3;
            w->unk62 = 0x40;
        }
        break;
    case 3:
        w->unk10 = w->unk1c;
        w->unk14 = 0x6800;
        break;
    }
}

/* --------------------------------------------------------- sequence */
static void GameOverSeq_Start(const s8 (*tbl)[20], const s8 *wt) {
    u16 i;
    u32 total = 0;
    s16 x, cur = 0;
    int lang = ui_lang();
    memset(&s_go, 0, sizeof(s_go));
    s_go.active = 1;
    BrightnessFade_Create(0x42, 0x100);
    for (i = 0; i < 20; i++) {
        if (tbl[lang][i] == -2)
            break;
        if (tbl[lang][i] == -1)
            total += (u32)wt[0x17];
        else
            total += (u32)wt[(u8)tbl[lang][i] % 24];
    }
    x = (s16)((0x100 - (s32)total) >> 1);
    if (x < 0)
        x = 0;
    for (i = 0; i < 20; i++) {
        if (tbl[lang][i] == -2)
            break;
        if (tbl[lang][i] == -1) {
            cur += wt[0x17];
        } else {
            GameOver_Create(tbl[lang][i], x, 0x108, 0x68, i, cur);
            cur += wt[(u8)tbl[lang][i] % 24];
        }
    }
    if (g_ui_hooks.music)
        g_ui_hooks.music(g_ui_hooks.ctx, 0x14); /* Sound_PlaySeq(0, 0x14) */
}

void GameOverSeq_StartGameOver(void) { GameOverSeq_Start(g_ui_data.go_str, g_ui_data.go_w); }
void GameOverSeq_StartTimeOver(void) { GameOverSeq_Start(g_ui_data.to_str, g_ui_data.to_w); }

void GameOverSeq_Main(const InputState *in) {
    u16 held = ui_pad_bits(in->held);
    int i;
    if (!s_go.active)
        return;
    if (s_go.timer < 500) {
        if (held & 0xc0b)
            s_go.timer = 500;
    }
    if (s_go.timer == 0x168)
        ScreenShake_Start(2);
    if (s_go.timer == 0x170)
        ScreenShake_Start(4);
    if (s_go.timer == 0x200)
        BrightnessFade_Create(5, 0x100);
    s_go.timer++;
    if (s_go.timer > 0x200 && BrightnessFade_IsDone()) {
        GameOverSeq_Destroy(); /* Task_DestroyAll; SceneChange_ToSegaLogo */
        return;
    }
    ScreenShake_Main();
    ui_emit_flat(UI_SP_SCREEN, 0, 0, 256, 192, 0xff000000);
    /* the letter tasks: StageObj_Main -> state, draw at x/y + ObjDraw global offset */
    for (i = 0; i < GO_MAX; i++) {
        GoLetter *l = &s_go.l[i];
        if (!l->used)
            continue;
        GameOver_State_Animate(l, held);
        l->spr.x = (s16)((l->unk10 >> 8) + s_go.shake.x);
        l->spr.y = (s16)((l->unk14 >> 8) + s_go.shake.y);
        ui_emit_spr(&l->spr, UI_SP_SCREEN);
    }
}
