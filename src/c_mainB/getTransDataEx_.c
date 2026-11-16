#include <nitro/types.h>

typedef struct {
    u32 info;
    u32 offset;
} UnkAnmInfo_02026f88;

typedef struct {
    u8 unk0[4];
    u16 numFrame;
    u8 unk6[2];
    u32 flag;
} UnkAnm_02026f88;

void getTransDataEx_(fx32 *out, fx32 frame, const UnkAnmInfo_02026f88 *ai, const UnkAnm_02026f88 *anm)
{
    fx32 w;
    const u8 *data;
    fx32 v1;
    fx32 v0;
    u32 info;
    u32 idx;
    s32 shift;
    s32 step;

    idx = frame >> 12;
    data = (const u8 *)anm + ai->offset;
    info = ai->info;
    if (idx == anm->numFrame - 1) {
        if (info & 0xc0000000) {
            if (info & 0x40000000)
                idx = (idx & 1) + (idx >> 1);
            else
                idx = (idx & 3) + (idx >> 2);
        }
        if (anm->flag & 2) {
            w = frame & 0xfff;
            if (info & 0x20000000) {
                v0 = ((const s16 *)data)[idx];
                v1 = ((const s16 *)data)[0];
            } else {
                v0 = ((const fx32 *)data)[idx];
                v1 = ((const fx32 *)data)[0];
            }
            *out = v0 + ((w * (v1 - v0)) >> 12);
            return;
        }
        if (info & 0x20000000)
            *out = ((const s16 *)data)[idx];
        else
            *out = ((const fx32 *)data)[idx];
        return;
    }
    if (info & 0xc0000000) {
        u32 last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (idx >= last) {
                idx = last >> 1;
                goto step1;
            }
            idx >>= 1;
            w = frame & 0x1fff;
            step = 2;
            shift = 1;
        } else {
            if (idx >= last) {
                idx = (idx & 3) + (idx >> 2);
                goto step1;
            }
            idx >>= 2;
            w = frame & 0x3fff;
            step = 4;
            shift = 2;
        }
    } else {
    step1:
        step = 1;
        w = frame & 0xfff;
        shift = 0;
    }
    if (info & 0x20000000) {
        v0 = ((const s16 *)data)[idx];
        v1 = ((const s16 *)data)[idx + 1];
    } else {
        v0 = ((const fx32 *)data)[idx];
        v1 = ((const fx32 *)data)[idx + 1];
    }
    *out = (v0 * step + ((w * (v1 - v0)) >> 12)) >> shift;
}
