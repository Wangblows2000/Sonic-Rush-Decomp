/*
 * Boss stages: the boss health panel (NaviB), the navigator (NaviMsg, NaviMsgFace)
 * and the boss help prompt (BossHelp), translated from src/game/UI/Navi/ and
 * src/game/UI/Hud/BossHelp_*.
 *
 * On the DS, NaviB and NaviMsg live on the second (touch) screen: a <c>_b_b.bbg
 * frame ("SONIC THE HEDGEHOG / BOSS"), the boss gauge sprites, the navigator's
 * face (Tails / Cream) and a speech balloon with a pre-rendered message panel.
 * The Dreamcast shows one 4:3 frame, so they are composed into it (UI.md 4.3):
 *   - the BOSS box of <c>_b_b and the NaviB sprites: a panel in the bottom-right
 *     corner of the safe area, HUD scale (2x), drawn while NaviB exists;
 *   - the balloon, the message panel and the navigator's face: a timed overlay
 *     under the HUD's top row, shown while the DS message BG (sub BG0) is on, with
 *     a short native fade.  The face keeps its original animation (idle, blink,
 *     the talk sequences) and is drawn only with the message.
 * Everything else keeps the original logic: NaviB health / damage animation (the
 * lost icon grows and fades, the counter flashes, the remaining icons blink),
 * NaviMsg panel loading and hide timer, the face sequences (data_0207b8d4) with
 * their tile-streaming delay, and the voice clips with their delays.
 *
 * DS services replaced:
 *   OBJ palette rows 6/7/8 (NaviB_Create loads rows 7/8 from anims 0x18/0x17)
 *     -> colour maps built from those anims' images (UI_VAR_MAP0/1);
 *   sub BG0/BG1 planes, BLDALPHA of the lost icon -> overlay visibility / quad alpha;
 *   File_Load / FS_ReadFile of the BBG panels and faces -> res/navi_<c>_<lang>
 *     (tools/uibg.py) and res/navifaces_<c>.bin;
 *   Sound_PlayVoice(handle, arc, id) -> g_ui_hooks.voice(arc, id).
 */
#include <stdio.h>
#include <stdlib.h>
#include "ui_internal.h"

static s32 s_boss_health;     /* gBossStatus.unk8 */
void ui_set_boss_health(s32 health) { s_boss_health = health; }

static int navi_chara(void) { return G.character == 1 ? 1 : 0; }   /* data_0207b8a4 = gGameState.unk0 */
static int boss_stage(void) { return !UI_NORMAL_STAGE() && !(W.zone == 8 && W.act == 2); }

/* =================================================================== NaviB */
typedef struct NaviBWork {
    UiSpr spr[17];           /* 0x010: anims 0..14, [15] zone plate, [16] gauge frame */
    u16 prio[17];            /* AnimatorSprite unk44: OBJ palette row (6 normal, 7 blink, 8 lost) */
    struct {                 /* task4: NaviB_Main_HealthSprite */
        void (*fn)(struct NaviBWork *w);
        s32 unk0;
        u16 unk8;
    } t4;
    struct {                 /* task8: NaviB_Main_Health */
        void (*fn)(struct NaviBWork *w);
        s32 unk0;
        u16 unk8, unka, unkc;
    } t8;
    int active;
} NaviBWork;
static NaviBWork s_nb;
static int s_frozen;          /* System_Freeze: draw, do not advance */

int NaviB_IsActive(void) { return s_nb.active; }
void NaviB_Destroy(void) { s_nb.active = 0; }

/* AnimatorSprite_Draw on the sub screen at the sprite's position */
static void nb_draw(NaviBWork *w, UiSpr *s, int i, s32 sx, s32 sy, u32 argb) {
    int var = w->prio[i] == 7 ? UI_VAR_MAP0 : w->prio[i] == 8 ? UI_VAR_MAP0 + 1 : 0;
    ui_emit(s, UI_SP_NAVI_B, sx, sy, 0, argb, var);
}

void NaviB_DrawIcons(NaviBWork *w, s32 n, u16 prio) {
    s32 i;
    for (i = 0; i < n && i < 8; i++) {
        u16 k = g_ui_navi.navib_icon[i];
        UiSpr *s = &w->spr[k];
        s->x = g_ui_navi.navib_pos[i][0];
        s->y = g_ui_navi.navib_pos[i][1];
        w->prio[k] = prio;
        nb_draw(w, s, k, 0x1000, 0x1000, 0xffffffff);
    }
    nb_draw(w, &w->spr[16], 16, 0x1000, 0x1000, 0xffffffff);   /* w->extra */
}

static void NaviB_Main_Health(NaviBWork *w);
static void NaviB_Main_HealthLost(NaviBWork *w) {
    s32 idx = w->t8.unk8 - 1, v;
    if (!s_frozen)
        w->t8.unk0 += 0x100;
    if (w->t8.unk0 >= 0x1000) {
        w->t8.fn = NaviB_Main_Health;
        w->t8.unk8 = (u16)s_boss_health;
        /* G2x_SetBlendAlpha_(sub, 0, 0, 0x1f, 0x1f); BLDCNT 0x40, BLDALPHA 0x1f1f */
    } else {
        s32 t = w->t8.unk0 < 0 ? 0 : w->t8.unk0 > 0x1000 ? 0x1000 : w->t8.unk0;
        v = UiLerpN(0x1f, 0, t, 3);
        if (v != 0 && idx >= 0 && idx < 8) {
            /* BLDALPHA eva = v, evb = 0x1f - v over the sub BG: the lost icon, OBJ
             * palette row 8, semi-transparent, scaled up (DrawRotScale) */
            u16 k = g_ui_navi.navib_icon[idx];
            UiSpr *spr = &w->spr[k];
            s32 sx = UiLerpN(0x1000, 0x1199, t, 3), sy = UiLerpN(0x1000, 0x2000, t, 3);
            u32 a = (u32)(v * 255 / 31);
            w->prio[k] = 8;
            spr->x = g_ui_navi.navib_pos[idx][0];
            spr->y = g_ui_navi.navib_pos[idx][1];
            nb_draw(w, spr, k, sx, sy, a << 24 | 0xffffff);
            w->prio[k] = 6;
        }
    }
    if (!s_frozen && ++w->t8.unka > 2) {
        w->t8.unkc ^= 1;
        w->t8.unka = 0;
    }
    NaviB_DrawIcons(w, idx, (u16)(w->t8.unkc + 6));
}

static void NaviB_Main_Health(NaviBWork *w) {
    if (!s_frozen && ++w->t8.unka > 2) {
        w->t8.unkc ^= 1;
        w->t8.unka = 0;
    }
    NaviB_DrawIcons(w, w->t8.unk8, (u16)(w->t8.unkc + 6));
    if (w->t8.unk8 == (u16)s_boss_health || s_frozen)
        return;
    w->t8.fn = NaviB_Main_HealthLost;
    w->t8.unk0 = 0;
    /* G2x_SetBlendAlpha_(sub, 0, 4, 0x1f, 0); BLDCNT 0x440, BLDALPHA 0x1f */
}

static void NaviB_Main_HealthSprite(NaviBWork *w);
static void NaviB_Main_HealthSpriteChange(NaviBWork *w) {
    if (!s_frozen && --w->t4.unk0 <= 0) {
        w->t4.fn = NaviB_Main_HealthSprite;
        w->t4.unk8 = (u16)s_boss_health;
    }
    nb_draw(w, &w->spr[14], 14, 0x1000, 0x1000, 0xffffffff);   /* p + 0x400: the hit flash */
}
static void NaviB_Main_HealthSprite(NaviBWork *w) {
    u16 n = w->t4.unk8;
    if (n != (u16)s_boss_health && !s_frozen) {
        w->t4.fn = NaviB_Main_HealthSpriteChange;
        w->t4.unk0 = 2;
    }
    if (n + 5 < 14)
        nb_draw(w, &w->spr[n + 5], n + 5, 0x1000, 0x1000, 0xffffffff);   /* the hit-point counter */
}

void NaviB_Create(void) {
    NaviBWork *w = &s_nb;
    u16 i, anim;
    s32 v;
    memset(w, 0, sizeof(*w));
    w->active = 1;
    for (i = 0; i <= 14; i++) {
        UiSpr_Init(&w->spr[i], UI_RES_NAVIB, i, 0);
        w->prio[i] = 6;
    }
    /* time attack / mode 3: the 8-point gauge; story: gGameState.unk14 */
    v = (W.mode == 2 || W.mode == 3) ? 1 : (s32)G.difficulty;
    anim = v == 1 ? 0x1a : 0x19;
    UiSpr_Init(&w->spr[16], UI_RES_NAVIB, anim, 0);
    w->prio[16] = 6;
    UiSpr_Init(&w->spr[15], UI_RES_NAVIB, (u16)(StoryProgress_GetZoneIndex(W.zone) + 0xf), 0);
    w->prio[15] = 6;
    /* AnimatorSprite_Init(&tmp, 0x18, ...) prio 7 / (&tmp, 0x17, ...) prio 8: the palette
     * rows the blink and the lost icon use; images 24 / 23 are the pill of image 0 */
    ui_palmap_set(0, UI_RES_NAVIB, 0, 24);
    ui_palmap_set(1, UI_RES_NAVIB, 0, 23);
    w->t4.fn = NaviB_Main_HealthSprite;
    w->t4.unk8 = (u16)s_boss_health;
    w->t8.fn = NaviB_Main_Health;
    w->t8.unk8 = (u16)s_boss_health;
}

/* NaviB_Main + its two sub tasks (prio 0x3000, 0x4000, 0x4000) */
static void navib_run(void) {
    NaviBWork *w = &s_nb;
    if (!w->active)
        return;
    nb_draw(w, &w->spr[15], 15, 0x1000, 0x1000, 0xffffffff);   /* NaviB_Main: zone plate */
    w->t4.fn(w);
    w->t8.fn(w);
}

/* ================================================================= NaviMsg */
typedef struct NaviMsgWork NaviMsgWork;
struct NaviMsgWork {
    s32 mode, idx;           /* 0x000 data_0207b894 (1: boss), data_0207b8a4 (character) */
    u32 flags;               /* 0x0D4 1 visible, 2 running, 0x100 face seq ended, 0x10000 face buffer,
                                0x20000 face ready (VBlank), 0x40000/0x80000/0x100000 face streaming,
                                0x200000 panel loaded, 0x400000 BG0 on, 0x800000 BG0 off */
    void (*fnd8)(NaviMsgWork *w);   /* face state */
    void (*fndc)(NaviMsgWork *w);   /* fade state (zone map only) */
    void (*fne0)(NaviMsgWork *w);   /* panel timer */
    s32 unke4, unke8, unkec;        /* face anim: sequence, frame index, timer */
    const UiNaviSeq *seq;           /* 0x0F0 */
    s32 cur;                        /* 0x0F4 (pair index) */
    s32 unkf8;                      /* idle time until the next blink */
    s32 fidx;                       /* 0x100 message panel in bg94 */
    s32 unk104;                     /* panel hide timer */
    s32 unk108, unk10c, unk110;     /* voice id (-1 none), delay, arc */
    int handle;                     /* unk114 != NULL */
    int face_pending;               /* the frame being streamed (unkc) */
    int face_shown;                 /* the frame on screen */
    int bg0, bg1;                   /* sub DISPCNT BG0 (balloon + panel) / BG1 (face) */
    int panel_shown;                /* bg94 content */
    s32 alpha;                      /* native overlay fade, 0..0x10 */
    int active;
};
static NaviMsgWork s_nm;

/* navigator faces (NaviMsg_ReadFaceFrameMap / Chr source): navifaces_<c>.bin */
static u8 *s_faces;
static u32 s_faces_size;
static int s_face_n, s_face_w, s_face_h;

int ui_navi_face_expand(int f, u16 *dst, int stride, int *w, int *h) {
    const u8 *b;
    const u16 *pal;
    int x, y;
    if (!s_faces || f < 0 || f >= s_face_n)
        return 0;
    b = s_faces + 12 + (size_t)f * (512 + (size_t)s_face_w * s_face_h);
    pal = (const u16 *)b;
    b += 512;
    for (y = 0; y < s_face_h; y++)
        for (x = 0; x < s_face_w; x++) {
            u8 p = b[y * s_face_w + x];
            u16 c = pal[p];
            dst[y * stride + x] = p ? (u16)(0x8000 | (c & 0x1f) << 10 | (c & 0x3e0) | (c >> 10 & 0x1f)) : 0;
        }
    *w = s_face_w;
    *h = s_face_h;
    return 1;
}

static void voice(s32 arc, s32 id) {
    if (g_ui_hooks.voice)
        g_ui_hooks.voice(g_ui_hooks.ctx, arc, id);
}

/* gSaveData progress gates (NaviMsg_Create / SetVisible / SetFaceAnim / ShowMessage) */
static int navi_locked(void) {
    if (G.character == 0 && G.save_progress[0] < 3)
        return 1;
    if (G.character == 1 && (G.save_progress[1] < 4 || (G.save_progress[1] >= 0x1b && G.save_progress[1] < 0x1c)))
        return 1;
    return 0;
}

/* NaviMsg_ReadFaceFrameMap: start streaming face frame f */
static void NaviMsg_ReadFaceFrameMap(NaviMsgWork *w, int f) { w->face_pending = f; }

static void NaviMsg_StartFaceSeq(NaviMsgWork *w, s32 n) {
    w->unke4 = n;
    w->unkec = 0;
    w->seq = &g_ui_navi.seq[w->idx][n < 0 || n >= 14 ? 0 : n];
    w->unke8 = 0;
    w->cur = 0;
    w->flags &= ~0x100u;
    if (w->seq->count == 0) {   /* a zone-map sequence (no frames converted) */
        w->flags |= 0x100;
        return;
    }
    NaviMsg_ReadFaceFrameMap(w, w->seq->fr[0][1]);
    w->flags |= 0x40000;
}

static void NaviMsg_UpdateFaceAnim(NaviMsgWork *w) {
    if (w->flags & 0x100)
        return;
    w->unkec++;
    if (w->seq->fr[w->cur][0] > w->unkec)
        return;
    w->unkec = 0;
    w->unke8++;
    if (w->unke8 >= w->seq->count || w->unke8 >= 40) {
        if (w->seq->flags & 1) {
            w->unke8 = 0;
            w->cur = 0;
        } else {
            w->flags |= 0x100;
            return;
        }
    } else {
        w->cur++;
    }
    NaviMsg_ReadFaceFrameMap(w, w->seq->fr[w->cur][1]);
    w->flags |= 0x40000;
}

static void NaviMsgFace_State_Idle(NaviMsgWork *s);
static void NaviMsgFace_StartIdle(NaviMsgWork *s) {
    s->fnd8 = NaviMsgFace_State_Idle;
    NaviMsg_StartFaceSeq(s, 0);
    s->unkf8 = (UiRand() & 0x3f) + 0x2d0;
}
static void NaviMsgFace_State_Blink(NaviMsgWork *s) {
    if (!(s->flags & 0x100))
        return;
    NaviMsgFace_StartIdle(s);
}
static void NaviMsgFace_State_Idle(NaviMsgWork *s) {
    s->unkf8--;
    if (s->unkf8 > 0)
        return;
    s->fnd8 = NaviMsgFace_State_Blink;
    NaviMsg_StartFaceSeq(s, 1);
}
static void NaviMsgFace_State_Play(NaviMsgWork *s) {
    if (!(s->flags & 0x100))
        return;
    if (s->unke4 <= 8) {
        NaviMsgFace_StartIdle(s);
        return;
    }
    NaviMsg_SetFaceAnim(10);
}
static void NaviMsgFace_Play(NaviMsgWork *s, s32 n) {
    s->fnd8 = NaviMsgFace_State_Play;
    NaviMsg_StartFaceSeq(s, n);
}

void NaviMsg_SetFaceAnim(s32 n) {
    NaviMsgWork *w = &s_nm;
    if (!w->active)
        return;
    if (w->mode == 0 && navi_locked())
        return;
    /* data_0207b9d4[n]: Idle, Blink, then Play for 2..13 */
    w->fnd8 = n == 0 ? NaviMsgFace_State_Idle : n == 1 ? NaviMsgFace_State_Blink : NaviMsgFace_State_Play;
    NaviMsg_StartFaceSeq(w, n);
}

static void NaviMsg_State_HideTimer(NaviMsgWork *s) {
    s->unk104--;
    if (s->unk104 <= 0) {
        s->fne0 = NULL;
        s->flags |= 0x800000;
    }
}

static void NaviMsg_LoadPanel(NaviMsgWork *w, s32 idx, s32 v) {
    if (w->fidx == idx) {
        w->flags |= 0x400000;
        w->fne0 = NaviMsg_State_HideTimer;
    } else {
        w->fidx = idx;           /* FS_ReadFile + NaviMsg_ParsePanelHeader */
        w->bg0 = 0;              /* DISPCNT &= ~BG0 */
        w->flags |= 0x200000;
    }
    w->unk104 = v;
}

void NaviMsg_ShowMessage(s32 a, s32 b) {
    NaviMsgWork *w = &s_nm;
    if (!w->active) {
        /* no navigator yet (early story): the hurt line has a voice only */
        if (!boss_stage() || a != 0x11)
            return;
        voice(0, 0xcd);
        return;
    }
    if (w->mode == 0 && navi_locked())
        return;
    NaviMsg_LoadPanel(w, a, b);
    if (w->mode == 1) {
        const s32 *v = g_ui_navi.voice[w->idx];   /* arc, 0xf, 0x10, 0x11, 0x12, 0x13, 0x13 zone 7 */
        switch (a) {
        case 0xf:
            w->unk108 = v[1];
            w->unk10c = 1;
            break;
        case 0x10:
            w->unk108 = v[2];
            w->unk10c = 0x14;
            break;
        case 0x11:
            if (W.zone == 6) {
                w->unk108 = v[3];
            } else {
                if ((UiRand() & 1) == 0)
                    w->unk108 = 0xcd;
                else
                    w->unk108 = v[3];
            }
            w->unk10c = 0x14;
            break;
        case 0x12:
            w->unk108 = v[4];
            w->unk10c = 0x14;
            break;
        case 0x13:
            w->unk108 = W.zone != 7 ? v[5] : v[6];
            w->unk10c = 1;
            break;
        }
        w->unk110 = 0;
        w->handle = 0;
        return;
    }
    /* mode 0 (zone map navigator): belongs to the menus module */
}

/* VBlank task (prio 0xf100, after every other task) */
static void NaviMsg_VBlankMain(NaviMsgWork *w) {
    if (w->flags & 0x20000) {
        w->flags &= ~0x20000u;
        w->flags ^= 0x10000;
        w->face_shown = w->face_pending;   /* BG1CNT swap + NaviMsg_LoadFacePalette */
    }
    if (w->flags & 0x400000) {
        w->bg0 = 1;
        w->flags &= ~0x400000u;
    } else if (w->flags & 0x800000) {
        w->bg0 = 0;
        w->flags &= ~0x800000u;
    }
}

void NaviMsg_Main(void) {
    NaviMsgWork *w = &s_nm;
    if (!w->active)
        return;
    if (w->flags & 2) {
        if (w->fnd8)
            w->fnd8(w);
        if (w->fndc)
            w->fndc(w);
        if (w->fne0)
            w->fne0(w);
        NaviMsg_UpdateFaceAnim(w);
        /* the face frame and the panel reach VRAM over several frames */
        if (w->flags & 0x40000) {
            w->flags &= ~0x40000u;
            w->flags |= 0x80000;
        } else if (w->flags & 0x80000) {
            w->flags &= ~0x80000u;           /* NaviMsg_ReadFaceFrameChr(.., 0) */
            w->flags |= 0x100000;
        } else if (w->flags & 0x100000) {
            w->flags &= ~0x100000u;          /* NaviMsg_ReadFaceFrameChr(.., 1) */
            w->flags |= 0x20000;
        } else if (w->flags & 0x200000) {
            w->panel_shown = w->fidx;        /* Background_Draw(&w->bg94) */
            w->flags &= ~0x200000u;
            w->flags |= 0x400000;
            w->fne0 = NaviMsg_State_HideTimer;
        }
    }
    if (w->unk108 != -1) {
        w->unk10c--;
        if (w->unk10c <= 0) {
            voice(w->unk110, w->unk108);   /* NNS_SndPlayerStopSeq(handle) + Sound_PlayVoice */
            w->unk108 = -1;
        }
    }
    NaviMsg_VBlankMain(w);
    /* native: overlay fade (6 frames) */
    if (w->bg0 && w->alpha < 0x10)
        w->alpha = w->alpha + 3 > 0x10 ? 0x10 : w->alpha + 3;
    else if (!w->bg0 && w->alpha > 0)
        w->alpha = w->alpha - 3 < 0 ? 0 : w->alpha - 3;
}

/* NaviMsg_Create for data_0207b894 = 1 (NaviBg_Load(1, gGameState.unk0): boss stages) */
void NaviMsg_Create(void) {
    NaviMsgWork *w = &s_nm;
    memset(w, 0, sizeof(*w));
    if (W.mode == 0 && navi_locked())
        return;   /* only the panel background is drawn (no navigator yet) */
    w->active = 1;
    w->mode = 1;
    w->idx = navi_chara();
    w->face_shown = w->face_pending = -1;
    w->fidx = 0xf;            /* mode 1: data_022c4664[0xf] drawn into bg3 */
    w->panel_shown = 0xf;
    NaviMsgFace_Play(w, 10);  /* mode 1 */
    w->bg0 = 0;               /* SetPlane: BG2 only */
    w->bg1 = 0;
    w->flags |= 2;
    w->unk108 = -1;
    w->unk10c = 0;
}

void NaviMsg_Destroy(void) { s_nm.active = 0; }

void NaviMsg_SetVisible(int on) {
    NaviMsgWork *w = &s_nm;
    if (!w->active)
        return;
    if (w->mode == 0 && navi_locked())
        return;
    if (on) {
        w->flags |= 1;
        w->bg1 = 1;
    } else {
        w->flags &= ~1u;
        w->bg1 = 0;
    }
}

int ui_navi_message_visible(void) { return s_nm.active && (s_nm.flags & 1) && s_nm.bg0; }

/* NaviMsg_HidePanel */
void NaviMsg_HidePanel(void) {
    if (!s_nm.active)
        return;
    s_nm.unk104 = 0;
    s_nm.fne0 = NULL;
    s_nm.flags |= 0x800000;
}

/* ================================================================ BossHelp */
typedef struct {
    UiSpr spr;
    u32 idx;
    int active;
} BossHelpWork;
static BossHelpWork s_bh;

void BossHelp_Create(u32 idx) {
    BossHelpWork *w = &s_bh;
    memset(w, 0, sizeof(*w));
    w->active = 1;
    w->idx = idx;
    /* flags 0x34: loop; palette data_ov000_02364d8c[idx] (the converted atlas colours) */
    UiSpr_Init(&w->spr, UI_RES_BOSSHELP, (u16)idx, UI_SPR_LOOP);
}
void BossHelp_Destroy(void) { s_bh.active = 0; }

void BossHelp_Main(void) {
    BossHelpWork *w = &s_bh;
    if (!w->active)
        return;
    if (!s_frozen)
        UiSpr_ProcessAnimation(&w->spr);
    /* main-screen OBJ at x = y = 0 (the frames sit at the DS bottom right): the safe
     * area's bottom-right corner, above the boss panel */
    w->spr.x = 0;
    w->spr.y = (s16)(s_nb.active ? -(g_ui_navi.geo[navi_chara()][3] - g_ui_navi.geo[navi_chara()][1] + 4) : 0);
    g_ui_screen = 0;
    ui_emit_spr(&w->spr, UI_SP_HUD_BR);
}

/* =========================================================== DC composition */
void ui_navi_player_hurt(void) {
    if (UI_NORMAL_STAGE())
        return;
    NaviMsg_SetFaceAnim(0xc);
    NaviMsg_ShowMessage(0x11, 0x96);
}

void ui_boss_navi(void *ctx, s32 face, s32 msg, s32 time) {
    (void)ctx;
    NaviMsg_SetFaceAnim(face);
    NaviMsg_ShowMessage(msg, time);
}

/* the stage's navigator data: res/navi_<c>_<lang> and res/navifaces_<c>.bin */
static int s_loaded_c = -1, s_loaded_l = -1;
void ui_navi_stage_end(void) {
    NaviB_Destroy();
    NaviMsg_Destroy();
    BossHelp_Destroy();
    ui_res_free(UI_RES_NAVI);
    free(s_faces);
    s_faces = NULL;
    s_face_n = 0;
    s_loaded_c = s_loaded_l = -1;
}
void ui_navi_stage_start(void) {
    static const char *k_lang[6] = { "jpn", "eng", "fra", "deu", "ita", "spa" };   /* data_0207b9bc */
    char dir[128], id[48], path[200];
    int c = navi_chara(), l = ui_lang();
    NaviB_Destroy();
    NaviMsg_Destroy();
    BossHelp_Destroy();
    if (!boss_stage()) {
        if (s_loaded_c >= 0) {
            ui_res_free(UI_RES_NAVI);
            free(s_faces);
            s_faces = NULL;
            s_loaded_c = s_loaded_l = -1;
        }
        return;
    }
    if (s_loaded_c == c && s_loaded_l == l && s_faces)
        return;
    snprintf(dir, sizeof(dir), "%s/res", g_ui_dir);
    snprintf(id, sizeof(id), "navi_%c_%s", c ? 'c' : 't', k_lang[l]);
    ui_res_load_dir(UI_RES_NAVI, dir, id);
    free(s_faces);
    s_faces = NULL;
    s_face_n = 0;
    snprintf(path, sizeof(path), "%s/navifaces_%c.bin", dir, c ? 'c' : 't');
    if (ui_load_file(path, &s_faces, &s_faces_size) == 0) {
        if (s_faces_size >= 12 && memcmp(s_faces, "NVFC", 4) == 0) {
            s_face_n = s_faces[4] | s_faces[5] << 8;
            s_face_w = s_faces[6] | s_faces[7] << 8;
            s_face_h = s_faces[8] | s_faces[9] << 8;
            if (12u + (u32)s_face_n * (512u + (u32)s_face_w * s_face_h) > s_faces_size)
                s_face_n = 0;
        }
    }
    s_loaded_c = c;
    s_loaded_l = l;
}

/* Layout (HUD scale k, safe area x0..x1, y0..y1):
 *   boss panel : the BOSS box (geo panel rect) with its bottom-right at the safe corner;
 *   message    : the balloon (geo balloon rect) under the HUD's top row (y0 + 20k),
 *                left aligned; the message panel at its sub-screen offset inside it;
 *   face       : right of the balloon, top aligned with it, over the <c>_b_b
 *                background of its sub-screen place (a portrait window). */
void ui_navi_draw(void) {
    int k, x0, y0, x1, y1, c = navi_chara();
    const s16 *g = g_ui_navi.geo[c];
    NaviMsgWork *m = &s_nm;
    ui_hud_metrics(&k, &x0, &y0, &x1, &y1);
    g_ui_screen = 1;   /* composed in from the sub screen */
    if (s_nb.active) {
        float pw = (float)(g[2] - g[0]), ph = (float)(g[3] - g[1]);
        ui_space_set(UI_SP_NAVI_B, x1 - pw * k, y1 - ph * k, g[0], g[1], (float)k);
        navib_run();
        /* the BOSS box of <c>_b_b.bbg (sub BG2): behind the sprites, so emitted last */
        ui_emit_image(UI_RES_NAVI, 0, UI_SP_NAVI_B, g[0], g[1], -1e9f, -1e9f, 1e9f, 1e9f, 0xffffffff);
    }
    if (m->active && (m->flags & 1) && m->alpha > 0) {
        u32 a = (u32)(m->alpha * 255 / 16) << 24 | 0xffffff;
        float bx = (float)x0, by = (float)(y0 + 20 * k);
        float fx = bx + (float)(g[6] - g[4]) * k + 2 * k;
        ui_space_set(UI_SP_NAVI_MSG, bx, by, g[4], g[5], (float)k);
        ui_space_set(UI_SP_NAVI_FACE, fx, by, 0, 0, (float)k);
        if (m->face_shown >= 0)
            ui_emit_face(UI_SP_NAVI_FACE, 0, 0, m->face_shown, g[10] - g[8], g[11] - g[9], a);
        /* behind the face: the <c>_b_b background of that place (sub BG2 under BG1) */
        ui_emit_image(UI_RES_NAVI, 7, UI_SP_NAVI_FACE, 0, 0, -1e9f, -1e9f, 1e9f, 1e9f, a);
        /* message panel (bg94, sub BG0 over the balloon) at its sub-screen place */
        if (m->panel_shown >= 0xf && m->panel_shown <= 0x13)
            ui_emit_image(UI_RES_NAVI, 2 + (m->panel_shown - 0xf), UI_SP_NAVI_MSG, g[14], g[15], -1e9f, -1e9f, 1e9f,
                          1e9f, a);
        ui_emit_image(UI_RES_NAVI, 1, UI_SP_NAVI_MSG, g[4], g[5], -1e9f, -1e9f, 1e9f, 1e9f, a);
    }
    g_ui_screen = 0;
}

/* one frame of the boss-stage tasks (frozen = pause: draw only) */
void ui_navi_frame(int frozen) {
    s_frozen = frozen;
    if (!frozen)
        NaviMsg_Main();
    ui_navi_draw();
    BossHelp_Main();
    s_frozen = 0;
}
