#include <nitro/types.h>

typedef struct { u8 unk0[4]; u16 numFrame; u16 unk6; u32 flag; } UnkAnm_02026ba0;
typedef struct { u32 info; u32 offset; } UnkInfo_02026ba0;

void getScaleDataEx_(fx32 *out, fx32 frame, const UnkInfo_02026ba0 *info, const UnkAnm_02026ba0 *anm) {
    const u8 *base;
    u32 idx;
    u32 idx2;
    u32 frac;
    s32 mul;
    s32 shift;
    u32 data;
    fx32 a0, a1, b0, b1;
    base = (const u8 *)anm + info->offset;
    idx = frame >> 12;
    data = info->info;
    if (idx == anm->numFrame - 1) {
        if (data & 0xC0000000) {
            if (data & 0x40000000) {
                idx = (idx & 1) + (idx >> 1);
            } else {
                idx = (idx & 3) + (idx >> 2);
            }
        }
        if (anm->flag & 2) {
            idx2 = 0;
            goto calc;
        }
        if (data & 0x20000000) {
            out[0] = ((const s16 *)base)[idx * 2];
            out[1] = ((const s16 *)base)[idx * 2 + 1];
        } else {
            out[0] = ((const fx32 *)base)[idx * 2];
            out[1] = ((const fx32 *)base)[idx * 2 + 1];
        }
        return;
    }
    if (data & 0xC0000000) {
        u32 last = (data & 0x1FFF0000) >> 16;
        if (data & 0x40000000) {
            if (idx >= last) {
                idx = last >> 1;
                idx2 = idx + 1;
                goto calc;
            }
            idx >>= 1;
            idx2 = idx + 1;
            frac = frame & 0x1FFF;
            mul = 2;
            shift = 1;
        } else {
            if (idx >= last) {
                idx = (idx & 3) + (idx >> 2);
                idx2 = idx + 1;
                goto calc;
            }
            idx >>= 2;
            idx2 = idx + 1;
            frac = frame & 0x3FFF;
            mul = 4;
            shift = 2;
        }
        goto interp;
    }
    idx2 = idx + 1;
calc:
    frac = frame & 0xFFF;
    mul = 1;
    shift = 0;
interp:
    if (data & 0x20000000) {
        a0 = ((const s16 *)base)[idx * 2];
        a1 = ((const s16 *)base)[idx * 2 + 1];
        b0 = ((const s16 *)base)[idx2 * 2];
        b1 = ((const s16 *)base)[idx2 * 2 + 1];
    } else {
        a0 = ((const fx32 *)base)[idx * 2];
        a1 = ((const fx32 *)base)[idx * 2 + 1];
        b0 = ((const fx32 *)base)[idx2 * 2];
        b1 = ((const fx32 *)base)[idx2 * 2 + 1];
    }
    out[0] = (a0 * mul + (((b0 - a0) * (s32)frac) >> 12)) >> shift;
    out[1] = (a1 * mul + (((b1 - a1) * (s32)frac) >> 12)) >> shift;
}
