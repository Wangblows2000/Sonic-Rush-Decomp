#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbe4];
    u8 spr[0xc10 - 0xbe4];
    u32 flags0;
    u8 padc14[0xc58 - 0xc14];
    u32 flags1;
    u8 padc5c[0xcbc - 0xc5c];
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
} UnkStruct_022d1110;

void StaffRoll_CalcStopMotion(float *a, float *b, float c, float d);
void AnimatorSprite_SetAnimation(void *s, u16 id);
void StaffRoll_SetPictureState(UnkStruct_022d1110 *p, void (*fn)(UnkStruct_022d1110 *));
void StaffRoll_PictureState_Sonic6(UnkStruct_022d1110 *p);

BOOL func_0202e5e4(float a, float b);
#define FX_F32_TO_FX32_022d1110(x) ((fx32)(func_0202e5e4((x), 0.0f) ? ((x) * 4096.0f + 0.5f) : ((x) * 4096.0f - 0.5f)))

#pragma thumb on
void StaffRoll_PictureState_Sonic5(UnkStruct_022d1110 *p) {
    float a;
    float b;
    float t;

    p->flags0 |= 4;
    p->flags0 |= 0x80;
    p->flags1 |= 0x80;
    p->unkcbc = -0x14000;
    p->unkcc8 = 0x78000;
    p->unkcec = 0;
    p->unkcc0 = -0x28000;
    p->unkccc = 0x78000;
    p->unkcd8 = 0;
    p->unkcf0 = 0;
    StaffRoll_CalcStopMotion(&a, &b, 60.0f, 150.0f);
    t = a;
    p->unkcd4 = FX_F32_TO_FX32_022d1110(t);
    t = b;
    p->unkcec = FX_F32_TO_FX32_022d1110(t);
    AnimatorSprite_SetAnimation(p->spr, 0x13);
    StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic6);
}
#pragma thumb reset
