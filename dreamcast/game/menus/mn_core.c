/*
 * Front-end engine services (portable: host and KOS).
 *
 * DS service                       here
 * Task_Create/Destroy/...          a fixed task table run in priority order
 * Pad_Reset / Pad_Update           translated (GameSystem/Pad_*), keys from InputState
 * Background_Init/Draw (BBG)       MnBgLayer per engine, images from .mnbg (indexed)
 * PaletteTransfer to BG palettes   mn_bg_set_colors (texture re-expanded)
 * AnimatorSprite_*                 MnSpr on converted BAC frames.bin (DSPR v1)
 * OAM / BG compositing             MnQuad list sorted by DS priority rules
 * KeyframeAnim_* (.ama)            translated (Graphics/Model/KeyframeAnim_*)
 * master brightness                g_mn_bright[engine] -> overlay quad per screen
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mn_internal.h"
#ifdef _arch_dreamcast
#include "../engine/pak.h"   /* /cd/data.pak: per-stage files without a directory walk each */
#endif

char g_mn_dir[96] = "/cd/menus";
char g_mn_sprite_dir[96] = "/cd/sprites";
MnData g_mn_data;
u8 gGameLanguage = 1;
u32 gRandSeed = 0x12345678;
MnBgOfs gDisplayMain[4], gDisplaySub[4];
s16 g_mn_bright[2];
MnBgLayer g_mn_layer[2][4];
MnComp g_mn_comp;
MenuHooks g_menu_hooks;

static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 rd32(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }

int mn_lang(void) {
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return gGameLanguage;
    default:
        return 1;
    }
}

int mn_load_file(const char *path, u8 **out, u32 *size) {
    FILE *f;
    long n;
    u8 *b;
    *out = NULL;
    *size = 0;
#ifdef _arch_dreamcast
    {   /* /cd/data.pak (engine/pak.h) */
        void *pb;
        u32 ps;
        int k = pak_load(path, &pb, &ps);
        if (k == PAK_FOUND) {
            *out = (u8 *)pb;
            *size = ps;
            return 0;
        }
        if (k == PAK_MISSING)
            return -1;
    }
#endif
    f = fopen(path, "rb");
    if (!f)
        return -1;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n <= 0) {
        fclose(f);
        return -1;
    }
    b = (u8 *)malloc((size_t)n + 1);
    if (!b || fread(b, 1, (size_t)n, f) != (size_t)n) {
        free(b);
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = b;
    *size = (u32)n;
    return 0;
}

/* ------------------------------------------------------------ menudata.bin */
static int sect(const u8 *buf, u32 size, const char *name, void *dst, u32 want) {
    u32 n = rd16(buf + 6), i;
    for (i = 0; i < n; i++) {
        const u8 *d = buf + 8 + 20 * i;
        if (strncmp((const char *)d, name, 12) == 0) {
            u32 off = rd32(d + 12), sz = rd32(d + 16);
            if (sz != want || off + sz > size)
                return -1;
            memcpy(dst, buf + off, sz);
            return 0;
        }
    }
    return -1;
}

int mn_data_load(const u8 *buf, u32 size) {
    MnData *d = &g_mn_data;
    int e = 0;
    u8 icon[544];
    memset(d, 0, sizeof(*d));
    if (size < 8 || memcmp(buf, "RMND", 4) != 0 || rd16(buf + 4) != 1)
        return -1;
#define S(n, f) e |= sect(buf, size, n, &d->f, sizeof(d->f))
    S("save_sig", save_sig);
    S("story_s", story_s);
    S("story_b", story_b);
    S("zone_ord_s", zone_ord[0]);
    S("zone_ord_b", zone_ord[1]);
    S("tm_slide", tm_slide);
    S("zm_adj", zm_adj);
    S("zm_rects", zm_rects);
    S("zm_icons", zm_icons);
    S("as_rank1", as_rank1);
    S("as_rank2", as_rank2);
    S("st_widths", st_widths);
#undef S
    e |= sect(buf, size, "vmu_icon", icon, sizeof(icon));
    memcpy(d->vmu_pal, icon, 32);
    memcpy(d->vmu_icon, icon + 32, 512);
    d->ok = e == 0;
    return e;
}

/* ------------------------------------------------------------------ tasks */
static MnTask s_tasks[MN_MAX_TASKS];
MnTask *gTaskCurrent;

MnTask *Task_Create(void (*fn)(void), void (*dtor)(void), u16 flags, u16 prio, u16 tag, u32 size) {
    int i;
    (void)flags;
    for (i = 0; i < MN_MAX_TASKS; i++) {
        MnTask *t = &s_tasks[i];
        if (t->used)
            continue;
        memset(t, 0, sizeof(*t));
        t->used = 1;
        t->fresh = 1;
        t->func = fn;
        t->dtor = dtor;
        t->prio = prio;
        t->tag = tag;
        t->size = size;
        t->work = calloc(1, size ? size : 4);
        return t;
    }
    fprintf(stderr, "menus: task table full\n");
    return NULL;
}

/* The destructor runs at once (the DS frees the task at once: a destructor that
 * resets the display must run before the next scene sets it up); the work
 * memory of a task destroyed while the list runs stays valid until the end
 * of the frame, because the code after Task_Destroy(gTaskCurrent) may still
 * read it. */
static int s_running;
static void task_release(MnTask *t) {
    free(t->work);
    memset(t, 0, sizeof(*t));
}
void Task_Destroy(MnTask *t) {
    MnTask *prev = gTaskCurrent;
    if (!t || !t->used || t->dead)
        return;
    t->dead = 1;
    if (t->dtor) {
        gTaskCurrent = t;
        t->dtor();
        gTaskCurrent = prev;
    }
    if (!s_running)
        task_release(t);
}
void Task_DestroyAll(void) {
    int i;
    for (i = 0; i < MN_MAX_TASKS; i++)
        if (s_tasks[i].used && !s_tasks[i].dead)
            Task_Destroy(&s_tasks[i]);
}
void Task_DestroyRange(u16 lo, u16 hi) {
    int i;
    for (i = 0; i < MN_MAX_TASKS; i++)
        if (s_tasks[i].used && !s_tasks[i].dead && s_tasks[i].prio >= lo && s_tasks[i].prio <= hi)
            Task_Destroy(&s_tasks[i]);
}
void Task_DestroyByTag(u16 tag) {
    int i;
    for (i = 0; i < MN_MAX_TASKS; i++)
        if (s_tasks[i].used && !s_tasks[i].dead && s_tasks[i].tag == tag)
            Task_Destroy(&s_tasks[i]);
}
int Task_Count(void) {
    int i, n = 0;
    for (i = 0; i < MN_MAX_TASKS; i++)
        n += s_tasks[i].used && !s_tasks[i].dead;
    return n;
}

/* The DS list runs in priority order (low value first); tasks created during a
 * frame start on the next frame. */
void Task_RunAll(void) {
    int order[MN_MAX_TASKS], n = 0, i, j;
    for (i = 0; i < MN_MAX_TASKS; i++) {
        if (s_tasks[i].used)
            s_tasks[i].fresh = 0;
    }
    for (i = 0; i < MN_MAX_TASKS; i++)
        if (s_tasks[i].used)
            order[n++] = i;
    for (i = 1; i < n; i++) {   /* stable insertion sort by prio */
        int v = order[i];
        for (j = i; j > 0 && s_tasks[order[j - 1]].prio > s_tasks[v].prio; j--)
            order[j] = order[j - 1];
        order[j] = v;
    }
    s_running = 1;
    for (i = 0; i < n; i++) {
        MnTask *t = &s_tasks[order[i]];
        if (!t->used || t->dead || t->fresh || !t->func)
            continue;
        gTaskCurrent = t;
        t->func();
    }
    gTaskCurrent = NULL;
    s_running = 0;
    for (i = 0; i < MN_MAX_TASKS; i++)
        if (s_tasks[i].used && s_tasks[i].dead)
            task_release(&s_tasks[i]);
}

/* -------------------------------------------------------------------- pad */
MnPad gPad;
void Pad_Reset(MnPad *o, const u16 *a, const u16 *b) {
    int i;
    memset(o, 0, sizeof(*o));
    for (i = 0; i < 12; i++) {
        o->delay[i] = a ? a[i] : 0x14;
        o->interval[i] = b ? b[i] : 2;
    }
}
void Pad_Update(MnPad *p, u16 keys) {
    int i;
    p->prev = p->cur;
    p->cur = keys;
    p->trg = p->cur & (p->cur ^ p->prev);
    p->rel = p->prev & (p->cur ^ p->prev);
    p->rep = p->trg;
    for (i = 0; i < 12; i++) {
        u32 bit = 1u << i;
        if (!(p->cur & bit)) {
            p->cnt[i] = p->delay[i];
        } else if (p->cnt[i] != 0) {
            p->cnt[i]--;
        } else {
            p->rep |= bit;
            p->cnt[i] = p->interval[i];
        }
    }
}
/* DC pad -> DS keys: A->A, B->B, X->X, Y->Y, START->START, D-pad/analog -> D-pad */
u16 mn_pad_from_input(const InputState *in) {
    u16 o = 0;
    u32 b = in ? in->held : 0;
    if (b & IN_JUMP)
        o |= PAD_A;
    if (b & IN_TRICK)
        o |= PAD_B;
    if (b & IN_BOOST)
        o |= PAD_X;
    if (b & IN_ALT)
        o |= PAD_Y;
    if (b & IN_START)
        o |= PAD_START;
    if (b & IN_RIGHT)
        o |= PAD_RIGHT;
    if (b & IN_LEFT)
        o |= PAD_LEFT;
    if (b & IN_UP)
        o |= PAD_UP;
    if (b & IN_DOWN)
        o |= PAD_DOWN;
    return o;
}

/* ------------------------------------------------------------ BG images */
MnBgImg *mn_bg_load(const char *bbg_path) {
    char path[200], name[64];
    const char *base = strrchr(bbg_path, '/');
    const char *dot;
    u8 *b;
    u32 size;
    MnBgImg *img;
    size_t n;
    base = base ? base + 1 : bbg_path;
    dot = strrchr(base, '.');
    n = dot ? (size_t)(dot - base) : strlen(base);
    if (n >= sizeof(name))
        n = sizeof(name) - 1;
    memcpy(name, base, n);
    name[n] = 0;
    snprintf(path, sizeof(path), "%s/bg/%s.mnbg", g_mn_dir, name);
    if (mn_load_file(path, &b, &size) != 0) {
        fprintf(stderr, "menus: missing %s\n", path);
        return NULL;
    }
    if (size < 528 || memcmp(b, "MNBG", 4) != 0) {
        free(b);
        return NULL;
    }
    img = (MnBgImg *)calloc(1, sizeof(*img));
    img->w = rd16(b + 4);
    img->h = rd16(b + 6);
    img->type = rd16(b + 8);
    img->pofs = rd16(b + 10);
    img->pslot = rd16(b + 12);
    if (528u + (u32)img->w * img->h > size) {
        free(b);
        free(img);
        return NULL;
    }
    memcpy(img->pal, b + 16, 512);
    img->idx = (u8 *)malloc((size_t)img->w * img->h);
    memcpy(img->idx, b + 528, (size_t)img->w * img->h);
    free(b);
    for (img->tw = 8; img->tw < img->w; img->tw *= 2) {
    }
    for (img->th = 8; img->th < img->h; img->th *= 2) {
    }
    snprintf(img->name, sizeof(img->name), "%s", name);
    img->dirty = 1;
    return img;
}

void mn_bg_release_tex(MnBgImg *b);   /* mn_draw.c */
#define MN_TRASH 64
static MnRes *s_trash_res[MN_TRASH];
static MnBgImg *s_trash_bg[MN_TRASH];
static int s_ntrash_res, s_ntrash_bg;
static void bg_free_now(MnBgImg *b);
void mn_flush_freed(void);
void mn_bg_free(MnBgImg *b) {
    int e, l;
    if (!b)
        return;
    for (e = 0; e < 2; e++)
        for (l = 0; l < 4; l++)
            if (g_mn_layer[e][l].img == b)
                g_mn_layer[e][l].img = NULL;
    if (s_ntrash_bg >= MN_TRASH)
        mn_flush_freed();
    s_trash_bg[s_ntrash_bg++] = b;
}
static void bg_free_now(MnBgImg *b) {
    mn_bg_release_tex(b);
    free(b->idx);
    free(b->argb);
    free(b);
}

void mn_bg_set_colors(MnBgImg *b, int start, int count, const u16 *bgr) {
    int i;
    if (!b)
        return;
    for (i = 0; i < count && start + i < 256; i++) {
        if (b->pal[start + i] != bgr[i]) {
            b->pal[start + i] = bgr[i];
            b->dirty = 1;
        }
    }
}

void mn_bg_expand(MnBgImg *b) {
    u16 lut[256];
    int x, y;
    if (!b->argb)
        b->argb = (u16 *)calloc((size_t)b->tw * b->th, 2);
    for (x = 0; x < 256; x++) {
        u16 c = b->pal[x];
        lut[x] = (u16)(0x8000 | (c & 0x1f) << 10 | (c & 0x3e0) | (c >> 10 & 0x1f));
    }
    lut[0] = 0;
    for (y = 0; y < b->h; y++) {
        const u8 *s = b->idx + (size_t)y * b->w;
        u16 *d = b->argb + (size_t)y * b->tw;
        for (x = 0; x < b->w; x++)
            d[x] = lut[s[x]];
    }
}

void Background_Show(int engine, int layer, MnBgImg *img) {
    MnBgLayer *l = &g_mn_layer[engine & 1][layer & 3];
    l->img = img;
    l->visible = img != NULL;
    if (l->alpha == 0)
        l->alpha = 16;
}
void Background_SetSize(int engine, int layer, int w, int h) {
    g_mn_layer[engine & 1][layer & 3].wrap_w = (u16)w;
    g_mn_layer[engine & 1][layer & 3].wrap_h = (u16)h;
}
void Background_Hide(int engine, int layer) { g_mn_layer[engine & 1][layer & 3].visible = 0; }
void Background_SetPriority(int engine, int layer, int prio) { g_mn_layer[engine & 1][layer & 3].prio = (u8)(prio & 3); }

void mn_display_reset(void) {
    int e, l;
    for (e = 0; e < 2; e++)
        for (l = 0; l < 4; l++) {
            g_mn_layer[e][l].img = NULL;
            g_mn_layer[e][l].visible = 0;
            g_mn_layer[e][l].prio = (u8)l;
            g_mn_layer[e][l].alpha = 16;
            g_mn_layer[e][l].wrap_w = g_mn_layer[e][l].wrap_h = 0;
        }
    memset(gDisplayMain, 0, sizeof(gDisplayMain));
    memset(gDisplaySub, 0, sizeof(gDisplaySub));
}

/* -------------------------------------------------------- sprite resources */
MnRes *mn_res_load(const char *bac_path) {
    char id[64], path[220];
    const char *p = bac_path;
    u8 *b;
    u32 size, need;
    int i, k = 0;
    MnRes *r;
    while (*p == '/')
        p++;
    /* "act/title.bac" -> "act__title" (bac2dc.py naming) */
    for (; *p && *p != '.' && k < 60; p++) {
        if (*p == '/') {
            id[k++] = '_';
            id[k++] = '_';
        } else {
            id[k++] = *p;
        }
    }
    id[k] = 0;
    snprintf(path, sizeof(path), "%s/%s/frames.bin", g_mn_sprite_dir, id);
    if (mn_load_file(path, &b, &size) != 0) {
        fprintf(stderr, "menus: missing %s\n", path);
        return NULL;
    }
    if (size < 16 || memcmp(b, "DSPR", 4) != 0 || rd16(b + 4) != 1) {
        free(b);
        return NULL;
    }
    r = (MnRes *)calloc(1, sizeof(*r));
    snprintf(r->id, sizeof(r->id), "%s", id);
    r->n_pages = rd16(b + 6);
    r->n_images = rd16(b + 8);
    r->n_anims = rd16(b + 10);
    r->n_frames = rd16(b + 12);
    need = 16 + 4u * r->n_pages + 16u * r->n_images + 12u * r->n_anims + 4u * r->n_frames;
    if (need > size || r->n_pages > 4) {
        free(b);
        free(r);
        return NULL;
    }
    r->blob = b;
    r->pages = (const MnPage *)(b + 16);
    r->images = (const MnImage *)(b + 16 + 4 * r->n_pages);
    r->anims = (const MnAnimDef *)((const u8 *)r->images + 16 * r->n_images);
    r->frames = (const MnFrame *)((const u8 *)r->anims + 12 * r->n_anims);
    snprintf(path, sizeof(path), "%s/%s/palettes.bin", g_mn_sprite_dir, id);
    if (mn_load_file(path, &r->palblob, &size) == 0 &&
        sprpal_parse(&r->pal, r->palblob, size, r->n_pages, r->n_images, r->n_anims) != 0) {
        free(r->palblob);
        r->palblob = NULL;
        memset(&r->pal, 0, sizeof(r->pal));
    }
    for (i = 0; i < r->n_pages; i++) {
        u8 *px;
        u32 psz;
        if (sprpal_page_is_pal4(&r->pal, i)) {   /* PAL4 page: 4-bit texels + palettes */
            snprintf(path, sizeof(path), "%s/%s/atlas%d.pt4", g_mn_sprite_dir, id, i);
            if (mn_load_file(path, &px, &psz) == 0) {
                if (psz == (u32)r->pages[i].w * r->pages[i].h / 2) {
                    r->texels[i] = px;
                    continue;
                }
                free(px);
            }
        }
        snprintf(path, sizeof(path), "%s/%s/atlas%d.argb1555", g_mn_sprite_dir, id, i);
        if (mn_load_file(path, &px, &psz) == 0) {
            if (psz == (u32)r->pages[i].w * r->pages[i].h * 2)
                r->pixels[i] = (u16 *)px;
            else
                free(px);
        }
    }
    return r;
}

void mn_res_release_tex(MnRes *r);   /* mn_draw.c */
/* Freed files wait for the end of the frame: a task destroyed in the frame that drew it (Task_Destroy /
 * Task_DestroyAll after its draw, as the DS code does everywhere) leaves quads in this frame's list
 * that draw from its files.  mn_flush_freed() (end of menus_draw_tr, start of the next frame) frees
 * them. */
static void res_free_now(MnRes *r);
void mn_flush_freed(void) {
    int i;
    for (i = 0; i < s_ntrash_res; i++)
        res_free_now(s_trash_res[i]);
    for (i = 0; i < s_ntrash_bg; i++)
        bg_free_now(s_trash_bg[i]);
    s_ntrash_res = s_ntrash_bg = 0;
}
void mn_res_free(MnRes *r) {
    if (!r)
        return;
    if (s_ntrash_res >= MN_TRASH)
        mn_flush_freed();
    s_trash_res[s_ntrash_res++] = r;
}
static void res_free_now(MnRes *r) {
    int i;
    if (!r)
        return;
    mn_res_release_tex(r);
    for (i = 0; i < 4; i++) {
        free(r->pixels[i]);
        free(r->texels[i]);
    }
    free(r->palblob);
    free(r->blob);
    free(r);
}

/* -------------------------------------------------------- AnimatorSprite */
static s32 frame_wait(const MnSpr *s) {
    const MnAnimDef *a = &s->res->anims[s->anim];
    return (s32)s->res->frames[a->first + s->frame].duration << 12;
}
static int spr_valid(const MnSpr *s) { return s->res && s->res->blob && s->anim < s->res->n_anims; }
int AnimatorSprite_AnimCount(const MnSpr *s) { return s->res ? s->res->n_anims : 0; }

void AnimatorSprite_Init(MnSpr *s, const MnRes *r, u16 anim, u32 flags, int engine) {
    memset(s, 0, sizeof(*s));
    s->res = r;
    s->speed = 0x1000;
    s->flags = flags;
    s->engine = (u8)(engine & 1);
    s->alpha = 16;
    AnimatorSprite_SetAnimation(s, anim);
}

void AnimatorSprite_SetAnimation(MnSpr *s, u16 anim) {
    s->anim = anim;
    s->frame = 0;
    s->wait = 0;
    s->flags &= ~(u32)(SPR_STOPPED | SPR_ENDED);
}

/* AnimatorSprite_ProcessAnimation: the first call after SetAnimation runs the
 * first frame command (wait 0); then each frame waits duration frames. */
void AnimatorSprite_ProcessAnimation(MnSpr *s, s32 a, s32 b) {
    const MnAnimDef *ad;
    (void)a;
    (void)b;
    if (s->flags & SPR_STOPPED)
        return;
    s->flags &= ~(u32)SPR_ENDED;
    if (!spr_valid(s))
        return;
    ad = &s->res->anims[s->anim];
    if (ad->count == 0)
        return;
    if (s->wait > 0) {
        s->wait -= s->speed;
        if (s->wait > 0)
            return;
        if (s->frame + 1 >= ad->count) {
            s->flags |= SPR_ENDED;
            if (!(s->flags & SPR_LOOP)) {
                s->flags |= SPR_STOPPED;
                return;
            }
            s->frame = (u16)(ad->loop >= 0 && ad->loop < ad->count ? ad->loop : 0);
        } else {
            s->frame++;
        }
    }
    s->wait += frame_wait(s);
    if (s->wait <= 0)
        s->wait = 0;
}

static const MnImage *spr_image(const MnSpr *s) {
    const MnAnimDef *a;
    u16 im;
    if (!spr_valid(s))
        return NULL;
    a = &s->res->anims[s->anim];
    if (a->count == 0)
        return NULL;
    im = s->res->frames[a->first + (s->frame < a->count ? s->frame : a->count - 1)].image;
    if (im == 0xffff || im >= s->res->n_images)
        return NULL;
    return &s->res->images[im];
}

static MnQuad s_quads[MN_MAX_QUADS];
static int s_nquads;
static u8 s_max_oam;   /* the largest MnQuad.oam this frame */
int mn_quads(const MnQuad **out) {
    *out = s_quads;
    return s_nquads;
}
void mn_quads_reset(void) {
    s_nquads = 0;
    s_max_oam = 0;
    mn_3d_reset();
    mn_msgbox_gc();
    mn_flush_freed();
}

void AnimatorSprite_DrawScaled(MnSpr *s, fx32 scx, fx32 scy) {
    const MnImage *im;
    MnQuad *q;
    float sx = scx / 4096.0f, sy = scy / 4096.0f, l, t;
    if (s->flags & SPR_HIDDEN)
        return;
    im = spr_image(s);
    if (!im || s_nquads >= MN_MAX_QUADS || scx == 0 || scy == 0)
        return;
    l = (s->flags & SPR_HFLIP) ? -(float)(im->w - im->ox) : -(float)im->ox;
    t = (s->flags & SPR_VFLIP) ? -(float)(im->h - im->oy) : -(float)im->oy;
    q = &s_quads[s_nquads++];
    q->x = s->x + l * sx;
    q->y = s->y + t * sy;
    q->w = im->w * sx;
    q->h = im->h * sy;
    q->u0 = im->x;
    q->u1 = (float)(im->x + im->w);
    q->v0 = im->y;
    q->v1 = (float)(im->y + im->h);
    if (s->flags & SPR_HFLIP) {
        q->u0 = q->u1;
        q->u1 = im->x;
    }
    if (s->flags & SPR_VFLIP) {
        q->v0 = q->v1;
        q->v1 = im->y;
    }
    q->res = s->res;
    q->bg = NULL;
    q->page = im->page;
    q->image = (u16)(im - s->res->images);
    q->engine = s->engine;
    q->prio = (u8)((s->flags & SPR_PRIO_MASK) >> 17);
    q->kind = MNQ_SPRITE;
    q->argb = (s->alpha >= 16 ? 0xff000000u : (u32)(s->alpha * 255 / 16) << 24) | 0xffffff;
    q->oam = s->oam > 31 ? 31 : s->oam;
    if (q->oam > s_max_oam)
        s_max_oam = q->oam;
}

void AnimatorSprite_Draw(MnSpr *s) { AnimatorSprite_DrawScaled(s, 0x1000, 0x1000); }

void mn_emit_flat(int engine, int prio, float x, float y, float w, float h, u32 argb) {
    MnQuad *q;
    if (s_nquads >= MN_MAX_QUADS)
        return;
    q = &s_quads[s_nquads++];
    memset(q, 0, sizeof(*q));
    q->x = x;
    q->y = y;
    q->w = w;
    q->h = h;
    q->engine = (u8)engine;
    q->prio = (u8)prio;
    q->kind = MNQ_FLAT;
    q->argb = argb;
}

/* A part of a BG image as a quad in the sprite list of `engine` at priority `prio`
 * (scenes that scroll BG rows independently: TimeAttackMenu's BG1 HDMA bands, the
 * split-scroll of TryAgain). Emit them after the sprites of that priority: the
 * first quad emitted is in front. */
void mn_emit_bg_part(int engine, int prio, const MnBgImg *img, float x, float y, float sx, float sy, float w,
                     float h) {
    MnQuad *q;
    if (!img || s_nquads >= MN_MAX_QUADS || w <= 0 || h <= 0)
        return;
    q = &s_quads[s_nquads++];
    memset(q, 0, sizeof(*q));
    q->x = x;
    q->y = y;
    q->w = w;
    q->h = h;
    q->u0 = sx;
    q->v0 = sy;
    q->u1 = sx + w;
    q->v1 = sy + h;
    q->bg = img;
    q->engine = (u8)engine;
    q->prio = (u8)prio;
    q->kind = MNQ_BG;
    q->argb = 0xffffffff;
}

/* an affine BG (G2x_SetBGyAffine with a Scale * Rot matrix): image pixel (cx, cy) at screen (cx, cy),
 * `scale` texels per screen pixel (fx32), rotation `angle` (0x10000 a turn); outside the image nothing
 * (area over off).  Stored as a quad: x, y = centre, w = scale, h = angle, u0 = 1 (affine) */
void mn_emit_bg_affine(int engine, int prio, const MnBgImg *img, float cx, float cy, s32 scale, s32 angle) {
    MnQuad *q;
    if (!img || s_nquads >= MN_MAX_QUADS || scale <= 0)
        return;
    q = &s_quads[s_nquads++];
    memset(q, 0, sizeof(*q));
    q->x = cx;
    q->y = cy;
    q->w = scale / 4096.0f;
    q->h = (float)(angle & 0xffff) * (6.28318530718f / 65536.0f);
    q->bg = img;
    q->engine = (u8)engine;
    q->prio = (u8)prio;
    q->kind = MNQ_AFFINE;
    q->argb = 0xffffffff;
}
/* the four screen corners (DS px) of an affine quad's image: screen = c + R(-angle) (tex - c) / scale */
void mn_affine_corners(const MnQuad *q, float xy[4][2]) {
    static const float tc[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    float c = cosf(q->h), sn = sinf(q->h), inv = 1.0f / q->w;
    int k;
    for (k = 0; k < 4; k++) {
        float dx = tc[k][0] * q->bg->w - q->x, dy = tc[k][1] * q->bg->h - q->y;
        xy[k][0] = q->x + inv * (c * dx - sn * dy);
        xy[k][1] = q->y + inv * (sn * dx + c * dy);
    }
}

/* The affine quad cut to its region's source rectangle (the part of the DS screen the composition
 * shows: the DS clips the BG to its screen), in DS px of the engine with the image's texel u, v:
 * a convex polygon of up to 8 vertices {x, y, u, v} in order; returns the count (0: nothing left). */
int mn_affine_clip(const MnQuad *q, const MnRegion *r, float out[8][4]) {
    float a[8][4], b[8][4];
    float xy[4][2];
    static const int ring[4] = {0, 1, 3, 2};   /* the corners around the quad */
    int n = 4, e, i, k;
    mn_affine_corners(q, xy);
    for (k = 0; k < 4; k++) {
        int c = ring[k];
        a[k][0] = xy[c][0];
        a[k][1] = xy[c][1];
        a[k][2] = (c & 1) ? (float)q->bg->w : 0.0f;
        a[k][3] = (c & 2) ? (float)q->bg->h : 0.0f;
    }
    for (e = 0; e < 4 && n > 0; e++) {   /* Sutherland-Hodgman: x >= sx0, x <= sx1, y >= sy0, y <= sy1 */
        int axis = e >> 1, m = 0;
        float lim = e == 0 ? r->sx0 : e == 1 ? r->sx1 : e == 2 ? r->sy0 : r->sy1, sgn = (e & 1) ? -1.0f : 1.0f;
        for (i = 0; i < n; i++) {
            const float *p = a[i], *q2 = a[(i + 1) % n];
            float dp = (p[axis] - lim) * sgn, dq = (q2[axis] - lim) * sgn;
            if (dp >= 0.0f && m < 8)
                memcpy(b[m++], p, sizeof(b[0]));
            if ((dp >= 0.0f) != (dq >= 0.0f) && m < 8) {
                float t = dp / (dp - dq);
                for (k = 0; k < 4; k++)
                    b[m][k] = p[k] + (q2[k] - p[k]) * t;
                m++;
            }
        }
        n = m;
        memcpy(a, b, sizeof(a[0]) * (size_t)n);
    }
    memcpy(out, a, sizeof(a[0]) * (size_t)n);
    return n;
}

/* ---------------------------------------------------- composition */
void mn_comp_reset(int base_engine) {
    memset(&g_mn_comp, 0, sizeof(g_mn_comp));
    g_mn_comp.base = (u8)(base_engine & 1);
}
MnRegion *mn_comp_add(int engine, float x0, float y0, float x1, float y1, float dx, float dy, float scale, int hide_bg,
                      int hide_spr) {
    MnRegion *r;
    if (g_mn_comp.n >= MN_MAX_REGIONS)
        return &g_mn_comp.r[MN_MAX_REGIONS - 1];
    r = &g_mn_comp.r[g_mn_comp.n++];
    r->hide_bg = (u8)hide_bg;
    r->hide_spr = (u8)hide_spr;
    r->engine = (u8)(engine & 1);
    r->sx0 = x0;
    r->sy0 = y0;
    r->sx1 = x1;
    r->sy1 = y1;
    r->dx = dx;
    r->dy = dy;
    r->scale = scale;
    return r;
}
MnRegion *mn_comp_full(int engine) { return mn_comp_add(engine, 0, 0, 256, 192, 0, 0, 2.5f, 0, 0); }
int mn_tex_pow2(int v) {
    int p = 8;
    while (p < v && p < 1024)
        p *= 2;
    return p;
}

/* BG layers -> quads (wrapping scroll, as a DS text BG wraps at its size) */
static MnQuad s_bgq[2][4][8];
static int s_nbgq[2][4];
static MnDraw s_sorted[(MN_MAX_QUADS + 64) * 2];
static int s_nsorted;

/* BG layer -> quads (wrapping scroll, as a DS text BG wraps at its plane size) */
static void build_bg(int e, int layer) {
    const MnBgLayer *L = &g_mn_layer[e][layer];
    const MnBgOfs *ofs = e ? &gDisplaySub[layer] : &gDisplayMain[layer];
    int sx, sy, tx, ty, ww, wh;
    s_nbgq[e][layer] = 0;
    if (!L->visible || !L->img)
        return;
    ww = L->wrap_w ? L->wrap_w : (L->img->w > 256 ? L->img->w : 256);
    wh = L->wrap_h ? L->wrap_h : (L->img->h > 256 ? L->img->h : 256);
    sx = ofs->x % ww;
    sy = ofs->y % wh;
    for (ty = -sy; ty < 192; ty += wh) {
        for (tx = -sx; tx < 256; tx += ww) {
            MnQuad *q;
            if (s_nbgq[e][layer] >= 8)
                return;
            q = &s_bgq[e][layer][s_nbgq[e][layer]++];
            memset(q, 0, sizeof(*q));
            q->x = (float)tx;
            q->y = (float)ty;
            q->w = L->img->w;
            q->h = L->img->h;
            q->u1 = L->img->w;
            q->v1 = L->img->h;
            q->bg = L->img;
            q->engine = (u8)e;
            q->prio = L->prio;
            q->kind = MNQ_BG;
            q->argb = (L->alpha >= 16 ? 0xff000000u : (u32)(L->alpha * 255 / 16) << 24) | 0xffffff;
        }
    }
}

/* Back to front: the regions in the order the scene added them; inside a
 * region the DS compositing of its engine: for priority 3..0, BG layers of
 * that priority (higher BG number further back), then sprites of that
 * priority in reverse OAM order (the first sprite drawn is in front). */
int mn_sorted(const MnDraw **out) {
    int e, p, l, i, k, ri;
    s_nsorted = 0;
    for (e = 0; e < 2; e++)
        for (l = 0; l < 4; l++)
            build_bg(e, l);
    for (ri = 0; ri < g_mn_comp.n; ri++) {
        const MnRegion *r = &g_mn_comp.r[ri];
        e = r->engine;
        for (p = 3; p >= 0; p--) {
            for (l = 3; l >= 0; l--) {
                if (g_mn_layer[e][l].prio != p || (r->hide_bg & (1 << l)))
                    continue;
                if (l == 0 && g_mn_3d.on && g_mn_3d.engine == e && mn_3d_count() > 0 &&
                    s_nsorted < (int)(sizeof(s_sorted) / sizeof(s_sorted[0]))) {
                    /* BG0 is the 3D output: the models go here in the order (mn_model.c) */
                    static MnQuad marker;
                    marker.kind = MNQ_3D;
                    marker.engine = (u8)e;
                    marker.prio = (u8)p;
                    s_sorted[s_nsorted].q = &marker;
                    s_sorted[s_nsorted++].r = r;
                }
                for (k = 0; k < s_nbgq[e][l]; k++) {
                    s_sorted[s_nsorted].q = &s_bgq[e][l][k];
                    s_sorted[s_nsorted++].r = r;
                }
            }
            if (r->hide_spr)
                continue;
            {   /* OAM order: Oam_AllocObj lists by oamPrio (lower in front), draw order inside a list */
                int o;
                for (o = s_max_oam; o >= 0; o--)
                    for (i = s_nquads - 1; i >= 0; i--)
                        if (s_quads[i].engine == e && s_quads[i].prio == p && s_quads[i].oam == o &&
                            s_nsorted < (int)(sizeof(s_sorted) / sizeof(s_sorted[0]))) {
                            s_sorted[s_nsorted].q = &s_quads[i];
                            s_sorted[s_nsorted++].r = r;
                        }
            }
        }
    }
    *out = s_sorted;
    return s_nsorted;
}

/* ------------------------------------------------------------ KeyframeAnim */
void *mn_ama_load(const char *path) {
    char full[200];
    const char *base = strrchr(path, '/');
    u8 *b;
    u32 size;
    snprintf(full, sizeof(full), "%s/ama/%s", g_mn_dir, base ? base + 1 : path);
    if (mn_load_file(full, &b, &size) != 0) {
        fprintf(stderr, "menus: missing %s\n", full);
        return NULL;
    }
    if (size < 12 || memcmp(b, "#AMA", 4) != 0) {
        free(b);
        return NULL;
    }
    return b;
}

/* bank: +4 u32 count, +8 u32 offset of 0x20-byte sequences:
 * +0 flags (1 pos loops, 2 scale loops), +4 frames, +8 pos count, +0xC pos length,
 * +0x10 pos offset, +0x14 scale count, +0x18 scale length, +0x1C scale offset.
 * Keys: 0x20 bytes, pos {flags, frame, x, y, ease}, scale {flags, frame, x, y, z, e0, e1}. Offsets are
 * from the bank start. */
static u32 ama32(const u8 *p) { return rd32(p); }
s32 KeyframeAnim_SeqFrames(const void *hdr, u32 i) {
    const u8 *h = (const u8 *)hdr;
    if (!h || i >= ama32(h + 4))
        return 0;
    return (s32)ama32(h + ama32(h + 8) + i * 32 + 4);
}

void KeyframeAnim_Init(KeyframeAnim *a, const void *hdr, u32 seq) {
    memset(a, 0, sizeof(*a));
    a->hdr = (const u8 *)hdr;
    if (hdr && seq < ama32(a->hdr + 4))
        a->seq = seq;
    a->speed = 0x1000;
    a->unk20 = -1;
    a->unk24 = -1;
    a->flags |= 0x20;
}

BOOL KeyframeAnim_IsFinished(const KeyframeAnim *a) {
    if (a->flags & 2)
        return FALSE;
    if (a->flags & 0x40)
        return TRUE;
    return FALSE;
}

void KeyframeAnim_PlayReverse(KeyframeAnim *a) {
    s32 n = KeyframeAnim_SeqFrames(a->hdr, a->seq);
    n = n > 0 ? n - 1 : 0;
    a->flags &= ~0x41u;
    a->flags |= 0x28;
    a->frame = n << 12;
    a->unk20 = -1;
    a->unk24 = -1;
}

void KeyframeAnim_SetFrame(KeyframeAnim *a, s32 frame) {
    s32 n = KeyframeAnim_SeqFrames(a->hdr, a->seq);
    if (frame >= n * 0x1000) {
        if (n * 0x1000 > 0)
            frame = (n - 1) * 0x1000;
        else
            frame = 0;
    }
    a->flags &= ~0x40u;
    a->frame = frame;
    a->unk20 = -1;
    a->unk24 = -1;
    a->flags |= 0x20;
}

void KeyframeAnim_SetToLastFrame(KeyframeAnim *a) {
    s32 n = KeyframeAnim_SeqFrames(a->hdr, a->seq);
    n = n > 0 ? n - 1 : 0;
    a->frame = n << 12;
    a->unk20 = -1;
    a->unk24 = -1;
    a->flags |= 0x20;
}

void KeyframeAnim_SetAnim(KeyframeAnim *a, u32 seq) {
    if (a->hdr && seq < ama32(a->hdr + 4))
        a->seq = seq;
    a->flags &= ~0x49u;
    a->frame = 0;
    a->speed = 0x1000;
    a->unk20 = -1;
    a->unk24 = -1;
    a->flags |= 0x20;
}

static s32 KeyframeAnim_EaseCurve(s32 t, s32 e) { return FX_Mul(t, e) + FX_Mul(FX_Mul(t, t), 0x1000 - e); }

/* KeyframeAnim_EvalPosition / EvalScaleRotation share this walk. ksize = key
 * size, out[2] = lerped (x, y) (scale: x, y, z with z lerped separately). */
typedef struct { u32 flags, length, count, offset; } AmaTrack;
static void ama_lerp_pos(const u8 *k0, const u8 *k1, s32 ratio, s32 *out, u32 flags) {
    s32 e0;
    s32 x0 = (s32)rd32(k0 + 8), y0 = (s32)rd32(k0 + 12), x1 = (s32)rd32(k1 + 8), y1 = (s32)rd32(k1 + 12);
    if (ratio >= 0x1000) {
        out[0] = x1;
        out[1] = y1;
        return;
    }
    if (ratio <= 0 || !(rd32(k0) & 1)) {
        out[0] = x0;
        out[1] = y0;
        return;
    }
    e0 = (s32)rd32(k0 + 16);
    if (flags & 4)
        e0 = 0x1000;
    else if (flags & 0x10)
        e0 = 0x2000 - e0;
    if (e0 != 0x1000)
        ratio = KeyframeAnim_EaseCurve(ratio, e0);
    out[0] = x0 + FX_Mul(x1 - x0, ratio);
    out[1] = y0 + FX_Mul(y1 - y0, ratio);
}
static void ama_lerp_scale(const u8 *k0, const u8 *k1, s32 ratio, s32 *out, u32 flags) {
    s32 x0 = (s32)rd32(k0 + 8), y0 = (s32)rd32(k0 + 12), z0 = (s32)rd32(k0 + 16);
    s32 x1 = (s32)rd32(k1 + 8), y1 = (s32)rd32(k1 + 12), z1 = (s32)rd32(k1 + 16);
    (void)flags;   /* the DS computes the eased ratios and discards them */
    if (ratio >= 0x1000) {
        out[0] = x1;
        out[1] = y1;
        out[2] = z1;
        return;
    }
    if (ratio <= 0 || !(rd32(k0) & 1)) {
        out[0] = x0;
        out[1] = y0;
        out[2] = z0;
        return;
    }
    out[0] = x0 + FX_Mul(x1 - x0, ratio);
    out[1] = y0 + FX_Mul(y1 - y0, ratio);
    out[2] = (s32)(((u32)z0 + (u32)FX_Mul((z1 - z0) << 12, ratio)) >> 12);
}

static void ama_eval(KeyframeAnim *a, const AmaTrack *tr, int ksize, int loopbit, s32 *cur, s32 *out, int scale) {
    s32 n = (s32)tr->count, t, step, i, j, f0, f1, len, fr, frame;
    BOOL rev;
    const u8 *base = a->hdr + tr->offset, *k, *k0, *k1, *kn;
    if (n == 0) {
        if (scale) {
            out[0] = 0x1000;
            out[1] = 0x1000;
            out[2] = 0;
        } else {
            out[0] = 0x100000;
            out[1] = 0x100000;
        }
        return;
    }
    if (n == 1) {
        if (scale)
            ama_lerp_scale(base, base, 0, out, a->flags);
        else
            ama_lerp_pos(base, base, 0, out, a->flags);
        return;
    }
    frame = a->frame;
    t = frame >> 12;
    if ((t << 12) < frame)
        t++;
    if (tr->flags & (u32)loopbit)
        t = t % (s32)tr->length;
    else if (t < 0)
        t = 0;
    else if (t >= (s32)tr->length)
        t = (s32)tr->length - 1;
    if (a->flags & 8)
        rev = a->speed < 0 ? FALSE : TRUE;
    else
        rev = a->speed < 0 ? TRUE : FALSE;
    step = rev ? -1 : 1;
    i = *cur;
    if (i >= n)
        i = n - 1;
    if (i < 0)
        i = rev ? n - 1 : 0;
    k = base + i * ksize;
    if (rev) {
        if ((s32)rd32(k + 4) <= t)
            k = NULL;
    } else {
        if ((s32)rd32(k + 4) > t)
            k = NULL;
    }
    if (k == NULL)
        i = rev ? n - 1 : 0;
    for (;;) {
        k0 = base + i * ksize;
        j = i + step;
        if (j < 0)
            j = n - 1;
        if (j >= n)
            j = 0;
        k1 = base + j * ksize;
        kn = k1;
        f0 = (s32)rd32(k0 + 4);
        f1 = (s32)rd32(k1 + 4);
        if (rev) {
            if (f0 < f1)
                f0 += (s32)tr->length;
            if (f0 >= t && f1 <= t)
                break;
        } else {
            if (f0 > f1)
                f1 += (s32)tr->length;
            if (f0 <= t && f1 >= t)
                break;
        }
        i += step;
        if (i < 0)
            i = n - 1;
        if (i >= n)
            i = 0;
    }
    if (rev) {
        kn = k0;
        k0 = k1;
    }
    len = (s32)tr->length << 12;
    fr = frame - (frame / len) * len;
    f0 = (s32)rd32(k0 + 4) << 12;
    f1 = (s32)rd32(kn + 4) << 12;
    if (f0 > f1)
        f1 += len;
    if (f0 > fr)
        fr += len;
    if (fr > f1)
        fr = f1;
    if (f0 > fr)
        f0 = fr;
    if (scale)
        ama_lerp_scale(k0, kn, FX_Div(fr - f0, f1 - f0), out, a->flags);
    else
        ama_lerp_pos(k0, kn, FX_Div(fr - f0, f1 - f0), out, a->flags);
    *cur = i;
}

void KeyframeAnim_Update(KeyframeAnim *a) {
    const u8 *seq;
    AmaTrack tp, ts;
    s32 pos[2], sc[3], max;
    if (!a->hdr)
        return;
    if ((a->flags & 1) && !(a->flags & 0x20))
        return;
    if (a->seq >= ama32(a->hdr + 4))
        return;
    seq = a->hdr + ama32(a->hdr + 8) + a->seq * 32;
    max = ((s32)rd32(seq + 4) - 1) << 12;
    tp.flags = ts.flags = rd32(seq);
    tp.count = rd32(seq + 8);
    tp.length = rd32(seq + 12);
    tp.offset = rd32(seq + 16);
    ts.count = rd32(seq + 20);
    ts.length = rd32(seq + 24);
    ts.offset = rd32(seq + 28);
    ama_eval(a, &tp, 0x20, 1, &a->unk20, pos, 0);
    a->x = (s16)((pos[0] + 0x800) >> 12);
    a->y = (s16)((pos[1] + 0x800) >> 12);
    ama_eval(a, &ts, 0x20, 2, &a->unk24, sc, 1);
    a->unk4 = sc[0];
    a->unk8 = sc[1];
    a->unkc = (s16)sc[2];
    if (!(a->flags & 1)) {
        if (a->flags & 8) {
            a->frame -= a->speed;
            if (a->frame < 0) {
                if (a->flags & 2) {
                    a->frame = max;
                } else {
                    a->flags |= 0x41;
                    a->frame = 0;
                }
            }
        } else {
            a->frame += a->speed;
            if (a->frame > max && !(a->flags & 2)) {
                a->flags |= 0x41;
                a->frame = max;
            }
            if (a->frame < 0)
                a->frame = 0;
        }
    }
    a->flags &= ~0x20u;
}
