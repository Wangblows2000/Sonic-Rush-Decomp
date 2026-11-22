/*
 * The zone-map navigator: NaviBg_Load / NaviMsg_* mode 0 / NaviMsgFace_* and the
 * NaviM progress panel (NaviM_*, NaviMPartA_*, NaviMPartB_*), translated from
 * src/game/UI/Navi/.  The boss navigator (mode 1) is the UI module's (ui_navi.c).
 *
 * On the DS this is the whole second screen while the zone map is up: the
 * navigator frame <c>_m_b (sub BG2), the navigator's face (sub BG1, 128x128 at
 * (128,48), streamed frame by frame), the speech balloon m_bal with the current
 * message panel at (16,16) (sub BG0), and the NaviM sprites: the lives counter,
 * the story step counter and the emerald slots.  The Dreamcast frame is the map
 * at 2.5x; mn_navi_compose() places pieces of the sub engine into it as
 * elements (MENUS.md 3, 13): the balloon and message at the top left, the face
 * at the bottom right (both only while the DS shows them) and the NaviM panel at
 * the top right.  The navigator frame BG itself is left out (it is a screen
 * background); the NaviM panel keeps its part of it.
 *
 * DS services replaced:
 *   FS_ReadFile of message panels / face frames -> bg/navi_<c>_<lang>_NN.mnbg
 *     (loaded when the panel changes, as the DS reads them) and
 *     navi/faces_<c>.bin (the zone-map face frames, RLE-packed: about 0.4 MB
 *     while the map is up; one frame is expanded when it is shown);
 *   sub BG0..3 planes and DISPCNT bits -> g_mn_layer[1][*] (BG0 balloon,
 *     BG3 message panel: the DS draws both into BG0, so BG3 follows BG0);
 *   BLDCNT/BLDALPHA of the face fades -> the BG1 layer alpha;
 *   Sound_PlayVoice(handle, arc, id) -> Sound_PlayVoiceArc(arc, id);
 *   Sound_PlaySeqArc(0, arc, 9) -> Sound_PlaySeqArc(arc, 9).
 * OBJ palette rows (AnimatorSprite prio / ObjVram_Alloc) need nothing: the
 * converted sprite pages carry their colours.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mn_internal.h"
#include "../save/save.h"

static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }

/* ------------------------------------------------------------- tables */
/* data_02075664: the intro panel; data_0207566c: voice archive per navigator */
static const s32 data_02075664[2] = {26, 27};
static const s32 data_0207566c[2] = {2, 3};
/* data_020756ac..c0 [idx][0]: mode 0 voices (messages 2, 7..14, 1, 4, 3, 6) */
static const s32 data_020756ac[2] = {10, 10};
static const s32 data_020756b0[2] = {11, 11};
static const s32 data_020756b4[2] = {12, 12};
static const s32 data_020756b8[2] = {13, 13};
static const s32 data_020756bc[2] = {14, 14};
static const s32 data_020756c0[2] = {15, 15};
/* NaviM: data_020778e0 (step -> name anim), data_02077888 (palette anims, unused here),
 * data_020778b0 (step -> counter value), data_02077894 (emerald slot positions) */
static const u16 data_020778e0[16] = {0xffff, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
static const u16 data_020778b0[24] = {0, 0, 2, 5, 5, 6, 8, 8, 15, 20, 20, 30, 42, 42, 49, 55, 55, 57, 62, 62, 74, 81, 94, 0};
static const struct { s16 x, y; } data_02077894[7] = {
    {96, 83}, {82, 91}, {110, 91}, {96, 99}, {82, 108}, {110, 108}, {96, 115}};
static const char *const s_lang[6] = {"jpn", "eng", "fra", "deu", "ita", "spa"};   /* data_0207b9bc */

/* --------------------------------------------------------- NaviBg_Load */
s32 data_0207b894 = -1;   /* navigator mode (0 zone map, 1 boss) */
s32 data_0207b8a4 = -1;   /* navigator: 0 Tails (Sonic's story), 1 Cream (Blaze's) */
static MnBgImg *s_navi_bg, *s_navi_bal;

/* the face frames (data_022c466c files) and sequences (data_0207b8d4) */
typedef struct {
    u16 count, flags;
    u8 fr[48][2];   /* duration, frame */
} NvSeq;
static struct {
    u8 *blob;
    u32 size;
    u16 n, w, h, x0, y0, npal;
    const NvSeq *seq;
    const u8 *pals;     /* npal x u16[256] */
    const u8 *index;    /* n x {u16 pal, u16 0, u32 offset, u32 size} */
    const u8 *data;     /* RLE frames */
} s_faces;
static u32 rd32(const u8 *p) { return (u32)(p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24); }

static void navi_free_files(void) {
    mn_bg_free(s_navi_bg);
    mn_bg_free(s_navi_bal);
    s_navi_bg = s_navi_bal = NULL;
    free(s_faces.blob);
    memset(&s_faces, 0, sizeof(s_faces));
}

static int navi_load_faces(s32 idx) {
    char p[200];
    u8 *b;
    u32 size;
    snprintf(p, sizeof(p), "%s/navi/faces_%c.bin", g_mn_dir, idx ? 'c' : 't');
    u32 hdr = 20 + 14 * sizeof(NvSeq);
    if (mn_load_file(p, &b, &size) != 0) {
        fprintf(stderr, "menus: missing %s\n", p);
        return -1;
    }
    if (size < hdr || memcmp(b, "NVFM", 4) != 0 || rd16(b + 4) != 2) {
        fprintf(stderr, "menus: bad %s\n", p);
        free(b);
        return -1;
    }
    s_faces.blob = b;
    s_faces.size = size;
    s_faces.n = rd16(b + 6);
    s_faces.w = rd16(b + 8);
    s_faces.h = rd16(b + 10);
    s_faces.x0 = rd16(b + 12);
    s_faces.y0 = rd16(b + 14);
    s_faces.npal = rd16(b + 16);
    s_faces.seq = (const NvSeq *)(b + 20);
    s_faces.pals = b + hdr;
    s_faces.index = s_faces.pals + 512u * s_faces.npal;
    s_faces.data = s_faces.index + 12u * s_faces.n;
    if ((u32)(s_faces.data - b) > size) {
        free(b);
        memset(&s_faces, 0, sizeof(s_faces));
        return -1;
    }
    return 0;
}

void NaviBg_Load(s32 mode, s32 idx) {
    char p[64];
    navi_free_files();
    data_0207b894 = mode;
    data_0207b8a4 = idx;
    /* data_0207b8e4[idx][mode] -> sub BG2; data_0207b8bc[mode] -> sub BG0 */
    snprintf(p, sizeof(p), "bg2/navi/%c_m_b.bbg", idx ? 'c' : 't');
    s_navi_bg = mn_bg_load(p);
    snprintf(p, sizeof(p), "bg2/navi/m_bal_%c.bbg", idx ? 'c' : 't');
    s_navi_bal = mn_bg_load(p);
    Background_Show(1, 2, s_navi_bg);
    Background_SetPriority(1, 2, 3);
    Background_Show(1, 0, s_navi_bal);
    Background_SetPriority(1, 0, 1);
    g_mn_layer[1][0].visible = 0;   /* DISPCNT &= ~(BG0 | BG2) until NaviMsg_Create */
    g_mn_layer[1][2].visible = 0;
    navi_load_faces(idx);   /* the FS_OpenFile loops over the faces and panels */
}

/* NaviMsg_CloseFiles */
void NaviMsg_CloseFiles(void) {
    navi_free_files();
    data_0207b894 = -1;
    data_0207b8a4 = -1;
}

/* -------------------------------------------------------------- NaviMsg */
typedef struct NaviMsgWork {
    s32 mode, idx;
    s32 face_req;          /* unkc: face frame being streamed (data_022c466c entry) */
    s32 face_shown;
    MnBgImg face;          /* bg1/bg2: the two face buffers -> one texture */
    MnBgImg *panel;        /* bg94: the message panel */
    u32 flags;             /* unkd4 */
    void (*fnd8)(struct NaviMsgWork *w);   /* face state */
    void (*fndc)(struct NaviMsgWork *w);   /* intro / fade */
    void (*fne0)(struct NaviMsgWork *w);   /* panel hide timer */
    s32 seq;               /* unke4 */
    s32 timer;             /* unke8 */
    s32 pos;               /* unkec */
    s32 blink;             /* unkf8 */
    s32 cnt;               /* unkfc */
    s32 fidx;              /* panel shown in bg3 */
    s32 unk104;
    s32 unk108, unk10c, unk110;
    s16 alpha;             /* BLDALPHA EVA of BG1 (0..31), 31 = no blend */
} NaviMsgWork;
static MnTask *gNaviMsgTask;
#define NMW ((NaviMsgWork *)gNaviMsgTask->work)

static BOOL navi_locked(void) {
    if (gGameState.unk0 == 0 && gSaveData.unkC < 3)
        return TRUE;
    if (gGameState.unk0 == 1 && (gSaveData.unk64 < 4 || (gSaveData.unk64 >= 0x1b && gSaveData.unk64 < 0x1c)))
        return TRUE;
    return FALSE;
}

static void NaviMsg_ReadFaceFrameMap(NaviMsgWork *w, s32 f) { w->face_req = f; }

/* NaviMsg_ReadFaceFrameChr + NaviMsg_LoadFacePalette: the frame into the face texture */
/* the frames are kept RLE-packed (NVFM v2) and expanded here, one at a time,
 * as the DS streams one frame at a time from the card */
static void NaviMsg_LoadFaceFrame(NaviMsgWork *w, s32 f) {
    const u8 *e, *src, *end, *pal;
    u32 sz = (u32)s_faces.w * s_faces.h, o = 0, off, len;
    int i;
    if (!s_faces.blob || f < 0 || f >= s_faces.n)
        return;
    e = s_faces.index + 12u * (u32)f;
    off = rd32(e + 4);
    len = rd32(e + 8);
    if (len == 0 || rd16(e) >= s_faces.npal || (u32)(s_faces.data - s_faces.blob) + off + len > s_faces.size)
        return;   /* not a zone-map frame */
    pal = s_faces.pals + 512u * rd16(e);
    for (i = 0; i < 256; i++)
        w->face.pal[i] = rd16(pal + 2 * i);
    src = s_faces.data + off;
    end = src + len;
    while (src < end && o < sz) {
        u8 c = *src++;
        if (c < 0x80) {
            u32 k = c + 1u;
            if (k > sz - o)
                k = sz - o;
            if (k > (u32)(end - src))
                k = (u32)(end - src);
            memcpy(w->face.idx + o, src, k);
            src += k;
            o += k;
        } else if (src < end) {
            u32 k = c - 0x80u + 3u;
            if (k > sz - o)
                k = sz - o;
            memset(w->face.idx + o, *src++, k);
            o += k;
        }
    }
    if (o < sz)
        memset(w->face.idx + o, 0, sz - o);
    w->face.dirty = 1;
    if (w->face.argb)
        mn_bg_expand(&w->face);
    w->face_shown = f;
}

static void NaviMsg_StartFaceSeq(NaviMsgWork *w, s32 n) {
    w->seq = n;
    w->pos = 0;
    w->timer = 0;
    w->flags &= ~0x100u;
    if (s_faces.blob && s_faces.seq[n].count)
        NaviMsg_ReadFaceFrameMap(w, s_faces.seq[n].fr[0][1]);
    w->flags |= 0x40000;
}

static void NaviMsg_UpdateFaceAnim(NaviMsgWork *w) {
    const NvSeq *s;
    if (w->flags & 0x100)
        return;
    if (!s_faces.blob)
        return;
    s = &s_faces.seq[w->seq];
    w->timer++;
    if (s->fr[w->pos][0] > w->timer)
        return;
    w->timer = 0;
    w->pos++;
    if (w->pos >= s->count) {
        if (s->flags & 1) {
            w->pos = 0;
        } else {
            w->flags |= 0x100;
            return;
        }
    }
    NaviMsg_ReadFaceFrameMap(w, s->fr[w->pos][1]);
    w->flags |= 0x40000;
}

static void NaviMsgFace_State_Idle(NaviMsgWork *s);
static void NaviMsgFace_State_Blink(NaviMsgWork *s);
static void NaviMsgFace_State_Play(NaviMsgWork *s);

static void NaviMsgFace_StartIdle(NaviMsgWork *s) {
    s->fnd8 = NaviMsgFace_State_Idle;
    NaviMsg_StartFaceSeq(s, 0);
    s->blink = (mn_rand() & 0x3f) + 0x2d0;
}

static void NaviMsgFace_State_Idle(NaviMsgWork *s) {
    s->blink--;
    if (s->blink > 0)
        return;
    s->fnd8 = NaviMsgFace_State_Blink;
    NaviMsg_StartFaceSeq(s, 1);
}

static void NaviMsgFace_State_Blink(NaviMsgWork *s) {
    if (!(s->flags & 0x100))
        return;
    NaviMsgFace_StartIdle(s);
}

static void NaviMsgFace_State_Play(NaviMsgWork *s) {
    if (!(s->flags & 0x100))
        return;
    if (s->seq <= 8) {
        NaviMsgFace_StartIdle(s);
        return;
    }
    NaviMsg_SetFaceAnim(10);
}

static void NaviMsgFace_Play(NaviMsgWork *s, s32 n) {
    s->fnd8 = NaviMsgFace_State_Play;
    NaviMsg_StartFaceSeq(s, n);
}

/* data_0207b9d4: the face state for each sequence */
static void (*const data_0207b9d4[14])(NaviMsgWork *) = {
    NaviMsgFace_State_Idle, NaviMsgFace_State_Blink, NaviMsgFace_State_Play, NaviMsgFace_State_Play,
    NaviMsgFace_State_Play, NaviMsgFace_State_Play, NaviMsgFace_State_Play, NaviMsgFace_State_Play,
    NaviMsgFace_State_Play, NaviMsgFace_State_Play, NaviMsgFace_State_Play, NaviMsgFace_State_Play,
    NaviMsgFace_State_Play, NaviMsgFace_State_Play};

void NaviMsg_SetFaceAnim(s32 n) {
    NaviMsgWork *w;
    if (!gNaviMsgTask)
        return;
    w = NMW;
    if (w->mode == 0 && navi_locked())
        return;
    if (n < 0 || n >= 14)
        return;
    w->fnd8 = data_0207b9d4[n];
    NaviMsg_StartFaceSeq(w, n);
}

static void NaviMsg_State_HideTimer(NaviMsgWork *s) {
    s->unk104--;
    if (s->unk104 <= 0) {
        s->fne0 = NULL;
        s->flags |= 0x800000;
    }
}

/* NaviMsg_ParsePanelHeader + FS_ReadFile: the message panel file */
static void NaviMsg_ReadPanel(NaviMsgWork *w, s32 idx) {
    char p[64];
    int lang = gGameLanguage < 6 ? gGameLanguage : 1;
    mn_bg_free(w->panel);
    snprintf(p, sizeof(p), "bg2/navi/navi_%c_%s_%02d.bbg", w->idx ? 'c' : 't', s_lang[lang], (int)idx);
    w->panel = mn_bg_load(p);
}

static void NaviMsg_LoadPanel(NaviMsgWork *w, s32 idx, s32 v) {
    if (w->fidx == idx) {
        w->flags |= 0x400000;
        w->fne0 = NaviMsg_State_HideTimer;
    } else {
        w->fidx = idx;
        g_mn_layer[1][0].visible = 0;   /* DISPCNT &= ~BG0 */
        g_mn_layer[1][3].visible = 0;
        w->flags |= 0x200000;
    }
    w->unk104 = v;
}

void NaviMsg_ShowMessage(s32 a, s32 b) {
    NaviMsgWork *w;
    s32 snd = -1;
    if (!gNaviMsgTask) {
        if (data_0207b894 != 1)
            return;
        if (a != 0x11)
            return;
        Sound_PlayVoiceArc(0, 0xcd);
        return;
    }
    w = NMW;
    if (w->mode == 0 && navi_locked())
        return;
    NaviMsg_LoadPanel(w, a, b);
    if (w->mode == 1)
        return;   /* boss messages: ui_navi.c */
    switch (a) {
    case 2:
        w->unk110 = data_0207566c[w->idx];
        w->unk108 = data_020756ac[w->idx];
        w->unk10c = 1;
        break;
    case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
        snd = data_020756b0[w->idx];
        break;
    case 1:
        snd = data_020756b4[w->idx];
        break;
    case 4:
        snd = data_020756b8[w->idx];
        break;
    case 3:
        snd = data_020756bc[w->idx];
        break;
    case 6:
        snd = data_020756c0[w->idx];
        break;
    default:
        break;
    }
    if (snd == -1)
        return;
    Sound_PlayVoiceArc(data_0207566c[w->idx], snd);   /* NNS_SndPlayerStopSeq + Sound_PlayVoice */
}

void NaviMsg_SetVisible(BOOL on) {
    NaviMsgWork *w;
    if (!gNaviMsgTask)
        return;
    w = NMW;
    if (w->mode == 0 && navi_locked())
        return;
    if (on) {
        w->flags |= 1;
        g_mn_layer[1][1].visible = 1;
    } else {
        w->flags &= ~1u;
        g_mn_layer[1][1].visible = 0;
    }
}

static void NaviMsg_State_Intro(NaviMsgWork *w) {
    w->cnt++;
    if (w->cnt >= 60) {
        w->alpha = 31;
        w->fndc = NULL;
        NaviMsgFace_Play(w, 2);
        NaviMsg_LoadPanel(w, data_02075664[w->idx], 0xf0);
        Sound_PlaySeqArc(data_0207566c[w->idx], 9);
    } else {
        w->alpha = (s16)((w->cnt * 31 / 60) & 0x1f);
    }
}

void NaviMsg_StartIntro(void) {
    NaviMsgWork *w;
    if (!gNaviMsgTask)
        return;
    w = NMW;
    NaviMsgFace_StartIdle(w);
    w->fndc = NaviMsg_State_Intro;
    w->alpha = 0;
    w->cnt = 0;
    NaviMsg_SetVisible(1);
    g_mn_layer[1][1].visible = 1;   /* DISPCNT |= BG1 | BG2 */
    g_mn_layer[1][2].visible = 1;
}

static void NaviMsg_State_FadeIn(NaviMsgWork *w) {
    w->cnt++;
    if (w->cnt >= 30) {
        w->alpha = 31;
        w->fndc = NULL;
        return;
    }
    w->alpha = (s16)((w->cnt * 31 / 30) & 0x1f);
}

static void NaviMsg_State_FadeOut(NaviMsgWork *w) {
    w->cnt++;
    if (w->cnt >= 30) {
        w->alpha = 0;
        g_mn_layer[1][1].visible = 0;
        w->flags &= ~1u;
        w->fndc = NULL;
    } else {
        w->alpha = (s16)(31 - ((w->cnt * 31 / 30) & 0x1f));
    }
}

void NaviMsg_FadeVisible(BOOL on) {
    NaviMsgWork *w;
    if (!gNaviMsgTask)
        return;
    w = NMW;
    if (w->mode == 0 && navi_locked())
        return;
    if ((w->flags & 1) && on)
        return;
    if (!(w->flags & 1) && !on)
        return;
    w->flags |= 0x800000;
    NaviMsgFace_StartIdle(w);
    if (on) {
        w->alpha = 0;
        w->fndc = NaviMsg_State_FadeIn;
    } else {
        w->alpha = 31;
        w->fndc = NaviMsg_State_FadeOut;
    }
    w->cnt = 0;
    NaviMsg_SetVisible(1);
}

static void NaviMsg_VBlankMain(void) {
    NaviMsgWork *w;
    if (!gNaviMsgTask) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w = NMW;
    if (w->flags & 0x20000) {
        w->flags &= ~0x20000u;
        w->flags ^= 0x10000;
        NaviMsg_LoadFaceFrame(w, w->face_req);   /* BG1CNT swap + NaviMsg_LoadFacePalette */
    }
    if (w->flags & 0x400000) {
        g_mn_layer[1][0].visible = 1;
        g_mn_layer[1][3].visible = w->panel != NULL;
        w->flags &= ~0x400000u;
    } else if (w->flags & 0x800000) {
        g_mn_layer[1][0].visible = 0;
        g_mn_layer[1][3].visible = 0;
        w->flags &= ~0x800000u;
    }
}

static void NaviMsg_Main(void) {
    NaviMsgWork *w = TASK_WORK(NaviMsgWork);
    if (w->flags & 2) {
        if (w->fnd8)
            w->fnd8(w);
        if (w->fndc)
            w->fndc(w);
        if (w->fne0)
            w->fne0(w);
        NaviMsg_UpdateFaceAnim(w);
        if (w->flags & 0x40000) {
            w->flags &= ~0x40000u;   /* face map copied */
            w->flags |= 0x80000;
        } else if (w->flags & 0x80000) {
            w->flags &= ~0x80000u;   /* NaviMsg_ReadFaceFrameChr(.., 0) */
            w->flags |= 0x100000;
        } else if (w->flags & 0x100000) {
            w->flags &= ~0x100000u;  /* NaviMsg_ReadFaceFrameChr(.., 1) */
            w->flags |= 0x20000;
        } else if (w->flags & 0x200000) {
            NaviMsg_ReadPanel(w, w->fidx);   /* Background_Draw(&w->bg94) */
            Background_Show(1, 3, w->panel);
            g_mn_layer[1][3].visible = 0;
            w->flags &= ~0x200000u;
            w->flags |= 0x400000;
            w->fne0 = NaviMsg_State_HideTimer;
        }
    }
    {
        MnBgLayer *L = &g_mn_layer[1][1];
        L->alpha = w->alpha >= 31 ? 16 : (s16)(w->alpha * 16 / 31);
    }
    if (w->unk108 == -1)
        return;
    w->unk10c--;
    if (w->unk10c > 0)
        return;
    Sound_PlayVoiceArc(w->unk110, w->unk108);
    w->unk108 = -1;
}

static void NaviMsg_Destructor(void) {
    NaviMsgWork *w = TASK_WORK(NaviMsgWork);
    Background_Hide(1, 0);
    Background_Hide(1, 1);
    Background_Hide(1, 3);
    mn_bg_free(w->panel);
    w->panel = NULL;
    g_mn_layer[1][1].img = NULL;
    mn_bg_release_tex(&w->face);
    free(w->face.idx);
    free(w->face.argb);
    gNaviMsgTask = NULL;
}

void NaviMsg_Create(void) {
    NaviMsgWork *w;
    if (data_0207b894 == 1 && gGameState.unk10 == 0 && navi_locked()) {
        g_mn_layer[1][2].visible = 1;   /* only the panel background */
        return;
    }
    gNaviMsgTask = Task_Create(NaviMsg_Main, NaviMsg_Destructor, 0, 0x3000, 3, sizeof(NaviMsgWork));
    w = (NaviMsgWork *)gNaviMsgTask->work;
    memset(w, 0, sizeof(*w));
    w->mode = data_0207b894;
    w->idx = data_0207b8a4 < 0 ? 0 : data_0207b8a4;
    /* the face buffer (bg1 at (16,6) tiles = (128,48) px; BG1 plane 256x256) */
    w->face.w = s_faces.w ? s_faces.w : 8;
    w->face.h = s_faces.h ? s_faces.h : 8;
    for (w->face.tw = 8; w->face.tw < w->face.w; w->face.tw *= 2) {
    }
    for (w->face.th = 8; w->face.th < w->face.h; w->face.th *= 2) {
    }
    w->face.idx = (u8 *)calloc((size_t)w->face.w * w->face.h, 1);
    w->face.dirty = 1;
    snprintf(w->face.name, sizeof(w->face.name), "navi_face");
    w->face_req = w->face_shown = -1;
    Background_Show(1, 1, &w->face);
    Background_SetPriority(1, 1, 2);
    gDisplaySub[1].x = (u16)(256 - 128 - s_faces.x0);
    gDisplaySub[1].y = (u16)(256 - 48 - s_faces.y0);
    /* the panel: data_022c4664[3] (mode 0) / [0xf] (mode 1) drawn into bg3 */
    w->fidx = w->mode == 0 ? 3 : 0xf;
    NaviMsg_ReadPanel(w, w->fidx);
    Background_Show(1, 3, w->panel);
    Background_SetPriority(1, 3, 0);
    gDisplaySub[3].x = (u16)(256 - 16);   /* bg3 at (2,2) tiles */
    gDisplaySub[3].y = (u16)(256 - 16);
    g_mn_layer[1][0].visible = 0;
    g_mn_layer[1][1].visible = 0;
    g_mn_layer[1][3].visible = 0;
    g_mn_layer[1][2].visible = 1;   /* SetPlane: BG2 */
    w->alpha = 31;
    if (w->mode == 0)
        NaviMsgFace_StartIdle(w);
    else
        NaviMsgFace_Play(w, 10);
    w->flags |= 2;
    Task_Create(NaviMsg_VBlankMain, NULL, 0, 0xf100, 3, 0);
    w->unk108 = -1;
    w->unk10c = 0;
}

void NaviMsg_Destroy(void) {
    if (gNaviMsgTask)
        Task_Destroy(gNaviMsgTask);
}

/* ------------------------------------------------------------------ NaviM */
typedef struct NaviMWork {
    s32 unk4;               /* character */
    MnTask *task8, *taskC;  /* PartA (counter), PartB (emeralds) */
    MnSpr spr[24];
    MnSpr sprX;             /* spr[24]: the step name */
    s16 unk718, unk71a;     /* lives digits (-1: no tens) */
    s32 unk71c;
    u16 unk720;             /* map step */
} NaviMWork;
typedef struct {
    NaviMWork *owner;
    s32 cnt;
    s16 hi, lo;
    s32 cur, target, step;
} NaviMPartA;
typedef struct {
    NaviMWork *res;
    s16 t;
    u16 flags;              /* emeralds (gGameState.unk28) */
    s32 sel;                /* unkc: slot left empty (-1) */
    s32 unk10;              /* the new emerald */
} NaviMPartB;

static MnTask *gNaviMTask;
static MnRes *gNaviMSprite;
static u16 gNaviMNewEmeraldMask;
static u16 gNaviMCounterPending;
static u16 gNaviMSavedStep;
/* data_02077910: OBJ palette row of each sprite (kept for reference) */

void NaviM_Load(void) {
    mn_res_free(gNaviMSprite);
    gNaviMSprite = mn_res_load("act/navi_m.bac");
}

void NaviM_Release(void) {
    mn_res_free(gNaviMSprite);
    gNaviMSprite = NULL;
}

void NaviM_SetNewEmerald(s32 n) { gNaviMNewEmeraldMask = (u16)(1 << n); }
u16 NaviM_CheckStepAdvanced(void) {
    u16 n = gGameState.unkE;
    if (gNaviMSavedStep < n && n >= 1)
        return gNaviMCounterPending = 1;
    return gNaviMCounterPending = 0;
}
void NaviM_SaveStep(void) { gNaviMSavedStep = gGameState.unkE; }
void NaviM_ResetPending(void) {
    gNaviMNewEmeraldMask = 0;
    gNaviMCounterPending = 0;
    gNaviMSavedStep = gGameState.unkE;
}

/* NaviM_AdvanceSpriteAnim: run an animation dt frames ahead */
static void NaviM_AdvanceSpriteAnim(MnSpr *a, s16 dt) {
    while (dt-- > 0 && !(a->flags & SPR_ENDED))
        AnimatorSprite_ProcessAnimation(a, 0, 0);
}

static void NaviM_Draw(MnSpr *s) {
    AnimatorSprite_Draw(s);
}

static void NaviMPartA_Draw(NaviMPartA *c) {
    NaviMWork *w = c->owner;
    NaviM_Draw(&w->spr[12]);
    NaviM_Draw(&w->spr[11]);
    if (c->hi != -1) {
        MnSpr *s = &w->spr[c->hi];
        s->x = 0x2f;
        s->y = 0x6f;
        NaviM_Draw(s);
    }
    {
        MnSpr *s = &w->spr[c->lo];
        s->x = 0x39;
        s->y = 0x6f;
        NaviM_Draw(s);
    }
}

static void NaviMPartA_Main(void) { NaviMPartA_Draw(TASK_WORK(NaviMPartA)); }

static void NaviMPartA_State_Count(void) {
    NaviMPartA *w = TASK_WORK(NaviMPartA);
    w->cnt++;
    if (w->cnt >= 2) {
        w->cur += w->step;
        w->cnt = 0;
    }
    if (w->cur == w->target) {
        gTaskCurrent->func = NaviMPartA_Main;
        w->cur = w->target;
        gNaviMCounterPending = 0;
    }
    w->hi = (s16)(w->cur / 10);
    if (w->hi == 0)
        w->hi = -1;
    w->lo = (s16)(w->cur % 10);
    NaviMPartA_Draw(w);
}

static void NaviMPartB_Draw(NaviMPartB *w) {
    u16 mask = w->flags & ~gNaviMNewEmeraldMask;
    MnSpr *cur = &w->res->spr[14];
    MnSpr *icon = &w->res->spr[15];
    s32 i;
    for (i = 0; i < 7; i++, icon++) {
        if (mask & (1 << i)) {
            icon->x = data_02077894[i].x;
            icon->y = data_02077894[i].y;
            NaviM_Draw(icon);
        } else if (i != w->sel) {
            cur->x = data_02077894[i].x;
            cur->y = data_02077894[i].y;
            NaviM_Draw(cur);
        }
    }
}

static void NaviMPartB_Main(void) { NaviMPartB_Draw(TASK_WORK(NaviMPartB)); }

static void NaviMPartB_State_EmeraldAnim(void) {
    NaviMPartB *w = TASK_WORK(NaviMPartB);
    MnSpr *s = &w->res->spr[23];
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    NaviM_Draw(s);
    NaviMPartB_Draw(w);
    if (!(s->flags & SPR_ENDED))
        return;
    gTaskCurrent->func = NaviMPartB_Main;
    gNaviMNewEmeraldMask = 0;
    w->sel = -1;
}

static s32 FX_MulR(s32 a, s32 b) { return (s32)(((s64)a * b + 0x800) >> 12); }
static s32 NaviM_Ease(s32 from, s32 to, s32 t) {
    s32 i = 3;
    if (t < 0)
        t = 0;
    if (t > 0x1000)
        t = 0x1000;
    do {
        from += FX_MulR(to - from, t);
    } while (i-- != 0);
    return from;
}

static void NaviMPartB_State_EmeraldGrow(void) {
    NaviMPartB *w = TASK_WORK(NaviMPartB);
    s32 sx, sy;
    w->t = (s16)(w->t + 0x400);
    sx = NaviM_Ease(0xf33, 0x1400, w->t);
    sy = NaviM_Ease(0xf33, 0x1400, w->t);
    AnimatorSprite_DrawScaled(&w->res->spr[22], sx, sy);
    NaviMPartB_Draw(w);
    if (w->t < 0x1000)
        return;
    gTaskCurrent->func = NaviMPartB_State_EmeraldAnim;
    AnimatorSprite_SetAnimation(&w->res->spr[23], w->res->spr[23].anim);
}

static void NaviMPartB_State_EmeraldShrink(void) {
    NaviMPartB *w = TASK_WORK(NaviMPartB);
    s32 sx, sy;
    w->t = (s16)(w->t + 0x100);
    sx = NaviM_Ease(0x1400, 0xf33, w->t);
    sy = NaviM_Ease(0x1400, 0xf33, w->t);
    if (sx <= 0x1000)
        w->sel = w->unk10;
    AnimatorSprite_DrawScaled(&w->res->spr[22], sx, sy);
    NaviMPartB_Draw(w);
    if (w->t < 0x1000)
        return;
    gTaskCurrent->func = NaviMPartB_State_EmeraldGrow;
    w->t = 0;
}

static void NaviM_Main(void) {
    NaviMWork *w = TASK_WORK(NaviMWork);
    MnSpr *s;
    NaviM_Draw(&w->spr[13]);
    w->spr[10].x = -2;
    NaviM_Draw(&w->spr[10]);
    if (w->unk718 != -1) {
        s = &w->spr[w->unk718];
        s->x = 0x39;
        s->y = 0x57;
        NaviM_Draw(s);
    }
    s = &w->spr[w->unk71a];
    s->x = 0x43;
    s->y = 0x57;
    NaviM_Draw(s);
    if (w->unk71c != 0)
        NaviM_Draw(&w->sprX);
}

static void NaviM_Destructor(void) {
    NaviMWork *w = TASK_WORK(NaviMWork);
    if (w->task8)
        Task_Destroy(w->task8);
    if (w->taskC)
        Task_Destroy(w->taskC);
    gNaviMTask = NULL;
}

static void NaviM_InitSprite(MnSpr *s, u16 anim) {
    AnimatorSprite_Init(s, gNaviMSprite, anim, 0x40000, 1);   /* OBJ priority 2, sub engine */
    s->x = s->y = 0;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
}

void NaviM_Create(void) {
    NaviMWork *w;
    NaviMPartA *a;
    NaviMPartB *b;
    u16 i, sel, anim, v;
    if (!gNaviMSprite)
        return;
    gNaviMTask = Task_Create(NaviM_Main, NaviM_Destructor, 0, 0x3000, 0, sizeof(NaviMWork));
    w = (NaviMWork *)gNaviMTask->work;
    memset(w, 0, sizeof(*w));
    w->unk4 = gGameState.unk0;
    w->unk718 = (s16)(gGameState.lives / 10);
    if (w->unk718 == 0)
        w->unk718 = -1;
    w->unk71a = (s16)(gGameState.lives % 10);
    for (i = 0; i <= 12; i++)
        NaviM_InitSprite(&w->spr[i], i);
    w->unk720 = gGameState.unkE;
    sel = w->unk4 == 0 ? 0 : 0xb;
    anim = w->unk720 < 16 ? data_020778e0[w->unk720] : 0xffff;
    if (anim == 0xffff)
        anim = 0x23;
    else
        w->unk71c = 1;
    NaviM_InitSprite(&w->sprX, anim);
    for (i = 13; i <= 23; i++)
        NaviM_InitSprite(&w->spr[i], (u16)(i + sel));
    if (gNaviMNewEmeraldMask != 0) {
        /* the sparkle (data_0207788c) is started k*16-1 frames in for emerald k */
        s16 k;
        for (k = 0; k < 7; k++)
            if (gNaviMNewEmeraldMask & (1 << k))
                break;
        if (k != 0)
            NaviM_AdvanceSpriteAnim(&w->spr[23], (s16)(k * 16 - 1));
    }
    w->task8 = Task_Create(NaviMPartA_Main, NULL, 0, 0x4000, 0, sizeof(NaviMPartA));
    a = (NaviMPartA *)w->task8->work;
    memset(a, 0, sizeof(*a));
    a->owner = w;
    if (gNaviMCounterPending != 0 && w->unk720 > 0)
        v = data_020778b0[w->unk720 - 1];
    else
        v = data_020778b0[w->unk720 < 24 ? w->unk720 : 23];
    a->hi = (s16)(v / 10);
    if (a->hi == 0)
        a->hi = -1;
    a->lo = (s16)(v % 10);
    w->taskC = Task_Create(NaviMPartB_Main, NULL, 0, 0x4000, 0, sizeof(NaviMPartB));
    b = (NaviMPartB *)w->taskC->work;
    memset(b, 0, sizeof(*b));
    b->res = w;
    b->sel = -1;
    b->flags = gGameState.unk28;
}

void NaviM_Destroy(void) {
    if (gNaviMTask)
        Task_Destroy(gNaviMTask);
}

void NaviM_StartCounter(void) {
    NaviMWork *w;
    NaviMPartA *s;
    s32 d;
    if (gNaviMCounterPending == 0 || !gNaviMTask)
        return;
    w = (NaviMWork *)gNaviMTask->work;
    w->task8->func = NaviMPartA_State_Count;
    s = (NaviMPartA *)w->task8->work;
    s->cnt = 0;
    s->cur = data_020778b0[w->unk720 - 1];
    s->target = data_020778b0[w->unk720];
    d = s->target - s->cur;
    s->step = d == 0 ? 0 : d > 0 ? 1 : -1;
}

void NaviM_StartEmeraldAppear(void) {
    NaviMWork *w;
    NaviMPartB *s;
    s32 i;
    if (gNaviMNewEmeraldMask == 0 || !gNaviMTask)
        return;
    w = (NaviMWork *)gNaviMTask->work;
    w->taskC->func = NaviMPartB_State_EmeraldShrink;
    s = (NaviMPartB *)w->taskC->work;
    s->t = 0;
    for (i = 0; i < 7; i++)
        if (gNaviMNewEmeraldMask & (1 << i))
            break;
    s->unk10 = i;
    w->spr[23].x = w->spr[22].x = data_02077894[i].x;
    w->spr[23].y = w->spr[22].y = data_02077894[i].y;
}

BOOL NaviM_IsCounting(void) {
    return gNaviMTask && ((NaviMWork *)gNaviMTask->work)->task8->func == NaviMPartA_State_Count;
}

/* --------------------------------------------------------- composition */
/* Where the sub-screen pieces go in the 640x480 frame (DS px of the sub engine).
 * Scale 1.5 keeps the original pixels square at an integer-friendly size next
 * to the 2.5x map. */
#define NV_SCALE 1.5f
void mn_navi_compose(void) {
    NaviMsgWork *w = gNaviMsgTask ? NMW : NULL;
    if (gNaviMTask) {
        /* the NaviM panel: its piece of the frame BG with the counters and emerald slots */
        mn_comp_add(1, 16, 72, 128, 128, 640 - 16 - 112 * NV_SCALE, 16, NV_SCALE, (1 << 0) | (1 << 1) | (1 << 3), 0);
    }
    if (!w)
        return;
    if (g_mn_layer[1][1].visible)   /* the face, without the frame behind it */
        mn_comp_add(1, 128, 48, 256, 176, 640 - 128 * NV_SCALE, 480 - 128 * NV_SCALE, NV_SCALE,
                    (1 << 0) | (1 << 2) | (1 << 3), 1);
    if (g_mn_layer[1][0].visible)   /* the balloon and the message panel */
        mn_comp_add(1, 0, 0, 176, 88, 8, 8, NV_SCALE, (1 << 1) | (1 << 2), 1);
}

/* test aids */
int mn_navi_state(int *face, int *panel, int *alpha) {
    NaviMsgWork *w = gNaviMsgTask ? NMW : NULL;
    if (face)
        *face = w ? w->face_shown : -1;
    if (panel)
        *panel = w && g_mn_layer[1][0].visible && w->panel ? w->fidx : -1;
    if (alpha)
        *alpha = w ? (g_mn_layer[1][1].visible ? w->alpha : -1) : -1;
    return (w != NULL) | (gNaviMTask != NULL) << 1;
}
int mn_navi_counter(void) {
    NaviMPartA *a;
    if (!gNaviMTask)
        return -1;
    a = (NaviMPartA *)((NaviMWork *)gNaviMTask->work)->task8->work;
    return (a->hi < 0 ? 0 : a->hi * 10) + a->lo;
}
int mn_navi_face_pixels(void) {
    NaviMsgWork *w = gNaviMsgTask ? NMW : NULL;
    int i, n = 0;
    if (!w || !w->face.idx)
        return -1;
    for (i = 0; i < w->face.w * w->face.h; i++)
        n += w->face.idx[i] != 0;
    return n;
}
