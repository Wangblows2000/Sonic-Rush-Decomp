/*
 * Sprite resources and the native BAC animation player.
 *
 * Translation of AnimatorSprite_Init / _SetAnimation / _ProcessAnimation and
 * the BAC command handlers (src/game/Graphics/Sprite/BAC_FrameGroupFunc_*),
 * run on the converted frame table (frames.bin, DSPR v1) instead of the BAC
 * command stream:
 *   FrameAssembly + SpriteParts + Palette  -> the pre-composed atlas image
 *   Callback_7 (hit box)                   -> the .hbx events of the frame
 *   EndFrame                               -> frame_no / frame_cnt / wait
 *   EndAnimation                           -> jump to loop_frame
 * plus the animation half of StageObj_Draw2DEx / StageObj_SetAnimation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj_internal.h"

#ifdef _arch_dreamcast
int obj_draw_upload(ObjRes *r, const char *dir); /* obj_draw.c */
void obj_draw_release(ObjRes *r);
#endif

int obj_load_file(const char *path, u8 **out, u32 *size) {
    FILE *f = fopen(path, "rb");
    long n;
    u8 *b;
    *out = NULL;
    if (!f)
        return -1;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = (u8 *)malloc(n > 0 ? (size_t)n : 1);
    if (!b || fread(b, 1, (size_t)n, f) != (size_t)n) {
        free(b);
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = b;
    if (size)
        *size = (u32)n;
    return 0;
}

static u16 rd16(const u8 *p) { return (u16)(p[0] | p[1] << 8); }

int obj_res_load(ObjRes *r, const char *spr_dir, const char *obj_dir, const char *id) {
    char path[160];
    u32 size, need;
    u8 *b;
    memset(r, 0, sizeof(*r));
    strncpy(r->id, id, sizeof(r->id) - 1);
    if (!id[0])
        return -1;
    snprintf(path, sizeof(path), "%s/%s/frames.bin", spr_dir, id);
    if (obj_load_file(path, &b, &size) != 0)
        return -1;
    if (size < 16 || memcmp(b, "DSPR", 4) || rd16(b + 4) != 1) {
        free(b);
        return -1;
    }
    r->n_pages = rd16(b + 6);
    r->n_images = rd16(b + 8);
    r->n_anims = rd16(b + 10);
    r->n_frames = rd16(b + 12);
    need = 16u + r->n_pages * 4u + r->n_images * 16u + r->n_anims * 12u + r->n_frames * 4u;
    if (need != size) {
        free(b);
        return -1;
    }
    r->dspr = b;
    r->pages = (const DsprPage *)(b + 16);
    r->images = (const DsprImage *)(r->pages + r->n_pages);
    r->anims = (const DsprAnim *)(r->images + r->n_images);
    r->frames = (const DsprFrame *)(r->anims + r->n_anims);
    snprintf(path, sizeof(path), "%s/hbx/%s.hbx", obj_dir, id);
    if (obj_load_file(path, &b, &size) == 0) {
        if (size >= 16 && !memcmp(b, "HBOX", 4) && rd16(b + 4) == 1 && rd16(b + 6) == r->n_anims &&
            rd16(b + 8) == r->n_frames &&
            size == 16u + rd16(b + 6) * 4u + rd16(b + 8) * 4u + rd16(b + 10) * 12u) {
            r->hbx = b;
            r->hbx_frames = rd16(b + 8);
            r->hframes = (const HboxFrame *)(b + 16 + rd16(b + 6) * 4u);
            r->hevents = (const HboxEvent *)(r->hframes + r->hbx_frames);
        } else {
            free(b);
        }
    }
    r->loaded = 1;
#ifdef _arch_dreamcast
    snprintf(path, sizeof(path), "%s/%s", spr_dir, id);
    obj_draw_upload(r, path);
#endif
    return 0;
}

void obj_res_free(ObjRes *r) {
#ifdef _arch_dreamcast
    obj_draw_release(r);
#endif
    free(r->dspr);
    free(r->hbx);
    memset(r, 0, sizeof(*r));
}

/* ------------------------------------------------ AnimatorSprite_Init */
void spr_init(ObjSprite *s, const ObjRes *res, u16 anim, u32 flags) {
    memset(s, 0, sizeof(*s));
    s->res = res;
    s->speed = 0x10;
    s->flags = flags;
    s->anim = anim;
    s->pos = 0; /* unk1C = anims[idx].cmdPos */
}

/* ------------------------------------------ AnimatorSprite_SetAnimation */
void spr_set_anim(ObjSprite *s, u16 anim) {
    s->anim = anim;
    s->frame_no = 0;
    s->wait = 0;
    s->frame_t = 0;
    s->frame_cnt = 0;
    s->flags &= ~3u;
    s->pos = 0;
}

/* --------------------------------------- AnimatorSprite_ProcessAnimation */
void spr_process(ObjSprite *s, ObjSprCb cb, void *owner) {
    const ObjRes *r = s->res;
    const DsprAnim *a;
    u16 fi, k;
    if (!r || !r->loaded || s->anim >= r->n_anims)
        return;
    if (s->flags & SPR_STOPPED)
        return;
    s->flags &= ~SPR_ENDED;
    if (s->wait > 0) {
        s->wait -= s->speed;
        s->frame_t += s->speed;
        if (s->wait > 0)
            return;
        s->frame_t += s->wait;
        if (s->frame_no == s->frame_cnt) {
            s->flags |= SPR_ENDED;
            if (!(s->flags & SPR_LOOP)) {
                s->flags |= SPR_STOPPED;
                return;
            }
        }
    }
    a = &r->anims[s->anim];
    if (a->frame_count == 0)
        return;
    if (s->pos >= a->frame_count) {
        /* BAC_FrameGroupFunc_EndAnimation */
        s->frame_no = 0;
        s->frame_t = 0;
        s->pos = (u16)(a->loop_frame >= 0 ? a->loop_frame : 0);
    }
    fi = (u16)(a->first_frame + s->pos);
    /* Callback_7 commands of this frame (before EndFrame) */
    if (cb && r->hframes && fi < r->hbx_frames) {
        const HboxFrame *hf = &r->hframes[fi];
        for (k = 0; k < hf->count; k++)
            cb(owner, &r->hevents[hf->first_event + k]);
    }
    /* BAC_FrameGroupFunc_EndFrame */
    s->cur = fi;
    s->valid = 1;
    s->frame_cnt = a->frame_count;
    s->frame_no = (u16)(s->pos + 1);
    s->wait = (s16)(r->frames[fi].duration << 4);
    s->pos++;
}

const DsprImage *spr_image(const ObjSprite *s) {
    const ObjRes *r = s->res;
    u16 im;
    if (!r || !r->loaded || !s->valid || s->cur >= r->n_frames)
        return NULL;
    im = r->frames[s->cur].image;
    if (im == 0xFFFF || im >= r->n_images)
        return NULL;
    return &r->images[im];
}

/* ------------------------------------------------- StageObj_SetAnimation */
void obj_set_anim(Obj *o, u16 anim) {
    o->h.disp_flags &= ~8;
    o->h.disp_flags &= ~4;
    if (o->has_spr)
        spr_set_anim(&o->spr, anim);
}

/* ------------------------------------- StageObj_Draw2DEx (animation part) */
void obj_draw2d_ex(s32 x, s32 y, Obj *o, ObjSprite *s) {
    u16 f = 0;
    s16 save;
    s->flags |= 0x4000;
    if (o != NULL) {
        f = o->h.disp_flags;
        s->rot = (u8)(o->h.angle + o->h.grav_rot);
        if (o->has_spr)
            f ^= 1; /* unkA4 != NULL: 2D sprite art faces left */
        s->flags &= ~0x184u;
        o->h.disp_flags &= ~8;
        if (f & 4)
            s->flags |= SPR_LOOP;
        if (f & 1)
            s->flags |= SPR_HFLIP;
        if (f & 2)
            s->flags |= SPR_VFLIP;
    }
    s->screen = (u8)(!(W.objmgr & 4) || (f & 0x80));
    if (!(f & 0x2000)) {
        s->x = (s16)(x >> 8);
        s->y = (s16)(y >> 8);
    }
    if (o != NULL) {
        s->x += o->h.shake_x;
        s->y += o->h.shake_y;
    }
    save = s->speed;
    s->speed = (s16)((save * W.timescale) >> 8);
    if (f & 0x10)
        s->speed = 0;
    if (!(f & 0x1000))
        spr_process(s, o ? o->spr_cb : NULL, o);
    s->speed = save;
    s->hidden = (u8)((f & 0x20) != 0);
    /* rot-scale size: data_0207cf80 (0x100) << 4, scaled by obj @0x44/@0x46 */
    s->sx = 0x1000;
    s->sy = 0x1000;
    if (o != NULL) {
        if (o->scale_x != 0)
            s->sx = (s16)((s->sx * o->scale_x) >> 8);
        if (o->scale_y != 0)
            s->sy = (s16)((s->sy * o->scale_y) >> 8);
    }
    s->rotscale = (u8)(((f & 0x100) && s->rot != 0) || s->sx != 0x1000 || s->sy != 0x1000);
    if (o == NULL)
        return;
    if (s->flags & SPR_ENDED)
        o->h.disp_flags |= 8;
}

void obj_draw2d(Obj *o) { obj_draw2d_ex(o->h.x, o->h.y, o, &o->spr); }

/* --------------------------------------- MapObject_SpriteCallback_Default */
void obj_sprite_cb_default(void *owner, const HboxEvent *ev) {
    Obj *o = (Obj *)owner;
    if (ev->slot > 2)
        return;
    if (ev->l == ev->r && ev->t == ev->b) {
        o->coll[ev->slot].flags &= ~4;
        return;
    }
    o->coll[ev->slot].owner = o;
    o->coll[ev->slot].owner_kind = OBJ_OWNER_OBJECT;
    rect_set_box2d(&o->coll[ev->slot], ev->l, ev->t, ev->r, ev->b);
}
