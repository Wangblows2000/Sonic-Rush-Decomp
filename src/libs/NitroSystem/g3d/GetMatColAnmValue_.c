#include <nitro/types.h>

u16 GetMatColAnmValue_(const u8 *base, u32 info, u32 idx)
{
    const u16 *d;
    u32 last;
    u32 i0;
    u32 i1;
    u32 c1;
    u32 c0;

    if (info & 0x20000000)
        return (u16)info;
    d = (const u16 *)(base + (info & 0xffff));
    if (!(info & 0xc0000000))
        return d[idx];
    last = (info & 0x1fff0000) >> 16;
    if (info & 0x40000000) {
        if (idx & 1) {
            if (idx > last)
                return d[(last >> 1) + 1];
            i0 = idx >> 1;
            goto interp2;
        }
        return d[idx >> 1];
    } else {
        if (idx & 3) {
            if (idx > last)
                return (d + (last >> 2))[idx & 3];
            if (idx & 1) {
                if (idx & 2) {
                    i1 = idx >> 2;
                    i0 = i1 + 1;
                } else {
                    i0 = idx >> 2;
                    i1 = i0 + 1;
                }
                c0 = d[i0];
                c1 = d[i1];
                return (u16)(((((c0 & 0x7c1f) + (c0 & 0x7c1f) + (c0 & 0x7c1f) + (c1 & 0x7c1f)) >> 2) & 0x7c1f) | ((((c0 & 0x3e0) + (c0 & 0x3e0) + (c0 & 0x3e0) + (c1 & 0x3e0)) >> 2) & 0x3e0));
            }
            i0 = idx >> 2;
            goto interp2;
        }
        return d[idx >> 2];
    }
interp2:
    c0 = d[i0];
    c1 = d[i0 + 1];
    return (u16)(((((c0 & 0x7c1f) + (c1 & 0x7c1f)) >> 1) & 0x7c1f) | ((((c0 & 0x3e0) + (c1 & 0x3e0)) >> 1) & 0x3e0));
}
