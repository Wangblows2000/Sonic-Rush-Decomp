#include <nitro/types.h>

u32 GetTexSRTAnmSinCosVal_(const void *pTexAnm, u32 info, u32 data, u32 frame)
{
    u32 idx, idx_sub;
    u32 last_interp;
    const void *pDataHead;

    if (info & 0x20000000) {
        return data;
    }

    pDataHead = (const void *)((u8 *)pTexAnm + data);

    if (!(info & 0xc0000000)) {
        idx = frame;
        goto TEXSRT_SINCOS_NONINTERP;
    }

    last_interp = (0x0000ffff & info) >> 0;

    if (info & 0x40000000) {
        if (frame & 1) {
            if (frame > last_interp) {
                idx = (last_interp >> 1) + 1;
                goto TEXSRT_SINCOS_NONINTERP;
            } else {
                idx = frame >> 1;
                goto TEXSRT_SINCOS_INTERP_2;
            }
        } else {
            idx = frame >> 1;
            goto TEXSRT_SINCOS_NONINTERP;
        }
    } else {
        if (frame & 3) {
            if (frame > last_interp) {
                idx = (last_interp >> 2) + (frame & 3);
                goto TEXSRT_SINCOS_NONINTERP;
            }

            if (frame & 1) {
                fx32 s, s_sub;
                fx32 c, c_sub;

                if (frame & 2) {
                    idx_sub = (frame >> 2);
                    idx = idx_sub + 1;
                } else {
                    idx = (frame >> 2);
                    idx_sub = idx + 1;
                }

                s = *((const fx16 *)pDataHead + 2 * idx);
                c = *((const fx16 *)pDataHead + 2 * idx + 1);
                s_sub = *((const fx16 *)pDataHead + 2 * idx_sub);
                c_sub = *((const fx16 *)pDataHead + 2 * idx_sub + 1);

                s = (s + s + s + s_sub) >> 2;
                c = (c + c + c + c_sub) >> 2;
                return (u32)((s & 0xffff) | (c << 16));
            } else {
                idx = frame >> 2;
                goto TEXSRT_SINCOS_INTERP_2;
            }
        } else {
            idx = frame >> 2;
            goto TEXSRT_SINCOS_NONINTERP;
        }
    }

TEXSRT_SINCOS_NONINTERP:
    return *((const u32 *)pDataHead + idx);

TEXSRT_SINCOS_INTERP_2: {
    fx32 s0, s1;
    fx32 c0, c1;
    s0 = *((const fx16 *)pDataHead + 2 * idx);
    c0 = *((const fx16 *)pDataHead + 2 * idx + 1);

    s1 = *((const fx16 *)pDataHead + 2 * idx + 2);
    c1 = *((const fx16 *)pDataHead + 2 * idx + 3);

    return (u32)((((s0 + s1) >> 1) & 0xffff) | (((c0 + c1) >> 1) << 16));
}
}
