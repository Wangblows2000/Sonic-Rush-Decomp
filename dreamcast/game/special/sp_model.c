/*
 * sp_model.c - Model runtime (src/game/Graphics/Model/) for the special stage on
 * the converted G3D packages (.dcm/.dca/.dcv, dreamcast/game/model):
 * Model_Init, Model_InitSprite3D, Model_SetResource, Model_SetAnimation and
 * Model_Animate/Model_AnimateMdl with the original flag protocol (flags[i]:
 * 1 paused, 2 ended, 4 loop, 0x40 cross-fade; speed @0x114).  The state
 * machines read these (player states wait on 0x10C & 2), so the timing code is
 * kept as in the original (same as dreamcast/game/boss/boss_model.c).
 * Texture-pattern (NSBTP) and texture-SRT (NSBTA) animations are timing objects
 * whose frame selects a converted pattern variant / uv offset at draw time;
 * material-colour animations (NSBMA) are timing only.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

MtxFx43 g_sp_mtx_slot[32];
int g_sp_upload_textures;

int sp_mdl_load(SpMdlRes *r, const char *file) {
    char rel[160];
    u32 n;
    memset(r, 0, sizeof(*r));
    snprintf(rel, sizeof(rel), "models/%s", file);
    r->bytes = (u8 *)sp_file_load(rel, &n);
    if (!r->bytes)
        return -1;
    if (dcm_model_load(&r->mdl, r->bytes, n, g_sp_upload_textures) != 0) {
        free(r->bytes);
        r->bytes = NULL;
        return -1;
    }
    r->loaded = 1;
    return 0;
}

void sp_mdl_free(SpMdlRes *r) {
    if (r->loaded)
        dcm_model_free(&r->mdl);
    free(r->bytes);
    memset(r, 0, sizeof(*r));
}

/* every material lit by the global emission only (Map/sky models: NNS_G3dGlbMaterialColorSpecEmi
 * (0, 0x7fff), NNSi_G3dModifyMatFlag(.., 0x400), light bits masked out): full-bright texture */
void sp_mdl_unlit(SpMdlRes *r) {
    u32 i;
    dcm_material_t *mat;
    if (!r->loaded)
        return;
    mat = (dcm_material_t *)r->mdl.materials;
    for (i = 0; i < r->mdl.hdr->num_materials; i++) {
        mat[i].light_mask = 0;
        mat[i].diffuse = (mat[i].diffuse & 0xff000000u) | 0xffffff;
        mat[i].emission = 0xffffff;
    }
}

int sp_anm_load(SpAnmRes *r, const char *file, int kind) {
    char rel[160];
    u32 n;
    memset(r, 0, sizeof(*r));
    r->kind = kind;
    snprintf(rel, sizeof(rel), "models/%s", file);
    r->bytes = (u8 *)sp_file_load(rel, &n);
    if (!r->bytes)
        return -1;
    if (dca_set_load(&r->set, r->bytes, n) != 0) {
        free(r->bytes);
        r->bytes = NULL;
        return -1;
    }
    r->loaded = 1;
    return 0;
}

void sp_anm_free(SpAnmRes *r) {
    int i;
    free(r->bytes);
    for (i = 0; i < r->nvariants; i++)
        sp_mdl_free(&r->variants[i]);
    free(r->variants);
    memset(r, 0, sizeof(*r));
}

/* timing-only animation (NSBMA / NSBTP / NSBTA): nframes */
void sp_anm_timing(SpAnmRes *r, int kind, u16 nframes) {
    memset(r, 0, sizeof(*r));
    r->kind = kind;
    r->nframes_fixed = nframes;
    r->loaded = 1;
}

static u16 anim_frames(const SpAnmRes *res, u32 idx, int *ok) {
    *ok = 0;
    if (res == NULL || !res->loaded)
        return 0;
    if (res->kind == 0 || res->kind == 1) {
        if (res->set.hdr && idx < res->set.hdr->num_anims) {
            *ok = 1;
            return res->set.anims[idx].num_frames;
        }
        return 0;
    }
    *ok = 1;
    return res->nframes_fixed;
}

void Model_Init(SpModel *s, u32 a) {
    int i;
    memset(s, 0, sizeof(*s));
    s->type = 0;
    s->flag = a;
    s->cb[0] = 3;
    s->scale[0] = s->scale[1] = s->scale[2] = 0x1000;
    MTX_Identity33_(&s->rot);
    MTX_Identity43_(&s->unk60);
    for (i = 0; i < 5; i++) {
        s->speeds[i] = 0x1000;
        s->blendSpeeds[i] = 0x1000;
    }
    s->speed = 0x1000;
    s->blendSpeed = 0x1000;
}

/* Model_InitSprite3D(o, res, anim, 0, tex, pltt, 0, 0) */
void Model_InitSprite3D(SpModel *o, const SpSprRes *res, u16 anim) {
    memset(o, 0, sizeof(*o));
    o->type = 2;
    o->flag = 0;
    o->cb[0] = 3;
    o->scale[0] = o->scale[1] = o->scale[2] = 0x1000;
    MTX_Identity33_(&o->rot);
    MTX_Identity43_(&o->unk60);
    AnimatorSprite_Init(&o->spr, res, anim, 0, 2);
    o->unkDC = 0x1f0080;  /* polyAttr: alpha 31, mode bits as Model_InitSprite3D */
}

void Model_SetResource(SpModel *s, const SpMdlRes *res) {
    int i;
    s->res = res;
    for (i = 0; i < 5; i++) {
        s->anm[i] = NULL;
        s->anm2[i] = NULL;
    }
}

void Model_SetAnimation(SpModel *o, u32 slot, const SpAnmRes *res, u32 idx) {
    SpAnmObj *a;
    u16 nf;
    int ok;
    if (slot >= 5)
        return;
    if (o->anm2[slot] != NULL)
        o->anm2[slot] = NULL;
    switch (slot) {
    case 0:
    case 1:
    case 4:
        if (o->flags[slot] & 0x40) {
            o->flags[slot] &= ~0x80;
            o->anm2[slot] = o->anm[slot];
            o->anm[slot] = NULL;
            break;
        }
        /* fallthrough */
    case 2:
    case 3:
        o->anm[slot] = NULL;
        break;
    }
    nf = anim_frames(res, idx, &ok);
    if (!ok) {
        o->flags[slot] &= ~3;
        return;
    }
    a = (o->anm2[slot] == &o->anm_obj[slot][0]) ? &o->anm_obj[slot][1] : &o->anm_obj[slot][0];
    a->frame = 0;
    a->ratio = 0x1000;
    a->res = res;
    a->idx = (int)idx;
    a->numFrames = nf;
    o->anm[slot] = a;
    if (o->anm2[slot] != NULL) {
        o->anm[slot]->ratio = 0;
        o->anm2[slot]->ratio = 0x1000;
    }
    o->flags[slot] &= ~3;
}

static void Model_AnimateMdl(SpModel *ctx, SpModel *o) {
    u32 i;
    fx32 d = 0, end;
    for (i = 0; i < 5; i++) {
        o->flags[i] &= ~2;
        if ((o->flags[i] & 0x40) && o->anm2[i] != NULL) {
            if (!(o->flags[i] & 0x80)) {
                d = FX_Mul(o->blendSpeed, o->blendSpeeds[i]);
                o->anm[i]->ratio += d;
                o->anm2[i]->ratio -= d;
            }
            if (o->anm2[i]->ratio <= 0) {
                o->anm[i]->ratio = 0x1000;
                o->flags[i] &= ~0x40;
                o->flags[i] |= 0x80;
                o->anm2[i] = NULL;
            }
        }
        if (o->anm[i] != NULL) {
            end = o->anm[i]->numFrames << 12;
            if (!(ctx->flag & 2) && !(o->flags[i] & 1)) {
                d = FX_Mul(o->speed, o->speeds[i]);
                o->anm[i]->frame += d;
            }
            if (d > 0) {
                if (o->anm[i]->frame >= end) {
                    o->flags[i] |= 2;
                    if (o->flags[i] & 4)
                        o->anm[i]->frame -= end;
                    else
                        o->anm[i]->frame = end - 1;
                }
            } else if (d < 0) {
                if (o->anm[i]->frame < 0) {
                    o->flags[i] |= 2;
                    if (o->flags[i] & 4)
                        o->anm[i]->frame += end;
                    else
                        o->anm[i]->frame = 0;
                }
            }
        }
    }
}

void Model_Animate(SpModel *s) {
    switch (s->type) {
    case 0:
        Model_AnimateMdl(s, s);
        break;
    case 2:
        AnimatorSprite_ProcessAnimation(&s->spr, (s32)s->unkEC, (s32)s->unkF0);
        break;
    }
}
