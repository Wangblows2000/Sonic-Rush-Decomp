#include <nitro/types.h>

typedef struct {
    u32 info;
    u32 offset;
} UnkAnmInfo_02027104;

void func_02027104(fx32 *out, fx32 frame, const UnkAnmInfo_02027104 *ai, u32 base)
{
    u32 data;
    u32 idx;
    u32 info;
    u32 i1;
    u32 i0;

    idx = frame >> 12;
    data = base + ai->offset;
    info = ai->info;

    if (info & 0xc0000000) {
        u32 last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (idx & 1) {
                if (idx > last) {
                    idx = (last >> 1) + 1;
                    goto direct;
                }
                i0 = idx >> 1;
                goto interp2;
            }
            idx >>= 1;
            goto direct;
        } else {
            if (idx & 3) {
                if (idx > last) {
                    idx = (idx & 3) + (last >> 2);
                    goto direct;
                }
                if (idx & 1) {
                    if (idx & 2) {
                        i1 = idx >> 2;
                        i0 = i1 + 1;
                    } else {
                        i0 = idx >> 2;
                        i1 = i0 + 1;
                    }
                    if (info & 0x20000000) {
                        const s16 *p = (const s16 *)data;
                        fx32 v = p[i0];
                        fx32 vs = p[i1];
                        *out = (v + v + v + vs) >> 2;
                    } else {
                        const fx32 *p = (const fx32 *)data;
                        fx32 v = p[i0];
                        fx32 vs = p[i1];
                        *out = (fx32)(((s64)v + v + v + vs) >> 2);
                    }
                    return;
                }
                i0 = idx >> 2;
                goto interp2;
            }
            idx >>= 2;
            goto direct;
        }
    }
    goto direct;
interp2:
    if (info & 0x20000000) {
        const s16 *d = (const s16 *)data;
        *out = (d[i0] + d[i0 + 1]) >> 1;
    } else {
        const fx32 *d = (const fx32 *)data;
        *out = (d[i0] >> 1) + (d[i0 + 1] >> 1);
    }
    return;
direct:
    if (info & 0x20000000) {
        *out = ((const s16 *)data)[idx];
    } else {
        *out = ((const fx32 *)data)[idx];
    }
}
