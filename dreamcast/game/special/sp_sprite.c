/*
 * sp_sprite.c - AnimatorSprite subset on the converted BAC frame tables
 * (frames.bin "DSPR", tools/dreamcast/sprites/bac2dc.py): same timing rules as
 * AnimatorSprite_SetAnimation / AnimatorSprite_ProcessAnimation (wait =
 * duration << 4, speed 0x10 per frame, flag 2 = ended at the end of the last
 * frame, non-looping sprites stop there with flag 1).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"
#ifdef _arch_dreamcast
#include "../engine/render.h"
#endif

int sp_sprres_load(SpSprRes *r, const char *id) {
    char rel[128];
    u32 n;
    u8 *f;
    u16 hdr[8];
    memset(r, 0, sizeof(*r));
    snprintf(rel, sizeof(rel), "sprites/%s/frames.bin", id);
    f = (u8 *)sp_file_load(rel, &n);
    if (!f || n < 16 || memcmp(f, "DSPR", 4)) {
        free(f);
        return -1;
    }
    memcpy(hdr, f + 4, 12);
    /* header: version, pages, images, anims, frames */
    r->frames = f;
    r->n_images = hdr[2];
    r->n_anims = hdr[3];
    r->n_frames = hdr[4];
    memcpy(&r->page_w, f + 16, 2);
    memcpy(&r->page_h, f + 18, 2);
    r->images = (const SpDsprImage *)(f + 16 + 4 * hdr[1]);
    r->anims = (const SpDsprAnim *)((const u8 *)r->images + 16 * r->n_images);
    r->frametab = (const u16 *)((const u8 *)r->anims + 12 * r->n_anims);
#ifdef _arch_dreamcast
    {   /* the page as PVR PAL4 with its palette rows (ARGB1555 when it is a direct page) */
        char dir[256];
        snprintf(dir, sizeof(dir), "%s/sprites/%s", g_sp_path, id);
        r->atlas = calloc(1, sizeof(SprAtlas));
        if (r->atlas && spr_atlas_load((SprAtlas *)r->atlas, dir, f, 0) > 0) {
            r->loaded = 1;
            return 0;
        }
        if (r->atlas)
            spr_atlas_free((SprAtlas *)r->atlas);
        free(r->atlas);
        r->atlas = NULL;
    }
#endif
    snprintf(rel, sizeof(rel), "sprites/%s/atlas0.argb1555", id);
    r->pixels = (u16 *)sp_file_load(rel, &n);
    if (!r->pixels || n != (u32)r->page_w * r->page_h * 2) {
        free(r->pixels);
        free(f);
        memset(r, 0, sizeof(*r));
        return -1;
    }
    r->loaded = 1;
    return 0;
}

void sp_sprres_free(SpSprRes *r) {
#ifdef _arch_dreamcast
    if (r->atlas)
        spr_atlas_free((SprAtlas *)r->atlas);
#endif
    free(r->atlas);
    free(r->frames);
    free(r->pixels);
    memset(r, 0, sizeof(*r));
}

void AnimatorSprite_Init(SpSprite *s, const SpSprRes *res, u16 anim, u32 flags, int engine) {
    memset(s, 0, sizeof(*s));
    s->res = res;
    s->speed = 0x10;
    s->flags = flags;
    s->engine = (u8)engine;
    s->frame = -1;
    s->wait = 0;
    s->unk46 = 0;
    s->anim = anim;
    s->frameNo = 0;
    s->frameCount = 0;
    s->cur_image = -1;
}

void AnimatorSprite_SetAnimation(SpSprite *s, u16 anim) {
    s->anim = anim;
    s->frameNo = 0;
    s->wait = 0;
    s->frameCount = 0;
    s->flags &= ~3u;
    s->frame = -1;
}

static void sp_sprite_enter_frame(SpSprite *s) {
    const SpSprRes *r = s->res;
    const SpDsprAnim *a;
    if (!r || !r->loaded || s->anim >= r->n_anims) {
        s->cur_image = -1;
        s->wait = 0x7fffffff;
        return;
    }
    a = &r->anims[s->anim];
    s->frame++;
    if (s->frame >= a->frame_count) {
        /* EndAnimation: jump to the loop point (or the next animation) */
        if (a->next_anim >= 0 && a->next_anim < r->n_anims && a->loop_frame < 0) {
            s->anim = (u16)a->next_anim;
            a = &r->anims[s->anim];
            s->frame = 0;
        } else
            s->frame = a->loop_frame >= 0 ? a->loop_frame : 0;
    }
    if (a->frame_count == 0) {
        s->cur_image = -1;
        s->wait = 0x7fffffff;
        return;
    }
    {
        const u16 *fr = &r->frametab[(a->first_frame + s->frame) * 2];
        s->cur_image = fr[0] == 0xffff ? -1 : fr[0];
        s->wait = (s32)fr[1] << 4;
        s->frameNo = (u16)(s->frame + 1);
        s->frameCount = a->frame_count;
    }
}

void AnimatorSprite_ProcessAnimation(SpSprite *s, s32 a, s32 b) {
    (void)a;
    (void)b;
    if (s->flags & 1)
        return;
    s->flags &= ~2u;
    if (s->wait > 0) {
        s->wait -= s->speed;
        if (s->wait > 0)
            return;
        if (s->frameNo == s->frameCount) {
            s->flags |= 2;
            if (!(s->flags & 4)) {
                s->flags |= 1;
                return;
            }
        }
    }
    sp_sprite_enter_frame(s);
}

int sp_sprite_image(const SpSprite *s) { return s->cur_image; }
