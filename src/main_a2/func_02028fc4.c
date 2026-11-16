#include <nitro/types.h>

typedef struct { fx32 v[6]; } UnkScaleCache_02028fc4;
typedef struct {
    u32 flag;
    fx32 scale[3];
    fx32 invScale[6];
} UnkJntAnm_02028fc4;
typedef struct { u8 unk0[0xC4]; u32 bits[4]; } UnkRS_02028fc4;

extern UnkRS_02028fc4 *data_02084f9c;
extern UnkScaleCache_02028fc4 data_02085da0[];

void MIi_CpuCopy32(const void *src, void *dst, u32 size);

#define FX_MUL_02028fc4(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void func_02028fc4(UnkJntAnm_02028fc4 *r, const fx32 *scale, const u8 *data, u32 flag) {
    u32 n = data[1];
    u32 p = data[2];
    if (flag & 4) {
        r->flag |= 1;
        if (data_02084f9c->bits[p >> 5] & (1 << (p & 0x1F))) {
            data_02084f9c->bits[n >> 5] |= 1 << (n & 0x1F);
            r->flag |= 0x18;
        } else {
            MIi_CpuCopy32(&data_02085da0[p], &data_02085da0[n], 0x18);
            MIi_CpuCopy32(&data_02085da0[p], r->invScale, 0x18);
        }
    } else {
        r->scale[0] = scale[0];
        r->scale[1] = scale[1];
        r->scale[2] = scale[2];
        if (data_02084f9c->bits[p >> 5] & (1 << (p & 0x1F))) {
            MIi_CpuCopy32(scale, &data_02085da0[n], 0x18);
            data_02084f9c->bits[n >> 5] &= ~(1 << (n & 0x1F));
            r->flag |= 0x18;
        } else {
            data_02084f9c->bits[n >> 5] &= ~(1 << (n & 0x1F));
            data_02085da0[n].v[0] = FX_MUL_02028fc4(scale[0], data_02085da0[p].v[0]);
            data_02085da0[n].v[1] = FX_MUL_02028fc4(scale[1], data_02085da0[p].v[1]);
            data_02085da0[n].v[2] = FX_MUL_02028fc4(scale[2], data_02085da0[p].v[2]);
            data_02085da0[n].v[3] = FX_MUL_02028fc4(scale[3], data_02085da0[p].v[3]);
            data_02085da0[n].v[4] = FX_MUL_02028fc4(scale[4], data_02085da0[p].v[4]);
            data_02085da0[n].v[5] = FX_MUL_02028fc4(scale[5], data_02085da0[p].v[5]);
            MIi_CpuCopy32(&data_02085da0[p], r->invScale, 0x18);
        }
    }
}
