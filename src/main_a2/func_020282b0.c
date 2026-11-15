#include <nitro/types.h>

typedef struct { u32 param; u32 extra; } UnkTexData_020282b0;
typedef struct { u8 unk0[8]; u32 vramKey; u8 unkC[0xC]; u32 vramKey4x4; u8 unk1C[0x20]; u8 dict[4]; } UnkTex_020282b0;
typedef struct { u8 unk0[0x10]; u32 prmTexImage; u8 unk14[0x18]; u16 origW; u16 origH; fx32 magW; fx32 magH; } UnkMatResult_020282b0;

UnkTexData_020282b0 *func_0202603c(void *dict, const void *name);
fx32 func_0200225c(fx32 a, fx32 b);

void func_020282b0(UnkTex_020282b0 *tex, const void *name, UnkMatResult_020282b0 *res) {
    UnkTexData_020282b0 *d = func_0202603c(tex->dict, name);
    u32 base;
    u32 w;
    u32 h;
    if ((d->param & 0x1C000000) != 0x14000000) {
        base = tex->vramKey & 0xFFFF;
    } else {
        base = tex->vramKey4x4 & 0xFFFF;
    }
    res->prmTexImage &= 0xC00F0000;
    res->prmTexImage |= d->param + base;
    res->origW = d->extra & 0x7FF;
    res->origH = (d->extra & 0x3FF800) >> 11;
    w = d->extra & 0x7FF;
    h = (d->extra >> 11) & 0x7FF;
    res->magW = (w == res->origW) ? 0x1000 : func_0200225c(w << 12, res->origW << 12);
    res->magH = (h == res->origH) ? 0x1000 : func_0200225c(h << 12, res->origH << 12);
}
