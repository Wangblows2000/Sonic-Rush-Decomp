#include <nitro/types.h>

u32 GetTexSRTAnmSinCosVal_(const u8 *base, u32 info, u32 ofs, u32 idx)
{
    u32 last;
    u32 i0;
    u32 i1;
    const s16 *d;

    if (info & 0x20000000)
        return ofs;
    d = (const s16 *)(base + ofs);
    if (info & 0xc0000000) {
        last = info & 0xffff;
        if (info & 0x40000000) {
            if (idx & 1) {
                if (idx > last) {
                    idx = (last >> 1) + 1;
                } else {
                    i0 = idx >> 1;
                    goto interp2;
                }
            } else {
                idx >>= 1;
            }
        } else {
            if (idx & 3) {
                if (idx > last) {
                    idx = (idx & 3) + (last >> 2);
                } else if (idx & 1) {
                    if (idx & 2) {
                        i1 = idx >> 2;
                        i0 = i1 + 1;
                    } else {
                        i0 = idx >> 2;
                        i1 = i0 + 1;
                    }
                    {
                        s32 t1 = d[i1 * 2 + 1];
                        s32 t0 = d[i0 * 2 + 1];
                        s32 s1 = d[i1 * 2];
                        s32 s0 = d[i0 * 2];
                        return ((((s32)(s0 * 3U + s1)) >> 2) & 0xffff) | ((((s32)(t0 * 3U + t1)) >> 2) << 16);
                    }
                } else {
                    i0 = idx >> 2;
                    goto interp2;
                }
            } else {
                idx >>= 2;
            }
        }
    }
    return ((const u32 *)d)[idx];
interp2:
    return (((d[i0 * 2] + d[i0 * 2 + 2]) >> 1) & 0xffff) | (((d[i0 * 2 + 1] + d[i0 * 2 + 3]) >> 1) << 16);
}
