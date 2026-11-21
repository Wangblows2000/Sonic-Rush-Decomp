#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d0b1c;

typedef struct {
    u8 pad0[0xbe4];
    UnkSpr_022d0b1c spr[3];
    s32 unkcbc;
    s32 unkcc0;
    u8 padcc4[4];
    s32 unkcc8;
    s32 unkccc;
    u8 padcd0[4];
    s32 unkcd4;
    s32 unkcd8;
    u8 padcdc[0xcec - 0xcdc];
    s32 unkcec;
    s32 unkcf0;
    s32 unkcf4;
} UnkStruct_022d0b1c;

void StaffRoll_CalcStopMotion(float *a, float *b, float c, float d);
void AnimatorSprite_SetAnimation(void *s, u16 id);
void StaffRoll_SetPictureState(UnkStruct_022d0b1c *p, void (*fn)(UnkStruct_022d0b1c *));
void StaffRoll_PictureState_Blaze4(UnkStruct_022d0b1c *p);

BOOL _fgr(float a, float b);
#define FX_F32_TO_FX32_022d0b1c(x) ((fx32)(_fgr((x), 0.0f) ? ((x) * 4096.0f + 0.5f) : ((x) * 4096.0f - 0.5f)))

#pragma thumb on
void StaffRoll_PictureState_Blaze3(UnkStruct_022d0b1c *p) {
    float t;
    float a;
    float b;

    p->spr[0].flags &= ~4;
    p->spr[1].flags |= 4;
    p->spr[2].flags |= 4;
    p->spr[0].flags &= ~0x80;
    p->spr[1].flags &= ~0x80;
    p->spr[2].flags &= ~0x80;
    p->unkcbc = 0xa0000;
    p->unkcc8 = 0x78000;
    p->unkcd4 = 0;
    p->unkcec = 0;
    p->unkcc0 = -0x28000;
    p->unkccc = 0x78000;
    StaffRoll_CalcStopMotion(&a, &b, 220.0f, 140.0f);
    t = a;
    p->unkcd8 = FX_F32_TO_FX32_022d0b1c(t);
    t = b;
    p->unkcf0 = FX_F32_TO_FX32_022d0b1c(t);
    p->unkcf4 = -0x18;
    AnimatorSprite_SetAnimation(&p->spr[1], 0x1f);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze4);
}
#pragma thumb reset
