#include <nitro/types.h>

s32 func_020280d4(const u8 *anm, u32 info, u32 data, u32 frame) {
    const u8 *p;
    u32 last;
    s32 a;
    s32 b;
    if (info & 0x20000000) return data;
    p = anm + data;
    if (!(info & 0xC0000000)) goto direct;
    last = info & 0xFFFF;
    if (info & 0x40000000) {
        if (frame & 1) {
            if (frame > last) {
                frame = (last >> 1) + 1;
                goto direct;
            }
            frame >>= 1;
            goto half;
        }
        frame >>= 1;
        goto direct;
    } else {
        if (frame & 3) {
            if (frame > last) {
                frame = (frame & 3) + (last >> 2);
                goto direct;
            }
            if (frame & 1) {
                u32 w;
                if (frame & 2) {
                    frame = frame >> 2;
                    w = frame + 1;
                } else {
                    w = frame >> 2;
                    frame = w + 1;
                }
                if (info & 0x10000000) {
                    a = ((const s16 *)p)[w];
                    b = ((const s16 *)p)[frame];
                } else {
                    a = ((const s32 *)p)[w];
                    b = ((const s32 *)p)[frame];
                }
                return (a * 3 + b) >> 2;
            }
            frame >>= 2;
            goto half;
        }
        frame >>= 2;
        goto direct;
    }
direct:
    if (info & 0x10000000) return ((const s16 *)p)[frame];
    return ((const s32 *)p)[frame];
half:
    if (info & 0x10000000) {
        a = ((const s16 *)p)[frame];
        b = ((const s16 *)p)[frame + 1];
    } else {
        a = ((const s32 *)p)[frame];
        b = ((const s32 *)p)[frame + 1];
    }
    return (a + b) >> 1;
}
