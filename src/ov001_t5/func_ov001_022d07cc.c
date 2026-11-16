#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbe4];
    u8 spr[0xc10 - 0xbe4];
    u32 flags0;
    u8 padc14[0xc58 - 0xc14];
    u32 flags1;
    u8 padc5c[0xca0 - 0xc5c];
    u32 flags2;
    u8 padca4[0xcbc - 0xca4];
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
} UnkStruct_022d07cc;

void func_ov001_022cffdc(float *a, float *b, float c, float d);
void AnimatorSprite_SetAnimation(void *s, u16 id);
void StaffRoll_SetPictureState(UnkStruct_022d07cc *p, void (*fn)(UnkStruct_022d07cc *));
void func_ov001_022d0760(UnkStruct_022d07cc *p);

BOOL func_0202e5e4(float a, float b);
#define FX_F32_TO_FX32_022d07cc(x) ((fx32)(func_0202e5e4((x), 0.0f) ? ((x) * 4096.0f + 0.5f) : ((x) * 4096.0f - 0.5f)))

#pragma thumb on
void func_ov001_022d07cc(UnkStruct_022d07cc *p) {
    float t;
    float a;
    float b;

    p->flags0 |= 4;
    p->flags1 |= 4;
    p->flags2 |= 4;
    p->flags0 &= ~0x80;
    p->flags1 &= ~0x80;
    p->flags2 &= ~0x80;
    p->unkcbc = 0xa0000;
    p->unkcc8 = 0x78000;
    p->unkcc0 = 0x50000;
    p->unkccc = 0x78000;
    p->unkcd8 = 0;
    p->unkcf0 = 0;
    func_ov001_022cffdc(&a, &b, 90.0f, -40.0f);
    t = a;
    p->unkcd4 = FX_F32_TO_FX32_022d07cc(t);
    t = b;
    p->unkcec = FX_F32_TO_FX32_022d07cc(t);
    AnimatorSprite_SetAnimation(p->spr, 0x1c);
    StaffRoll_SetPictureState(p, func_ov001_022d0760);
}
#pragma thumb reset
