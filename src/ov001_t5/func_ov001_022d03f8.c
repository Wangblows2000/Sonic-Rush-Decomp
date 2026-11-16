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
} UnkStruct_022d03f8;

void func_ov001_022cffdc(float *a, float *b, float c, float d);
void func_02033550(void *s, u16 id);
void func_ov001_022d1404(UnkStruct_022d03f8 *p, void (*fn)(UnkStruct_022d03f8 *));
void func_ov001_022d0658(UnkStruct_022d03f8 *p);

BOOL func_0202e5e4(float a, float b);
#define FX_F32_TO_FX32_022d03f8(x) ((fx32)(func_0202e5e4((x), 0.0f) ? ((x) * 4096.0f + 0.5f) : ((x) * 4096.0f - 0.5f)))

#pragma thumb on
void func_ov001_022d03f8(UnkStruct_022d03f8 *p) {
    float t;
    float a;
    float b;

    p->unkcc8 = 0x78000;
    func_ov001_022cffdc(&a, &b, 90.0f, -70.0f);
    t = a;
    p->unkcd4 = FX_F32_TO_FX32_022d03f8(t);
    t = b;
    p->unkcec = FX_F32_TO_FX32_022d03f8(t);
    func_ov001_022cffdc(&a, &b, 90.0f, 50.0f);
    t = a;
    p->unkcd8 = FX_F32_TO_FX32_022d03f8(t);
    t = b;
    p->unkcf0 = FX_F32_TO_FX32_022d03f8(t);
    func_02033550(p->spr, 0x1c);
    func_ov001_022d1404(p, func_ov001_022d0658);
}
#pragma thumb reset
