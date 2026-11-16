#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x14];
    u16 unk44;
    u16 unk46;
} UnkSpr_022d3eb0;

typedef struct {
    u8 pad0[0x18];
    s32 offset;
} UnkRes_022d3eb0;

void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_022d3eb0 *s, UnkRes_022d3eb0 *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, void *h);

#pragma thumb on
void func_ov001_022d3eb0(UnkSpr_022d3eb0 *s, UnkRes_022d3eb0 *res, u32 idx, s32 mode, u16 a, u16 b) {
    func_0203363c(s, res, idx, 0, 0, 0, func_02039c18(0, *(u16 *)((u8 *)res + res->offset + 8)), 0, (void *)0x5000200);
    s->flags &= ~0x60000;
    s->flags |= mode << 17;
    s->unk44 = a;
    s->unk46 = b;
}
#pragma thumb reset
