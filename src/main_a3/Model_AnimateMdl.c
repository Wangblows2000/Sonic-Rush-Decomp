#include <nitro/types.h>

typedef struct { u8 unk0[4]; u16 numFrames; } UnkAnmRes_020313cc;
typedef struct { fx32 frame; fx32 ratio; UnkAnmRes_020313cc *res; } UnkAnmObj_020313cc;
typedef struct {
    u32 type;
    u8 unk4[0x8C];
    u8 renderObj[0x54];
    UnkAnmObj_020313cc *anm[5];
    UnkAnmObj_020313cc *anm2[5];
    u8 flags[5];
    u8 unk111[3];
    fx32 speed;
    fx32 speeds[5];
    s16 blendSpeed;
    s16 blendSpeeds[5];
} UnkObj_020313cc;

typedef struct { u32 unk0; u32 flags; } UnkCtx_020313cc;

extern u8 gSystemHeapAllocator[];

void NNS_G3dRenderObjRemoveAnmObj(void *renderObj, void *anm);
void func_02025b38(void *alloc, void *anm);

static inline fx32 FX_Mul_020313cc(fx32 a, fx32 b)
{
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

void Model_AnimateMdl(UnkCtx_020313cc *ctx, UnkObj_020313cc *o)
{
    u32 i;
    fx32 d;
    fx32 end;
    for (i = 0; i < 5; i++) {
        o->flags[i] &= ~2;
        if ((o->flags[i] & 0x40) && o->anm2[i] != NULL) {
            if (!(o->flags[i] & 0x80)) {
                d = FX_Mul_020313cc(o->blendSpeed, o->blendSpeeds[i]);
                o->anm[i]->ratio += d;
                o->anm2[i]->ratio -= d;
            }
            if (o->anm2[i]->ratio <= 0) {
                o->anm[i]->ratio = 0x1000;
                o->flags[i] &= ~0x40;
                o->flags[i] |= 0x80;
                NNS_G3dRenderObjRemoveAnmObj(o->renderObj, o->anm2[i]);
                func_02025b38(gSystemHeapAllocator, o->anm2[i]);
                o->anm2[i] = NULL;
            }
        }
        if (o->anm[i] != NULL) {
            end = o->anm[i]->res->numFrames << 12;
            if (!(ctx->flags & 2) && !(o->flags[i] & 1)) {
                d = FX_Mul_020313cc(o->speed, o->speeds[i]);
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
