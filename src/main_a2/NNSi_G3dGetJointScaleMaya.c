#include <nitro/types.h>

typedef struct { fx32 x, y, z; } UnkVec_02028bcc;
typedef struct { UnkVec_02028bcc s; UnkVec_02028bcc inv; } UnkScaleCache_02028bcc;
typedef struct { fx32 v[6]; } UnkScaleCacheW_02028bcc;
typedef struct {
    u32 flag;
    UnkVec_02028bcc scale;
    UnkVec_02028bcc invScale;
} UnkJntAnm_02028bcc;
typedef struct { u8 unk0[0xC4]; u32 bits[4]; } UnkRS_02028bcc;

extern UnkRS_02028bcc *data_02084f9c;
extern UnkScaleCache_02028bcc data_02085dac[];
extern UnkScaleCacheW_02028bcc data_02085db0[];
extern UnkScaleCacheW_02028bcc data_02085db4[];

void NNSi_G3dGetJointScaleMaya(UnkJntAnm_02028bcc *r, const fx32 *scale, const u8 *data, u32 flag) {
    u32 nflag = data[3];
    if (flag & 4) {
        r->flag |= 1;
        if (nflag & 2) {
            u32 n = data[1];
            data_02084f9c->bits[n >> 5] |= 1 << (n & 0x1F);
        }
    } else {
        r->scale.x = scale[0];
        r->scale.y = scale[1];
        r->scale.z = scale[2];
        if (nflag & 2) {
            u32 n = data[1];
            data_02084f9c->bits[n >> 5] &= ~(1 << (n & 0x1F));
            data_02085dac[n].s.x = scale[3];
            data_02085db0[n].v[0] = scale[4];
            data_02085db4[n].v[0] = scale[5];
        }
    }
    if (nflag & 1) {
        u32 p = data[2];
        r->flag |= 0x20;
        if (data_02084f9c->bits[p >> 5] & (1 << (p & 0x1F))) {
            r->flag |= 8;
        } else {
            r->invScale = data_02085dac[p].s;
        }
    }
    r->flag |= 0x10;
}
