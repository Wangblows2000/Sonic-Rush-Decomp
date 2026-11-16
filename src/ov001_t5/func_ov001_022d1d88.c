#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 timer;
    u8 pad18[0xb34 - 0x18];
    u8 unkb34[0xd0c - 0xb34];
    u8 unkd0c[0xf0c - 0xd0c];
    u8 unkf0c[4];
} UnkStruct_022d1d88;

BOOL func_0202e5e4(float a, float b);
void func_ov001_022d0070(UnkStruct_022d1d88 *p);
void func_ov001_022cfd7c(void *a, s32 b, float f, void *c, void *d);
void func_ov001_022d2234(UnkStruct_022d1d88 *p, void (*fn)(UnkStruct_022d1d88 *));
void func_ov001_022d1ce0(UnkStruct_022d1d88 *p);

#pragma thumb on
void func_ov001_022d1d88(UnkStruct_022d1d88 *p) {
    float f;

    func_ov001_022d0070(p);
    if (p->timer <= 0x40) {
        f = p->timer / 64.0f;
        if (func_0202e5e4(f, 1.0f)) f = 1.0f;
        func_ov001_022cfd7c(p->unkb34, 0x100, f, p->unkf0c, p->unkd0c);
    }
    if (p->timer > 0x1c6) {
        func_ov001_022d2234(p, func_ov001_022d1ce0);
    }
}
#pragma thumb reset
